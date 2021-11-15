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

// memory_resource* upstream_resource() const

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        TestResource r;
        const std::pmr::monotonic_buffer_resource buf_resource(&r);

        assert(buf_resource.upstream_resource() == &r);
    }
    {
        const std::pmr::monotonic_buffer_resource buf_resource(std::pmr::get_default_resource());

        assert(buf_resource.upstream_resource() == std::pmr::get_default_resource());
    }
    {
        const std::pmr::monotonic_buffer_resource buf_resource(std::pmr::null_memory_resource());

        assert(buf_resource.upstream_resource() == std::pmr::null_memory_resource());
    }
    return 0;
}
