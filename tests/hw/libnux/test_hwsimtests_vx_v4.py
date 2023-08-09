import random
import unittest
from numbers import Integral
from typing import Set, ClassVar

from dlens_vx_v4.hal import CADCConfig
from dlens_vx_v4.hxcomm import ManagedConnection
from dlens_vx_v4.sta import generate, DigitalInit, run
from dlens_vx_v4.halco import PPUOnDLS, iter_all, JTAGIdCodeOnDLS, TimerOnDLS,\
    CADCConfigOnDLS
from dlens_vx_v4.tools.run_ppu_program import load_and_start_program, \
    stop_program, wait_until_ppu_finished, PPUTimeoutError
from dlens_vx_v4 import logger
from pyhaldls_vx_v4 import Timer
from libnux_test_helpers.hwtest_common import get_special_binaries, \
    find_binaries, PpuHwTest


class LibnuxHwSimTestsVx(unittest.TestCase):
    TESTS: Set[PpuHwTest] = set()
    MANAGED_CONNECTION = ManagedConnection()
    CONNECTION = None
    CHIP_REVISION: ClassVar[Integral]

    @classmethod
    def setUpClass(cls) -> None:
        # Initialize remote connection (to sim or hardware)
        cls.CONNECTION = cls.MANAGED_CONNECTION.__enter__()

        # Initialize the chip and find chip version
        init_builder, _ = generate(DigitalInit())
        jtag_id_ticket = init_builder.read(JTAGIdCodeOnDLS())
        init_builder.write(TimerOnDLS(), Timer(0))
        init_builder.block_until(TimerOnDLS(), Timer.Value(1000))
        for cadc in iter_all(CADCConfigOnDLS):
            cadc_config = CADCConfig()
            cadc_config.enable = True
            init_builder.write(cadc, cadc_config)

        run(cls.CONNECTION, init_builder.done())
        jtag_id = jtag_id_ticket.get()
        cls.CHIP_REVISION = jtag_id.version

    @classmethod
    def tearDownClass(cls) -> None:
        # Disconnect the executor
        cls.MANAGED_CONNECTION.__exit__()

    @classmethod
    def generate_cases(cls):
        """
        Augment this class by test cases for all tests defined in `cls.TESTS`.
        """
        for test in cls.TESTS:
            def generate_test(ppu_test: PpuHwTest):
                def test_func(self: LibnuxHwSimTestsVx):
                    if not ppu_test.supports_revision(self.CHIP_REVISION):
                        self.skipTest("Incompatible chip rev. "
                                      f"{self.CHIP_REVISION}")

                    log = logger.get(f"LibnuxHwSimTestsVx.{ppu_test.name}")
                    ppu = random.choice(list(iter_all(PPUOnDLS)))
                    log.info(f"Running test on {ppu}.")

                    load_and_start_program(cls.CONNECTION, ppu_test.path, ppu)
                    try:
                        wait_until_ppu_finished(cls.CONNECTION,
                                                timeout=ppu_test.timeout,
                                                ppu=ppu)
                        self.assertFalse(ppu_test.expect_timeout,
                                         "Expected timeout did not happen.")
                    except PPUTimeoutError:
                        self.assertTrue(ppu_test.expect_timeout,
                                        "Unexpected timeout.")
                    finally:
                        returncode = stop_program(cls.CONNECTION, ppu=ppu)

                    self.assertEqual(ppu_test.expected_exit_code, returncode,
                                     f"Exit code was {returncode}, expected "
                                     f"{ppu_test.expected_exit_code}.")

                return test_func

            test_method = generate_test(test)
            test_method.__name__ = test.name

            setattr(LibnuxHwSimTestsVx, test.name, test_method)


LibnuxHwSimTestsVx.TESTS.update(
    get_special_binaries("vx", "v4").intersection(find_binaries("vx", "v4")))
LibnuxHwSimTestsVx.TESTS.update(find_binaries("vx", "v4"))
LibnuxHwSimTestsVx.generate_cases()

if __name__ == '__main__':
    unittest.main()
