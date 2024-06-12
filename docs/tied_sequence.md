# tied_sequence

```cpp
#include <flat_map/tied_sequence.hpp>

template <typename... Sequences>
class tied_sequence;
```

**Requirements**

- Each `Sequences` should meet [*Container*](https://en.cppreference.com/w/cpp/named_req/Container), [*AllocatorAwareContainer*](https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer), [*SequenceContainer*](https://en.cppreference.com/w/cpp/named_req/SequenceContainer), and [*ReversibleContainer*](https://en.cppreference.com/w/cpp/named_req/ReversibleContainer).

## Example

`flat_map` (and its variants), in this library, provides Array of Structure (AOS) style by default.
Use `tied_sequence` for `Container` template parameter to achieve Structure of Array (SOA) style flat map.

```cpp
#include <flat_map/flat_map.hpp>
#include <flat_map/tied_sequence.hpp>

flat_map::flat_map<
  /* Key */ int,
  /* T */ HugeValueType,
  /* Compare */ std::less<int>,
  /* Container */ flat_map::tied_sequence<std::vector<int>, std::deque<HugeValueType>>
> tied_map;
```

## Member types

```cpp
using value_type = std::tuple<typename Sequences::value_type...>;
using allocator_type = /* unspecified */;
using size_type = std::size_t;
using difference_type = std::ptrdiff_t;
using reference = std::tuple<typename Sequences::reference...>;
using const_reference = std::tuple<typename Sequences::const_reference...>;
using pointer = std::tuple<typename Sequences::pointer...>;
using const_pointer = std::tuple<typename Sequences::const_pointer...>;
using iterator = /* unspecified */;
using const_iterator = /* unspecified */;
using reverse_iterator = std::reverse_iterator<iterator>;
using const_reverse_iterator = std::reverse_iterator<const_iterator>;
```

## Constructors

```cpp
constexpr tied_sequence();
```

**Exceptions**

No exception only if for all sequences meet all of
- `std::is_nothrow_default_constructible_v<Sequence> == true`

```cpp
constexpr explicit tied_sequence(allocator_type const& alloc) noexcept;
constexpr explicit tied_sequence(typename Sequences::allocator_type const&... alloc) noexcept;
```

Construct sequences form corresponding allocator.

```cpp
constexpr tied_sequence(size_type count, value_type const& value);
constexpr tied_sequence(size_type count, value_type const& value, allocator_type const& alloc);
constexpr tied_sequence(size_type count, value_type const& value, typename Sequences::allocator_type const&... alloc);
constexpr tied_sequence(size_type count);
constexpr tied_sequence(size_type count, allocator_type const& alloc);
constexpr tied_sequence(size_type count, typename Sequences::allocator_type const&... alloc);
```

Construct sequence containers holding `count` copies of `value` or default-value.

```cpp
template <typename InputIterator>
constexpr tied_sequence(InputIterator first, InputIterator last);

template <typename InputIterator>
constexpr tied_sequence(InputIterator first, InputIterator last, allocator_type const& alloc);

template <typename InputIterator>
constexpr tied_sequence(InputIterator first, InputIterator last, typename Sequences::allocator_type const&... alloc);
```

Construct containers from `[first, last)`.

**Pre requirements**

`InputIterator` should meet [*InputIterator*](https://en.cppreference.com/w/cpp/named_req/InputIterator).


```cpp
constexpr tied_sequence(tied_sequence const& other);
constexpr tied_sequence(tied_sequence const& other, allocator_type const& alloc);
constexpr tied_sequence(tied_sequence const& other, typename Sequences::allocator_type const&... alloc);
```

Copy from other.

```cpp
constexpr tied_sequence(tied_sequence&& other);
constexpr tied_sequence(tied_sequence&& other, allocator_type const& alloc);
constexpr tied_sequence(tied_sequence&& other, typename Sequences::allocator_type const&... alloc);
```

Move entire elements from other.


```cpp
constexpr tied_sequence(std::initializer_list<value_type> init);
constexpr tied_sequence(std::initializer_list<value_type> init, allocator_type const& alloc);
constexpr tied_sequence(std::initializer_list<value_type> init, typename Sequences::allocator_type const&... alloc);
```

Construct from init.

## Assignments

```cpp
constexpr tied_sequence& operator=(tied_sequence const& other);
```

Copy from other.

```cpp
constexpr tied_sequence& operator=(tied_sequence&& other);
```

Move entire elements from other.

**Exceptions**

No exception only if each sequences meet all of
- `std::allocator_traits<typename Sequence::allocator_type>::propagate_on_container_move_assignment::value == true` and
- `std::allocator_traits<typename Sequence::allocator_type>::is_always_equal::value == true`

```cpp
constexpr tied_sequence& operator=(std::initializer_list<value_type> ilist);
```

**Complexity**

Linear.

```cpp
constexpr void assign(size_type count, value_type const& value);
```

Replace with `count` copies of `value` for each sequences.

**Complexity**

Linear.

```cpp
template <typename InputIterator>
constexpr void assign(InputIterator first, InputIterator last);
```

Replace containers from `[first, last)`.

**Pre requirements**

`InputIterator` should meet [*InputIterator*](https://en.cppreference.com/w/cpp/named_req/InputIterator).

```cpp
constexpr void assign(std::initializer_list<value_type> ilist);
```

## Allocator

```cpp
constexpr allocator_type get_allocator() const noexcept;
```

## Element access

### at

```cpp
constexpr reference at(size_type pos);
constexpr const_reference at(size_type pos);
```

**Complexity**

Worse case of `Sequences`.

### operator[]

```cpp
constexpr reference operator[](size_type pos);
constexpr const_reference operator[](size_type pos);
```

**Complexity**

Worse case of `Sequences`.

### front

```cpp
constexpr reference front();
constexpr const_reference front();
```

**Complexity**

Worse case of `Sequences`.

### back

```cpp
constexpr reference back() { return *std::prev(end()); }
constexpr const_reference back() const { return *std::prev(end()); }
```

**Complexity**

Worse case of `Sequences`.

## Iterators

### begin

```cpp
constexpr iterator begin() noexcept;
constexpr const_iterator begin() noexcept;
```

### cbegin

```cpp
constexpr const_iterator cbegin() const noexcept;
```

### end

```cpp
constexpr iterator end() noexcept;
constexpr const_iterator end() const noexcept;
```

### cend

```cpp
constexpr const_iterator cend() const noexcept;
```

### rbegin

```cpp
constexpr reverse_iterator rbegin() noexcept;
constexpr const_reverse_iterator rbegin() const noexcept;
```

### crbegin

```cpp
constexpr const_reverse_iterator crbegin() const noexcept;
```

### rend

```cpp
constexpr reverse_iterator rend() noexcept;
constexpr const_reverse_iterator rend() const noexcept;
```

### crend

```cpp
constexpr const_reverse_iterator crend() const noexcept;
```

## Capacity

### empty
```cpp
constexpr bool empty() const noexcept;
```

### size

```cpp
constexpr size_t size() const noexcept;
```

### max_size
```cpp
constexpr size_t max_size() const noexcept;
```

## Modifiers

### clear

```cpp
constexpr void clear() noexcept;
```

### insert

```cpp
constexpr iterator insert(const_iterator pos, value_type const& value);
constexpr iterator insert(const_iterator pos, value_type&& value);
```

Insert a `value`.

```cpp
constexpr iterator insert(const_iterator pos, size_type count, value_type const& value);
```

Insert `count` copies of `value`.

```cpp
template <typename InputIterator>
constexpr iterator insert(const_iterator pos, InputIterator first, InputIterator last);
```

Insert values from `[first, last)`.

```cpp
constexpr iterator insert(const_iterator pos, std::initializer_list<value_type> ilist);
```

Insert values from ilist.

### emplace

```cpp
template <typename... Args>
constexpr iterator emplace(const_iterator pos, Args&&... args);
```

Equivalent to `insert(pos, value_type(std::forward<Args>(args)...))`.

```cpp
template <typename... Args>
constexpr iterator emplace(const_iterator pos, std::piecewise_construct_t, Args&&... args);
```

Insert new elements with piecewise construction.

### erase

```cpp
constexpr iterator erase(const_iterator pos);
constexpr iterator erase(const_iterator first, const_iterator last);
```

**Return value**

An iterator that next to erased elements.

### push_back

```cpp
constexpr void push_back(value_type const& value);
constexpr void push_back(value_type&& value);
```

### emplace_back

```cpp
template <typename... Args>
constexpr reference emplace_back(Args&&... args);
```

**Return value**

Reference to inserted value.

### pop_back

```cpp
constexpr void pop_back();
```

### swap

```cpp
constexpr void swap(tied_sequence& other);
```

Swap elements with other.

**Exceptions**

No exception only if for all sequences meet all of
- `std::allocator_traits<typename Sequence::allocator_type>::propagate_on_container_swap::value == true`
- `std::allocator_traits<typename Sequence::allocator_type>::is_always_equal::value == true`

### extract

```cpp
constexpr std::tuple<Sequences...> extract() &&;
```

Extract the internal container of `*this`.

**Return value**
The internal container of `*this`.

### replace

```cpp
constexpr void replace(std::tuple<Sequences...>&& seq);
```

Replace the internal container of `*this` with `seq`.

**Exceptions**
No exception only if all sequences have the same length.

## Non member functions

### operator==

```cpp
template <typename... Sequences>
constexpr bool operator==(tied_sequence<Sequences...> const& lhs, tied_sequence<Sequences...> const& rhs);
```

### operator!=

```cpp
template <typename... Sequences>
bool operator!=(tied_sequence<Sequences...> const& lhs, tied_sequence<Sequences...> const& rhs);
```

**Standard**

This function is removed if
- `__cpp_impl_three_way_comparison` is defined, and
- `__cpp_lib_three_way_comparison` is defined.

### operator<

```cpp
template <typename... Sequences>
bool operator<(tied_sequence<Sequences...> const& lhs, tied_sequence<Sequences...> const& rhs);
```

**Standard**

This function is removed if
- `__cpp_impl_three_way_comparison` is defined, and
- `__cpp_lib_three_way_comparison` is defined.

### operator<=

```cpp
template <typename... Sequences>
bool operator<=(tied_sequence<Sequences...> const& lhs, tied_sequence<Sequences...> const& rhs);
```

**Standard**

This function is removed if
- `__cpp_impl_three_way_comparison` is defined, and
- `__cpp_lib_three_way_comparison` is defined.

### operator>

```cpp
template <typename... Sequences>
bool operator>(tied_sequence<Sequences...> const& lhs, tied_sequence<Sequences...> const& rhs);
```

**Standard**

This function is removed if
- `__cpp_impl_three_way_comparison` is defined, and
- `__cpp_lib_three_way_comparison` is defined.

### operator>=

```cpp
template <typename... Sequences>
bool operator>=(tied_sequence<Sequences...> const& lhs, tied_sequence<Sequences...> const& rhs);
```

**Standard**

This function is removed if
- `__cpp_impl_three_way_comparison` is defined, and
- `__cpp_lib_three_way_comparison` is defined.

### operator<=>

```cpp
template <typename... Sequences>
constexpr /* see below */ operator<=>(tied_sequence<Sequences...> const& lhs, tied_sequence<Sequences...> const& rhs);
```

**Return value**

`std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())`

**Standard**

This function is defined only if
- `__cpp_impl_three_way_comparison` is defined, and
- `__cpp_lib_three_way_comparison` is defined.

### swap

```
template <typename... Sequences>
constexpr void swap(tied_sequence<Sequences...> const& lhs, tied_sequence<Sequences...> const& rhs);
```

**Exceptions**

No exception only if `noexcept(lhs.swap(rhs))` is true.

### erase

```cpp
template <typename... Sequences, typename U>
constexpr typename tied_sequence<Sequences...>::size_type erase(tied_sequence<Sequences...>& c, U const& value);
```

Erase every elements which equivalent to `value`.

### erase_if

```cpp
template <typename... Sequences, typename Pred>
constexpr typename tied_sequence<Sequences...>::size_type erase_if(tied_sequence<Sequences...>& c, Pred pred);
```

Erase every elements which `pred` returned true.

### forward_allocator

```cpp
template <typename... Allocators>
/* unspecified */ forward_allocator(Allocators&&... alloc);
```

Forward allocators as `tied_sequence::allocator_type`.
