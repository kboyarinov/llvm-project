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

// void* do_deallocate(void* buffer, size_t bytes, size_t alignment)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"

struct MonotonicResource : std::pmr::monotonic_buffer_resource
{
    MonotonicResource(std::pmr::memory_resource* upstream)
        : std::pmr::monotonic_buffer_resource(upstream) {}
    void deallocate(void* buf, std::size_t bytes, std::size_t alignment)
    {
        do_deallocate(buf, bytes, alignment);
    }
};

int main(int, char**)
{
    {
        globalMemCounter.reset();
        MonotonicResource buf_resource = std::pmr::new_delete_resource();


        void* ptr = buf_resource.allocate(50, alignof(std::max_align_t));
        buf_resource.deallocate(ptr, 50, alignof(std::max_align_t));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        ptr = buf_resource.allocate(1, alignof(std::max_align_t));
        buf_resource.deallocate(ptr, 1, alignof(std::max_align_t));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        buf_resource.deallocate(ptr, 10000, 15000);
        assert(globalMemCounter.checkDeleteCalledEq(0));

        ptr = buf_resource.allocate(500, alignof(std::max_align_t));
        buf_resource.deallocate(ptr, 500, 4);
        assert(globalMemCounter.checkDeleteCalledEq(0));
    }

    return 0;
}
