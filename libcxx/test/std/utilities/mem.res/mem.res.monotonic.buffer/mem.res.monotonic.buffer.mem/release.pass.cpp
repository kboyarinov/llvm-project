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

// void release()

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"

int main(int, char**)
{
    {
        globalMemCounter.reset();
        std::pmr::monotonic_buffer_resource buf_resource(std::pmr::new_delete_resource());

        std::size_t allocate_size = 100;

        for (std::size_t i = 0; i < 10; ++i)
        {
            [[maybe_unused]] void* ret = buf_resource.allocate(allocate_size,
                                                               alignof(std::max_align_t));
            allocate_size *= 2;
        }

        int new_count = globalMemCounter.new_called;
        assert(new_count != 0);

        buf_resource.release();

        assert(buf_resource.upstream_resource() == std::pmr::new_delete_resource());
        assert(globalMemCounter.checkDeleteCalledEq(new_count));
    }

    return 0;
}
