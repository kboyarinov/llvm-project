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

// template <class T1, class T2, class... Args1. class... Args2>
// void polymorphic_allocator<T>::construct(std::pair<T1, T2>* p,
//                                          std::piecewise_construct_t,
//                                          std::tuple<Args1...>, std::tuple<Args2...>);

#include <memory_resource>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "uses_alloc_types.h"

template <class Pair, class Arg1, class Arg2>
void testConstructPairBasic(Arg1&& arg1, Arg2&& arg2,
                            UsesAllocatorType expected1, UsesAllocatorType expected2)
{
    std::pmr::polymorphic_allocator<int> int_alloc;
    std::pmr::polymorphic_allocator<Pair> alloc = int_alloc;

    Pair* p = alloc.allocate(1);

    int_alloc.construct(p, std::piecewise_construct, std::forward_as_tuple(std::forward<Arg1>(arg1)),
                                                     std::forward_as_tuple(std::forward<Arg2>(arg2)));
    checkConstruct<Arg1&&>(p->first, expected1);
    checkConstruct<Arg2&&>(p->second, expected2);

    p->~Pair();
    alloc.deallocate(p, 1);
}

template <class Pair>
void testConstruct(UsesAllocatorType expected1, UsesAllocatorType expected2) {
    int arg1 = 1;
    int arg2 = 2;
    const int& carg1 = arg1;
    const int& carg2 = arg2;

    testConstructBasic<PairType>(arg1, carg2, expected1, expected2);
    testConstructBasic<PairType>(carg1, arg2, expected1, expected2);
    testConstructBasic<PairType>(std::move(arg1), std::move(carg2), expected1, expected2);
    testConstructBasic<PairType>(std::move(carg1), std::move(arg2), expected1, expected2);
}

template <class Alloc>
void testDoesNotUseAndDoesNotUse() {
    using DoesNotUseType = DoesNotUsesAllocator<Alloc, /*Args = */1>;
    using PairType = std::pair<DoesNotUseType, DoesNotUseType>;
    testConstruct<PairType>(UA_None, UA_None);
}

template <class Alloc>
void testDoesNotUseAndUsesLeading() {
    using DoesNotUseType = DoesNotUsesAllocator<Alloc, /*Args = */1>;
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */1>;
    using PairType1 = std::pair<DoesNotUseType, UsesLeadingType>;
    using PairType2 = std::pair<UsesLeadingType, DoesNotUseType>;
    testConstruct<PairType1>(UA_None, UA_AllocArg);
    testConstruct<PairType2>(UA_AllocArg, UA_None);
}

template <class Alloc>
void testDoesNotUseAndUsesTrailing() {
    using DoesNotUseType = DoesNotUsesAllocator<Alloc, /*Args = */1>;
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */1>;
    using PairType1 = std::pair<DoesNotUseType, UsesTrailingType>;
    using PairType2 = std::pair<UsesTrailingType, DoesNotUseType>;
    testConstruct<PairType1>(UA_None, UA_AllocLast);
    testConstruct<PairType2>(UA_AllocLast, UA_None);
}

template <class Alloc>
void testDoesNotUseAndUsesBoth() {
    using DoesNotUseType = DoesNotUsesAllocator<Alloc, /*Args = */1>;
    using UsesBothType = UsesAllocatorV3<Alloc, /*Args = */1>;
    using PairType1 = std::pair<DoesNotUseType, UsesBothType>;
    using PairType2 = std::pair<UsesBothType, DoesNotUseType>;
    testConstruct<PairType1>(UA_None, UA_AllocArg);
    testConstruct<PairType2>(UA_AllocArg, UA_None);
}

template <class Alloc>
void testUsesLeadingAndUsesLeading() {
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */1>;
    using PairType = std::pair<UsesLeadingType, UsesLeadingType>;
    testConstruct<PairType>(UA_AllocArg, UA_AllocArg);
}

