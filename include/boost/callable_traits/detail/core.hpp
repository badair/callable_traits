/*

@Copyright Barrett Adair 2016-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

*/

#ifndef BOOST_CLBL_TRTS_DETAIL_CORE_HPP
#define BOOST_CLBL_TRTS_DETAIL_CORE_HPP

#include <boost/callable_traits/detail/utility.hpp>
#include <boost/callable_traits/detail/traits.hpp>
#include <boost/callable_traits/detail/function_object.hpp>
#include <boost/callable_traits/detail/function.hpp>
#include <boost/callable_traits/detail/pmf.hpp>
#include <boost/callable_traits/detail/pmd.hpp>


namespace boost { namespace callable_traits { namespace detail {

template<typename T, typename = typename traits<T>::is_function_reference>
struct remove_nonfunction_reference : std::remove_reference<T> {};

template<typename T>
struct remove_nonfunction_reference<T, std::true_type> { using type = T; };

template<typename T>
using shallow_decay_except_func_ref = typename std::remove_cv<
    typename remove_nonfunction_reference<T>::type>::type;

template<typename T>
using is_abominable = std::integral_constant<bool, 
    function<T>::value && (function<T>::is_const_member::value
    || function<T>::is_volatile_member::value
    || function<T>::is_reference_member::value)>;

template<typename T, typename = is_abominable<T>>
struct add_lvalue_reference_safe_impl : std::add_lvalue_reference<T> {};

template<typename T>
struct add_lvalue_reference_safe_impl<T, std::true_type> { using type = T; };

template<typename T>
using add_lvalue_reference_safe = typename add_lvalue_reference_safe_impl<T>::type;

template<typename T, typename = is_abominable<T>>
struct add_rvalue_reference_safe_impl : std::add_rvalue_reference<T> {};

template<typename T>
struct add_rvalue_reference_safe_impl<T, std::true_type> { using type = T; };

template<typename T>
using add_rvalue_reference_safe = typename add_rvalue_reference_safe_impl<T>::type;

template<typename T, typename U>
struct copy_cv_ref_impl {

    using consted = typename std::conditional<
        std::is_const<typename std::remove_reference<U>::type>::value,
        typename std::add_const<typename std::remove_reference<T>::type>::type,
        typename std::remove_reference<T>::type>::type;

    using volatiled = typename std::conditional<
        std::is_volatile<typename std::remove_reference<U>::type>::value,
        typename std::add_volatile<consted>::type,
        consted>::type;

    using l_reffed = typename std::conditional<
        std::is_lvalue_reference<U>::value && !is_abominable<T>::value,
        add_lvalue_reference_safe<volatiled>,
        volatiled>::type;

    using type = typename std::conditional<
        std::is_rvalue_reference<U>::value && !is_abominable<T>::value,
        add_rvalue_reference_safe<l_reffed>,
        l_reffed>::type;
};

template<typename T, typename U>
using copy_cv_ref = typename copy_cv_ref_impl<T, U>::type;

}}} // namespace boost::callable_traits::detail

#endif // #ifndef BOOST_CLBL_TRTS_DETAIL_CORE_HPP
