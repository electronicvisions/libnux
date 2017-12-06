def options(opt):
    opt.load('nux_compiler')

def configure(conf):
    conf.load('nux_compiler')
    conf.load('objcopy')

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
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_unittest.bin',
        source = ['test/test_unittest.c'],
        use = ['nux', 'nux_runtime'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = 'test_vector.bin',
        source = ['test/test_vector.c'],
        use = ['nux', 'nux_runtime'],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = "test_fxvsel.bin",
        source = ["test/test_fxvsel.c"],
        use = ["nux", "nux_runtime"],
    )

    bld.program(
        features = 'c objcopy',
        objcopy_bfdname = 'binary',
        target = "test_synram_rw_v2.bin",
        source = "test/test_synram_rw_v2.c",
        use = ["nux", "nux_runtime"],
    )
