#include <gtest/gtest.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/container/v2/board.h"
#include "haldls/container/v2/chip.h"
#include "haldls/io/v2/experiment.h"
#include "haldls/io/v2/playback.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::container::v2;
using namespace haldls::io::v2;

class ReadbackTest : public ::testing::Test {
protected:
	void SetUp() override
	{
		// get the board id
		std::vector<std::string> board_ids = available_board_usb_serial_numbers();
		ASSERT_EQ(1, board_ids.size()) << "number of allocated boards is not one";

		test_board = board_ids.front();
	}

	std::string test_board;
}; // ReadbackTest

TEST_F(ReadbackTest, ExternalNeuronSwitchesAreDisabled) {
	Board board;
	Chip chip;

	Unique const unique;
	NeuronOnDLS const output_neuron{0};
	NeuronOnDLS const input_neuron{5};

	chip.enable_external_voltage_output(output_neuron);
	EXPECT_EQ(output_neuron, chip.get_external_voltage_output_neuron());
	chip.enable_external_current_input(input_neuron);
	EXPECT_EQ(input_neuron, chip.get_external_current_input_neuron());

	PlaybackProgramBuilder builder;
	auto chip_ticket = builder.get_container<Chip>(unique);
	auto output_neuron_ticket = builder.get_container<NeuronDigitalConfig>(output_neuron);
	auto input_neuron_ticket = builder.get_container<NeuronDigitalConfig>(input_neuron);
	builder.halt();
	auto program = builder.done();

	ExperimentControl ctrl = connect(test_board);
	ctrl.configure_static(board, chip);
	ctrl.run(program);

	auto const chip_config = program.get(chip_ticket);
	EXPECT_EQ(output_neuron, chip_config.get_external_voltage_output_neuron());
	EXPECT_EQ(input_neuron, chip.get_external_current_input_neuron());

	auto const output_neuron_config = program.get(output_neuron_ticket);
	EXPECT_FALSE(output_neuron_config.get_enable_external_voltage_output());

	auto const input_neuron_config = program.get(input_neuron_ticket);
	EXPECT_FALSE(input_neuron_config.get_enable_external_current_input());
}
