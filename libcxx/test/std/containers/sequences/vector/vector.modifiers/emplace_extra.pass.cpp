//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// template <class... Args> iterator emplace(const_iterator pos, Args&&... args);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"

int main(int, char**)
{
    {
        std::vector<int> v;
        v.reserve(3);
        assert(is_contiguous_container_asan_correct(v));
        v = { 1, 2, 3 };
        v.emplace(v.begin(), v.back());
        assert(v[0] == 3);
        assert(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int> v;
        v.reserve(4);
        assert(is_contiguous_container_asan_correct(v));
        v = { 1, 2, 3 };
        v.emplace(v.begin(), v.back());
        assert(v[0] == 3);
        assert(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int, min_allocator<int>> v;
        v.reserve(3);
        assert(is_contiguous_container_asan_correct(v));
        v = { 1, 2, 3 };
        v.emplace(v.begin(), v.back());
        assert(v[0] == 3);
        assert(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int, min_allocator<int>> v;
        v.reserve(4);
        assert(is_contiguous_container_asan_correct(v));
        v = { 1, 2, 3 };
        v.emplace(v.begin(), v.back());
        assert(v[0] == 3);
        assert(is_contiguous_container_asan_correct(v));
    }
    {
        const size_t reserve = 8;
        const size_t size = 4;
        const int value = 160;
        std::vector<int> v;
        v.reserve(reserve);
        v.resize(size);

        v.emplace(v.cend(), value);

        assert(v.size() == size + 1);
        assert(v.capacity() == reserve);
        assert(v[size] == value);
    }
  return 0;
}
