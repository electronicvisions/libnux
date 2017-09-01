#include <gtest/gtest.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/container/v2/capmem.h"
#include "haldls/io/v2/experiment.h"
#include "haldls/io/v2/playback.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::container::v2;
using namespace haldls::io::v2;

class PlaybackTest : public ::testing::Test {
protected:
	void SetUp() override
	{
		// get the board id
		std::vector<std::string> board_ids = available_board_usb_serial_numbers();
		ASSERT_EQ(1, board_ids.size()) << "number of allocated boards is not one";

		test_board = board_ids.front();
	}

	std::string test_board;
}; // PlaybackTest

TEST_F(PlaybackTest, CapMem) {
	Unique const unique;

	CapMem capmem_config;

	for (auto cell : iter_all<CapMemCellOnDLS>()) {
		capmem_config.set(cell, CapMemCell(CapMemCell::Value(cell.toEnum())));
	}

	CapMemCell capmemcell0(CapMemCell::Value(123));
	CapMemCell capmemcell1(CapMemCell::Value(321));
	CapMemCell capmemcell2(CapMemCell::Value(334));
	capmem_config.set(CapMemCellOnDLS(halco::common::Enum(0)), capmemcell0);
	capmem_config.set(CapMemCellOnDLS(halco::common::Enum(1)), capmemcell1);
	capmem_config.set(CapMemCellOnDLS(halco::common::Enum(2)), capmemcell2);

	PlaybackProgramBuilder builder;
	builder.set_container(unique, capmem_config);
	builder.wait_until(100);
	auto capmem_ticket = builder.get_container(unique, capmem_config);
	auto capmemcell_ticket =
		builder.get_container<CapMemCell>(CapMemCellOnDLS(halco::common::Enum(2)));
	builder.halt();

	auto program = builder.done();

	// Builder is reset (starts a new program) every time .done() is called
	auto empty_new_program = builder.done();
	EXPECT_NE(program.serial_number(), empty_new_program.serial_number());

	// No data available yet
	EXPECT_THROW(auto capmem_copy = program.get(capmem_ticket), std::runtime_error);
	EXPECT_THROW(auto capmemcell_copy = program.get(capmemcell_ticket), std::runtime_error);

	auto capmem_ticket_ = builder.get_container(unique, capmem_config);
	auto program_ = builder.done();

	// Using Ticket issued for a different program
	EXPECT_THROW(auto capmem_copy = program.get(capmem_ticket_), std::invalid_argument);
	EXPECT_THROW(auto capmem_copy = program_.get(capmem_ticket), std::invalid_argument);

	ExperimentControl ctrl = connect(test_board);
	ctrl.soft_reset();
	ctrl.run(program);

	EXPECT_THROW(auto capmem_copy = program.get(capmem_ticket_), std::invalid_argument);

	auto capmem_copy = program.get(capmem_ticket);
	auto capmemcell_copy = program.get(capmemcell_ticket);

	EXPECT_EQ(capmem_config, capmem_copy);
	EXPECT_EQ(capmemcell2, capmemcell_copy);
}

TEST_F(PlaybackTest, InvalidState) {
	PlaybackProgram invalid_program; // not obtained via builder
	EXPECT_EQ(PlaybackProgram::invalid_serial_number, invalid_program.serial_number());

	ExperimentControl ctrl = connect(test_board);
	EXPECT_THROW(ctrl.transfer(invalid_program), std::logic_error);
	EXPECT_THROW(ctrl.run(invalid_program), std::logic_error);
}
