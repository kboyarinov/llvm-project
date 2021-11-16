//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

//       reference operator[](size_type __i);
// const_reference operator[](size_type __i) const;
//
//       reference at(size_type __i);
// const_reference at(size_type __i) const;
//
//       reference front();
// const_reference front() const;
//
//       reference back();
// const_reference back() const;
// libc++ marks these as 'noexcept' (except 'at')

#include <vector>
#include <cassert>

#include "min_allocator.h"
#include "test_macros.h"

struct EqComparable {
    int value;

    EqComparable(int v) : value(v) {}

    friend bool operator==(const EqComparable& lhs, const EqComparable& rhs) {
        return lhs.value == rhs.value;
    }
};

template <typename C>
C
make(int size, int start = 0)
{
    C c;
    for (int i = 0; i < size; ++i)
        c.push_back(start + i);
    return c;
}

template <typename Vector>
void test_get_basic(Vector& c, int N = 0) {
    for (int i = 0; i < 10; ++i)
        assert(c[i] == N + i);
    for (int i = 0; i < 10; ++i)
        assert(c.at(i) == N + i);

#ifndef TEST_HAS_NO_EXCEPTIONS
    try {
        c.at(10);
        assert(false);
    } catch (std::out_of_range&) {}
#endif

    assert(c.front() == N);
    assert(c.back() == N + 9);
}

template <typename Vector>
void test_get(int N = 0) {
    Vector c = make<Vector>(10, N);
    const Vector& cc = c;
    test_get_basic(c, N);
    test_get_basic(cc, N);
}

template <typename Vector>
void test_set(int N = 0) {
    Vector c = make<Vector>(10, N);

    for (int i = 0; i < 10; ++i) {
        assert(c[i] == N + i);
        c[i] = N + i + 1;
        assert(c[i] == N + i + 1);
    }
    for (int i = 0; i < 10; ++i) {
        assert(c.at(i) == N + i + 1);
        c.at(i) = N + i + 2;
        assert(c.at(i) == N + i + 2);
    }

    assert(c.front() == N + 2);
    c.front() = N + 3;
    assert(c.front() == N + 3);

    assert(c.back() == N + 9 + 2);
    c.back() = N + 9 + 3;
    assert(c.back() == N + 9 + 3);
}

template <typename Vector>
void test() {
    test_get<Vector>();
    test_set<Vector>();

    test_get<Vector>(36);
    test_set<Vector>(36);

    Vector c;
    const Vector& cc = c;
    ASSERT_SAME_TYPE(typename Vector::reference, decltype(c[0]));
    ASSERT_SAME_TYPE(typename Vector::const_reference, decltype(cc[0]));

    ASSERT_SAME_TYPE(typename Vector::reference, decltype(c.at(0)));
    ASSERT_SAME_TYPE(typename Vector::const_reference, decltype(cc.at(0)));

    ASSERT_SAME_TYPE(typename Vector::reference, decltype(c.front()));
    ASSERT_SAME_TYPE(typename Vector::const_reference, decltype(cc.front()));

    ASSERT_SAME_TYPE(typename Vector::reference, decltype(c.back()));
    ASSERT_SAME_TYPE(typename Vector::const_reference, decltype(cc.back()));
}

int main(int, char**)
{
    test<std::vector<int> >();
    test<std::vector<EqComparable> >();
#if TEST_STD_VER >= 11
    test<std::vector<int, min_allocator<int> > >();
#endif

  return 0;
}
