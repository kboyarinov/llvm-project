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

struct MonotonicResource : std::pmr::monotonic_buffer_resource
{
    MonotonicResource() = default;
    MonotonicResource(std::pmr::memory_resource* upstream)
        : std::pmr::monotonic_buffer_resource(upstream) {}
    bool is_equal(const std::pmr::memory_resource& other) const noexcept
    {
        return do_is_equal(other);
    }
};

int main(int, char**)
{
    {
        MonotonicResource buf_resource;
        assert(buf_resource.is_equal(buf_resource));
    }
    {
        MonotonicResource buf_resource = std::pmr::null_memory_resource();
        assert(buf_resource.is_equal(buf_resource));
        assert(!buf_resource.is_equal(*std::pmr::null_memory_resource()));
    }
    {
        MonotonicResource buf_resource1;
        MonotonicResource buf_resource2;

        assert(!buf_resource1.is_equal(buf_resource2));
    }

    return 0;
}
