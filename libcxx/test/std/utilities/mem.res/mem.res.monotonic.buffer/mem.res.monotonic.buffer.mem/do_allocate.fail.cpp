//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// class monotonic_buffer_resource

// void* do_allocate(size_t bytes, size_t alignment)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"

int main(int, char**)
{
    {
        std::pmr::monotonic_buffer_resource buf_resource;

        [[maybe_unused]] void* ret = buf_resource.do_allocate(1, 1); // expected-error{{'do_allocate' is a protected member of 'std::pmr::monotonic_buffer_resource'}}
    }
    return 0;
}
