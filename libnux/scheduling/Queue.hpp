#pragma once
#include <stddef.h>

/**
 * Circular buffer of fixed maximal size.
 * \tparam N Maximal number of elements stored
 * \tparam T Type of stored elements
 */
template <size_t N, typename T>
class Queue
{
	// buffer
	T m_array[N];
	// current size
	size_t m_size_current;
	// current writer position
	size_t m_write_addr;
	// debug values
	// number of elements overwriten because of overflow.
	size_t m_overflow_count;
	// max number of elements stored at once since last init.
	size_t m_max_size;

public:
	/**
	 * Default constructor.
	 */
	Queue();

	/**
	 * Append value at end of queue.
	 * \param value Value to append
	 */
	void push(T value);

	/**
	 * Get first element in queue and set first element to be the following.
	 * \param getter Reference to storage, where first element is copiet to
	 * \return Whether element was popped or queue is empty
	 */
	bool pop(T& getter);

	/**
	 * Get number of currently stored elements.
	 * \return Number of elements stored
	 */
	size_t get_size();

	/**
	 * Access element from indexed beginning, that is write pointer - current size.
	 * \param index Index of element to access
	 * \return Reference to element at index
	 */
	T& operator[](size_t index);

	/**
	 * Access element from indexed beginning, that is write pointer - current size.
	 * \param index Index of element to access
	 * \return Constant reference to element at index
	 */
	T const& operator[](size_t index) const;

	/**
	 * Get number of elements lost by overflow.
	 * \return Number of elements lost
	 */
	size_t get_overflow_count();
	/**
	 * Get maximal number of simultaneously stored elements.
	 * \return Maximal number of elements stored
	 */
	size_t get_max_size();

	/**
	 * Initialize read and write pointer and debug values.
	 */
	void init();
};

template <size_t N, class T>
Queue<N, T>::Queue()
{
	init();
}

template <size_t N, class T>
size_t Queue<N, T>::get_size()
{
	return m_size_current;
}

template <size_t N, class T>
size_t Queue<N, T>::get_overflow_count()
{
	return m_overflow_count;
}

template <size_t N, class T>
size_t Queue<N, T>::get_max_size()
{
	return m_max_size;
}

template <size_t N, class T>
void Queue<N, T>::push(T val)
{
	m_array[m_write_addr] = val;
	m_write_addr = (m_write_addr + 1) % N;
	if (m_size_current == N) {
		// overflow occured
		++m_overflow_count;
	} else {
		// no overflow
		++m_size_current;
		// update max number of elements stored in queue
		if (m_max_size < m_size_current) {
			m_max_size = m_size_current;
		}
	}
}

template <size_t N, class T>
bool Queue<N, T>::pop(T& t)
{
	if (m_size_current > 0) {
		t = m_array[((N + m_write_addr) - m_size_current) % N];
		--m_size_current;
		return true;
	}
	return false;
}

template <size_t N, class T>
T& Queue<N, T>::operator[](size_t index)
{
	// range check and if overrange return last element
	if (!(index < m_size_current)) {
		index = m_size_current - 1;
	}
	return m_array[((N + m_write_addr) - m_size_current + index) % N];
}

template <size_t N, class T>
T const& Queue<N, T>::operator[](size_t index) const
{
	// range check and if overrange return last element
	if (!(index < m_size_current)) {
		index = m_size_current - 1;
	}
	return m_array[((N + m_write_addr) - m_size_current + index) % N];
}


template <size_t N, class T>
void Queue<N, T>::init()
{
	m_size_current = 0;
	m_max_size = 0;
	m_write_addr = 0;
	m_overflow_count = 0;
}
