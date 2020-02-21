import json
import os
from os.path import join
import unittest

from dlens_vx.halco import iter_all, PPUOnDLS, SynapseRowOnSynram, \
    SynapseOnSynapseRow
from dlens_vx.sta import PlaybackProgramExecutor, generate, DigitalInit
from dlens_vx import logger
from dlens_vx.tools.run_ppu_program import load_and_start_program, \
    wait_until_ppu_finished, stop_program
from pyhaldls_vx import SynapseQuad

_THIS_DIR = os.path.dirname(os.path.realpath(__file__))
TEST_BINARY_PATH = os.environ.get("TEST_BINARY_PATH",
                                  join(_THIS_DIR,
                                       os.pardir,
                                       os.pardir,
                                       os.pardir,
                                       "build",
                                       "libnux",
                                       "test",
                                       "with_hostcode")
                                  )

FLANGE_LOG_PATH = os.environ.get("FLANGE_LOG_PATH",
                                 join(_THIS_DIR,
                                      os.pardir,
                                      os.pardir,
                                      os.pardir,
                                      "hxfpga",
                                      "units",
                                      "synplify_wrapper",
                                      "sim",
                                      "xrun.log")
                                 )


class LibnuxAccessPatternTestVx(unittest.TestCase):
    EXECUTOR = PlaybackProgramExecutor()

    class LogParser:
        LOG_PREFIX = "HX_SIMULATION_LOG: "

        def __init__(self, pattern: str, offset_bytes: int):
            self.pattern = pattern
            self.offset_bytes = offset_bytes
            self.logfile = open(FLANGE_LOG_PATH, "rt")

        def __iter__(self):
            self.logfile.seek(self.offset_bytes)
            return self

        def __next__(self):
            for line in self.logfile:
                if line.startswith(self.LOG_PREFIX):
                    event = json.loads(line[len(self.LOG_PREFIX):])
                    if self.pattern in event:
                        return event[self.pattern]

            raise StopIteration

        def __del__(self):
            self.logfile.close()

    @classmethod
    def setUpClass(cls) -> None:
        # Connect to some executor (sim or hardware)
        cls.EXECUTOR.connect()

        # Initialize the chip
        init_builder, _ = generate(DigitalInit())
        cls.EXECUTOR.run(init_builder.done())

    @classmethod
    def tearDownClass(cls) -> None:
        # Disconnect the executor
        cls.EXECUTOR.disconnect()

    def run_ppu_program(self, ppu: PPUOnDLS, program_path: str, timeout: int):
        load_and_start_program(self.EXECUTOR, program_path, ppu)
        wait_until_ppu_finished(self.EXECUTOR, timeout=timeout, ppu=ppu)
        ret_code = stop_program(self.EXECUTOR, ppu=ppu,
                                print_mailbox=False)

        self.assertEqual(0, ret_code,
                         f"PPU exit code was {ret_code}, expected 0.")

    def test_refgen_access(self):
        log = logger.get("LibnuxAccessPatternTestVx.test_refgen_access")
        program = join(TEST_BINARY_PATH, "refgen_access_pattern-ppu.bin")
        initial_logsize = os.path.getsize(FLANGE_LOG_PATH)
        log.debug("Initial size of %s: %dbytes." % (FLANGE_LOG_PATH,
                                                    initial_logsize))

        for ppu in iter_all(PPUOnDLS):
            log.info("Running test on %s." % ppu)
            self.run_ppu_program(ppu, program, int(5e5))

        # Evaluate flange log
        log_parser = self.LogParser("refgen_config_write", initial_logsize)
        log_events = iter(log_parser)

        initial_run = True
        for _ in iter_all(PPUOnDLS):
            # Reset by the first PPU has no effect, all values are already zero
            if not initial_run:
                self.assertDictEqual(next(log_events)["rg_i_amp_c"],
                                     {'0': 0, '1': 0, '2': 15, '3': 20})
                self.assertDictEqual(next(log_events)["rg_i_amp_c"],
                                     {'0': 0, '1': 0, '2': 0, '3': 0})
                self.assertDictEqual(next(log_events)["rg_i_offset_c"],
                                     {'0': 0, '1': 0, '2': 35, '3': 40})
                self.assertDictEqual(next(log_events)["rg_i_offset_c"],
                                     {'0': 0, '1': 0, '2': 0, '3': 0})
                self.assertDictEqual(next(log_events)["rg_i_slope_c"],
                                     {'0': 0, '1': 0, '2': 55, '3': 60})
                self.assertDictEqual(next(log_events)["rg_i_slope_c"],
                                     {'0': 0, '1': 0, '2': 0, '3': 0})

            initial_run = False
            self.assertDictEqual(next(log_events)["rg_i_amp_c"],
                                 {'0': 5, '1': 10, '2': 0, '3': 0})
            self.assertDictEqual(next(log_events)["rg_i_amp_c"],
                                 {'0': 5, '1': 10, '2': 15, '3': 20})
            self.assertDictEqual(next(log_events)["rg_i_offset_c"],
                                 {'0': 25, '1': 30, '2': 0, '3': 0})
            self.assertDictEqual(next(log_events)["rg_i_offset_c"],
                                 {'0': 25, '1': 30, '2': 35, '3': 40})
            self.assertDictEqual(next(log_events)["rg_i_slope_c"],
                                 {'0': 45, '1': 50, '2': 0, '3': 0})
            self.assertDictEqual(next(log_events)["rg_i_slope_c"],
                                 {'0': 45, '1': 50, '2': 55, '3': 60})

        with self.assertRaises(StopIteration):
            next(log_events)

    def test_synram_access(self):
        log = logger.get("LibnuxAccessPatternTestVx.test_synram_access")
        program = join(TEST_BINARY_PATH, "synram_access_pattern-ppu.bin")
        initial_logsize = os.path.getsize(FLANGE_LOG_PATH)
        log.debug("Initial size of %s: %dbytes." % (FLANGE_LOG_PATH,
                                                    initial_logsize))

        for ppu in iter_all(PPUOnDLS):
            log.info("Running test on %s." % ppu)
            self.run_ppu_program(ppu, program, int(5e7))

        # Evaluate flange log
        log_parser = self.LogParser("synram_write_access", initial_logsize)
        log_events = iter(log_parser)

        num_vectors_per_row = 2
        vector_size = SynapseOnSynapseRow.size // num_vectors_per_row

        for ppu in iter_all(PPUOnDLS):
            diagonal_value = 1
            for row in range(SynapseRowOnSynram.size):
                for vec in range(num_vectors_per_row):
                    for vector_element in range(vector_size):
                        col = 2 * vector_element + vec

                        self.assertDictEqual(
                            next(log_events),
                            {
                                "data":
                                    0 if row != col else
                                    self.permute_weights(diagonal_value),
                                "column":
                                    col + SynapseOnSynapseRow.size
                                    * ppu.toEnum().value()
                            }
                        )

                diagonal_value += 1
                if diagonal_value > SynapseQuad.Synapse.Weight.max:
                    diagonal_value = 1

        with self.assertRaises(StopIteration):
            next(log_events)

    @staticmethod
    def permute_weights(before: int):
        after = 0
        after |= (bool(before & (1 << 5))) << 5
        after |= (bool(before & (1 << 0))) << 4
        after |= (bool(before & (1 << 1))) << 3
        after |= (bool(before & (1 << 2))) << 2
        after |= (bool(before & (1 << 3))) << 1
        after |= (bool(before & (1 << 4))) << 0
        return after


if __name__ == '__main__':
    unittest.main()
