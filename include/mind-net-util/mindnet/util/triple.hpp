//
// Created by robertvokac on 12/11/25.
//

#pragma once
#include <utility>   // for std::forward
#include <type_traits>
namespace mindnet::util {
template<typename T1, typename T2, typename T3>
struct triple
{
    T1 first;
    T2 second;
    T3 third;

    // Default constructor
    constexpr triple()
        : first(), second(), third() {}

    // Copy constructor
    constexpr triple(const T1& a, const T2& b, const T3& c)
        : first(a), second(b), third(c) {}

    // Perfect forwarding constructor
    template<typename U1, typename U2, typename U3>
    constexpr triple(U1&& a, U2&& b, U3&& c)
        : first(std::forward<U1>(a)),
          second(std::forward<U2>(b)),
          third(std::forward<U3>(c)) {}

    // Copy / Move default
    constexpr triple(const triple&) = default;
    constexpr triple(triple&&) noexcept = default;
    constexpr triple& operator=(const triple&) = default;
    constexpr triple& operator=(triple&&) noexcept = default;

    // Comparison operators
    constexpr bool operator==(const triple& other) const {
        return first == other.first &&
               second == other.second &&
               third == other.third;
    }

    constexpr bool operator!=(const triple& other) const {
        return !(*this == other);
    }

    constexpr bool operator<(const triple& other) const {
        if (first < other.first) return true;
        if (other.first < first) return false;
        if (second < other.second) return true;
        if (other.second < second) return false;
        return third < other.third;
    }
};

// CTAD helper
template<typename T1, typename T2, typename T3>
triple(T1, T2, T3) -> triple<T1, T2, T3>;

}