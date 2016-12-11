/*<-
Copyright (c) 2016 Barrett Adair

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
->*/

#include <boost/callable_traits/config.hpp>
#include "test.hpp"


#ifndef CALLABLE_TRAITS_ENABLE_TRANSACTION_SAFE
int main(){}
#else

#include <boost/callable_traits/remove_transaction_safe.hpp>

template<typename Safe, typename NotSafe>
void test() {

    CT_ASSERT(std::is_same<NotSafe,  remove_transaction_safe_t<Safe>>::value);

    //sanity check
    CT_ASSERT(!std::is_same<Safe, NotSafe>::value);
}

#define TEST_TRANSACTION_SAFE(not_safe) test<not_safe transaction_safe, not_safe>()

int main() {

    TEST_TRANSACTION_SAFE(int(int) &);
    TEST_TRANSACTION_SAFE(int(*)(int));
    TEST_TRANSACTION_SAFE(int(int, ...) &&);
    TEST_TRANSACTION_SAFE(int(*)(int, ...));

    struct foo;

    TEST_TRANSACTION_SAFE(int(foo::*)(int));
    TEST_TRANSACTION_SAFE(int(foo::*)(int) const);
    TEST_TRANSACTION_SAFE(int(foo::*)(int, ...));
    TEST_TRANSACTION_SAFE(int(foo::*)(int, ...) volatile);
}

#endif

