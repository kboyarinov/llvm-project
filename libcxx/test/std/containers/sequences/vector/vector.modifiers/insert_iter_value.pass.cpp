//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// iterator insert(const_iterator position, const value_type& x);

#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"

int main(int, char**)
{
    {
        std::vector<int> v(100);
        const int value = 1;
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, value);
        assert(v.size() == 101);
        assert(is_contiguous_container_asan_correct(v));
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        assert(v[j] == 1);
        for (++j; j < 101; ++j)
            assert(v[j] == 0);
    }
    {
        const size_t size = 100;
        std::vector<int> v(size);
        v.reserve(size + 1);
        const int value = 1;

        // no reallocation expected
        std::vector<int>::iterator it = v.insert(v.cbegin() + size, value);

        assert(v.size() == size + 1);
        assert(is_contiguous_container_asan_correct(v));
        assert(it == v.begin() + size);
        for (size_t i = 0; i < size; ++i) {
            assert(v[i] == 0);
        }
        assert(v[size] == value);
    }
    {
        std::vector<int> v(100);
        while(v.size() < v.capacity()) v.push_back(0); // force reallocation
        size_t sz = v.size();
        const int value = 1;
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, value);
        assert(v.size() == sz + 1);
        assert(is_contiguous_container_asan_correct(v));
        assert(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        assert(v[j] == 1);
        for (++j; j < v.size(); ++j)
            assert(v[j] == 0);
    }
    {
        std::vector<int> v(100);
        while(v.size() < v.capacity()) v.push_back(0);
        v.pop_back(); v.pop_back(); // force no reallocation
        size_t sz = v.size();
        const int value = 1;
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, value);
        assert(v.size() == sz + 1);
        assert(is_contiguous_container_asan_correct(v));
        assert(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        assert(v[j] == 1);
        for (++j; j < v.size(); ++j)
            assert(v[j] == 0);
    }
    {
        std::vector<int, limited_allocator<int, 300> > v(100);
        const int value = 1;
        std::vector<int, limited_allocator<int, 300> >::iterator i = v.insert(v.cbegin() + 10, value);
        assert(v.size() == 101);
        assert(is_contiguous_container_asan_correct(v));
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        assert(v[j] == 1);
        for (++j; j < 101; ++j)
            assert(v[j] == 0);
    }
#if TEST_STD_VER >= 11
    {
        std::vector<int, min_allocator<int>> v(100);
        const int value = 1;
        std::vector<int, min_allocator<int>>::iterator i = v.insert(v.cbegin() + 10, value);
        assert(v.size() == 101);
        assert(is_contiguous_container_asan_correct(v));
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        assert(v[j] == 1);
        for (++j; j < 101; ++j)
            assert(v[j] == 0);
    }
#endif

  return 0;
}
