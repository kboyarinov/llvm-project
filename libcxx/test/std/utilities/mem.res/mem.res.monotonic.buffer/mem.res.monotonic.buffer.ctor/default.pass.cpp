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

// monotonic_buffer_resource()

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        std::pmr::monotonic_buffer_resource buffer_resource;
        assert(buffer_resource.upstream_resource() == std::pmr::get_default_resource());
        assert(buffer_resource.upstream_resource() == std::pmr::new_delete_resource());
    }
    {
        TestResource r;
        std::pmr::set_default_resource(&r);

        std::pmr::monotonic_buffer_resource buffer_resource;
        assert(buffer_resource.upstream_resource() == std::pmr::get_default_resource());
        assert(buffer_resource.upstream_resource() == &r);
    }
    return 0;
}
