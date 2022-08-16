import os
from os.path import join
import unittest
from typing import ClassVar

from dlens_vx_v3.hal import Barrier, NeuronConfig, SpikeCounterReset
from dlens_vx_v3.halco import iter_all, PPUOnDLS, BarrierOnFPGA, \
    JTAGIdCodeOnDLS, NeuronConfigOnDLS, SpikeCounterReadOnDLS, \
    SpikeCounterResetOnDLS
from dlens_vx_v3.hxcomm import ManagedConnection
from dlens_vx_v3.sta import generate, DigitalInit, run, \
    PlaybackProgramBuilder
from dlens_vx_v3 import logger
from dlens_vx_v3.tools.run_ppu_program import load_and_start_program, \
    wait_until_ppu_finished, stop_program
from dlens_vx_v3 import hal, halco

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


class LibnuxNeuronResetVectorTestVx(unittest.TestCase):
    MANAGED_CONNECTION = ManagedConnection()
    CONNECTION = None
    CHIP_REVISION: ClassVar[int]

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

    def test_neuron_reset(self):
        if self.CHIP_REVISION < 2:
            self.skipTest(f"Incompatible chip rev. {self.CHIP_REVISION} (<2).")

        log = logger.get("LibnuxAccessPatternTestVx.test_neuron_reset")
        program = join(TEST_BINARY_PATH, "neuron_reset_vector-ppu_vx_v3.bin")

        builder = PlaybackProgramBuilder()
        config = NeuronConfig()
        config.enable_fire = True
        for nrn in iter_all(NeuronConfigOnDLS):
            builder.write(nrn, config)
        config = hal.NeuronBackendConfig()
        config.enable_spike_out = True
        config.refractory_time = 10
        for nrn in iter_all(halco.NeuronBackendConfigOnDLS):
            builder.write(nrn, config)
        config = hal.CommonNeuronBackendConfig()
        config.enable_event_registers = True
        config.enable_clocks = True
        for coord in iter_all(halco.CommonNeuronBackendConfigOnDLS):
            builder.write(coord, config)
        for coord in iter_all(halco.CommonCorrelationConfigOnDLS):
            builder.write(coord, hal.CommonCorrelationConfig())
        for ctr in iter_all(SpikeCounterResetOnDLS):
            builder.write(ctr, SpikeCounterReset())
        run(self.CONNECTION, builder.done())

        for ppu in iter_all(PPUOnDLS):
            log.info("Running test on %s." % ppu)
            self.run_ppu_program(ppu, program, int(5e5))

        tickets = []
        for ctr in iter_all(SpikeCounterReadOnDLS):
            tickets.append(builder.read(ctr))
        builder.write(halco.TimerOnDLS(), hal.Timer(0))
        builder.block_until(halco.TimerOnDLS(), 10000)
        run(self.CONNECTION, builder.done())

        for i, ticket in enumerate(tickets):
            # in the PPU program we reset all odd columns once
            # and all even columns twice
            if i % 2:
                self.assertEqual(ticket.get().count, 1)
            else:
                self.assertEqual(ticket.get().count, 2)


if __name__ == '__main__':
    unittest.main()
