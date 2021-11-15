//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// class monotonic_buffer_resource

// void* do_allocate(size_t bytes, size_t alignment)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"

struct MonotonicResource : public std::pmr::monotonic_buffer_resource
{
    MonotonicResource(void* buffer, std::size_t initial_size,
                      std::pmr::memory_resource* upstream)
        : std::pmr::monotonic_buffer_resource(buffer, initial_size, upstream) {}
    void* allocate(size_t bytes, size_t alignment)
    {
        return do_allocate(bytes, alignment);
    }
};

std::size_t bytesForAlignment(void* buf, std::size_t alignment)
{
    char* ptr1 = reinterpret_cast<char*>(buf);
    char* ptr2 = reinterpret_cast<char*>(reinterpret_cast<std::size_t>(ptr1 + (alignment - 1)) &
                                         ~(alignment - 1));
    return static_cast<std::size_t>(ptr2 - reinterpret_cast<char*>(buf));
}

bool isAligned(void* buf, std::size_t alignment)
{
    return reinterpret_cast<std::uintptr_t>(buf) % alignment == 0;
}

void testWithAlignmend(std::size_t alignment)
{
    std::size_t first_buffer_size = 10;
    void* first_buffer = operator new(first_buffer_size);
    globalMemCounter.reset();
    MonotonicResource buf_resource(first_buffer, first_buffer_size,
                                   std::pmr::new_delete_resource());

    // Test first allocation from first_buffer
    std::size_t first_block_size = first_buffer_size / 2;
    void* ptr = buf_resource.allocate(first_block_size, alignment);
    assert(ptr != nullptr);

    std::size_t alignment_bytes = bytesForAlignment(ptr, alignment);
    void* first_block = reinterpret_cast<char*>(first_buffer) + alignment_bytes;
    assert(first_block == ptr);
    assert(isAligned(ptr, alignment));
    assert(globalMemCounter.checkNewCalledEq(0));

    // Test second allocation from upstream resource
    std::size_t second_block_size = first_buffer_size;
    ptr = buf_resource.allocate(second_block_size, alignment);
    assert(ptr != nullptr);

    assert(isAligned(ptr, alignment));
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.checkLastNewSizeGreaterThan(second_block_size));

    // Test third allocation of huge block from upstream resource
    std::size_t third_block_size = 10 * 1024;
    ptr = buf_resource.allocate(third_block_size, alignment);
    assert(ptr != nullptr);

    assert(isAligned(ptr, alignment));
    assert(globalMemCounter.checkNewCalledEq(2));
    assert(globalMemCounter.checkLastNewSizeGreaterThan(third_block_size));

    operator delete(first_buffer);
}

int main(int, char**)
{
    {
        testWithAlignmend(alignof(std::max_align_t));
        testWithAlignmend(alignof(std::max_align_t) * 2);
    }
    return 0;
}
