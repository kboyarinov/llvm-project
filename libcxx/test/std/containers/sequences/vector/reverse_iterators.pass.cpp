//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// reverse_iterator       rbegin();
// reverse_iterator       rend();
// const_reverse_iterator rbegin()  const;
// const_reverse_iterator rend()    const;
// const_reverse_iterator crbegin() const;
// const_reverse_iterator crend()   const;

#include <vector>
#include <cassert>
#include <iterator>

#include "min_allocator.h"

template <class Vector>
void check_vector_reverse_iterators() {
    {
        Vector vec;
        typename Vector::reverse_iterator i = vec.rbegin();
        typename Vector::reverse_iterator j = vec.rend();

        assert(std::distance(i, j) == 0);
        assert(i == j);

        typename Vector::const_reverse_iterator ci = vec.crbegin();
        typename Vector::const_reverse_iterator cj = vec.crend();

        assert(std::distance(ci, cj) == 0);
        assert(ci == cj);
    }
    {
        Vector vec;
        vec.reserve(10);
        for (int i = 0; i < 10; ++i)
            vec.push_back(i);
        int iterations = 0;

        for (typename Vector::const_reverse_iterator it = vec.crbegin(); it != vec.crend(); ++it) {
            assert(*it == (10 - iterations - 1));
            ++iterations;
        }
        assert(iterations == 10);
        iterations = 0;

        const Vector& cvec = vec;

        for (typename Vector::const_reverse_iterator it = cvec.rbegin(); it != cvec.rend(); ++it) {
            assert(*it == (10 - iterations - 1));
            ++iterations;
        }
        assert(iterations == 10);
        iterations = 0;

        for (typename Vector::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) {
            assert(*it == (10 - iterations - 1));
            *it = 40;
            assert(*it == 40);
            ++iterations;
        }
        assert(iterations == 10);

        assert(std::distance(vec.rbegin(), vec.rend()) == 10);
        assert(std::distance(cvec.rbegin(), cvec.rend()) == 10);
        assert(std::distance(vec.crbegin(), vec.crend()) == 10);
    }
}

int main(int, char**) {
    check_vector_reverse_iterators<std::vector<int> >();
    check_vector_reverse_iterators<std::vector<int, min_allocator<int> > >();

    return 0;
}
