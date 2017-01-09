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
        source = ['src/exp.c', 'src/fxv.c', 'src/mailbox.c'],
        use = ['nux_inc'],
    )

    bld(
        name = 'nux_runtime',
        target = 'crt.o',
        source = ['src/crt.s'],
        use = ['asm'],
    )
