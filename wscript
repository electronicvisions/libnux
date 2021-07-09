import os
from os.path import join

from waflib.TaskGen import feature, after_method
from waflib.extras import test_base
from waflib.extras.test_base import summary
from waflib.extras.symwaf2ic import get_toplevel_path


def depends(dep):
    dep("haldls")

    if getattr(dep.options, 'with_libnux_test_hostcode', True):
        dep("libnux", "test/with_hostcode")


def options(opt):
    opt.load('nux_assembler')
    opt.load('nux_compiler')
    opt.load('test_base')
    opt.load('pytest')

    hopts = opt.add_option_group('libnux options')
    hopts.add_option("--enable-stack-protector",
                     default=False,
                     action='store_true',
                     dest='stack_protector',
                     help="Enable stack frame overflow check.")
    hopts.add_option("--enable-stack-redzone",
                     default=False,
                     action='store_true',
                     dest='stack_redzone',
                     help="Enable stack redzone check towards the program memory region.")
    hopts.add_option("--disable-mailbox",
                     default=False,
                     action='store_true',
                     dest='disable_mailbox',
                     help="Disable mailbox memory region.")
    hopts.add_withoption('libnux-test-hostcode', default=True,
                         help='Toggle the generation and build of with_hostcode tests')


def configure(conf):
    if getattr(conf.options, 'with_libnux_test_hostcode', True):
        # host-based python stuff also needed for cross-env tests
        conf.load('pytest')

    # now configure for nux cross compiler
    env = conf.env
    conf.setenv('nux')
    conf.load('nux_assembler')
    conf.load('nux_compiler')
    conf.load('test_base')
    conf.env.append_value('LINKFLAGS', '-T%s' % conf.path.find_node('libnux/elf32nux.x').abspath())
    conf.env.append_value('ASLINKFLAGS', '-T%s' % conf.path.find_node('libnux/elf32nux.x').abspath())
    if(conf.options.stack_protector):
        conf.define("LIBNUX_STACK_PROTECTOR", True)
        conf.env.append_value('LIBNUX_STACK_PROTECTOR_ENABLED', 'True')
        conf.env.append_value('CFLAGS', '-fstack-protector-all')
        conf.env.append_value('CXXFLAGS', '-fstack-protector-all')
    else:
        conf.env.append_value('LIBNUX_STACK_PROTECTOR_ENABLED', 'False')

    if(conf.options.stack_redzone):
        conf.define("LIBNUX_STACK_REDZONE", True)
        conf.env.append_value('LIBNUX_STACK_REDZONE_ENABLED', 'True')
        conf.env.append_value('CFLAGS', '-fstack-limit-symbol=stack_redzone')
        conf.env.append_value('CXXFLAGS', '-fstack-limit-symbol=stack_redzone')
    else:
        conf.env.append_value('LIBNUX_STACK_REDZONE_ENABLED', 'False')

    if(not conf.options.disable_mailbox):
        conf.env.append_value('ASLINKFLAGS', '--defsym=mailbox_size=4096')
        conf.env.append_value('LINKFLAGS', '-Wl,--defsym=mailbox_size=4096')

    # specialize for vx-v1
    conf.setenv('nux_vx_v1', env=conf.all_envs['nux'])
    conf.env.append_value('CXXFLAGS', '-mcpu=s2pp_hx')

    # specialize for vx-v2
    conf.setenv('nux_vx_v2', env=conf.all_envs['nux'])
    conf.env.append_value('CXXFLAGS', '-mcpu=s2pp_hx')

    # restore env
    conf.setenv('', env=env)

