import unittest
from typing import Set

from dlens_vx.sta import PlaybackProgramExecutor, generate, InitGenerator
from run_ppu_program import load_and_start_program, stop_program, \
    wait_until_ppu_finished, PPUTimeoutError
from helpers.hwtest_common import get_special_binaries, find_binaries, \
    PpuHwTest


class LibnuxHwSimTestsVx(unittest.TestCase):
    TESTS: Set[PpuHwTest] = set()
    EXECUTOR = PlaybackProgramExecutor()

    @classmethod
    def setUpClass(cls) -> None:
        # Connect to some executor (sim or hardware)
        cls.EXECUTOR.connect()

        # Initialize the chip
        init_builder, _ = generate(InitGenerator())
        cls.EXECUTOR.run(init_builder.done())

    @classmethod
    def tearDownClass(cls) -> None:
        # Disconnect the executor
        cls.EXECUTOR.disconnect()

    @classmethod
    def generate_cases(cls):
        """
        Augment this class by test cases for all tests defined in `cls.TESTS`.
        """
        for test in cls.TESTS:
            def generate_test(ppu_test: PpuHwTest):
                def test_func(self: LibnuxHwSimTestsVx):
                    load_and_start_program(cls.EXECUTOR, ppu_test.path)
                    try:
                        wait_until_ppu_finished(cls.EXECUTOR, timeout=int(5e5))
                        self.assertFalse(ppu_test.expect_timeout,
                                         "Expected timeout did not happen.")
                    except PPUTimeoutError:
                        self.assertTrue(ppu_test.expect_timeout,
                                        "Unexpected timeout.")
                    finally:
                        returncode = stop_program(cls.EXECUTOR)

                    self.assertEqual(ppu_test.expected_exit_code, returncode,
                                     f"Exit code was {returncode}, expected "
                                     f"{ppu_test.expected_exit_code}.")

                return test_func

            test_method = generate_test(test)
            test_method.__name__ = test.name

            setattr(LibnuxHwSimTestsVx, test.name, test_method)


LibnuxHwSimTestsVx.TESTS.update(get_special_binaries("vx"))
LibnuxHwSimTestsVx.TESTS.update(find_binaries("vx"))
LibnuxHwSimTestsVx.generate_cases()

if __name__ == '__main__':
    unittest.main()
