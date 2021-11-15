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

struct monotonic_buffer_resource::_BufferHolder
{
    _BufferHolder(_BufferHolder* __prev, size_t __size,
                  size_t __alignment, void* __buf)
        : __prev_holder(__prev), __buffer_size(__size),
          __buffer_alignment(__alignment), __buffer(__buf) {}

    _BufferHolder* __prev_holder;
    size_t __buffer_size;
    size_t __buffer_alignment;
    void* __buffer;
};

void* monotonic_buffer_resource::__allocate_from_next_buffer(size_t __bytes, size_t __alignment)
{
    size_t __buffer_size = __bytes > __next_buffer_size ? __bytes : __next_buffer_size;
    // Allocate the holder and next_buffer
    void* __holder_and_buffer = __upstream_resource->allocate(__buffer_size + sizeof(_BufferHolder),
                                                              __alignment);
    void* __next_buffer = reinterpret_cast<char*>(__holder_and_buffer) + sizeof(_BufferHolder);

    // Construct the holder
    ::new(__holder_and_buffer) _BufferHolder(__current_holder, __buffer_size,
                                             __alignment, __next_buffer);
    __current_holder = reinterpret_cast<_BufferHolder*>(__holder_and_buffer);

    __current_buffer = __next_buffer;
    __next_buffer_size = __next_buffer_size * __growth_factor;
    __remaining_bytes = __buffer_size;

    void* __ptr = align(__alignment, __bytes, __next_buffer, __remaining_bytes);
    _LIBCPP_ASSERT(__ptr != nullptr, nullptr);

    __current_buffer = reinterpret_cast<char*>(__current_buffer) + __bytes;
    __remaining_bytes -= __bytes;
    return __ptr;
}

void monotonic_buffer_resource::release()
{
    while (__current_holder)
    {
        _BufferHolder* __current = __current_holder;
        __current_holder = __current_holder->__prev_holder;

        // Deallocate __current
        size_t __allocated_size = sizeof(_BufferHolder) + __current->__buffer_size;
        size_t __alignment = __current->__buffer_alignment;

        __current->~_BufferHolder();
        __upstream_resource->deallocate(__current, __allocated_size, __alignment);
    }
}

_LIBCPP_END_NAMESPACE_PMR
