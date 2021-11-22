//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// vector& operator=(const vector& c);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "allocators.h"

int main(int, char**)
{
    {
        std::vector<int, test_allocator<int> > l(3, 2, test_allocator<int>(5));
        std::vector<int, test_allocator<int> > l2(l, test_allocator<int>(3));
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == test_allocator<int>(3));
    }
    {
        std::vector<int, other_allocator<int> > l(3, 2, other_allocator<int>(5));
        std::vector<int, other_allocator<int> > l2(l, other_allocator<int>(3));
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == other_allocator<int>(5));
    }
#if TEST_STD_VER >= 11
    {
        // Test with Allocator::propagate_on_container_copy_assignment == false_type
        A2<int>::copy_called = false;
        assert(!std::allocator_traits<A1<int> >::propagate_on_container_copy_assignment::value);
        std::vector<int, A1<int> > l(3, 2, A1<int>(5));
        std::vector<int, A1<int> > l2(l, A1<int>(3));
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == A1<int>(3));
        assert(!A2<int>::copy_called);
    }
    {
        // Test with Allocator::propagate_on_container_copy_assignment == true_type
        // and equal allocators
        A3<int>::copy_called = false;
        assert(std::allocator_traits<A3<int> >::propagate_on_container_copy_assignment::value);
        std::vector<int, A3<int> > l(3, 2, A3<int>(5));
        std::vector<int, A3<int> > l2(l, A3<int>(5));
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == A3<int>(5));
        assert(A3<int>::copy_called);
    }
    {
        // Test with Allocator::propagate_on_container_copy_assignment == true_type
        // and unequal allocators
        A3<int>::copy_called = false;
        std::vector<int, A3<int> > l(3, 2, A3<int>(5));
        std::vector<int, A3<int> > l2(l, A3<int>(3));
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == A3<int>(5));
        assert(A3<int>::copy_called);
    }
    {
        std::vector<int, min_allocator<int> > l(3, 2, min_allocator<int>());
        std::vector<int, min_allocator<int> > l2(l, min_allocator<int>());
        l2 = l;
        assert(l2 == l);
        assert(l2.get_allocator() == min_allocator<int>());
    }
#endif

  return 0;
}
