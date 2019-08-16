import os

from waflib.TaskGen import feature, after_method
from waflib.extras import test_base
from waflib.extras.test_base import summary


def depends(dep):
    if not dep.options.dls_version:
        raise RuntimeError("Please specify DLS version to use (2 or 3).")

    if int(dep.options.dls_version) == 2:
        dep("hicann-dls-scripts")

    if int(dep.options.dls_version) == 3:
        dep("hicann-dls-scripts", branch="v3testing")


def options(opt):
    opt.load('nux_assembler')
    opt.load('nux_compiler')
    opt.load('test_base')
    opt.load('pytest')
    opt.add_option("--dls-version",
                   choices=["2", "3"],
                   help="DLS version to use (2 or 3).")
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
    if not conf.options.dls_version:
        raise RuntimeError("Please specify DLS version to use (2 or 3).")
    conf.define("LIBNUX_DLS_VERSION", int(conf.options.dls_version))
    conf.load('nux_assembler')
    conf.load('nux_compiler')
    conf.load('objcopy')
    conf.load('test_base')
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
        conf.env.append_value('LINKFLAGS', '-Wl,--defsym=__mailbox__=1')
    # restore env
    conf.setenv('', env=env)

def build(bld):
    bld(
        target = 'nux_inc',
        export_includes = ['.'],
        env = bld.all_envs['nux'],
    )

    bld.stlib(
        target = 'nux',
        source = [
                'src/bitformatting.cpp',
                'src/exp.cpp',
                'src/fxv.cpp',
                'src/mailbox.cpp',
                'src/unittest.cpp',
                'src/unittest_mailbox.cpp',
                'src/stack_guards.cpp',
                'src/spikes.cpp',
                'src/random.cpp',
                'src/time.cpp',
                'src/counter.cpp',
                'src/correlation.cpp',
                ],
        use = ['nux_inc'],
        env = bld.all_envs['nux'],
    )

    bld(
        features = 'cxx',
        name = 'start_obj',
        source = 'src/start.cpp',
        env = bld.all_envs['nux'],
    )

    bld(
        features = 'cxx',
        name = 'initdeinit_obj',
        source = 'src/initdeinit.cpp',
        env = bld.all_envs['nux'],
    )

    bld(
        features = 'cxx',
        name = 'cxa_pure_virtual_obj',
        source = 'src/cxa_pure_virtual.cpp',
        use = 'nux_inc',
        env = bld.all_envs['nux'],
    )

    bld(
        name = 'nux_runtime',
        target = 'crt.o',
        source = ['src/crt.s'],
        features = 'use asm',
        use = ['initdeinit_obj', 'cxa_pure_virtual_obj', 'start_obj'],
        env = bld.all_envs['nux'],
    )
    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector_sync.bin',
        source = ['test/test_vector_sync.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_returncode.bin',
        source = ['test/test_returncode.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector_alignment.bin',
        source = ['test/test_vector_alignment.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_neuron_counter.bin',
        source = ['test/test_neuron_counter.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_unittest.bin',
        source = ['test/test_unittest.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector.bin',
        source = ['test/test_vector.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector_cc.bin',
        source = ['test/test_vector_cc.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_noinline_vector_argument.bin',
        source = ['test/test_noinline_vector_argument.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
        cxxflags = ['-O2'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_inline_vector_argument.bin',
        source = ['test/test_inline_vector_argument.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
        cxxflags = ['-O2'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = "test_fxvsel.bin",
        source = ["test/test_fxvsel.cpp"],
        use = ["nux", "nux_runtime"],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features='cxx objcopy',
        objcopy_bfdname='binary',
        target="test_synram_rw.bin",
        source="test/test_synram_rw.cpp",
        use=["nux", "nux_runtime", "random"],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = "test_many_vectors.bin",
        source = "test/test_many_vectors.cpp",
        use = ["nux", "nux_runtime"],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = "test_bitformatting.bin",
        source = "test/test_bitformatting.cpp",
        use = ["nux", "nux_runtime"],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_helper.bin',
        source = ['test/test_helper.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_stack_redzone.bin',
        source = ['test/test_stack_redzone.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_stack_guard.bin',
        source = ['test/test_stack_guard.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_malloc.bin',
        source = ['test/test_malloc.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    def max_size_empty():
        stack_protector = bld.all_envs['nux'].LIBNUX_STACK_PROTECTOR_ENABLED[0].lower() == "true"
        stack_redzone = bld.all_envs['nux'].LIBNUX_STACK_REDZONE_ENABLED[0].lower() == "true"
        build_profile = bld.options.build_profile

        if not stack_protector and not stack_redzone:
            if build_profile == 'release':
                return 400
            else:
                return 512

        if stack_protector and not stack_redzone:
            if build_profile == 'release':
                return 784
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
                return 976

    bld.program(
        features = 'cxx objcopy check_size',
        check_size_max = max_size_empty(),
        objcopy_bfdname = 'binary',
        target = 'test_empty.bin',
        source = ['test/test_empty.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_xorshift_vector.bin',
        source = ['test/test_xorshift_vector.cpp'],
        use = ['random','nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_return_vector.bin',
        source = ['test/test_return_vector.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'example_stdp.bin',
        source = ['examples/stdp.cpp'],
        use = ['nux', 'nux_runtime', 'correlation'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_bool.bin',
        source = ['test/test_bool.cpp'],
        use = ['random','nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_measure_time.bin',
        source = ['test/test_measure_time.cpp'],
        use = ['nux', 'nux_runtime', 'time'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_fxvadd.bin',
        source = ['test/test_fxvadd.cpp'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld(
        name='libnux_hwtests',
        tests='test/test_hwtests.py',
        features='use pytest',
        use='hdls-scripts_runprogram',
        install_path='${PREFIX}/bin/tests',
        skip_run=False,
        env = bld.all_envs[''],
        test_environ = dict(STACK_PROTECTION=bld.all_envs['nux'].LIBNUX_STACK_PROTECTOR_ENABLED[0],
                            STACK_REDZONE=bld.all_envs['nux'].LIBNUX_STACK_REDZONE_ENABLED[0],
                            TEST_BINARY_PATH=os.path.join(bld.env.PREFIX, 'build', 'libnux'))
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
