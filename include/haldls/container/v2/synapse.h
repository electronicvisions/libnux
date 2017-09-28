#pragma once

#include <ostream>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"

namespace haldls {
namespace container {
namespace v2 {

class CommonSynramConfig
{
public:
	struct PCConf : public halco::common::detail::RantWrapper<PCConf, uint_fast16_t, 15, 0>
	{
		constexpr explicit PCConf(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct WConf : public halco::common::detail::RantWrapper<WConf, uint_fast32_t, 255, 0>
	{
		constexpr explicit WConf(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct WaitCtrClear
		: public halco::common::detail::RantWrapper<WaitCtrClear, uint_fast16_t, 7, 0>
	{
		constexpr explicit WaitCtrClear(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	CommonSynramConfig() HALDLS_VISIBLE;

	PCConf get_pc_conf() const HALDLS_VISIBLE;
	void set_pc_conf(PCConf const& value) HALDLS_VISIBLE;
	WConf get_w_conf() const HALDLS_VISIBLE;
	void set_w_conf(WConf const& value) HALDLS_VISIBLE;
	WaitCtrClear get_wait_ctr_clear() const HALDLS_VISIBLE;
	void set_wait_ctr_clear(WaitCtrClear const& value) HALDLS_VISIBLE;

	bool operator==(CommonSynramConfig const& other) const HALDLS_VISIBLE;
	bool operator!=(CommonSynramConfig const& other) const HALDLS_VISIBLE;

private:
	PCConf m_pc_conf;
	WConf m_w_conf;
	WaitCtrClear m_wait_ctr_clear;
};


class SynapseBlock
{
public:
	/// \brief Single synapse containing a 6 bit weight and address.
	/// For each synapse, individual calibration bits can control the STDP time constant
	/// and amplitude and may be used to compensate the synapse-to-synapse variations of
	/// the correlation measurements.
	/// \note The calibration bits change both the causal and anticausal branch at
	///       once. In other words: causal and anticausal branches cannot be controlled
	///       individually.
	/// \see <a href="http://www.kip.uni-heidelberg.de/Veroeffentlichungen/details.php?id=3380">
	///      Bachelor's Thesis by Timo Wunderlich</a> for measurements.
	struct Synapse
	{
		struct Weight : public halco::common::detail::RantWrapper<Weight, uint_fast16_t, 63, 0>
		{
			constexpr explicit Weight(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
		};

		struct Address : public halco::common::detail::RantWrapper<Address, uint_fast16_t, 63, 0>
		{
			constexpr explicit Address(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
		};

		// The more bits set, the shorter the time constant
		struct TimeCalib : public halco::common::detail::RantWrapper<TimeCalib, uint_fast16_t, 3, 0>
		{
			constexpr explicit TimeCalib(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
		};

		// The more bits set, the lower the amplitude
		struct AmpCalib : public halco::common::detail::RantWrapper<AmpCalib, uint_fast16_t, 3, 0>
		{
			constexpr explicit AmpCalib(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
		};

		Synapse() HALDLS_VISIBLE;

		Weight get_weight() const HALDLS_VISIBLE;
		void set_weight(Weight const& value) HALDLS_VISIBLE;

		Address get_address() const HALDLS_VISIBLE;
		void set_address(Address const& value) HALDLS_VISIBLE;

		TimeCalib get_time_calib() const HALDLS_VISIBLE;
		void set_time_calib(TimeCalib const& value) HALDLS_VISIBLE;

		AmpCalib get_amp_calib() const HALDLS_VISIBLE;
		void set_amp_calib(AmpCalib const& value) HALDLS_VISIBLE;

		bool operator==(Synapse const& other) const HALDLS_VISIBLE;
		bool operator!=(Synapse const& other) const HALDLS_VISIBLE;

	private:
		Weight m_weight;
		Address m_address;
		TimeCalib m_time_calib;
		AmpCalib m_amp_calib;
	};

	SynapseBlock() HALDLS_VISIBLE;

	Synapse get_synapse(halco::hicann_dls::v2::SynapseOnSynapseBlock const& synapse) const
		HALDLS_VISIBLE;
	void set_synapse(
		halco::hicann_dls::v2::SynapseOnSynapseBlock const& synapse,
		Synapse const& value) HALDLS_VISIBLE;

	bool operator==(SynapseBlock const& other) const HALDLS_VISIBLE;
	bool operator!=(SynapseBlock const& other) const HALDLS_VISIBLE;

private:
	halco::common::typed_array<Synapse, halco::hicann_dls::v2::SynapseOnSynapseBlock> m_synapses;
};

class ColumnCorrelationBlock
{
public:
	struct ColumnCorrelationSwitch
	{
		enum class Config : uint_fast8_t
		{
			disabled = 0,
			external = 1,
			internal = 2,
			readout = 3
		};

		ColumnCorrelationSwitch() HALDLS_VISIBLE;

		Config get_causal_config() const HALDLS_VISIBLE;
		void set_causal_config(Config const value) HALDLS_VISIBLE;

		Config get_acausal_config() const HALDLS_VISIBLE;
		void set_acausal_config(Config const value) HALDLS_VISIBLE;

		bool operator==(ColumnCorrelationSwitch const& other) const HALDLS_VISIBLE;
		bool operator!=(ColumnCorrelationSwitch const& other) const HALDLS_VISIBLE;

	private:
		Config m_causal;
		Config m_acausal;
	};

	ColumnCorrelationBlock() HALDLS_VISIBLE;

	ColumnCorrelationSwitch get_switch(
		halco::hicann_dls::v2::ColumnCorrelationSwitchOnColumnBlock const& correlation_switch) const
		HALDLS_VISIBLE;
	void set_switch(
		halco::hicann_dls::v2::ColumnCorrelationSwitchOnColumnBlock const& correlation_switch,
		ColumnCorrelationSwitch const& value) HALDLS_VISIBLE;

	bool operator==(ColumnCorrelationBlock const& other) const HALDLS_VISIBLE;
	bool operator!=(ColumnCorrelationBlock const& other) const HALDLS_VISIBLE;

private:
	halco::common::typed_array<
		ColumnCorrelationSwitch,
		halco::hicann_dls::v2::ColumnCorrelationSwitchOnColumnBlock>
		m_switches;
};

class ColumnCurrentBlock
{
public:
	struct ColumnCurrentSwitch
	{
		enum class Config : uint_fast8_t
		{
			disabled = 0,
			external = 1,
			internal = 2,
			readout = 3
		};

		ColumnCurrentSwitch() HALDLS_VISIBLE;

		Config get_exc_config() const HALDLS_VISIBLE;
		void set_exc_config(Config const value) HALDLS_VISIBLE;

		Config get_inh_config() const HALDLS_VISIBLE;
		void set_inh_config(Config const value) HALDLS_VISIBLE;

		bool operator==(ColumnCurrentSwitch const& other) const HALDLS_VISIBLE;
		bool operator!=(ColumnCurrentSwitch const& other) const HALDLS_VISIBLE;

	private:
		Config m_exc;
		Config m_inh;
	};

	ColumnCurrentBlock() HALDLS_VISIBLE;

	ColumnCurrentSwitch get_switch(
		halco::hicann_dls::v2::ColumnCurrentSwitchOnColumnBlock const& current_switch) const
		HALDLS_VISIBLE;
	void set_switch(
		halco::hicann_dls::v2::ColumnCurrentSwitchOnColumnBlock const& current_switch,
		ColumnCurrentSwitch const& value) HALDLS_VISIBLE;

	bool operator==(ColumnCurrentBlock const& other) const HALDLS_VISIBLE;
	bool operator!=(ColumnCurrentBlock const& other) const HALDLS_VISIBLE;

private:
	halco::common::
		typed_array<ColumnCurrentSwitch, halco::hicann_dls::v2::ColumnCurrentSwitchOnColumnBlock>
			m_switches;
};


class SynapseDrivers
{
public:
	struct PulseLength
		: public halco::common::detail::RantWrapper<PulseLength, uint_fast16_t, 31, 0>
	{
		constexpr explicit PulseLength(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	enum class State : uint_fast8_t
	{
		disabled = 0,
		excitatory,
		inhibitory
	};

	typedef std::array<State, halco::hicann_dls::v2::SynapseDriverOnDLS::size> states_type;

	SynapseDrivers() HALDLS_VISIBLE;

	states_type get_states() const HALDLS_VISIBLE;
	void set_states(states_type const& values) HALDLS_VISIBLE;

	State get_state(halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver) const
		HALDLS_VISIBLE;
	void set_state(halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver, State value)
		HALDLS_VISIBLE;

	PulseLength get_pulse_length() const HALDLS_VISIBLE;
	void set_pulse_length(PulseLength const& value) HALDLS_VISIBLE;

	bool operator==(SynapseDrivers const& other) const HALDLS_VISIBLE;
	bool operator!=(SynapseDrivers const& other) const HALDLS_VISIBLE;

private:
	PulseLength m_pulse_length;
	states_type m_states;
};

} // namespace v2
} // namespace container
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonSynramConfig::PCConf)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonSynramConfig::WConf)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonSynramConfig::WaitCtrClear)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::SynapseBlock::Synapse::Weight)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::SynapseBlock::Synapse::Address)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::SynapseBlock::Synapse::TimeCalib)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::SynapseBlock::Synapse::AmpCalib)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::SynapseDrivers::PulseLength)

} // namespace std