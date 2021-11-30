//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <map>

// class map

// allocator_type get_allocator() const

#include <map>
#include <cassert>
#include <string>

#include "test_allocator.h"
#include "test_macros.h"

int main(int, char**) {
    {
        std::allocator<std::pair<const int, std::string> > alloc;
        const std::map<int, std::string> m(alloc);
        assert(m.get_allocator() == alloc);
    }
#if TEST_STD_VER >= 11
    {
        other_allocator<std::pair<const int, std::string> > alloc(1);
        const std::map<int, std::string, std::less<int>,
                       other_allocator<std::pair<const int, std::string>>> m(alloc);
        assert(m.get_allocator() == alloc);
    }
#endif
    return 0;
}
