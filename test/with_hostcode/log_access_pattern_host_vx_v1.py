import json
import os
from os.path import join
import unittest
from typing import List, Iterator, ClassVar

from dlens_vx_v1.halco import iter_all, PPUOnDLS, SynapseRowOnSynram, \
    SynapseOnSynapseRow, JTAGIdCodeOnDLS, BarrierOnFPGA
from dlens_vx_v1.hxcomm import ManagedConnection
from dlens_vx_v1.sta import generate, DigitalInit, run
from dlens_vx_v1 import logger
from dlens_vx_v1.tools.run_ppu_program import load_and_start_program, \
    wait_until_ppu_finished, stop_program
from pyhaldls_vx_v1 import SynapseQuad, Barrier

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
    MANAGED_CONNECTION = ManagedConnection()
    CONNECTION = None
    CHIP_REVISION: ClassVar[int]

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
        cls.CONNECTION = cls.MANAGED_CONNECTION.__enter__()

        # Initialize the chip and find chip version
        init_builder, _ = generate(DigitalInit())
        jtag_id_ticket = init_builder.read(JTAGIdCodeOnDLS())
        init_builder.block_until(BarrierOnFPGA(), Barrier.jtag)
        run(cls.CONNECTION, init_builder.done())
        jtag_id = jtag_id_ticket.get()
        cls.CHIP_REVISION = jtag_id.version

    @classmethod
    def tearDownClass(cls) -> None:
        # Disconnect the executor
        cls.MANAGED_CONNECTION.__exit__()

    def run_ppu_program(self, ppu: PPUOnDLS, program_path: str, timeout: int):
        load_and_start_program(self.CONNECTION, program_path, ppu)
        wait_until_ppu_finished(self.CONNECTION, timeout=timeout, ppu=ppu)
        ret_code = stop_program(self.CONNECTION, ppu=ppu,
                                print_mailbox=False)

        self.assertEqual(0, ret_code,
                         f"PPU exit code was {ret_code}, expected 0.")

    def test_refgen_access(self):
        if self.CHIP_REVISION < 2:
            self.skipTest(f"Incompatible chip rev. {self.CHIP_REVISION} (<2).")

        log = logger.get("LibnuxAccessPatternTestVx.test_refgen_access")
        program = join(TEST_BINARY_PATH,
                       "refgen_access_pattern-ppu_vx_v1.bin")
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
        if self.CHIP_REVISION < 2:
            self.skipTest(f"Incompatible chip rev. {self.CHIP_REVISION} (<2).")

        log = logger.get("LibnuxAccessPatternTestVx.test_synram_access")
        program = join(TEST_BINARY_PATH,
                       "synram_access_pattern-ppu_vx_v1.bin")
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
                if diagonal_value > SynapseQuad.Weight.max:
                    diagonal_value = 1

        with self.assertRaises(StopIteration):
            next(log_events)

    def test_correlation_reset_causal(self):
        if self.CHIP_REVISION < 2:
            self.skipTest(f"Incompatible chip rev. {self.CHIP_REVISION} (<2).")

        log = logger.get("LibnuxAccessPatternTestVx."
                         "test_correlation_reset_causal")
        program = join(TEST_BINARY_PATH,
                       "correlation_reset_causal-ppu_vx_v1.bin")
        initial_logsize = os.path.getsize(FLANGE_LOG_PATH)
        log.debug("Initial size of %s: %dbytes." % (FLANGE_LOG_PATH,
                                                    initial_logsize))

        for ppu in iter_all(PPUOnDLS):
            log.info("Running test on %s." % ppu)
            self.run_ppu_program(ppu, program, int(5e5))

        # Evaluate flange log
        log_parser = self.LogParser("correlation_reset_event", initial_logsize)
        events = iter(log_parser)

        for ppu in iter_all(PPUOnDLS):
            # Test full reset
            self.eval_creset(events, ppu, 0, [True] * 256, [True] * 256)
            self.eval_creset(events, ppu, 10, [True] * 256, [True] * 256)
            self.eval_creset(events, ppu, 255, [True] * 256, [True] * 256)

            # Test causal-only reset
            self.eval_creset(events, ppu, 0, [True] * 256, [False] * 256)
            self.eval_creset(events, ppu, 20, [True] * 256, [False] * 256)
            self.eval_creset(events, ppu, 255, [True] * 256, [False] * 256)

            # Test acausal-only reset
            self.eval_creset(events, ppu, 0, [False] * 256, [True] * 256)
            self.eval_creset(events, ppu, 30, [False] * 256, [True] * 256)
            self.eval_creset(events, ppu, 255, [False] * 256, [True] * 256)

            # Test no reset at all
            self.eval_creset(events, ppu, 0, [False] * 256, [False] * 256)
            self.eval_creset(events, ppu, 40, [False] * 256, [False] * 256)
            self.eval_creset(events, ppu, 255, [False] * 256, [False] * 256)

            # Test specific odd column reset
            causal_mask = [False] * 256
            causal_mask[21] = True  # Column 21 is the 10th odd column
            self.eval_creset(events, ppu, 0, causal_mask, [False] * 256)
            self.eval_creset(events, ppu, 50, causal_mask, [False] * 256)
            self.eval_creset(events, ppu, 255, causal_mask, [False] * 256)

            # Test specific even column reset
            causal_mask = [False] * 256
            causal_mask[40] = True  # Column 40 is the 20th even column
            self.eval_creset(events, ppu, 0, causal_mask, [False] * 256)
            self.eval_creset(events, ppu, 60, causal_mask, [False] * 256)
            self.eval_creset(events, ppu, 255, causal_mask, [False] * 256)

    def test_neuron_reset_global(self):
        if self.CHIP_REVISION < 2:
            self.skipTest(f"Incompatible chip rev. {self.CHIP_REVISION} (<2).")

        log = logger.get("LibnuxAccessPatternTestVx.test_neuron_reset_global")
        program = join(TEST_BINARY_PATH,
                       "neuron_reset_global-ppu_vx_v1.bin")
        initial_logsize = os.path.getsize(FLANGE_LOG_PATH)
        log.debug("Initial size of %s: %dbytes." % (FLANGE_LOG_PATH,
                                                    initial_logsize))

        for ppu in iter_all(PPUOnDLS):
            log.info("Running test on %s." % ppu)
            self.run_ppu_program(ppu, program, int(5e5))

        # Evaluate flange log
        parser = self.LogParser("global_neuron_reset_event", initial_logsize)
        log_events = iter(parser)

        for ppu in iter_all(PPUOnDLS):
            # Test full reset
            self.eval_neuron_reset(log_events, ppu, [True] * 256)

            # Test no reset at all
            # no edge at sa_corres_a => no output in log

            # Test specific odd column reset
            reset_mask = [False] * 256
            reset_mask[21] = True  # Column 21 is the 10th odd column
            self.eval_neuron_reset(log_events, ppu, reset_mask)

            # Test specific even column reset
            reset_mask = [False] * 256
            reset_mask[40] = True  # Column 40 is the 20th even column
            self.eval_neuron_reset(log_events, ppu, reset_mask)

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

    # pylint: disable=too-many-arguments,too-many-locals
    def eval_creset(self,
                    events: Iterator[dict],
                    ppu: PPUOnDLS,
                    row: int,
                    causal_mask: List[bool],
                    acausal_mask: List[bool]):
        """
        Evaluate the next correlation reset events in an event stream against
        abstract expectations.
        """
        expected_corenres_enable = 0b1100 if (ppu == PPUOnDLS.top) else 0b0011

        expected_corenres_address = (row << 8) | row
        if ppu == PPUOnDLS.top:
            expected_corenres_address <<= 16

        for vector_parity in range(2):
            expected_corres_c = 0
            for idx, val in enumerate(causal_mask):
                if not val:
                    continue
                if idx % 2 == vector_parity:
                    expected_corres_c |= 1 << (255 - idx)

            expected_corres_a = 0
            for idx, val in enumerate(acausal_mask):
                if not val:
                    continue
                if idx % 2 == vector_parity:
                    expected_corres_a |= 1 << (255 - idx)

            if ppu == PPUOnDLS.top:
                expected_corres_a <<= 256
                expected_corres_c <<= 256

            expectation = {
                "sa_corenres_enable": expected_corenres_enable,
                "sa_corres_c": expected_corres_c,
                "sa_corres_a": expected_corres_a,
                "sa_corenres_address": expected_corenres_address
            }

            # Two rising bit per event => two log outputs
            for _ in range(2):
                self.assertDictEqual(expectation, next(events))

    def eval_neuron_reset(self,
                          events: Iterator[dict],
                          ppu: PPUOnDLS,
                          reset_mask: List[bool]):
        """
        Evaluate the next global neuron reset events in an event stream against
        abstract expectations.
        """
        expected_corenres_neuron = 0b1100 if (ppu == 0) else 0b0011

        # Events are only visible if C is actually pulled
        parities = list()
        if any([val for idx, val in enumerate(reset_mask) if (idx % 2) == 0]):
            parities.append(0)
        if any([val for idx, val in enumerate(reset_mask) if (idx % 2) != 0]):
            parities.append(1)

        for vector_parity in parities:
            expected_corres_a = 0
            for idx, val in enumerate(reset_mask):
                if not val:
                    continue
                if idx % 2 == vector_parity:
                    expected_corres_a |= 1 << (255 - idx)

            if ppu == PPUOnDLS.top:
                expected_corres_a <<= 256

            expectation = {
                "sa_corenres_neuron": expected_corenres_neuron,
                "sa_corres_c": 0,
                "sa_corres_a": expected_corres_a,
            }

            self.assertDictEqual(expectation, next(events))
            next(events)  # Drop falling sa_corres_a edge event


if __name__ == '__main__':
    unittest.main()
