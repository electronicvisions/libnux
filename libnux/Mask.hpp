#pragma once
#include <s2pp.h>
#include <stddef.h>
#include "libnux/attrib.h"
#include "libnux/iterator.hpp"

// boolean mask vector, mask is encoded in mask[i] != 0
ATTRIB_ALWAYS_INLINE __vector uint8_t
if_(__vector uint8_t mask, __vector uint8_t const v_true, __vector uint8_t const v_false)
{
	// clang-format off
	asm volatile(
		"fxvcmpb %[mask]\n"
		"fxvsel %[mask], %[v_false], %[v_true], 1\n"
		: [mask] "=&kv"(mask)
		: [v_true] "kv"(v_true), [v_false] "kv"(v_false)
		:);
	// clang-format on
	return mask;
}

ATTRIB_ALWAYS_INLINE __vector uint8_t
not_if_(__vector uint8_t mask, __vector uint8_t const v_true, __vector uint8_t const v_false)
{
	return if_(mask, v_false, v_true);
}

// encode mask information in uint8_t value, mask, if mask[i] != cond
ATTRIB_ALWAYS_INLINE __vector uint8_t if_eq(
    __vector uint8_t mask,
    uint8_t const cond,
    __vector uint8_t const v_true,
    __vector uint8_t const v_false)
{
	mask -= vec_splat_u8(cond);
	// clang-format off
	asm volatile(
		"fxvcmpb %[mask]\n"
		"fxvsel %[mask], %[v_false], %[v_true], 3\n"
		: [mask] "+&kv"(mask)
		: [v_true] "kv"(v_true), [v_false] "kv"(v_false)
		:);
	// clang-format on
	return mask;
}

ATTRIB_ALWAYS_INLINE __vector uint8_t not_if_eq(
    __vector uint8_t mask,
    uint8_t const cond,
    __vector uint8_t const v_true,
    __vector uint8_t const v_false)
{
	return if_eq(mask, cond, v_false, v_true);
}

// return (exists i: v[i] == c)
ATTRIB_ALWAYS_INLINE bool any(__vector uint8_t const v, uint8_t const c)
{
	for (uint32_t i = 0; i < 16; ++i) {
		if (v[i] == c) {
			return true;
		}
	}
	return false;
}

// define type of vector addresses
typedef uint8_t vector_synram_address;

// pair of vector and address references
struct addressed_vector_ref
{
	vector_synram_address& address;
	__vector uint8_t& vec;
};

// Mask provides one uint8_t value per synapse
// of every partially enabled vector
// and one address for every partially or fully enabled vector.
// num_partial_vectors of partially enabled vectors,
// num_full_vectors of fully enabled vectors.
// When used for masking commands:
// vectors[i][j]=0 == don't execute command,
// vectors[i][j]>0 == execute command (e.g. weight update rule).
// Mask additionally provides a functor to iterate synapes per vector
// with an arbitrary class member function.
template <size_t num_partial_vectors, size_t num_full_vectors>
struct Mask
{
	__vector uint8_t const one;
	Mask() :
	    one{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    full_vector_synram_address(),
	    partial_vector_synram_address(),
	    vectors()
	{}

	vector_synram_address full_vector_synram_address[num_full_vectors];

	vector_synram_address partial_vector_synram_address[num_partial_vectors];
	__vector uint8_t vectors[num_partial_vectors];
	template <class T>
	void apply_vector_rule(
	    T* t, void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec));

	addressed_vector_ref operator[](size_t i);
	Iterator<Mask> begin();
	Iterator<Mask> end();
};

template <size_t num_partial_vectors, size_t num_full_vectors>
template <class T>
void Mask<num_partial_vectors, num_full_vectors>::apply_vector_rule(
    T* t, void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec))
{
	for (size_t i = 0; i < num_partial_vectors; ++i) {
		(t->*vector_rule)(this->partial_vector_synram_address[i], this->vectors[i]);
	}

	for (size_t i = 0; i < num_full_vectors; ++i) {
		(t->*vector_rule)(this->full_vector_synram_address[i], one);
	}
}

template <size_t num_partial_vectors, size_t num_full_vectors>
addressed_vector_ref Mask<num_partial_vectors, num_full_vectors>::operator[](size_t i)
{
	// clang-format off
	return i < num_full_vectors ? addressed_vector_ref{full_vector_synram_address[i], one}
		: addressed_vector_ref{partial_vector_synram_address[i - num_full_vectors],
						vectors[i - num_full_vectors]};
	// clang-format on
}

template <size_t num_partial_vectors, size_t num_full_vectors>
Iterator<Mask<num_partial_vectors, num_full_vectors> >
Mask<num_partial_vectors, num_full_vectors>::begin()
{
	return Iterator<Mask>(*this);
}

template <size_t num_partial_vectors, size_t num_full_vectors>
Iterator<Mask<num_partial_vectors, num_full_vectors> >
Mask<num_partial_vectors, num_full_vectors>::end()
{
	return Iterator<Mask>(*this, num_partial_vectors + num_full_vectors);
}

