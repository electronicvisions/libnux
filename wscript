import os

from waflib.TaskGen import feature, after_method
from waflib.extras import test_base
from waflib.extras.test_base import summary


def options(opt):
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
                'src/exp.c',
                'src/fxv.c',
                'src/mailbox.c',
                'src/unittest.c',
                'src/unittest_mailbox.c',
                'src/stack_guards.c',
                ],
        use = ['nux_inc'],
        env = bld.all_envs['nux'],
    )

    bld(
        features = 'c',
        name = 'initdeinit_obj',
        source = 'src/initdeinit.c',
        env = bld.all_envs['nux'],
    )

    bld(
        features = 'c',
        name = 'cxa_pure_virtual_obj',
        source = 'src/cxa_pure_virtual.c',
        use = 'nux_inc',
        env = bld.all_envs['nux'],
    )

    bld(
        name = 'nux_runtime',
        target = 'crt.o',
        source = ['src/crt.s'],
        use = ['asm', 'initdeinit_obj'],
        env = bld.all_envs['nux'],
    )

    bld(
        name = 'nux_runtime_cpp',
        target = 'crt.o',
        source = ['src/crt.s'],
        use = ['asm', 'initdeinit_obj', 'cxa_pure_virtual_obj'],
        env = bld.all_envs['nux'],
    )

    bld.objects(
        features='c',
        target='spikes',
        name='spikes',
        source=['libnux/spikes.c'],
        use=['nux'],
        env = bld.all_envs['nux'],
    )

    bld.objects(
        features='c',
        target='random',
        name='random',
        source=['libnux/random.c'],
        use=['nux'],
        env = bld.all_envs['nux'],
    )

    bld.objects(
        features='c',
        target='time',
        name='time',
        source=['libnux/time.c'],
        use=['nux'],
        env = bld.all_envs['nux'],
    )

    bld.objects(
        features='c',
        target='counter',
        name='counter',
        source=['libnux/counter.c'],
        use=['nux'],
        env = bld.all_envs['nux'],
    )

    bld.objects(
        features='c',
        target='correlation',
        name='correlation',
        source=['libnux/correlation.c'],
        use=['nux'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector_sync.bin',
        source = ['test/test_vector_sync.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_returncode.bin',
        source = ['test/test_returncode.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector_alignment.bin',
        source = ['test/test_vector_alignment.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_neuron_counter.bin',
        source = ['test/test_neuron_counter.c'],
        use = ['nux', 'nux_runtime', 'counter'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_unittest.bin',
        source = ['test/test_unittest.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector.bin',
        source = ['test/test_vector.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector_cc.bin',
        source = ['test/test_vector_cc.cc'],
        use = ['nux', 'nux_runtime_cpp'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_noinline_vector_argument.bin',
        source = ['test/test_noinline_vector_argument.cc'],
        use = ['nux', 'nux_runtime_cpp'],
        env = bld.all_envs['nux'],
        cxxflags = ['-O2'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_inline_vector_argument.bin',
        source = ['test/test_inline_vector_argument.cc'],
        use = ['nux', 'nux_runtime_cpp'],
        env = bld.all_envs['nux'],
        cxxflags = ['-O2'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = "test_fxvsel.bin",
        source = ["test/test_fxvsel.c"],
        use = ["nux", "nux_runtime"],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features='c objcopy',
        objcopy_bfdname='binary',
        target="test_synram_rw.bin",
        source="test/test_synram_rw.c",
        use=["nux", "nux_runtime", "random"],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = "test_many_vectors.bin",
        source = "test/test_many_vectors.c",
        use = ["nux", "nux_runtime"],
        env = bld.all_envs['nux'],
    )

    def max_size_empty():
        stack_protector = bld.all_envs['nux'].LIBNUX_STACK_PROTECTOR_ENABLED[0].lower() == "true"
        stack_redzone = bld.all_envs['nux'].LIBNUX_STACK_REDZONE_ENABLED[0].lower() == "true"
        build_profile = bld.options.build_profile

        if not stack_protector and not stack_redzone:
            if build_profile == 'release':
                return 368
            else:
                return 352

        if stack_protector and not stack_redzone:
            if build_profile == 'release':
                return 656
            else:
                return 624

        if not stack_protector and stack_redzone:
            return 416

        if stack_protector and stack_redzone:
            return 752

    bld.program(
        features = 'c objcopy check_size',
        check_size_max = max_size_empty(),
        objcopy_bfdname = 'binary',
        target = 'test_empty.bin',
        source = ['test/test_empty.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_stack_redzone.bin',
        source = ['test/test_stack_redzone.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_stack_guard.bin',
        source = ['test/test_stack_guard.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_malloc.bin',
        source = ['test/test_malloc.c'],
        use = ['nux', 'nux_runtime_cpp'],
        env = bld.all_envs['nux'],
    )

    def max_size_empty_cc():
        stack_protector = bld.all_envs['nux'].LIBNUX_STACK_PROTECTOR_ENABLED[0].lower() == "true"
        stack_redzone = bld.all_envs['nux'].LIBNUX_STACK_REDZONE_ENABLED[0].lower() == "true"

        if not stack_protector and not stack_redzone:
            return 416

        if stack_protector and not stack_redzone:
            return 720

        if not stack_protector and stack_redzone:
            return 480

        if stack_protector and stack_redzone:
            return 848

    bld.program(
        features = 'cxx objcopy check_size',
        check_size_max = max_size_empty_cc(),
        objcopy_bfdname = 'binary',
        target = 'test_empty_cc.bin',
        source = ['test/test_empty_cc.cc'],
        use = ['nux', 'nux_runtime_cpp'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_xorshift_vector.bin',
        source = ['test/test_xorshift_vector.c'],
        use = ['random','nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_return_vector.bin',
        source = ['test/test_return_vector.c'],
        use = ['nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'example_stdp.bin',
        source = ['examples/stdp.c'],
        use = ['nux', 'nux_runtime', 'correlation'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_bool.bin',
        source = ['test/test_bool.c'],
        use = ['random','nux', 'nux_runtime'],
        env = bld.all_envs['nux'],
    )

    bld.program(
        features = 'cxx objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_measure_time.bin',
        source = ['test/test_measure_time.cc'],
        use = ['nux', 'nux_runtime_cpp', 'time'],
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
