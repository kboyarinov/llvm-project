//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// virtual void do_deallocate(void* p, size_t bytes, size_t alignment)

#include <memory_resource>

#include "test_pmr.h"

int main(int, char**)
{
    TestResource r1(1);
    std::pmr::memory_resource* resource = &r1;

    resource->do_deallocate(nullptr, 0, 0); // expected-error{{'do_deallocate' is a private member}}

    return 0;
}
