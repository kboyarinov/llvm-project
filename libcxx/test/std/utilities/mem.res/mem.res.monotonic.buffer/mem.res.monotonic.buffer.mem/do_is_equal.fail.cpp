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

// bool do_is_equal(const memory_resource& other) const noexcept

#include <memory_resource>
#include <cassert>

#include "test_macros.h"

int main(int, char**)
{
    {
        const std::pmr::monotonic_buffer_resource buf_resource;

        [[maybe_unused]] bool res = buf_resource.do_is_equal(buf_resource); // expected-error{{'do_is_equal' is a protected member of 'std::pmr::monotonic_buffer_resource'}}
    }
    return 0;
}
