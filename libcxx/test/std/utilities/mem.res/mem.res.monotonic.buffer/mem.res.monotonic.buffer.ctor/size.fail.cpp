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

// explicit monotonic_buffer_resource(size_t initial_size)

#include <memory_resource>

int main(int, char**)
{
    {
        std::pmr::monotonic_buffer_resource buf_resource = 42UL; // expected-error{{no viable conversion from 'unsigned long' to 'std::pmr::monotonic_buffer_resource'}}
    }
    return 0;
}
