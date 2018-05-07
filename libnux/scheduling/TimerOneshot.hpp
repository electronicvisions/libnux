#pragma once
#include "libnux/scheduling/types.hpp"

class TimerOneshot
{
	// timing parameter
	// latest start time
	time_type m_deadline;
	// earliest start time = m_deadline - m_window
	time_type m_window;
	// Event to be run
	service_id m_id;

	bool m_has_been_run;

public:
	TimerOneshot();
	TimerOneshot(time_type t);
	TimerOneshot(time_type deadline, time_type window);
	// get 'next' event, return true, if new event
	bool next_event(Event& event, time_type t);
	// set service id directly
	void set_service_id(service_id id);
	// set service id by service
	template <class S>
	void set_service(S& s);
	// set timing
	void set_deadline(time_type t);
	void set_window(time_type window);
};

TimerOneshot::TimerOneshot()
{
	this->m_deadline = 0;
	this->m_window = 0;
	this->m_has_been_run = true;
}

TimerOneshot::TimerOneshot(time_type deadline)
{
	this->m_deadline = deadline;
	this->m_window = 0;
	this->m_has_been_run = false;
}

TimerOneshot::TimerOneshot(time_type deadline, time_type window)
{
	this->m_deadline = deadline;
	this->m_window = window;
	this->m_has_been_run = false;
}

void TimerOneshot::set_deadline(time_type t)
{
	this->m_deadline = t;
	this->m_has_been_run = false;
}

void TimerOneshot::set_window(time_type t)
{
	this->m_window = t;
	this->m_has_been_run = false;
}

void TimerOneshot::set_service_id(service_id id)
{
	this->m_id = id;
}

template <class S>
void TimerOneshot::set_service(S& s)
{
	this->m_id = s.id;
}

bool TimerOneshot::next_event(Event& event, time_type t)
{
	if (not this->m_has_been_run and t >= this->m_deadline - this->m_window) {
		event.id = this->m_id;
		event.deadline = this->m_deadline;
		this->m_has_been_run = true;
		return true;
	}
	return false;
}
