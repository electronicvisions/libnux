#pragma once
#include <tuple>
#include <utility>

#include "libnux/scheduling/Queue.hpp"
#include "libnux/scheduling/types.hpp"

/**
 * Earliest-deadline first scheduler with a fixed size event queue.
 * Tasks wrapped by services are schedled based on execution requests generated
 * by event sources.
 * \tparam queue_size Size of event queue.
 */
template <size_t queue_size>
class Scheduler
{
	Queue<queue_size, Event> m_queue;

public:
	/**
	 * Initialize scheduler and its queue.
	 */
	Scheduler();

	/**
	 * Scheduler loop.
	 * \tparam Signaller Scheduler loop control type
	 * \tparam S Service types
	 * \tparam E Event source types
	 * \param signaller Scheduler loop control instance
	 * \param services Tuple of references to services
	 * \param event_sources Tuple of references to event sources
	 */
	template <typename Signaller, typename... S, typename... E>
	void execute(
	    Signaller& signaller, std::tuple<S&...>& services, std::tuple<E&...>& event_sources);

	/**
	 * Get maximal number of elements stored in queue since creation.
	 */
	size_t get_max_queue_size();

	/**
	 * Get count of dropped events in queue.
	 */
	size_t get_dropped_events_count();

	/**
	 * Fetch new events from event sources based on provided time.
	 * Fills the queue with fetched events.
	 * \tparam E Event sources types
	 * \param event_sources Tuple of references to event sources
	 * \param t Time to fetch events for
	 */
	template <typename... E>
	void fetch_events_timed(std::tuple<E&...>& event_sources, time_type t);

	/**
	 * Flush event queue.
	 */
	void flush_queue();

	/**
	 * Sort earliest deadline event first in event queue.
	 */
	void sort_earliest_first();

	/**
	 * Execute services task corresponding to earliest deadline event in queue.
	 * \tparam S Service types
	 * \param services Tuple of references to services to execute from
	 */
	template <typename... S>
	void run_queue_service(std::tuple<S&...>& services);

private:
	/**
	 * Fetch events iterating recursively over all event sources.
	 * \tparam E Event sources types
	 * \tparam I First index
	 * \tparam Is Succeeding indices
	 * \param sources Tuple of references to event sources
	 * \param t Time to fetch events for
	 */
	template <typename... E, size_t I, size_t... Is>
	void fetch_events_from_source(
	    std::tuple<E...>& sources, std::index_sequence<I, Is...>, time_type t);

	/**
	 * End of fetch events iterating over recursively all event sources.
	 * \tparam E Event sources types
	 * \tparam I Index
	 * \param sources Tuple of references to event sources
	 * \param t Time to fetch events for
	 */
	template <typename... E, size_t I>
	void fetch_events_from_source(std::tuple<E...>& sources, std::index_sequence<I>, time_type t);


	/**
	 * Execute by interating recursively over all services.
	 * \tparam S Services types
	 * \tparam I First index
	 * \tparam Is Succeeding indices
	 * \param sources Tuple of references to services
	 * \param id Service ID of service to execute
	 */
	template <typename... S, size_t I, size_t... Is>
	void run_service(std::tuple<S...>& services, std::index_sequence<I, Is...>, service_id id);

	/**
	 * End of execute by interating recursively over all services.
	 * \tparam S Services types
	 * \tparam I Index
	 * \param sources Tuple of references to services
	 * \param id Service ID of service to execute
	 */
	template <typename... S, size_t I>
	void run_service(std::tuple<S...>& services, std::index_sequence<I>, service_id id);
};

template <size_t queue_size>
Scheduler<queue_size>::Scheduler()
{}

template <size_t queue_size>
size_t Scheduler<queue_size>::get_max_queue_size()
{
	return m_queue.get_max_size();
}

template <size_t queue_size>
size_t Scheduler<queue_size>::get_dropped_events_count()
{
	return m_queue.get_overflow_count();
}

template <size_t queue_size>
void Scheduler<queue_size>::flush_queue()
{
	m_queue.flush();
}

template <size_t queue_size>
template <typename... E>
inline __attribute__((always_inline)) void Scheduler<queue_size>::fetch_events_timed(
    std::tuple<E&...>& event_sources, time_type t)
{
	fetch_events_from_source(event_sources, std::make_index_sequence<sizeof...(E)>(), t);
}

template <size_t queue_size>
template <typename... E, size_t I, size_t... Is>
inline __attribute__((always_inline)) void Scheduler<queue_size>::fetch_events_from_source(
    std::tuple<E...>& sources, std::index_sequence<I, Is...>, time_type t)
{
	Event e;
	while (std::get<sizeof...(E) - 1 - sizeof...(Is)>(sources).next_event(e, t)) {
		m_queue.push(e);
	}
	fetch_events_from_source(sources, std::make_index_sequence<sizeof...(Is)>(), t);
}

template <size_t queue_size>
template <typename... E, size_t I>
inline __attribute__((always_inline)) void Scheduler<queue_size>::fetch_events_from_source(
    std::tuple<E...>& sources, std::index_sequence<I>, time_type t)
{
	Event e;
	while (std::get<sizeof...(E) - 1>(sources).next_event(e, t)) {
		m_queue.push(e);
	}
}

template <size_t queue_size>
inline __attribute__((always_inline)) void Scheduler<queue_size>::sort_earliest_first()
{
	// linear search for earliest deadline
	time_type earliest = m_queue[0].deadline;
	size_t min_index = 0;
	for (size_t i = 1; i < m_queue.get_size(); ++i) {
		if (m_queue[i].deadline < earliest) {
			min_index = i;
			earliest = m_queue[i].deadline;
		}
	}
	// set earliest deadline event to start of queue
	Event tmp = m_queue[0];
	m_queue[0] = m_queue[min_index];
	m_queue[min_index] = tmp;
}

template <size_t queue_size>
template <typename... S>
inline __attribute__((always_inline)) void Scheduler<queue_size>::run_queue_service(
    std::tuple<S&...>& services)
{
	Event e;
	if (m_queue.pop(e)) {
		run_service(services, std::make_index_sequence<sizeof...(S)>(), e.id);
	}
}

template <size_t queue_size>
template <typename... S, size_t I, size_t... Is>
inline __attribute__((always_inline)) void Scheduler<queue_size>::run_service(
    std::tuple<S...>& services, std::index_sequence<I, Is...>, service_id id)
{
	if (std::get<sizeof...(Is)>(services).id == id) {
		std::get<sizeof...(Is)>(services).exec();
	} else {
		run_service(services, std::make_index_sequence<sizeof...(Is)>(), id);
	}
}

template <size_t queue_size>
template <typename... S, size_t I>
inline __attribute__((always_inline)) void Scheduler<queue_size>::run_service(
    std::tuple<S...>& services, std::index_sequence<I>, service_id id)
{
	if (std::get<0>(services).id == id) {
		std::get<0>(services).exec();
	}
}

template <size_t queue_size>
template <typename Signaller, typename... S, typename... E>
void Scheduler<queue_size>::execute(
    Signaller& signaller, std::tuple<S&...>& services, std::tuple<E&...>& event_sources)
{
	scheduler_signal signal = signaller.signal();
	while (not(signal == scheduler_exit)) {
		if (not(signal == scheduler_wait)) {
			if ((signal == scheduler_run) or (signal == scheduler_finish)) {
				if (not(signal == scheduler_finish)) {
					fetch_events_timed(event_sources, get_time());
				}
				sort_earliest_first();
				run_queue_service(services);
			}
		}
		signal = signaller.signal();
	}
}
