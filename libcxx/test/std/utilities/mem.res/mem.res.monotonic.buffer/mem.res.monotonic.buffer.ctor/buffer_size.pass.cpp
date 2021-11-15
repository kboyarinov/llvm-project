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

// monotonic_buffer_resource(void* buffer, size_t buffer_size)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        void* buffer = operator new(100, std::align_val_t(alignof(std::max_align_t)));

        globalMemCounter.reset();
        std::pmr::monotonic_buffer_resource buf_resource(buffer, 100);

        assert(buf_resource.upstream_resource() == std::pmr::get_default_resource());
        assert(buf_resource.upstream_resource() == std::pmr::new_delete_resource());

        // First allocation should be done from the buffer
        void* ret = buf_resource.allocate(1, alignof(std::max_align_t));
        assert(ret == buffer);

        // Second allocation should be done from the upstream resource
        ret = buf_resource.allocate(200, alignof(std::max_align_t));
        assert(globalMemCounter.checkLastNewSizeGreaterThan(100));

        operator delete(buffer);
    }
    {
        void* buffer = operator new(100);

        TestResource r;
        std::pmr::set_default_resource(&r);

        std::pmr::monotonic_buffer_resource buf_resource(buffer, 100);

        assert(buf_resource.upstream_resource() == std::pmr::get_default_resource());
        assert(buf_resource.upstream_resource() == &r);

        operator delete(buffer);
    }
}
