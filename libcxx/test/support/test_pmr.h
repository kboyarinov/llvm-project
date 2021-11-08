//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef SUPPORT_TEST_PMR_H
#define SUPPORT_TEST_PMR_H

// Copy of the part of test_memory_resource.h, but uses std::pmr::polymorphic_allocator

#include <memory_resource>
#include <memory>
#include <type_traits>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include "test_macros.h"
#include "controlled_allocators.h"
#include "uses_alloc_types.h"

template <class ProviderT, int = 0>
class TestResourceImp : public std::pmr::memory_resource
{
public:
    static int resource_alive;
    static int resource_constructed;
    static int resource_destructed;

    static void resetStatics() {
        assert(resource_alive == 0);
        resource_alive = 0;
        resource_constructed = 0;
        resource_destructed = 0;
    }

    using memory_resource = std::pmr::memory_resource;
    using Provider = ProviderT;

    int value;

    explicit TestResourceImp(int val = 0) : value(val) {
        ++resource_alive;
        ++resource_constructed;
    }

    ~TestResourceImp() noexcept {
        --resource_alive;
        ++resource_destructed;
    }

    void reset() { C.reset(); P.reset(); }
    AllocController& getController() { return C; }

    bool checkAlloc(void* p, std::size_t s, std::size_t a) const
      { return C.checkAlloc(p, s, a); }

    bool checkDealloc(void* p, std::size_t s, std::size_t a) const
      { return C.checkDealloc(p, s, a); }

    bool checkIsEqualCalledEq(int n) const { return C.checkIsEqualCalledEq(n); }

protected:
    virtual void * do_allocate(std::size_t s, std::size_t a) {
        if (C.throw_on_alloc) {
#ifndef TEST_HAS_NO_EXCEPTIONS
            throw TestException{};
#else
            assert(false);
#endif
        }
        void* ret = P.allocate(s, a);
        C.countAlloc(ret, s, a);
        return ret;
    }

    virtual void do_deallocate(void * p, std::size_t s, std::size_t a) {
        C.countDealloc(p, s, a);
        P.deallocate(p, s, a);
    }

    virtual bool do_is_equal(memory_resource const & other) const noexcept {
        C.countIsEqual();
        TestResourceImp const * o = dynamic_cast<TestResourceImp const *>(&other);
        return o && o->value == value;
    }
private:
    mutable AllocController C;
    mutable Provider P;
    DISALLOW_COPY(TestResourceImp);
};

template <class Provider, int N>
int TestResourceImp<Provider, N>::resource_alive = 0;

template <class Provider, int N>
int TestResourceImp<Provider, N>::resource_constructed = 0;

template <class Provider, int N>
int TestResourceImp<Provider, N>::resource_destructed = 0;

template <size_t Size = 4096 * 10> // 10 pages worth of memory.
struct BufferProvider {
    char buffer[Size];
    void* next = &buffer;
    size_t space = Size;

    BufferProvider() {}

    void* allocate(size_t s, size_t a) {
        void* ret = std::align(s, a, next, space);
        if (ret == nullptr) {
#ifndef TEST_HAS_NO_EXCEPTIONS
            throw std::bad_alloc();
#else
            assert(false);
#endif
        }

        return ret;
    }

    void deallocate(void*, size_t, size_t) {}

    void reset() {
        next = &buffer;
        space = Size;
    }
private:
    DISALLOW_COPY(BufferProvider);
};

using TestResource  = TestResourceImp<BufferProvider<>, 0>;
using TestResource1 = TestResourceImp<BufferProvider<>, 1>;
using TestResource2 = TestResourceImp<BufferProvider<>, 2>;

struct CopyableResource : public std::pmr::memory_resource {
    int value;

    CopyableResource(int v) : value(v) {}
    CopyableResource(const CopyableResource&) = default;
    CopyableResource& operator=(const CopyableResource&) = default;
    ~CopyableResource() override = default;

    void* do_allocate(std::size_t size, std::size_t align) override {
        return ::operator new(size, std::align_val_t(align));
    }

    void do_deallocate(void* ptr, std::size_t, std::size_t) override {
        ::operator delete(ptr);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return value == dynamic_cast<const CopyableResource&>(other).value;
    }
};

#endif /* SUPPORT_TEST_PMR_H */
