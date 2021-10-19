//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// allocator_type get_allocator() const

#include <vector>
#include <cassert>

template <class Vector>
bool check_vector_get_allocator(typename Vec::allocator_type alloc) {
    const Vector vec(alloc);
    return vec.get_allocator() == alloc;
}

int main(int, char**) {
    {
        typedef std::vector<int> vec_type;
        std::allocator<int> alloc;
        assert(check_vector_get_allocator<vec_type>(alloc));
    }
    {
        typedef std::vector<float> vec_type;
        std::allocator<float> alloc;
        assert(check_vector_get_allocator<vec_type>(alloc));
    }
    {
        typedef std::vector<int, min_allocator<int> > vec_type;
        min_allocator<int> alloc;
        assert(check_vector_get_allocator<vec_type>(alloc));
    }
    {
        typedef std::vector<float, min_allocator<float> > vec_type;
        min_allocator<float> alloc;
        assert(check_vector_get_allocator<vec_type>(alloc));
    }
    {
        typedef std::vector<int, test_allocator<int> > vec_type;
        test_allocator<int> alloc(5);
        assert(check_vector_get_allocator<vec_type>(alloc));
    }
    {
        typedef std::vector<float, test_allocator<float> > vec_type;
        test_allocator<float> alloc(6);
        assert(check_vector_get_allocator<vec_type>(alloc));
    }

    return 0;
}