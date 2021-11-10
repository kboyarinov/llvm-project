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
// polymorphic_allocator& polymorphic_allocator<T>::operator=(const polymorphic_allocator&) = delete;

#include <memory_resource>

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc1;
        std::pmr::polymorphic_allocator<int> alloc2;

        // expected-error {{use of deleted function}}
        alloc1 = alloc2;
    }
    return 0;
}
