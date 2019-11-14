from __future__ import annotations

import os
from os.path import join
import unittest
import re

from typing import Set

_THIS_DIR = os.path.dirname(os.path.realpath(__file__))
TEST_BINARY_PATH = os.environ.get("TEST_BINARY_PATH",
                                  join(_THIS_DIR,
                                       os.pardir,
                                       os.pardir,
                                       os.pardir,
                                       "build",
                                       "libnux")
                                  )

if not os.path.isdir(TEST_BINARY_PATH):
    raise RuntimeError(f"Libnux test binary folder ({TEST_BINARY_PATH}) "
                       f"does not exist!")


class PpuHwTest(object):
    """
    Encapsulation for a PPU hardware test. Tests are single programs to be run
    on hardware.
    """

    def __init__(self, binary_path: str,
                 expected_exit_code: int = 0,
                 expect_timeout: bool = False):
        """
        :param binary_path: Absolute path to the test binary or its name
        :param expected_exit_code: Exit code expected for the binary
        :param expect_timeout: Is the test supposed to time out?
        """
        if not os.path.isabs(binary_path):
            raise RuntimeError("Test binary path is not absolute")

        self.path = os.path.realpath(binary_path)
        self.expected_exit_code = expected_exit_code
        self.expect_timeout = expect_timeout

    @property
    def name(self):
        """
        Name of the unit test to be created for the respective binary.
        """
        test_name = os.path.splitext(os.path.basename(self.path))[0]
        if not test_name.startswith(unittest.TestLoader.testMethodPrefix):
            test_name = unittest.TestLoader.testMethodPrefix + "_" + test_name
        return test_name

    def __eq__(self, other: PpuHwTest) -> bool:
        return self.path == other.path

    def __hash__(self) -> int:
        return hash(self.path)


def find_binaries(dls_version: str) -> Set[PpuHwTest]:
    """
    Find test-binaries matching `(?:\b|_)[Tt]est`.

    :param dls_version: DLS version the list is compiled for.
    """
    ret = set()

    test_regex = re.compile(r'(?:\b|_)[Tt]est.*' + dls_version + r'\.binary')
    for path, directories, files in os.walk(TEST_BINARY_PATH):
        for f in files:
            if test_regex.search(f):
                binary_path = os.path.realpath(join(path, f))
                ret.add(PpuHwTest(binary_path))

    return ret


def get_special_binaries(dls_version: str) -> Set[PpuHwTest]:
    """
    Compile a set of "special" hardware tests. Tests are special, if the
    default constructor of :class:`PpuHwTest` does not apply.

    :param dls_version: DLS version the set is compiled for.
    :return: Set of special tests
    """
    stack_protection = os.environ.get("STACK_PROTECTION", "").lower() == "true"
    stack_redzone = os.environ.get("STACK_REDZONE", "").lower() == "true"

    return {
        PpuHwTest(
            join(TEST_BINARY_PATH, f"test_unittest_{dls_version}.binary"),
            expected_exit_code=1),
        PpuHwTest(
            join(TEST_BINARY_PATH, f"test_returncode_{dls_version}.binary"),
            expected_exit_code=-559038737),
        PpuHwTest(
            join(TEST_BINARY_PATH, f"test_stack_guard_{dls_version}.binary"),
            expected_exit_code=stack_protection * -559038737),
        PpuHwTest(
            join(TEST_BINARY_PATH, f"test_stack_redzone_{dls_version}.binary"),
            expected_exit_code=stack_redzone * 12,
            expect_timeout=not stack_redzone)
    }
