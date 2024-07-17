#pragma once
#include "libnux/scheduling/Service.hpp"
#include "libnux/scheduling/types.hpp"

class TimerOneshot
{
	// timing parameter
	// latest start time
	time_type m_deadline;
	// earliest start time = m_deadline - m_window
	time_type m_window;
	// Event to be run
	Service m_service;

	bool m_has_been_run;

public:
	TimerOneshot(Service service);
	TimerOneshot(Service service, time_type t);
	TimerOneshot(Service service, time_type deadline, time_type window);
	// get 'next' event, return true, if new event
	bool next_event(Event& event, time_type t);
	// set service
	void set_service(Service service);
	// set timing
	void set_deadline(time_type t);
	void set_window(time_type window);
};

TimerOneshot::TimerOneshot(Service service)
{
	this->m_deadline = 0;
	this->m_window = 0;
	this->m_has_been_run = true;
	this->m_service = service;
}

TimerOneshot::TimerOneshot(Service service, time_type deadline)
{
	this->m_deadline = deadline;
	this->m_window = 0;
	this->m_has_been_run = false;
	this->m_service = service;
}

TimerOneshot::TimerOneshot(Service service, time_type deadline, time_type window)
{
	this->m_deadline = deadline;
	this->m_window = window;
	this->m_has_been_run = false;
	this->m_service = service;
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

void TimerOneshot::set_service(Service service)
{
	this->m_service = service;
}

bool TimerOneshot::next_event(Event& event, time_type t)
{
	if (not this->m_has_been_run and t >= this->m_deadline - this->m_window) {
		event.service = &(this->m_service);
		event.deadline = this->m_deadline;
		this->m_has_been_run = true;
		return true;
	}
	return false;
}
