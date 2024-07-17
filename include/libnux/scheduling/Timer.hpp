#pragma once
#include "libnux/scheduling/types.hpp"

// Timer is a periodic timer with start and period times and number of periods to run.
// Timer emits an event to be executed when the Timer wants a run,
// i.e. the time given corresponds to a new period.
class Timer
{
	// timing variables
	time_type m_num_periods;
	time_type m_period;
	time_type m_start;
	// Event to be run
	Service m_service;
	// debug
	// count missed runs
	time_type m_missed;

	time_type m_last_run;
	time_type m_end_run;

public:
	Timer(Service service);
	// get next event, return true, if new event
	bool next_event(Event& event, time_type t);
	// set service id directly
	void set_service(Service service);
	// set timing
	void set_first_deadline(time_type first);
	time_type get_first_deadline() const;
	void set_num_periods(time_type num);
	void set_period(time_type period);
	// debug
	// get number of missed runs
	time_type get_missed_count();
};

inline Timer::Timer(Service service)
{
	this->m_start = 0;
	this->m_num_periods = 0;
	this->m_period = 0;
	this->m_last_run = 0;
	this->m_end_run = 0;
	this->m_missed = 0;
	this->m_service = service;
}

inline void Timer::set_service(Service service)
{
	this->m_service = service;
}

inline void Timer::set_first_deadline(time_type first)
{
	this->m_start = first;
	m_last_run = first;
	m_end_run = m_start + m_period * m_num_periods;
}

inline time_type Timer::get_first_deadline() const
{
	return this->m_start;
}

inline void Timer::set_num_periods(time_type num)
{
	this->m_num_periods = num;
	m_end_run = m_start + m_period * m_num_periods;
}

inline void Timer::set_period(time_type period)
{
	this->m_period = period;
	m_end_run = m_start + m_period * m_num_periods;
}

inline bool Timer::next_event(Event& event, time_type t)
{
	if ((t >= m_last_run) && (t < m_end_run)) {
		this->m_last_run += this->m_period;
		while (this->m_last_run < t) {
			++this->m_missed;
			this->m_last_run += this->m_period;
		}
		event.service = &(this->m_service);
		event.deadline = this->m_last_run;
		return true;
	}
	return false;
}

inline time_type Timer::get_missed_count()
{
	return this->m_missed;
}