def build(bld):
    bld.env.cube_partition = "cube" == os.environ.get("SLURM_JOB_PARTITION")

    bld(
        name = "libnux_test_helper",
        features = "py",
        source = bld.path.ant_glob("libnux_test_helper/*.py"),
        relative_trick = True,
        install_path = "${PREFIX}/lib",
        pylint_config=join(get_toplevel_path(), "code-format", "pylintrc"),
        pycodestyle_config=join(get_toplevel_path(), "code-format", "pycodestyle"),
    )

    chip_revision_list = ["vx"]
    chip_version_list = [["v1", "v2"]]

    for chip_idx, chip_revision in enumerate(chip_revision_list):
        for chip_version in chip_version_list[chip_idx]:

            env = bld.all_envs["nux_" + chip_revision + "_" + chip_version]

            bld(
                target = "nux_inc_" + chip_revision + "_" + chip_version,
                export_includes = ["."],
                env = env,
            )

            bld.stlib(
                target = "nux_" + chip_revision + "_" + chip_version,
                source = bld.path.ant_glob("src/" + chip_revision + "/*.cpp")
                       + bld.path.ant_glob("src/" + chip_revision + "/" + chip_version + "/*.cpp"),
                use = ["nux_inc_" + chip_revision + "_" + chip_version],
                env = env,
            )

            bld(
                features = "cxx",
                name = "nux_runtime_obj_" + chip_revision + "_" + chip_version,
                source = ["src/start.cpp",
                          "src/initdeinit.cpp",
                          "src/cxa_pure_virtual.cpp"],
                use = "nux_inc_" + chip_revision + "_" + chip_version,
                env = env,
            )

            bld(
                name = "nux_runtime_shutdown_" + chip_revision + "_" + chip_version,
                target = "crt_shutdown.o",
                source = ["src/crt_shutdown.s"],
                features = "use asm",
                env = env,
            )

            bld(
                name = "nux_runtime_" + chip_revision + "_" + chip_version,
                target = "crt.o",
                source = ["src/crt.s"],
                features = "use asm",
                use = ["nux_runtime_obj_" + chip_revision + "_" + chip_version,
                       "nux_runtime_shutdown_" + chip_revision + "_" + chip_version],
                env=env,
            )

            program_list = []
            program_list += ["test/" + chip_revision + "/" + os.path.basename(str(f)) 
                             for f in bld.path.ant_glob("test/" + chip_revision + "/*.cpp")]
            program_list += ["test/" + chip_revision + "/" + chip_version + "/" + os.path.basename(str(f))
                             for f in bld.path.ant_glob("test/" + chip_revision + "/" + chip_version + "/*.cpp")]
            program_list += ["examples/stdp.cpp"]

            for program in program_list:
                bld.program(
                    features = "cxx",
                    target = program.replace(".cpp", "") + "_" + chip_revision + "_" + chip_version + ".bin",
                    source = [program],
                    use = ["nux_" + chip_revision + "_" + chip_version,
                           "nux_runtime_" + chip_revision + "_" + chip_version],
                    env = bld.all_envs["nux_" + chip_revision + "_" + chip_version],
                )

            def max_size_empty():
                stack_protector = env.LIBNUX_STACK_PROTECTOR_ENABLED[0].lower() == "true"
                stack_redzone = env.LIBNUX_STACK_REDZONE_ENABLED[0].lower() == "true"
                build_profile = bld.options.build_profile

                if not stack_protector and not stack_redzone:
                    if build_profile == 'release':
                        return 400
                    else:
                        return 544

                if stack_protector and not stack_redzone:
                    if build_profile == 'release':
                        return 816
                    else:
                        return 864

                if not stack_protector and stack_redzone:
                    if build_profile == 'release':
                        return 496
                    else:
                        return 608

                if stack_protector and stack_redzone:
                    if build_profile == 'release':
                        return 928
                    else:
                        return 1008

            bld.program(
                features = "cxx check_size",
                check_size_max = max_size_empty(),
                target = "test_empty_" + chip_revision + "_" + chip_version + ".bin",
                source = ["test/helpers/test_empty.cpp"],
                use = ["nux_" + chip_revision + "_" + chip_version,
                       "nux_runtime_" + chip_revision + "_" + chip_version],
                env = env,
            )

            bld(
                name = "libnux_hwsimtests_" + chip_revision + "_" + chip_version,
                tests = "test/test_hwsimtests_" + chip_revision + "_" + chip_version + ".py",
                features = "use pytest pylint pycodestyle",
                use = ["dlens_" + chip_revision + "_" + chip_version, "libnux_test_helper"],
                install_path = "${PREFIX}/bin/tests",
                skip_run = not (bld.env.cube_partition or ("FLANGE_SIMULATION_RCF_PORT" in os.environ)),
                env = bld.all_envs[''],
                test_environ = dict(STACK_PROTECTION=env.LIBNUX_STACK_PROTECTOR_ENABLED[0],
                                    STACK_REDZONE=env.LIBNUX_STACK_REDZONE_ENABLED[0],
                                    TEST_BINARY_PATH=os.path.join(bld.env.PREFIX, "build", "libnux", "test")),
                pylint_config = join(get_toplevel_path(), "code-format", "pylintrc"),
                pycodestyle_config = join(get_toplevel_path(), "code-format", "pycodestyle"),
                test_timeout = 20000
            )

    bld.add_post_fun(summary)


class check_size(test_base.TestBase):
    def run(self):
        test = self.inputs[0]
        test_abspath = test.abspath()
        xmlfile_abspath = self.getXMLFile(test).abspath()
        max_size = self.generator.check_size_max
        cmd = ['python test/helpers/check_obj_size.py {} {} {}'.format(
            test_abspath, xmlfile_abspath, max_size)]
        self.runTest(test, cmd)


@feature('check_size')
@after_method('apply_link', 'process_use', 'propagate_uselib_vars')
def check_size_run_test(self):
    if self.testsDisabled():
        return
    if self.isTestExecutionEnabled() and getattr(self, 'link_task', None):
        t = self.create_task('check_size', self.link_task.outputs)
        self.bld.env = self.env
        t.init(self)
