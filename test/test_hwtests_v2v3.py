import unittest
import os
from typing import Set
try:
    from dlens_v2.tools.run_ppu_program import run_program, PPUTimeoutError
except ImportError:  # No dlens available => DLSv3 run_program
    from run_ppu_program import run_program, PPUTimeoutError

from helpers.hwtest_common import get_special_binaries, find_binaries, \
    PpuHwTest

# --- Parse the env --- #
TEST_DLS_VERSION = os.environ.get("TEST_DLS_VERSION")


class LibnuxHWTestsV2V3(unittest.TestCase):
    TESTS: Set[PpuHwTest] = set()

    @classmethod
    def generate_cases(cls):
        """
        Augment this class by test cases for all tests defined in `cls.TESTS`.
        """
        for test in cls.TESTS:
            def generate_test(ppu_test):
                def test_func(self):
                    try:
                        retcode = run_program(ppu_test.path, as_string=True)
                        self.assertEqual(ppu_test.expected_exit_code, retcode,
                                         "Exit code was %d, expected %d" %
                                         (retcode, ppu_test.expected_exit_code))

                        # TODO (#3038): Assert ppu_test.expect_timeout == False
                    except PPUTimeoutError as e:
                        if ppu_test.expect_timeout:
                            print("Expected timeout happened: %s" % e)
                        else:
                            raise e

                return test_func

            test_method = generate_test(test)
            test_method.__name__ = test.name

            setattr(LibnuxHWTestsV2V3, test.name, test_method)


LibnuxHWTestsV2V3.TESTS.update(get_special_binaries(TEST_DLS_VERSION))
LibnuxHWTestsV2V3.TESTS.update(find_binaries(TEST_DLS_VERSION))
LibnuxHWTestsV2V3.generate_cases()

if __name__ == '__main__':
    unittest.main()
