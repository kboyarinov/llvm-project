//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// bool is_equal(const memory_resource& other) const noexcept

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        std::pmr::memory_resource const* resource = nullptr;
        ASSERT_SAME_TYPE(decltype(resource->is_equal(*resource)), bool);
    }
    {
        std::pmr::memory_resource const* resource = nullptr;
        ASSERT_NOEXCEPT(resource->is_equal(*resource));
    }
    {
        TestResource1 r1(1);
        auto& controller1 = r1.getController();
        std::pmr::memory_resource const* resource1 = &r1;

        TestResource1 r2(1);
        auto& controller2 = r2.getController();
        std::pmr::memory_resource const* resource2 = &r2;

        assert(resource1->is_equal(*resource2) == true);
        assert(controller1.checkIsEqualCalledEq(1));
        assert(controller2.checkIsEqualCalledEq(0));

        assert(resource2->is_equal(*resource1) == true);
        assert(controller1.checkIsEqualCalledEq(1));
        assert(controller2.checkIsEqualCalledEq(1));
    }
    {
        TestResource1 r1(1);
        auto& controller1 = r1.getController();
        std::pmr::memory_resource const* resource1 = &r1;

        TestResource1 r2(2);
        auto& controller2 = r2.getController();
        std::pmr::memory_resource const* resource2 = &r2;

        assert(resource1->is_equal(*resource2) == false);
        assert(controller1.checkIsEqualCalledEq(1));
        assert(controller2.checkIsEqualCalledEq(0));

        assert(resource2->is_equal(*resource1) == false);
        assert(controller1.checkIsEqualCalledEq(1));
        assert(controller2.checkIsEqualCalledEq(1));
    }
    {
        TestResource1 r1(1);
        auto& controller1 = r1.getController();
        std::pmr::memory_resource const* resource1 = &r1;

        TestResource2 r2(1);
        auto& controller2 = r2.getController();
        std::pmr::memory_resource const* resource2 = &r2;

        assert(resource1->is_equal(*resource2) == false);
        assert(controller1.checkIsEqualCalledEq(1));
        assert(controller2.checkIsEqualCalledEq(0));

        assert(resource2->is_equal(*resource1) == false);
        assert(controller1.checkIsEqualCalledEq(1));
        assert(controller2.checkIsEqualCalledEq(1));
    }
    {
        CopyableResource r(1);
        std::pmr::memory_resource const* resource1 = &r;

        CopyableResource r2 = r;
        std::pmr::memory_resource const* resource2 = &r2;

        assert(resource1->is_equal(*resource2) == true);
        assert(resource2->is_equal(*resource1) == true);
    }
    {
        CopyableResource r1(1);
        std::pmr::memory_resource const* resource1 = &r1;

        CopyableResource r2(2);
        std::pmr::memory_resource* resource2 = &r2;

        r2 = r1;

        assert(resource1->is_equal(*resource2) == true);
        assert(resource2->is_equal(*resource1) == true);
    }

    return 0;
}
