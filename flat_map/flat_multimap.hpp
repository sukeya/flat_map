// Copyright (c) 2021 Kohei Takahashi
// This software is released under the MIT License, see LICENSE.

#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <vector>

#include "flat_map/__config.hpp"
#include "flat_map/__flat_tree.hpp"
#include "flat_map/__fwd.hpp"
#include "flat_map/__type_traits.hpp"
#include "flat_map/enum.hpp"

namespace flat_map {

template <
    typename Key,
    typename T,
    typename Compare   = std::less<Key>,
    typename Container = std::vector<std::pair<Key, T>>>
class flat_multimap
    : private detail::
          _flat_tree_base<flat_multimap<Key, T, Compare, Container>, Key, Compare, Container> {
    using _super = typename flat_multimap::_flat_tree_base;

    // To lookup private comparator
    friend _super;

    static constexpr range_order_t<range_order::sorted> _order{};

   public:
    using key_type               = typename _super::key_type;
    using mapped_type            = T;
    using value_type             = typename _super::value_type;
    using size_type              = typename _super::size_type;
    using difference_type        = typename _super::difference_type;
    using key_compare            = typename _super::key_compare;
    using allocator_type         = typename _super::allocator_type;
    using reference              = typename _super::reference;
    using const_reference        = typename _super::const_reference;
    using pointer                = typename _super::pointer;
    using const_pointer          = typename _super::const_pointer;
    using iterator               = typename _super::iterator;
    using const_iterator         = typename _super::const_iterator;
    using reverse_iterator       = typename _super::reverse_iterator;
    using const_reverse_iterator = typename _super::const_reverse_iterator;
    using node_type              = typename _super::node_type;
    struct value_compare {
       protected:
        Compare c;

        value_compare(Compare c) : c{std::move(c)} {}

       public:
        bool operator()(const_reference lhs, const_reference rhs) const {
            return c(std::get<0>(lhs), std::get<0>(rhs));
        }
    };

   private:
    struct _comparator final : value_compare {
        _comparator(Compare const& comp) : value_compare{comp} {}

        using value_compare::operator();

        template <typename K, typename = std::enable_if_t<!std::is_convertible_v<K, value_type>>>
        auto operator()(const_reference lhs, K const& rhs) const {
            return this->c(std::get<0>(lhs), rhs);
        }

        template <typename K, typename = std::enable_if_t<!std::is_convertible_v<K, value_type>>>
        auto operator()(K const& lhs, const_reference rhs) const {
            return this->c(lhs, std::get<0>(rhs));
        }
    };

    template <typename V>
    static auto& _key_extractor(V const& value) {
        return std::get<0>(value);
    }

   public:
    flat_multimap() = default;

    explicit flat_multimap(Compare const& comp, allocator_type const& alloc = allocator_type())
        : _super{comp, alloc} {}

    explicit flat_multimap(allocator_type const& alloc) : _super{alloc} {}

    template <typename InputIterator>
    flat_multimap(
        InputIterator         first,
        InputIterator         last,
        Compare const&        comp  = Compare(),
        allocator_type const& alloc = allocator_type()
    )
        : _super{comp, alloc} {
        this->_initialize_container(first, last);
    }

    template <typename InputIterator>
    flat_multimap(InputIterator first, InputIterator last, allocator_type const& alloc)
        : _super{alloc} {
        this->_initialize_container(first, last);
    }

    flat_multimap(flat_multimap const& other) = default;
    flat_multimap(flat_multimap const& other, allocator_type const& alloc) : _super{other, alloc} {}

    flat_multimap(flat_multimap&& other) = default;
    flat_multimap(flat_multimap&& other, allocator_type const& alloc)
        : _super{std::move(other), alloc} {}

    flat_multimap(
        std::initializer_list<value_type> init,
        Compare const&                    comp  = Compare(),
        allocator_type const&             alloc = allocator_type()
    )
        : _super{comp, alloc} {
        this->_initialize_container(init.begin(), init.end());
    }

    flat_multimap(std::initializer_list<value_type> init, allocator_type const& alloc)
        : _super{alloc} {
        this->_initialize_container(init.begin(), init.end());
    }

    explicit flat_multimap(range_order order, Container const& cont) : _super{order, cont} {}

    explicit flat_multimap(
        range_order           order,
        Container const&      cont,
        Compare const&        comp,
        allocator_type const& alloc = allocator_type()
    )
        : _super{
            order, Container{cont, alloc},
             comp
    } {}

    explicit flat_multimap(range_order order, Container const& cont, allocator_type const& alloc)
        : _super{
            order, Container{cont, alloc}
    } {}

    explicit flat_multimap(range_order order, Container&& cont) : _super{order, std::move(cont)} {}

    explicit flat_multimap(
        range_order           order,
        Container&&           cont,
        Compare const&        comp,
        allocator_type const& alloc = allocator_type()
    )
        : _super{
            order, Container{std::move(cont), alloc},
             comp
    } {}

    explicit flat_multimap(range_order order, Container&& cont, allocator_type const& alloc)
        : _super{
            order, Container{std::move(cont), alloc}
    } {}

    flat_multimap& operator=(flat_multimap const& other) = default;

    flat_multimap& operator=(flat_multimap&& other
    ) noexcept(std::is_nothrow_move_assignable_v<_super>)
#if FLAT_MAP_WORKAROUND(FLAT_MAP_COMPILER_GCC, < FLAT_MAP_COMPILER_VERSION(10))
    {
        _super::operator=(std::move(other));
        return *this;
    }
#else
        = default;
#endif

    flat_multimap& operator=(std::initializer_list<value_type> ilist) {
        this->_initialize_container(ilist.begin(), ilist.end());
        return *this;
    }

    using _super::get_allocator;

    using _super::begin;
    using _super::cbegin;
    using _super::cend;
    using _super::crbegin;
    using _super::crend;
    using _super::end;
    using _super::rbegin;
    using _super::rend;

    using _super::clear;
    using _super::empty;
    using _super::max_size;
    using _super::size;

    using _super::emplace;
    using _super::emplace_hint;
    using _super::insert;

    using _super::erase;

    void swap(flat_multimap& other) noexcept(noexcept(this->_super::swap(other))) {
        _super::swap(other);
    }

    using _super::extract;
    using _super::replace;
    using _super::get_container;

    template <typename Comp, typename Allocator>
    void merge(std::map<key_type, mapped_type, Comp, Allocator>& source) {
        this->_merge(source, std::false_type{});
    }

    template <typename Comp, typename Allocator>
    void merge(std::map<key_type, mapped_type, Comp, Allocator>&& source) {
        this->_merge(source, std::false_type{});
    }

    template <typename Comp, typename Allocator>
    void merge(std::multimap<key_type, mapped_type, Comp, Allocator>& source) {
        this->_merge(source, std::true_type{});
    }

    template <typename Comp, typename Allocator>
    void merge(std::multimap<key_type, mapped_type, Comp, Allocator>&& source) {
        this->_merge(source, std::true_type{});
    }

    // extension
    template <typename Comp, typename Cont>
    void merge(flat_map<key_type, mapped_type, Comp, Cont>& source) {
        this->_merge(source, std::false_type{});
    }

    // extension
    template <typename Comp, typename Cont>
    void merge(flat_map<key_type, mapped_type, Comp, Cont>&& source) {
        this->_merge(source, std::false_type{});
    }

    // extension
    template <typename Comp, typename Cont>
    void merge(flat_multimap<key_type, mapped_type, Comp, Cont>& source) {
        this->_merge(source, std::true_type{});
    }

    // extension
    template <typename Comp, typename Cont>
    void merge(flat_multimap<key_type, mapped_type, Comp, Cont>&& source) {
        this->_merge(source, std::true_type{});
    }

    using _super::contains;
    using _super::count;
    using _super::equal_range;
    using _super::find;
    using _super::key_comp;
    using _super::lower_bound;
    using _super::upper_bound;
    using _super::value_comp;
};

template <typename Key, typename T, typename Compare, typename Container>
bool operator==(
    flat_multimap<Key, T, Compare, Container> const& lhs,
    flat_multimap<Key, T, Compare, Container> const& rhs
) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

#ifndef FLAT_MAP_HAS_THREE_WAY_COMPARISON
template <typename Key, typename T, typename Compare, typename Container>
bool operator!=(
    flat_multimap<Key, T, Compare, Container> const& lhs,
    flat_multimap<Key, T, Compare, Container> const& rhs
) {
    return !(lhs == rhs);
}

template <typename Key, typename T, typename Compare, typename Container>
bool operator<(
    flat_multimap<Key, T, Compare, Container> const& lhs,
    flat_multimap<Key, T, Compare, Container> const& rhs
) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Key, typename T, typename Compare, typename Container>
bool operator<=(
    flat_multimap<Key, T, Compare, Container> const& lhs,
    flat_multimap<Key, T, Compare, Container> const& rhs
) {
    return !(rhs < lhs);
}

template <typename Key, typename T, typename Compare, typename Container>
bool operator>(
    flat_multimap<Key, T, Compare, Container> const& lhs,
    flat_multimap<Key, T, Compare, Container> const& rhs
) {
    return rhs < lhs;
}

template <typename Key, typename T, typename Compare, typename Container>
bool operator>=(
    flat_multimap<Key, T, Compare, Container> const& lhs,
    flat_multimap<Key, T, Compare, Container> const& rhs
) {
    return !(lhs < rhs);
}
#else
template <typename Key, typename T, typename Compare, typename Container>
auto operator<=>(
    flat_multimap<Key, T, Compare, Container> const& lhs,
    flat_multimap<Key, T, Compare, Container> const& rhs
) {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
#endif

template <typename Key, typename T, typename Compare, typename Container>
void swap(
    flat_multimap<Key, T, Compare, Container>& lhs, flat_multimap<Key, T, Compare, Container>& rhs
) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

template <typename Key, typename T, typename Compare, typename Container, typename Pred>
constexpr typename flat_multimap<Key, T, Compare, Container>::size_type erase_if(
    flat_multimap<Key, T, Compare, Container>& c, Pred pred
) {
    auto itr = std::remove_if(c.begin(), c.end(), std::forward<Pred>(pred));
    auto r   = std::distance(itr, c.end());
    c.erase(itr, c.end());
    return r;
}

template <
    typename InputIterator,
    typename Compare   = std::less<detail::iter_key_t<InputIterator>>,
    typename Allocator = typename detail::iter_cont_t<InputIterator>::allocator_type,
    typename           = std::enable_if_t<!detail::is_allocator_v<Compare>>,
    typename           = std::enable_if_t<detail::is_allocator_v<Allocator>>>
flat_multimap(InputIterator, InputIterator, Compare = Compare(), Allocator = Allocator())
    -> flat_multimap<
        detail::iter_key_t<InputIterator>,
        detail::iter_val_t<InputIterator>,
        Compare,
        detail::iter_cont_t<InputIterator, Allocator>>;

template <
    typename Key,
    typename T,
    typename Compare   = std::less<Key>,
    typename Allocator = typename std::vector<std::pair<Key, T>>::allocator_type,
    typename           = std::enable_if_t<!detail::is_allocator_v<Compare>>,
    typename           = std::enable_if_t<detail::is_allocator_v<Allocator>>>
flat_multimap(
    std::initializer_list<std::pair<Key, T>>, Compare = Compare(), Allocator = Allocator()
) -> flat_multimap<Key, T, Compare, std::vector<std::pair<Key, T>, Allocator>>;

template <
    typename InputIterator,
    typename Allocator,
    typename = std::enable_if_t<detail::is_allocator_v<Allocator>>>
flat_multimap(InputIterator, InputIterator, Allocator) -> flat_multimap<
    detail::iter_key_t<InputIterator>,
    detail::iter_val_t<InputIterator>,
    std::less<detail::iter_key_t<InputIterator>>,
    detail::iter_cont_t<InputIterator, Allocator>>;

template <
    typename Key,
    typename T,
    typename Allocator,
    typename = std::enable_if_t<detail::is_allocator_v<Allocator>>>
flat_multimap(std::initializer_list<std::pair<Key, T>>, Allocator)
    -> flat_multimap<Key, T, std::less<Key>, std::vector<std::pair<Key, T>, Allocator>>;

}  // namespace flat_map
