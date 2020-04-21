import os
from os.path import join

from waflib.TaskGen import feature, after_method
from waflib.extras import test_base
from waflib.extras.test_base import summary
from waflib.extras.symwaf2ic import get_toplevel_path


def depends(dep):
    dep("haldls")
    dep("libnux", "test/with_hostcode")


def options(opt):
    opt.load('nux_assembler')
    opt.load('nux_compiler')
    opt.load('test_base')
    opt.load('pytest')
    opt.add_option("--enable-stack-protector",
                   default=False,
                   action='store_true',
                   dest='stack_protector',
                   help="Enable stack frame overflow check.")
    opt.add_option("--enable-stack-redzone",
                   default=False,
                   action='store_true',
                   dest='stack_redzone',
                   help="Enable stack redzone check towards the program memory region.")
    opt.add_option("--disable-mailbox",
                   default=False,
                   action='store_true',
                   dest='disable_mailbox',
                   help="Disable mailbox memory region.")


def configure(conf):
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
    # specialize for v2
    conf.setenv('nux_v2', env=conf.all_envs['nux'])
    conf.define('LIBNUX_DLS_VERSION_V2', True)

    # specialize for vx
    conf.setenv('nux_vx', env=conf.all_envs['nux'])
    conf.define('LIBNUX_DLS_VERSION_VX', True)
    conf.env.append_value('CXXFLAGS', '-mcpu=s2pp_hx')

    # restore env
    conf.setenv('', env=env)

def build(bld):
    bld.env.dls_partition = "dls" == os.environ.get("SLURM_JOB_PARTITION")
    bld.env.cube_partition = "cube" == os.environ.get("SLURM_JOB_PARTITION")

    for dls_version in ['v2', 'vx']:
        env = bld.all_envs['nux_' + dls_version]

        bld(
            target = 'nux_inc_' + dls_version,
            export_includes = ['.'],
            env = env,
        )

        nux_sources = [
            'src/bitformatting.cpp',
            'src/correlation.cpp',
            'src/counter.cpp',
            'src/exp.cpp',
            'src/fxv.cpp',
            'src/mailbox.cpp',
            'src/malloc.cpp',
            'src/random.cpp',
            'src/stack_guards.cpp',
            'src/spikes.cpp',
            'src/time.cpp',
            'src/unittest.cpp',
            'src/unittest_mailbox.cpp',
        ]

        bld.stlib(
            target = 'nux_' + dls_version,
            source = nux_sources,
            use = ['nux_inc_' + dls_version],
            env = env,
        )

        bld(
            features = 'cxx',
            name = 'nux_runtime_obj_' + dls_version,
            source = ['src/start.cpp',
                      'src/initdeinit.cpp',
                      'src/cxa_pure_virtual.cpp'],
            use = 'nux_inc_' + dls_version,
            env = env,
        )

        bld(
            name='nux_runtime_shutdown_' + dls_version,
            target='crt_shutdown.o',
            source=['src/crt_shutdown.s'],
            features='use asm',
            env = env,
        )

        bld(
            name='nux_runtime_' + dls_version,
            target='crt.o',
            source=['src/crt.s'],
            features='use asm',
            use=['nux_runtime_obj_' + dls_version,
                 'nux_runtime_shutdown_' + dls_version],
            env=env,
        )

        program_list = [
            'examples/stdp.cpp',
            'test/test_bitformatting.cpp',
            'test/test_bool.cpp',
            'test/test_fxvadd.cpp',
            'test/test_fxvsel.cpp',
            'test/test_helper.cpp',
            'test/test_inline_vector_argument.cpp',
            'test/test_malloc.cpp',
            'test/test_many_vectors.cpp',
            'test/test_measure_time.cpp',
            'test/test_neuron_counter.cpp',
            'test/test_noinline_vector_argument.cpp',
            'test/test_return_vector.cpp',
            'test/test_returncode.cpp',
            'test/test_stack_guard.cpp',
            'test/test_stack_redzone.cpp',
            'test/test_synram_rw.cpp',
            'test/test_unittest.cpp',
            'test/test_vector.cpp',
            'test/test_vector_alignment.cpp',
            'test/test_vector_cc.cpp',
            'test/test_vector_splat.cpp',
            'test/test_vector_sync.cpp',
        ]

        if dls_version != 'vx':
            # These tests don't work for HX, see Issue #3365
            program_list += [
                'test/test_xorshift_vector.cpp', # HX does not yet implement xorshift_vector
            ]
        else:
            # These tests only work for HX
            program_list += [
                'test/test_cadc.cpp',
                'test/test_fpga_memory_vector_access.cpp',
                'test/test_synram.cpp',
            ]

        for program in program_list:
            bld.program(
                features = 'cxx',
                target = program.replace('.cpp', '') + '_' + dls_version + '.bin',
                source = [program],
                use = ['nux_' + dls_version, 'nux_runtime_' + dls_version],
                env = bld.all_envs['nux_' + dls_version],
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
            features = 'cxx check_size',
            check_size_max = max_size_empty(),
            target = 'test_empty_' + dls_version + '.bin',
            source = ['test/test_empty.cpp'],
            use = ['nux_' + dls_version, 'nux_runtime_' + dls_version],
            env = env,
        )

    bld(
        name='libnux_hwsimtests_vx',
        tests='test/test_hwsimtests_vx.py',
        features='use pytest pylint pycodestyle',
        use='dlens_vx',
        install_path='${PREFIX}/bin/tests',
        skip_run=not (bld.env.cube_partition or ("FLANGE_SIMULATION_RCF_PORT" in os.environ)),
        env = bld.all_envs[''],
        test_environ = dict(STACK_PROTECTION=env.LIBNUX_STACK_PROTECTOR_ENABLED[0],
                            STACK_REDZONE=env.LIBNUX_STACK_REDZONE_ENABLED[0],
                            TEST_BINARY_PATH=os.path.join(bld.env.PREFIX, 'build', 'libnux', 'test')),
        pylint_config=join(get_toplevel_path(), "code-format", "pylintrc"),
        pycodestyle_config=join(get_toplevel_path(), "code-format", "pycodestyle"),
        test_timeout = 5400
    )

    bld(
        name='libnux_hwtests_v2',
        tests='test/test_hwtests_v2.py',
        features='use pytest',
        use='dlens_v2',
        install_path='${PREFIX}/bin/tests',
        skip_run=not bld.env.dls_partition,
        env = bld.all_envs[''],
        test_environ = dict(STACK_PROTECTION=env.LIBNUX_STACK_PROTECTOR_ENABLED[0],
                            STACK_REDZONE=env.LIBNUX_STACK_REDZONE_ENABLED[0],
                            TEST_BINARY_PATH=os.path.join(bld.env.PREFIX, 'build', 'libnux', 'test'))
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
