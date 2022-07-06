from __future__ import annotations

import os
from numbers import Integral
from os.path import join
import unittest
import re
from typing import Set, Callable

_THIS_DIR = os.path.dirname(os.path.realpath(__file__))
TEST_BINARY_PATH = os.environ.get("TEST_BINARY_PATH",
                                  join(_THIS_DIR,
                                       os.pardir,
                                       os.pardir,
                                       os.pardir,
                                       "build",
                                       "libnux",
                                       "tests",
                                       "hw",
                                       "libnux")
                                  )

if not os.path.isdir(TEST_BINARY_PATH):
    raise RuntimeError(f"Libnux test binary folder ({TEST_BINARY_PATH}) "
                       f"does not exist!")


class PpuHwTest:
    """
    Encapsulation for a PPU hardware test. Tests are single programs to be run
    on hardware.
    """
    # pylint: disable=too-many-arguments
    def __init__(self, binary_path: str,
                 expected_exit_code: int = 0,
                 expect_timeout: bool = False,
                 timeout: int = int(5e5),
                 supports_revision: Callable[[Integral],
                                             bool] = lambda _: True):
        """
        :param binary_path: Absolute path to the test binary or its name
        :param expected_exit_code: Exit code expected for the binary
        :param expect_timeout: Is the test supposed to time out?
        :param timeout: Maximum execution time until a timeout is raised
        :param supports_revision: Evaluate whether this test supports
                                  evaluation on a given chip revision
        """
        if not os.path.isabs(binary_path):
            raise RuntimeError("Test binary path is not absolute")

        self.path = os.path.realpath(binary_path)
        self.expected_exit_code = expected_exit_code
        self.expect_timeout = expect_timeout
        self.timeout = timeout
        self.supports_revision = supports_revision

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


def find_binaries(chip_revision: str, chip_version: str) -> Set[PpuHwTest]:
    """
    Find test-binaries matching `(?:\b|_)[Tt]est`.

    :param dls_version: DLS version the list is compiled for.
    """
    ret = set()

    test_regex = re.compile(r"(?:\b|_)[Tt]est.*" + chip_revision + r"_"
                            + chip_version + r"\.bin$")
    for path, _, files in os.walk(TEST_BINARY_PATH):
        for value in files:
            if test_regex.search(value):
                binary_path = os.path.realpath(join(path, value))
                ret.add(PpuHwTest(binary_path))

    return ret


def get_special_binaries(chip_revision: str, chip_version: str) \
        -> Set[PpuHwTest]:
    """
    Compile a set of "special" hardware tests. Tests are special, if the
    default constructor of :class:`PpuHwTest` does not apply.

    :param dls_version: DLS version the set is compiled for.
    :return: Set of special tests
    """
    stack_protection = os.environ.get("STACK_PROTECTION", "").lower() == "true"
    stack_redzone = os.environ.get("STACK_REDZONE", "").lower() == "true"

    test_list = {
        PpuHwTest(
            join(TEST_BINARY_PATH, chip_revision,
                 f"test_unittest_{chip_revision}_{chip_version}.bin"),
            expected_exit_code=1),
        PpuHwTest(
            join(TEST_BINARY_PATH, chip_revision,
                 f"test_returncode_{chip_revision}_{chip_version}.bin"),
            expected_exit_code=-559038737),
        PpuHwTest(
            join(TEST_BINARY_PATH, chip_revision,
                 f"test_stack_guard_{chip_revision}_{chip_version}.bin"),
            expected_exit_code=stack_protection * -559038737),
        PpuHwTest(
            join(TEST_BINARY_PATH, chip_revision,
                 f"test_stack_redzone_{chip_revision}_{chip_version}.bin"),
            expected_exit_code=12 if stack_redzone else 2),
        PpuHwTest(
            join(TEST_BINARY_PATH, chip_revision,
                 f"test_frac_{chip_revision}_{chip_version}.bin"),
            timeout=int(1e6))
    }

    simulation = os.environ.get("FLANGE_SIMULATION_RCF_PORT")
    test_list.update({
        PpuHwTest(
            join(TEST_BINARY_PATH, chip_revision,
                 f"test_cadc_static_{chip_revision}_{chip_version}.bin"),
            expected_exit_code=0 if simulation is not None else 1,
            supports_revision=lambda rev: rev >= 2),
        PpuHwTest(
            join(TEST_BINARY_PATH, chip_revision,
                 f"test_synram_{chip_revision}_{chip_version}.bin"),
            timeout=int(2e6),
            supports_revision=lambda rev: rev >= 2),
    })

    return test_list
