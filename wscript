import os
from os.path import join
import re
import yaml
from enum import Enum, auto

from waflib import Logs
from waflib.TaskGen import feature, after_method
from waflib.extras import test_base
from waflib.extras.test_base import summary
from waflib.extras.symwaf2ic import get_toplevel_path
from waflib.Errors import BuildError, ConfigurationError


def depends(dep):
    # we can't build the full stack in the wafer app
    if (os.environ.get("SINGULARITY_APPNAME") is None) or ("wafer" not in os.environ.get("SINGULARITY_APPNAME")):
        dep("libnux", "tests")

    dep("hate")


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
    hopts.add_option("--time64",
                   default=False,
                   action='store_true',
                   help="Use 64 bit time type (instead of 32 bit).")
    hopts.add_option("--time-shift",
                   default=0,
                   help="Bits to shift time resolution.")


def configure(conf):
    if getattr(conf.options, 'with_libnux_test_hostcode', True):
        # host-based python stuff also needed for cross-env tests
        conf.load('pytest')

    # now configure for nux cross compiler
    env = conf.env
    conf.setenv('nux')
    have_ppu_toolchain = True
    try:
        conf.load('nux_assembler')
        conf.load('nux_compiler')
    except ConfigurationError:
        have_ppu_toolchain = False
        Logs.warn("ppu-toolchain not found, disabling PPU support")
    conf.load('test_base')
    conf.env.append_value('LINKFLAGS', '-T%s' % conf.path.find_node('share/libnux/elf32nux.x').abspath())
    conf.env.append_value('ASLINKFLAGS', '-T%s' % conf.path.find_node('share/libnux/elf32nux.x').abspath())
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
    if(conf.options.time64):
        conf.define("LIBNUX_TIME64", 1)
    conf.define("LIBNUX_TIME_RESOLUTION_SHIFT", int(conf.options.time_shift))

    # specialize for vx
    conf.setenv('nux_vx', env=conf.all_envs['nux'])
    conf.env.append_value('CXXFLAGS', '-mcpu=s2pp_hx')

    # specialize for vx-v1
    conf.setenv('nux_vx_v1', env=conf.all_envs['nux_vx'])

    # specialize for vx-v2
    conf.setenv('nux_vx_v2', env=conf.all_envs['nux_vx'])

    # specialize for vx-v3
    conf.setenv('nux_vx_v3', env=conf.all_envs['nux_vx'])

    # restore env
    conf.setenv('', env=env)
    conf.env.have_ppu_toolchain = have_ppu_toolchain

def build(bld):
    if not bld.env.have_ppu_toolchain:
        return

    env = bld.all_envs["nux_vx"]

    bld(
        target = 'nux_inc_host_vx',
        export_includes = ['include']
    )

    bld(
        target = "nux_inc_vx",
        export_includes = ["include"],
        env = env,
    )

    bld.stlib(
        target = "nux_vx",
        source = bld.path.ant_glob("src/libnux/vx/*.cpp"),
        use = ["nux_inc_vx"],
        env = env,
    )

    bld.install_files(
        dest = '${PREFIX}/',
        files = bld.path.ant_glob('libnux/*.(h|tcc|hpp)')
                + bld.path.ant_glob('libnux/vx/*.(h|tcc|hpp)')
                + bld.path.ant_glob('libnux/scheduling/*.(h|tcc|hpp)'),
        name = f"nux_vx_header",
        relative_trick = True
    )

    bld.install_files(
        dest = '${PREFIX}/',
        files = bld.path.ant_glob('share/libnux/*.x'),
        name = f"nux_vx_linker_file",
        relative_trick = True
    )

    for chip_version_number in [1, 2, 3]:
        env = bld.all_envs[f"nux_vx_v{chip_version_number}"]

        bld.install_files(
            dest = '${PREFIX}/',
            files = bld.path.ant_glob(f'include/libnux/vx/v{chip_version_number}/**/*.(h|tcc|hpp)'),
            name = f"nux_vx_v{chip_version_number}_header",
            depends_on = ["nux_vx_header"],
            relative_trick = True
        )

        bld(
            target = f"nux_inc_vx_v{chip_version_number}",
            export_includes = ["include"],
            depends_on = [f"nux_vx_v{chip_version_number}_header",
                          f"nux_vx_linker_file"],
            env = env,
        )

        bld.stlib(
            target = f"nux_vx_v{chip_version_number}",
            source = bld.path.ant_glob("src/libnux/vx/*.cpp")
                     + bld.path.ant_glob(f"src/libnux/vx/v{chip_version_number}/*.cpp"),
            use = [f"nux_inc_vx_v{chip_version_number}", 'hate_inc'],
            install_path = "${PREFIX}/lib",
            env = env,
        )

        bld(
            features = "cxx",
            name = f"nux_runtime_obj_vx_v{chip_version_number}",
            source = ["src/nux_runtime/start.cpp",
                      "src/nux_runtime/initdeinit.cpp",
                      "src/nux_runtime/cxa_pure_virtual.cpp",
                      "src/nux_runtime/stack_guards.cpp"],
            use = f"nux_inc_vx_v{chip_version_number}",
            env = env,
        )

        bld(
            name = f"nux_runtime_shutdown_vx_v{chip_version_number}",
            target = "crt_shutdown.o",
            source = ["src/nux_runtime/crt_shutdown.s"],
            features = "use asm",
            env = env,
        )

        bld(
            name = f"nux_runtime_vx_v{chip_version_number}",
            target = "crt.o",
            source = ["src/nux_runtime/crt.s"],
            features = "use asm",
            use = [f"nux_runtime_obj_vx_v{chip_version_number}",
                   f"nux_runtime_shutdown_vx_v{chip_version_number}"],
            env=env,
        )

        bld.stlib(
            name = f"nux_runtime_vx_v{chip_version_number}.o",
            target = f"nux_runtime_vx_v{chip_version_number}.o",
            source = ["src/nux_runtime/crt.s"],
            features = "cxx",
            use = [f"nux_inc_vx_v{chip_version_number}",
                   f"nux_runtime_obj_vx_v{chip_version_number}",
                   f"nux_runtime_shutdown_vx_v{chip_version_number}"],
            install_path = "${PREFIX}/lib",
            env=env,
        )

        program_list = ["examples/stdp.cpp"]

        for program in program_list:
            bld.program(
                features = "cxx",
                target = f"{program.replace('.cpp', '')}_vx_v{chip_version_number}.bin",
                source = [program],
                use = [f"nux_vx_v{chip_version_number}",
                       f"nux_runtime_vx_v{chip_version_number}"],
                env = bld.all_envs[f"nux_vx_v{chip_version_number}"],
            )

    bld.add_post_fun(summary)
