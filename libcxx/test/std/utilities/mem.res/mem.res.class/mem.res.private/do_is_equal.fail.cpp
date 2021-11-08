//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// virtual void* do_is_equal(const memory_resource& other)

#include <memory_resource>

#include "test_pmr.h"

int main(int, char**)
{
    TestResource r1(1);
    std::pmr::memory_resource* resource = &r1;

    resource->do_is_equal(*resource); // expected-error{{'do_is_equal' is a private member}}

    return 0;
}
