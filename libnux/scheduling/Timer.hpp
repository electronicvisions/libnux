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
	service_id m_id;
	// debug
	// count missed runs
	time_type m_missed;

	time_type m_last_run;

public:
	Timer();
	// get next event, return true, if new event
	bool next_event(Event& event, time_type t);
	// set service id directly
	void set_service_id(service_id id);
	// set service id by service
	template <class S>
	void set_service(S& s);
	// set timing
	void set_first_deadline(time_type first);
	void set_num_periods(time_type num);
	void set_period(time_type period);
	// debug
	// get number of missed runs
	time_type get_missed_count();
};

Timer::Timer()
{
	this->m_id = 0;
	this->m_start = 0;
	this->m_num_periods = 0;
	this->m_period = 0;
	this->m_last_run = 0;
	this->m_missed = 0;
}

void Timer::set_service_id(service_id id)
{
	this->m_id = id;
}

template <class T>
void Timer::set_service(T& t)
{
	this->m_id = t.id;
}

void Timer::set_first_deadline(time_type first)
{
	this->m_start = first;
}

void Timer::set_num_periods(time_type num)
{
	this->m_num_periods = num;
}

void Timer::set_period(time_type period)
{
	this->m_period = period;
}

bool Timer::next_event(Event& event, time_type t)
{
	if (t >= this->m_start) {
		if ((t -= this->m_start) < this->m_period * this->m_num_periods) {
			if (t >= this->m_last_run) {
				this->m_last_run += this->m_period;
				while (this->m_last_run < t) {
					++this->m_missed;
					this->m_last_run += this->m_period;
				}
				event.id = this->m_id;
				event.deadline = this->m_last_run + this->m_start;
				return true;
			}
		}
	}
	return false;
}

time_type Timer::get_missed_count()
{
	return this->m_missed;
}
