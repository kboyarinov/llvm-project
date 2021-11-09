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

#ifndef __LIBCPP_HAS_NO_ATOMIC_HEADER
#include <atomic>
#elif !defined(_LIBCPP_HAS_NO_THREADS)
#include <mutex>
#endif

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

#ifndef __LIBCPP_HAS_NO_ATOMIC_HEADER
struct __default_resource_holder
{
    atomic<memory_resource*> __resource_ptr = new_delete_resource();

    memory_resource* get() _NOEXCEPT
    {
        return __resource_ptr.load(memory_order_acquire);
    }

    memory_resource* exchange(memory_resource* __desired) _NOEXCEPT
    {
        __desired = __desired != nullptr ? __desired : new_delete_resource();
        return __resource_ptr.exchange(__desired, memory_order_release);
    }
};
#elif !defined(_LIBCPP_HAS_NO_THREADS)
struct __default_resource_holder
{
    mutex __resource_mutex;
    memory_resource* __resource_ptr = new_delete_resource();

    memory_resource* get() _NOEXCEPT
    {
        lock_guard<mutex> g(__resource_lock);
        return __resource_ptr;
    }

    memory_resource* exchange(memory_resource* __desired) _NOEXCEPT
    {
        __desired = __desired != nullptr ? __desired : new_delete_resource();
        lock_guard<mutex> g(__resource_lock);
        return exchange(__resource_ptr, __desired);
    }
};
#else
struct __default_resource_holder
{
    memory_resource* __resource_ptr = new_delete_resource();

    memory_resource* get() _NOEXCEPT
    {
        return __resource_ptr;
    }

    memory_resource* exchange(memory_resource* __desired) _NOEXCEPT
    {
        __desired = __desired != nullptr ? __desired : new_delete_resource();
        return exchange(__resource_ptr, __desired);
    }
};
#endif

static __default_resource_holder __default_resource;

memory_resource* get_default_resource() _NOEXCEPT
{
    return __default_resource.get();
}

memory_resource* set_default_resource(memory_resource* __desired) _NOEXCEPT
{
    return __default_resource.exchange(__desired);
}

_LIBCPP_END_NAMESPACE_PMR