template <class Alloc>
void testUsesLeadingAndUsesTrailing() {
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */1>;
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */1>;
    using PairType1 = std::pair<UsesLeadingType, UsesTrailingType>;
    using PairType2 = std::pair<UsesTrailingType, UsesLeadingType>;
    testConstruct<PairType1>(UA_AllocArg, UA_AllocLast);
    testConstruct<PairType2>(UA_AllocLast, UA_AllocArg);
}

template <class Alloc>
void testUsesLeadingAndUsesBoth() {
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */1>;
    using UsesBothType = UsesAllocatorV3<Alloc, /*Args = */1>;
    using PairType1 = std::pair<UsesLeadingType, UsesBothType>;
    using PairType2 = std::pair<UsesBothType, UsesLeadingType>;
    testConstruct<PairType1>(UA_AllocArg, UA_AllocArg);
    testConstruct<PairType2>(UA_AllocArg, UA_AllocArg);
}

template <class Alloc>
void testUsesTrailingAndUsesTrailing() {
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */1>;
    using PairType = std::pair<UsesTrailingType, UsesTrailingType>;
    testConstruct<PairType>(UA_AllocLast, UA_AllocArg);
    testConstruct<PairType>(UA_AllocArg, UA_AllocLast);
}

template <class Alloc>
void testUsesTrailingAndUsesBoth() {
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */1>;
    using UsesBothType = UsesAllocatorV3<Alloc, /*Args = */1>;
    using PairType1 = std::pair<UsesTrailingType, UsesBothType>;
    using PairType2 = std::pair<UsesBothType, UsesTrailingType>;
    testConstruct<PairType1>(UA_AllocLast, UA_AllocArg);
    testConstruct<PairType1>(UA_AllocArg, UA_AllocLast);
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc;
        std::pair<int, int>* pair_ptr = nullptr;
        ASSERT_SAME_TYPE(decltype(alloc.construct(pair_ptr, std::piecewise_construct_t,
                                                  std::tuple{}, std::tuple{})), void);
        ASSERT_NOT_NOEXCEPT(alloc.construct(pair_ptr, std::piecewise_construct, std::tuple{}, std::tuple{}));
    }
    {
        testDoesNotUseAndDoesNotUse<std::pmr::memory_resource*>();
        testDoesNotUseAndDoesNotUse<std::pmr::polymorphic_allocator>();
    }
    {
        testDoesNotUseAndUsesLeading<std::pmr::memory_resource*>();
        testDoesNotUseAndUsesLeading<std::pmr::polymorphic_allocator>();
    }
    {
        testDoesNotUseAndUsesTrailing<std::pmr::memory_resource*>();
        testDoesNotUseAndUsesTrailing<std::pmr::polymorphic_allocator>();
    }
    {
        testDoesNotUseAndUsesBoth<std::pmr::memory_resource*>();
        testDoesNotUseAndUsesBoth<std::pmr::polymorphic_allocator>();
    }
    {
        testUsesLeadingAndUsesLeading<std::pmr::memory_resource*>();
        testUsesLeadingAndUsesLeading<std::pmr::polymorphic_allocator>();
    }
    {
        testUsesLeadingAndUsesTrailing<std::pmr::memory_resource*>();
        testUsesLeadingAndUsesTrailing<std::pmr::polymorphic_allocator>();
    }
    {
        testUsesLeadingAndUsesBoth<std::pmr::memory_resource*>();
        testUsesLeadingAndUsesBoth<std::pmr::polymorphic_allocator>();
    }
    {
        testUsesTrailingAndUsesTrailing<std::pmr::memory_resource*>();
        testUsesTrailingAndUsesTrailing<std::pmr::polymorphic_allocator>();
    }
    {
        testUsesTrailingAndUsesBoth<std::pmr::memory_resource*>();
        testUsesTrailingAndUsesBoth<std::pmr::polymorphic_allocator>();
    }

    return 0;
}