// specializations to prevent arrays with size 0

template <size_t num_partial_vectors>
struct Mask<num_partial_vectors, 0>
{
	vector_synram_address partial_vector_synram_address[num_partial_vectors];
	__vector uint8_t vectors[num_partial_vectors];
	Mask() : partial_vector_synram_address(), vectors() {}

	template <class T>
	void apply_vector_rule(
	    T* t, void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec));

	addressed_vector_ref operator[](size_t i);
	Iterator<Mask> begin();
	Iterator<Mask> end();
};

template <size_t num_partial_vectors>
template <class T>
void Mask<num_partial_vectors, 0>::apply_vector_rule(
    T* t, void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec))
{
	for (size_t i = 0; i < num_partial_vectors; ++i) {
		(t->*vector_rule)(this->partial_vector_synram_address[i], this->vectors[i]);
	}
}

template <size_t num_partial_vectors>
addressed_vector_ref Mask<num_partial_vectors, 0>::operator[](size_t i)
{
	return addressed_vector_ref{partial_vector_synram_address[i], vectors[i]};
}

template <size_t num_partial_vectors>
Iterator<Mask<num_partial_vectors, 0> > Mask<num_partial_vectors, 0>::begin()
{
	return Iterator<Mask<num_partial_vectors, 0> >(*this);
}

template <size_t num_partial_vectors>
Iterator<Mask<num_partial_vectors, 0> > Mask<num_partial_vectors, 0>::end()
{
	return Iterator<Mask<num_partial_vectors, 0> >(*this, num_partial_vectors);
}

template <size_t num_full_vectors>
struct Mask<0, num_full_vectors>
{
	__vector uint8_t const one;
	vector_synram_address full_vector_synram_address[num_full_vectors];
	Mask() : one{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, full_vector_synram_address() {}
	template <class T>
	void apply_vector_rule(
	    T* t, void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec));

	addressed_vector_ref operator[](size_t i);
	Iterator<Mask> begin();
	Iterator<Mask> end();
};

template <size_t num_full_vectors>
template <class T>
void Mask<0, num_full_vectors>::apply_vector_rule(
    T* t, void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec))
{
	for (size_t i = 0; i < num_full_vectors; ++i) {
		(t->*vector_rule)(this->full_vector_synram_address[i], one);
	}
}

template <size_t num_full_vectors>
addressed_vector_ref Mask<0, num_full_vectors>::operator[](size_t i)
{
	return addressed_vector_ref{full_vector_synram_address[i], one};
}

template <size_t num_full_vectors>
Iterator<Mask<0, num_full_vectors> > Mask<0, num_full_vectors>::begin()
{
	return Iterator<Mask<0, num_full_vectors> >(*this);
}

template <size_t num_full_vectors>
Iterator<Mask<0, num_full_vectors> > Mask<0, num_full_vectors>::end()
{
	return Iterator<Mask<0, num_full_vectors> >(*this, num_full_vectors);
}


template <>
struct Mask<0, 0>
{
	template <class T>
	void apply_vector_rule(
	    T* t, void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec))
	{}
};

// MaskTagged stores mask information in tags, uint8_t values.
// - provides one uint8_t tag storage per synapse.
// - provides apply_vector_rule, iterating over the vectors incorporating the specified tag,
//   updating only the synapses, which tag is specified.
template <size_t num_vectors>
struct MaskTagged
{
	static constexpr size_t& size = num_vectors;
	__vector uint8_t vectors[num_vectors];
	vector_synram_address addresses[num_vectors];
	MaskTagged() : vectors(), addresses() {}

	template <class T>
	void apply_vector_rule(
	    T* t,
	    void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec),
	    uint8_t const tag);

	addressed_vector_ref operator[](size_t i);
	Iterator<MaskTagged> begin();
	Iterator<MaskTagged> end();
};

template <size_t num_vectors>
template <class T>
void MaskTagged<num_vectors>::apply_vector_rule(
    T* t,
    void (T::*vector_rule)(vector_synram_address const addr, __vector uint8_t const& vec),
    uint8_t tag)
{
	__vector uint8_t const one = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	__vector uint8_t const zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	for (uint32_t i = 0; i < num_vectors; ++i) {
		if (any(vectors[i], tag)) {
			(t->*vector_rule)(addresses[i], if_eq(vectors[i], tag, one, zero));
		}
	}
}

template <size_t num_vectors>
addressed_vector_ref MaskTagged<num_vectors>::operator[](size_t i)
{
	return addressed_vector_ref{addresses[i], vectors[i]};
}

template <size_t num_vectors>
Iterator<MaskTagged<num_vectors> > MaskTagged<num_vectors>::begin()
{
	return Iterator<MaskTagged<num_vectors> >(*this);
}

template <size_t num_vectors>
Iterator<MaskTagged<num_vectors> > MaskTagged<num_vectors>::end()
{
	return Iterator<MaskTagged<num_vectors> >(*this, num_vectors);
}
