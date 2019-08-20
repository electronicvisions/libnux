import unittest
import os
from os.path import join
import re
from run_ppu_program import run_program, PPUTimeoutError

# --- Parse the env --- #
THIS_DIR = os.path.dirname(os.path.realpath(__file__))
COMPILED_TESTS_PATH = os.environ.get(
    "TEST_BINARY_PATH", join(THIS_DIR, os.pardir, os.pardir, "build", "libnux"))
STACK_PROTECTION = os.environ.get("STACK_PROTECTION", "false").lower() == "true"
STACK_REDZONE = os.environ.get("STACK_REDZONE", "false").lower() == "true"
TEST_DLS_VERSION = os.environ.get("TEST_DLS_VERSION")

if not os.path.isdir(COMPILED_TESTS_PATH):
    raise RuntimeError("Libnux test binary folder (%s) does not exist!" %
                       COMPILED_TESTS_PATH)


class PpuHwTest(object):
    def __init__(self, binary_path, expected_exit_code=0, expect_timeout=False):
        if os.path.isabs(binary_path):
            self.path = os.path.realpath(binary_path)
        else:
            self.path = os.path.realpath(join(COMPILED_TESTS_PATH, binary_path))

        self.expected_exit_code = expected_exit_code
        self.expect_timeout = expect_timeout

    @property
    def name(self):
        test_name = os.path.splitext(os.path.basename(self.path))[0]
        if not test_name.startswith(unittest.TestLoader.testMethodPrefix):
            test_name = unittest.TestLoader.testMethodPrefix + "_" + test_name
        return test_name


class LibnuxHWTests(unittest.TestCase):
    """
    :cvar TESTS: List of all PPU hardware tests to be run
    :type TESTS: list[PpuHwTest]
    """

    # Add tests that need special handling (e.g. non-default exit code) here.
    # This list will be completed by whatever is found in an arbitrary search
    # path once `LibnuxHWTests.find_binaries()` is called.
    TESTS = [
        PpuHwTest("test_unittest_" + TEST_DLS_VERSION + ".binary", 1),
        PpuHwTest("test_returncode_" + TEST_DLS_VERSION + ".binary", -559038737),
        PpuHwTest("test_stack_guard_" + TEST_DLS_VERSION + ".binary", STACK_PROTECTION * -559038737),
        PpuHwTest("test_stack_redzone_" + TEST_DLS_VERSION + ".binary",
                  expected_exit_code=STACK_REDZONE * 12,
                  expect_timeout=not STACK_REDZONE)
    ]

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

            setattr(LibnuxHWTests, test.name, test_method)

    @classmethod
    def find_binaries(cls, search_path):
        """
        Add test-binaries matching `(?:\b|_)[Tt]est` to `cls.TESTS`.

        :param search_path: Path to search for binaries
        :type search_path: str
        """
        test_regex = re.compile(r'(?:\b|_)[Tt]est.*' + TEST_DLS_VERSION + '\.binary')
        for path, directories, files in os.walk(search_path):
            for f in files:
                if test_regex.search(f):
                    binary_path = os.path.realpath(join(path, f))
                    if binary_path in [t.path for t in cls.TESTS]:
                        continue

                    cls.TESTS.append(PpuHwTest(binary_path))


LibnuxHWTests.find_binaries(COMPILED_TESTS_PATH)
LibnuxHWTests.generate_cases()
