#pragma once
#include <type_traits>
#include <boost/utility/enable_if.hpp>

namespace libnux::vx::detail {

template <typename T, typename = void>
struct IsVector : public std::false_type
{};

template <typename T>
struct IsVector<T, typename boost::enable_if_has_type<typename T::Data>::type>
    : public std::true_type
{};

template <typename T, typename = void>
struct IsVectorHalfRow : public std::false_type
{};

template <typename T>
struct IsVectorHalfRow<T, typename boost::enable_if_has_type<typename T::Quarter>::type>
    : public std::true_type
{};

template <typename T, typename = void>
struct IsVectorRow : public std::false_type
{};

template <typename T>
struct IsVectorRow<T, typename boost::enable_if_has_type<typename T::HalfRow>::type>
    : public std::true_type
{};

} // namespace libnux::vx::detail
