import unittest
from os.path import join
from dlens_vx_v3.hxcomm import ManagedConnection
from dlens_vx_v3.sta import PlaybackProgramBuilder, \
    generate, DigitalInit
from dlens_vx_v3.hal import PPUMemoryWord, Timer
from dlens_vx_v3.halco import iter_all, PPUOnDLS
from dlens_vx_v3.tools.run_ppu_program import load_and_start_program, \
    stop_program
import dlens_vx_v3.halco as halco
import dlens_vx_v3.sta as sta
from pylola_vx_v3 import PPUElfFile
from helpers.hwtest_common import TEST_BINARY_PATH


class LibnuxBarrierTest(unittest.TestCase):
    MANAGED_CONNECTION = ManagedConnection()
    CONNECTION = None

    @classmethod
    def setUpClass(cls) -> None:
        # Connect to some executor (sim or hardware)
        cls.CONNECTION = cls.MANAGED_CONNECTION.__enter__()  # pylint: disable=unnecessary-dunder-call

        # Initialize the chip
        init_builder, _ = generate(DigitalInit())
        sta.run(cls.CONNECTION, init_builder.done())

    @classmethod
    def tearDownClass(cls) -> None:
        # Disconnect the executor
        cls.MANAGED_CONNECTION.__exit__()

    def test(self):
        """
        Augment this class by test cases for all tests defined in `cls.TESTS`.
        """
        path = join(TEST_BINARY_PATH, "barrier_two_ppus_vx_v3.bin")
        program_file = PPUElfFile(path)
        counter_coord = program_file.read_symbols()["counter"].coordinate
        wait_coord = program_file.read_symbols()["wait"].coordinate
        begin_coord = program_file.read_symbols()["begin"].coordinate
        local_index_coord = \
            program_file.read_symbols()["local_index"].coordinate
        returncode = []
        counter = []
        for ppu in iter_all(PPUOnDLS):
            load_and_start_program(self.CONNECTION, path, ppu)
        for ppu in iter_all(PPUOnDLS):
            builder = PlaybackProgramBuilder()
            builder.write(
                halco.PPUMemoryWordOnDLS(wait_coord.toMin(), ppu),
                PPUMemoryWord(PPUMemoryWord.Value(
                    100000 if ppu == PPUOnDLS.top else 100)))
            builder.write(halco.PPUMemoryWordOnDLS(local_index_coord.toMin(),
                                                   ppu),
                          PPUMemoryWord(PPUMemoryWord.Value(int(ppu))))
            builder.write(
                halco.PPUMemoryWordOnDLS(begin_coord.toMin(), ppu),
                PPUMemoryWord(PPUMemoryWord.Value(1)))
            sta.run(self.CONNECTION, builder.done())

        builder = PlaybackProgramBuilder()
        builder.write(halco.TimerOnDLS(), Timer())
        builder.block_until(halco.TimerOnDLS(), Timer.Value(int(1e8)))
        sta.run(self.CONNECTION, builder.done())
        for ppu in iter_all(PPUOnDLS):
            returncode.append(stop_program(self.CONNECTION, ppu=ppu,
                                           print_mailbox=True))
            ticket = builder.read(halco.PPUMemoryWordOnDLS(
                counter_coord.toMin(), ppu))
            builder.write(halco.TimerOnDLS(), Timer())
            builder.block_until(halco.TimerOnDLS(), Timer.Value(1000))
            sta.run(self.CONNECTION, builder.done())
            counter.append(ticket.get().value.value())

        for ret in returncode:
            self.assertEqual(
                ret, 0,
                f"Return value({ret}) does not match expectation({0}).")

        for ctr in counter:
            self.assertEqual(
                ctr, 1000,
                f"Counter value({ctr}) does not match expectation({1000}).")


if __name__ == '__main__':
    unittest.main()
