//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// template <class T> class polymorphic_allocator

// template <class U>
// void polymorphic_allocator<T>::destroy(U* ptr)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"

struct Destroyable {
    static int count;

    Destroyable() { ++count; }
    ~Destroyable() { --count; }
};

int Destroyable::count = 0;

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc;
        int* ptr = nullptr;
        ASSERT_SAME_TYPE(decltype(alloc.destroy(ptr)), void);
        ASSERT_NOT_NOEXCEPT(alloc.destroy(ptr));
    }
    {
        std::pmr::polymorphic_allocator<Destroyable> alloc;
        std::pmr::polymorphic_allocator<int> int_alloc;
        Destroyable* ptr = alloc.allocate(1);

        alloc.construct(ptr);
        assert(Destroyable::count == 1);

        int_alloc.destroy(ptr);
        assert(Destroyable::count == 0);

        alloc.deallocate(ptr, 1);
    }

    return 0;
}
