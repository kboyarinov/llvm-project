//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// virtual ~memory_resource();

#include <memory_resource>
#include <cassert>
#include <new>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        static_assert(std::has_virtual_destructor_v<std::pmr::memory_resource>);
        static_assert(std::is_nothrow_destructible_v<std::pmr::memory_resource>);
        static_assert(std::is_abstract_v<std::pmr::memory_resource>);
    }
    {
        std::pmr::memory_resource* resource = new TestResource(42);
        assert(TestResource::resource_alive == 1);
        assert(TestResource::resource_constructed == 1);
        assert(TestResource::resource_destructed == 0);

        delete resource;

        assert(TestResource::resource_alive == 0);
        assert(TestResource::resource_constructed == 1);
        assert(TestResource::resource_destructed == 1);
    }
    return 0;
}