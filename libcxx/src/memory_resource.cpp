// -*- C++ -*-
//===-------------------------- memory_resource.cpp -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <memory_resource>
#include <new>
#include <cstdio>

_LIBCPP_BEGIN_NAMESPACE_PMR

class _LIBCPP_TYPE_VIS __new_delete_memory_resource
    : public memory_resource
{
    void* do_allocate(size_t __bytes, size_t __alignment) override
    {
        return _VSTD::__libcpp_allocate(__bytes, __alignment);
    }

    void do_deallocate(void* __p, size_t __bytes, size_t __alignment) override
    {
        return _VSTD::__libcpp_deallocate(__p, __bytes, __alignment);
    }

    bool do_is_equal(const memory_resource& __other) const noexcept override
    {
        return this == &__other;
    }
public:
    ~__new_delete_memory_resource() override = default;
}; // class __new_delete_memory_resource

class _LIBCPP_TYPE_VIS __null_memory_resource
    : public memory_resource
{
    void* do_allocate(size_t, size_t) override
    {
        __throw_bad_alloc();
    }

    void do_deallocate(void*, size_t, size_t) override {}

    bool do_is_equal(const memory_resource& __other) const noexcept override
    {
        return this == &__other;
    }
public:
    ~__null_memory_resource() override = default;
}; // class __null_resource

static __new_delete_memory_resource __new_delete_resource;
static __null_memory_resource __null_resource;

memory_resource* new_delete_resource() _NOEXCEPT
{
    return &__new_delete_resource;
}

memory_resource* null_memory_resource() _NOEXCEPT
{
    return &__null_resource;
}

_LIBCPP_END_NAMESPACE_PMR
