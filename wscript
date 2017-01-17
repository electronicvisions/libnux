from waflib import TaskGen
TaskGen.declare_chain(
    name         = 'strip',
    rule         = 'powerpc-eabi-objcopy -O binary ${SRC} ${TGT}',
    ext_in       = '.bin',
    ext_out      = '.raw',
    install_path = 'bin',
)


def options(opt):
    opt.load('nux_compiler')

def configure(conf):
    conf.load('nux_compiler')

def build(bld):
    bld(
        target = 'nux_inc',
        export_includes = ['.'],
    )

    bld.stlib(
        target = 'nux',
        source = [
                'src/exp.c',
                'src/fxv.c',
                'src/mailbox.c',
                'src/unittest.c',
                'src/unittest_mailbox.c',
                ],
        use = ['nux_inc'],
    )

    bld(
        name = 'nux_runtime',
        target = 'crt.o',
        source = ['src/crt.s'],
        use = ['asm'],
    )

    bld.program(
        target = 'test_unittest.bin',
        source = ['test/test_unittest.c'],
        use = ['nux', 'nux_runtime'],
        install_path = None,
    )

    bld.program(
        target = 'test_vector.bin',
        source = ['test/test_vector.c'],
        use = ['nux', 'nux_runtime'],
        install_path = None,
    )

    bld.program(
        target = "test_fxvsel.bin",
        source = ["test/test_fxvsel.c"],
        use = ["nux", "nux_runtime"],
        install_path = None,
    )

    bld.program(
        target = "test_synram_rw_v2.bin",
        source = "test/test_synram_rw_v2.c",
        use = ["nux", "nux_runtime"],
        install_path = None,
    )

    bld(
        source=[
                "test_unittest.bin",
                "test_vector.bin",
                "test_fxvsel.bin",
                "test_synram_rw_v2.bin"
                ]
    )
