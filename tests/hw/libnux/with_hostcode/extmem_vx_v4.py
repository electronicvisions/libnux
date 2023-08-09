import os
from os.path import join
import unittest
from typing import ClassVar

from dlens_vx_v4.hal import Barrier
from dlens_vx_v4.halco import iter_all, PPUOnDLS, BarrierOnFPGA, \
    JTAGIdCodeOnDLS, ExternalPPUMemoryByteOnFPGA, ExternalPPUMemoryBlockOnFPGA
from dlens_vx_v4.hxcomm import ManagedConnection
from dlens_vx_v4.sta import generate, DigitalInit, run, \
    PlaybackProgramBuilder
from dlens_vx_v4.tools.run_ppu_program import load_and_start_program, \
    wait_until_ppu_finished, stop_program

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
                                       "libnux",
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

    def test_extmem(self):
        program = join(TEST_BINARY_PATH, "extmem-ppu_vx_v4.bin")

        for ppu in iter_all(PPUOnDLS):
            self.run_ppu_program(ppu, program, int(5e5))

        builder = PlaybackProgramBuilder()
        ticket = builder.read(ExternalPPUMemoryBlockOnFPGA(
            ExternalPPUMemoryByteOnFPGA(1 << 16),
            ExternalPPUMemoryByteOnFPGA((1 << 16) + 255)))
        builder.block_until(BarrierOnFPGA(), Barrier.omnibus)
        run(self.CONNECTION, builder.done())

        block = ticket.get()
        for i in range(256):
            self.assertEqual(int(block.bytes[i].value), i)


if __name__ == '__main__':
    unittest.main()
