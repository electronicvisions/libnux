#pragma once
//#include "libnux/vx/mailbox.h"
#include "libnux/scheduling/types.hpp"
#include "libnux/vx/spr.h"

///**
// * Scheduler loop control that listens on a mailbox address for signals
// * \tparam signal_address Signal address in mailbox
// */
// template <uint32_t signal_address>
// class SchedulerSignallerMailbox
//{
// public:
//	/**
//	 * Get current signal state.
//	 * \return Signal state
//	 */
//	inline __attribute__((always_inline)) scheduler_signal signal();
//};
//
// template <uint32_t signal_address>
// inline __attribute__((always_inline)) scheduler_signal
// SchedulerSignallerMailbox<signal_address>::signal()
//{
//	uint8_t signal = libnux_mailbox_read_u8(signal_address);
//	switch (signal) {
//		case scheduler_wait:
//			return scheduler_wait;
//		case scheduler_run:
//			return scheduler_run;
//		case scheduler_finish:
//			return scheduler_finish;
//		case scheduler_exit:
//			return scheduler_exit;
//		// if unknown signal is encountered, wait
//		default:
//			return scheduler_wait;
//	}
//	return scheduler_exit; // This should never be called
//}

// scheduler signaller, that:
// waits until start,
// runs from start to finish,
// finishes from finish to stop
// and exits at stop.
class SchedulerSignallerTimer
{
	time_type m_start;
	time_type m_finish;
	time_type m_stop;

public:
	SchedulerSignallerTimer(time_type start, time_type finish, time_type stop);
	SchedulerSignallerTimer(time_type start, time_type stop);
	SchedulerSignallerTimer(time_type stop);
	inline __attribute__((always_inline)) scheduler_signal signal();
};

SchedulerSignallerTimer::SchedulerSignallerTimer(time_type start, time_type finish, time_type stop)
{
	this->m_start = start;
	this->m_finish = finish;
	this->m_stop = stop;
}

SchedulerSignallerTimer::SchedulerSignallerTimer(time_type start, time_type stop)
{
	this->m_start = start;
	this->m_finish = stop;
	this->m_stop = stop;
}

SchedulerSignallerTimer::SchedulerSignallerTimer(time_type stop)
{
	this->m_start = 0;
	this->m_finish = stop;
	this->m_stop = stop;
}

inline __attribute__((always_inline)) scheduler_signal SchedulerSignallerTimer::signal()
{
	time_type t = get_time();
	if (t < this->m_start) {
		return scheduler_wait;
	} else if (t >= this->m_start and t < this->m_finish) {
		return scheduler_run;
	} else if (t >= this->m_finish and t < this->m_stop) {
		return scheduler_finish;
	} else if (t >= this->m_stop) {
		return scheduler_exit;
	}
	return scheduler_exit; // This will never be called.
}
