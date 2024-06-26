// Copyright (c) 2021 Kohei Takahashi
// This software is released under the MIT License, see LICENSE.

#pragma once

// libc++
#ifdef __has_include
#    if __has_include(<__config>)
#        include <__config>
#    elif __has_include(<bits/c++config.h>) || defined(__GLIBCXX__)
#        include <bits/c++config.h>
#    endif
#endif

#if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION
#    define FLATMAP_BEGIN_STD _LIBCPP_BEGIN_NAMESPACE_STD
#    define FLATMAP_END_STD   _LIBCPP_END_NAMESPACE_STD
#elif defined(__GLIBCXX__)
#    define FLATMAP_BEGIN_STD \
        namespace std {       \
        _GLIBCXX_BEGIN_NAMESPACE_VERSION
#    define FLATMAP_END_STD            \
        _GLIBCXX_END_NAMESPACE_VERSION \
        }
#else
#    define FLATMAP_BEGIN_STD namespace std {
#    define FLATMAP_END_STD   }
#endif

FLATMAP_BEGIN_STD
template <typename T, typename Allocator>
class vector;
template <typename Key, typename T, typename Compare, typename Allocator>
class map;
template <typename Key, typename T, typename Compare, typename Allocator>
class multimap;
template <typename Key, typename Compare, typename Allocator>
class set;
template <typename Key, typename Compare, typename Allocator>
class multiset;
FLATMAP_END_STD

namespace flat_map {
template <typename Key, typename T, typename Compare, typename Container>
class flat_map;
template <typename Key, typename T, typename Compare, typename Container>
class flat_multimap;
template <typename Key, typename Compare, typename Container>
class flat_set;
template <typename Key, typename Compare, typename Container>
class flat_multiset;
template <typename... Sequences>
class tied_sequence;
}  // namespace flat_map
