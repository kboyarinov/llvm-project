//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// memory_resource* get_default_resource() noexcept;
// memory_resource* set_default_resource(memory_resource* r) noexcept;

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**)
{
    TestResource r(1);
    {
        ASSERT_SAME_TYPE(decltype(std::pmr::get_default_resource()),
                         std::pmr::memory_resource*);
        ASSERT_SAME_TYPE(decltype(std::pmr::set_default_resource(nullptr)),
                         std::pmr::memory_resource*);
    }
    {
        ASSERT_NOEXCEPT(std::pmr::get_default_resource());
        ASSERT_NOEXCEPT(std::pmr::set_default_resource(nullptr));
    }
    {
        std::pmr::memory_resource* def_resource = std::pmr::get_default_resource();
        assert(def_resource != nullptr);
        assert(def_resource == std::pmr::new_delete_resource());
        assert(def_resource == std::pmr::get_default_resource());
    }
    {
        std::pmr::memory_resource* new_default = &r;
        std::pmr::memory_resource* old_default = std::pmr::set_default_resource(new_default);
        assert(old_default != nullptr);
        assert(old_default == std::pmr::new_delete_resource());

        std::pmr::memory_resource* current_default = std::pmr::get_default_resource();
        assert(current_default != nullptr);
        assert(current_default == new_default);
        assert(current_default == std::pmr::get_default_resource());
    }
    {
        std::pmr::memory_resource* old_default = std::pmr::set_default_resource(nullptr);
        assert(old_default == &r);
        std::pmr::memory_resource* current_default = std::pmr::get_default_resource();
        assert(current_default != nullptr);
        assert(current_default == std::pmr::new_delete_resource());
        assert(current_default == std::pmr::get_default_resource());
    }

    return 0;
}
