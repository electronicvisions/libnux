#include "libnux/scheduling/Scheduler.hpp"
#include "libnux/scheduling/SchedulerSignaller.hpp"
#include "libnux/scheduling/Service.hpp"
#include "libnux/scheduling/Timer.hpp"
#include "libnux/scheduling/TimerOneshot.hpp"
// For communicating results
#include "libnux/mailbox.h"

/**
 * This program shows a usage example of the earliest-deadline first scheduler
 * scheduling two tasks, a function call and a class member function call.
 */

/// Variable to alter by function call
int var = 0;

/// Function describing a complicated task to be performed
void Function()
{
	var += 1;
}

/**
 * \brief Stores member variable and is capable of altering is through a member
 * function call.
 */
class Class
{
	int m_var;

public:
	Class() { m_var = 27; }
	void something_interesting() { m_var = 42; }
	int get_var() { return m_var; }
};

/// main function, program execution starts here
int start()
{
	// Create scheduler with queue size 10
	Scheduler<10> scheduler;

	// Create services
	// These wrap the functions / class member functions to be called
	// Service_Function<ID, &function>
	Service_Function<1, &Function> service_function;
	// instance to alter
	Class instance;
	// Service_Class<ID, class, &class::member_function>(instance)
	Service_Class<2, Class, &Class::something_interesting> service_class(instance);

	// Create service reference tuple to pass over to scheduler loop
	auto services = std::tie(service_function, service_class);

	// Create periodic timer for function service
	Timer timer;
	timer.set_service(service_function);
	timer.set_first_deadline(10000);
	timer.set_num_periods(100);
	timer.set_period(5000);

	// Create oneshot timer for member function
	TimerOneshot timerO;
	timerO.set_service(service_class);
	// Execute between this and...
	timerO.set_deadline(257000);
	// ...(deadline - this).
	timerO.set_window(1000);

	// Create timer reference tuple to pass over to scheduler loop
	auto timers = std::tie(timer, timerO);

	// create signaller starting at 5e3, running until 8e5, exiting at 1e6
	auto signaller = SchedulerSignallerTimer(5000, 800000, 1000000);

	scheduler.execute(signaller, services, timers);

	// should state 100==num_periods of service function
	libnux_mailbox_write_string("var = ");
	libnux_mailbox_write_int(var);
	// should state 42
	libnux_mailbox_write_string(", Class.m_var = ");
	libnux_mailbox_write_int(instance.get_var());

	// Analyze scheduling performance
	// Did the periodic timer miss periods?
	libnux_mailbox_write_string("\nperiodic timer missed periods = ");
	libnux_mailbox_write_int(timer.get_missed_count());

	// What was the maximal count of events in the scheduler's queue?
	libnux_mailbox_write_string("\nmax. queue size = ");
	libnux_mailbox_write_int(scheduler.get_max_queue_size());

	// If the queue overflowed, how many events did we loose?
	libnux_mailbox_write_string("\ndropped events count = ");
	libnux_mailbox_write_int(scheduler.get_dropped_events_count());

	return 0;
}
