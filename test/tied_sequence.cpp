// Copyright (c) 2021,2023 Kohei Takahashi
// This software is released under the MIT License, see LICENSE.

#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <map>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <vector>

#include "flat_map/tied_sequence.hpp"
#include "test_case/catch2_tuple.hpp"
#include "test_case/memory.hpp"

TEST_CASE("zip_iterator", "[iterator]") {
    std::vector<int>   vi{1, 2, 3};
    std::vector<float> vf{1.1f, 2.2f, 3.3f};

    SECTION("construct") {
        [[maybe_unused]] flat_map::detail::zip_iterator i{std::make_tuple(vi.begin(), vf.begin())};
        static_assert(std::is_same_v<decltype(i)::value_type, flat_map::detail::tuple<int, float>>);
    }

    SECTION("base") {
        flat_map::detail::zip_iterator i{std::make_tuple(vi.begin(), vf.begin())};
        REQUIRE(flat_map::get_iterator<0>(i) == vi.begin());
        REQUIRE(flat_map::get_iterator<1>(i) == vf.begin());
    }

    SECTION("comparison") {
        flat_map::detail::zip_iterator b{std::make_tuple(vi.begin(), vf.begin())};
        flat_map::detail::zip_iterator e{std::make_tuple(vi.end(), vf.end())};

        REQUIRE(b == b);
        REQUIRE_FALSE(b == e);

        REQUIRE_FALSE(b != b);
        REQUIRE(b != e);

        REQUIRE_FALSE(b < b);
        REQUIRE(b < e);

        REQUIRE(b <= b);
        REQUIRE(b <= e);

        REQUIRE_FALSE(b > b);
        REQUIRE_FALSE(b > e);

        REQUIRE(b >= b);
        REQUIRE_FALSE(b >= e);
    }

    SECTION("left const comparison") {
        flat_map::detail::zip_iterator b{std::make_tuple(vi.begin(), vf.begin())};
        flat_map::detail::zip_iterator e{std::make_tuple(vi.cend(), vf.cend())};

        REQUIRE(b == b);
        REQUIRE_FALSE(b == e);

        REQUIRE_FALSE(b != b);
        REQUIRE(b != e);

        REQUIRE_FALSE(b < b);
        REQUIRE(b < e);

        REQUIRE(b <= b);
        REQUIRE(b <= e);

        REQUIRE_FALSE(b > b);
        REQUIRE_FALSE(b > e);

        REQUIRE(b >= b);
        REQUIRE_FALSE(b >= e);
    }

    SECTION("right const comparison") {
        flat_map::detail::zip_iterator b{std::make_tuple(vi.cbegin(), vf.cbegin())};
        flat_map::detail::zip_iterator e{std::make_tuple(vi.end(), vf.end())};

        REQUIRE(b == b);
        REQUIRE_FALSE(b == e);

        REQUIRE_FALSE(b != b);
        REQUIRE(b != e);

        REQUIRE_FALSE(b < b);
        REQUIRE(b < e);

        REQUIRE(b <= b);
        REQUIRE(b <= e);

        REQUIRE_FALSE(b > b);
        REQUIRE_FALSE(b > e);

        REQUIRE(b >= b);
        REQUIRE_FALSE(b >= e);
    }

    SECTION("const comparison") {
        flat_map::detail::zip_iterator b{std::make_tuple(vi.cbegin(), vf.cbegin())};
        flat_map::detail::zip_iterator e{std::make_tuple(vi.cend(), vf.cend())};

        REQUIRE(b == b);
        REQUIRE_FALSE(b == e);

        REQUIRE_FALSE(b != b);
        REQUIRE(b != e);

        REQUIRE_FALSE(b < b);
        REQUIRE(b < e);

        REQUIRE(b <= b);
        REQUIRE(b <= e);

        REQUIRE_FALSE(b > b);
        REQUIRE_FALSE(b > e);

        REQUIRE(b >= b);
        REQUIRE_FALSE(b >= e);
    }

    SECTION("forward/backward") {
        flat_map::detail::zip_iterator i{std::make_tuple(vi.begin(), vf.begin())};
        flat_map::detail::zip_iterator n{std::make_tuple(vi.begin() + 1, vf.begin() + 1)};

        REQUIRE((i + 1) == n);
        REQUIRE(i == (n - 1));

        REQUIRE((i++) == (n - 1));
        REQUIRE((i--) == n);

        REQUIRE((++i) == n);
        REQUIRE((--i) == (n - 1));

        REQUIRE((i += 1) == n);
        REQUIRE((i -= 1) == (n - 1));
    }

    SECTION("dereference") {
        flat_map::detail::zip_iterator b{std::make_tuple(vi.begin(), vf.begin())};
        static_assert(std::
                          is_same_v<decltype(b)::reference, flat_map::detail::tuple<int&, float&>>);

        decltype(b)::reference r = *b;
        auto& [i, f]             = r;

        REQUIRE(i == 1);
        REQUIRE(f == 1.1f);

        REQUIRE(&i == &vi[0]);
        REQUIRE(&f == &vf[0]);
    }

    SECTION("swap") {
        flat_map::detail::zip_iterator b{std::make_tuple(vi.cbegin(), vf.cbegin())};
        flat_map::detail::zip_iterator e{std::make_tuple(vi.cend(), vf.cend())};

        swap(b, e);

        REQUIRE(flat_map::get_iterator<0>(b) == vi.cend());
        REQUIRE(flat_map::get_iterator<1>(b) == vf.cend());
        REQUIRE(flat_map::get_iterator<0>(e) == vi.cbegin());
        REQUIRE(flat_map::get_iterator<1>(e) == vf.cbegin());
    }

    SECTION("iter_swap") {
        auto                           vvi = vi;
        auto                           vvf = vf;
        flat_map::detail::zip_iterator b{std::make_tuple(vvi.begin(), vvf.begin())};
        flat_map::detail::zip_iterator n{std::make_tuple(vvi.begin() + 1, vvf.begin() + 1)};

        iter_swap(b, n);

        REQUIRE(flat_map::get_iterator<0>(b) == vvi.begin());
        REQUIRE(flat_map::get_iterator<1>(b) == vvf.begin());
        REQUIRE(flat_map::get_iterator<0>(n) == (vvi.begin() + 1));
        REQUIRE(flat_map::get_iterator<1>(n) == (vvf.begin() + 1));

        REQUIRE(*(flat_map::get_iterator<0>(b)) == 2);
        REQUIRE(*(flat_map::get_iterator<1>(b)) == 2.2f);
        REQUIRE(*(flat_map::get_iterator<0>(n)) == 1);
        REQUIRE(*(flat_map::get_iterator<1>(n)) == 1.1f);
    }

    SECTION("value swap") {
        auto                           vvi = vi;
        auto                           vvf = vf;
        flat_map::detail::zip_iterator b{std::make_tuple(vvi.begin(), vvf.begin())};
        flat_map::detail::zip_iterator n{std::make_tuple(vvi.begin() + 1, vvf.begin() + 1)};

        swap(*b, *n);

        REQUIRE(flat_map::get_iterator<0>(b) == vvi.begin());
        REQUIRE(flat_map::get_iterator<1>(b) == vvf.begin());
        REQUIRE(flat_map::get_iterator<0>(n) == (vvi.begin() + 1));
        REQUIRE(flat_map::get_iterator<1>(n) == (vvf.begin() + 1));

        REQUIRE(*(flat_map::get_iterator<0>(b)) == 2);
        REQUIRE(*(flat_map::get_iterator<1>(b)) == 2.2f);
        REQUIRE(*(flat_map::get_iterator<0>(n)) == 1);
        REQUIRE(*(flat_map::get_iterator<1>(n)) == 1.1f);
    }
}

TEST_CASE("unzip_iterator", "[iterator]") {
    std::vector<int>   vi{1, 2, 3};
    std::vector<float> vf{1.1f, 2.2f, 3.3f};

    flat_map::detail::zip_iterator b{std::make_tuple(vi.begin(), vf.begin())};
    flat_map::detail::zip_iterator n{std::make_tuple(vi.begin() + 1, vf.begin() + 1)};

    SECTION("construct") {
        [[maybe_unused]] auto          ex0 = flat_map::detail::unzip<0>(b);
        [[maybe_unused]] auto          ex1 = flat_map::detail::unzip<1>(b);
        [[maybe_unused]] decltype(ex0) d;
        static_assert(std::is_same_v<decltype(ex0)::value_type, int>);
        static_assert(std::is_same_v<decltype(ex1)::value_type, float>);
    }

    SECTION("comparison") {
        auto b0 = flat_map::detail::unzip<0>(b);
        auto n0 = flat_map::detail::unzip<0>(n);

        REQUIRE(b0 == b0);
        REQUIRE_FALSE(b0 == n0);

        REQUIRE_FALSE(b0 != b0);
        REQUIRE(b0 != n0);

        REQUIRE_FALSE(b0 < b0);
        REQUIRE(b0 < n0);

        REQUIRE(b0 <= b0);
        REQUIRE(b0 <= n0);

        REQUIRE_FALSE(b0 > b0);
        REQUIRE_FALSE(b0 > n0);

        REQUIRE(b0 >= b0);
        REQUIRE_FALSE(b0 >= n0);
    }

    SECTION("forward/backward") {
        auto b0 = flat_map::detail::unzip<0>(b);
        auto n0 = flat_map::detail::unzip<0>(n);

        REQUIRE((b0 + 1) == n0);
        REQUIRE(b0 == (n0 - 1));

        REQUIRE((b0++) == (n0 - 1));
        REQUIRE((b0--) == n0);

        REQUIRE((++b0) == n0);
        REQUIRE((--b0) == (n0 - 1));

        REQUIRE((b0 += 1) == n0);
        REQUIRE((b0 -= 1) == (n0 - 1));
    }

    SECTION("dereference") {
        auto b0 = flat_map::detail::unzip<0>(b);
        auto b1 = flat_map::detail::unzip<1>(b);
        static_assert(std::is_same_v<decltype(b0)::reference, int&>);
        static_assert(std::is_same_v<decltype(b1)::reference, float&>);

        auto& i0 = *b0;
        auto& f1 = *b1;

        REQUIRE(i0 == 1);
        REQUIRE(f1 == 1.1f);

        REQUIRE(&i0 == &vi[0]);
        REQUIRE(&f1 == &vf[0]);
    }

    SECTION("dereference std::map with move") {
        std::map<int, float> m{
            {1, 1.1f}
        };
        auto        b0 = flat_map::detail::unzip<0>(std::make_move_iterator(m.begin()));
        auto        b1 = flat_map::detail::unzip<1>(std::make_move_iterator(m.begin()));
        const int&& i  = *b0;
        float&&     f  = *b1;
        REQUIRE(i == 1);
        REQUIRE(f == 1.1f);
    }

    SECTION("swap") {
        auto l0 = flat_map::detail::unzip<0>(b);
        auto r0 = flat_map::detail::unzip<0>(n);

        swap(l0, r0);

        REQUIRE(&*l0 == (vi.data() + 1));
        REQUIRE(&*r0 == vi.data());
    }

    SECTION("value swap") {
        using std::swap;

        auto                           vvi = vi;
        auto                           vvf = vf;
        flat_map::detail::zip_iterator b{std::make_tuple(vvi.begin(), vvf.begin())};
        flat_map::detail::zip_iterator n{std::make_tuple(vvi.begin() + 1, vvf.begin() + 1)};
        auto                           b0 = flat_map::detail::unzip<0>(b);
        auto                           n0 = flat_map::detail::unzip<0>(n);

        swap(*b0, *n0);

        REQUIRE(&*b0 == vvi.data());
        REQUIRE(&*n0 == (vvi.data() + 1));

        REQUIRE(*b0 == 2);
        REQUIRE(*n0 == 1);
    }
}

TEST_CASE("construction", "[construction]") {
    SECTION("default construction") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;
    }

    SECTION("count copies") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts(4, {1, 2});
        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{1, 2});
        REQUIRE(ts[1] == std::tuple{1, 2});
        REQUIRE(ts[2] == std::tuple{1, 2});
        REQUIRE(ts[3] == std::tuple{1, 2});
    }

    SECTION("count default-initialized") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts(4);
        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple<int, int>{});
        REQUIRE(ts[1] == std::tuple<int, int>{});
        REQUIRE(ts[2] == std::tuple<int, int>{});
        REQUIRE(ts[3] == std::tuple<int, int>{});
    }

    SECTION("from sequence") {
        std::vector v = {
            std::tuple{0, 1},
            std::tuple{2, 3},
            std::tuple{4, 5},
            std::tuple{6, 7},
        };

        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts(v.begin(), v.end());
        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});
    }

    SECTION("copy ctor") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> src(4, {1, 2});
        auto                                                        dst = src;

        REQUIRE(src.begin() != dst.begin());
        REQUIRE(src.size() == 4);
        REQUIRE(src[0] == std::tuple{1, 2});
        REQUIRE(src[1] == std::tuple{1, 2});
        REQUIRE(src[2] == std::tuple{1, 2});
        REQUIRE(src[3] == std::tuple{1, 2});
        REQUIRE(dst.size() == 4);
        REQUIRE(dst[0] == std::tuple{1, 2});
        REQUIRE(dst[1] == std::tuple{1, 2});
        REQUIRE(dst[2] == std::tuple{1, 2});
        REQUIRE(dst[3] == std::tuple{1, 2});
    }

    SECTION("move ctor") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> src(4, {1, 2});
        auto                                                        dst = std::move(src);

        REQUIRE(src.begin() != dst.begin());
        REQUIRE(src.empty());
        REQUIRE(dst.size() == 4);
        REQUIRE(dst[0] == std::tuple{1, 2});
        REQUIRE(dst[1] == std::tuple{1, 2});
        REQUIRE(dst[2] == std::tuple{1, 2});
        REQUIRE(dst[3] == std::tuple{1, 2});
    }

    SECTION("initializer_list") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});
    }

    SECTION("sequences") {
        std::vector<int> a = {0, 2};
        std::vector<int> b = {1, 3};
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts(std::move(a), std::move(b));

        REQUIRE(ts.size() == 2);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
    }
}

TEST_CASE("assignment", "[assignment]") {
    SECTION("copy assignment") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> src(4, {1, 2}), dst;
        dst = src;

        REQUIRE(src.begin() != dst.begin());
        REQUIRE(src.size() == 4);
        REQUIRE(src[0] == std::tuple{1, 2});
        REQUIRE(src[1] == std::tuple{1, 2});
        REQUIRE(src[2] == std::tuple{1, 2});
        REQUIRE(src[3] == std::tuple{1, 2});
        REQUIRE(dst.size() == 4);
        REQUIRE(dst[0] == std::tuple{1, 2});
        REQUIRE(dst[1] == std::tuple{1, 2});
        REQUIRE(dst[2] == std::tuple{1, 2});
        REQUIRE(dst[3] == std::tuple{1, 2});
    }

    SECTION("move assignment") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> src(4, {1, 2}), dst;
        dst = std::move(src);

        REQUIRE(src.begin() != dst.begin());
        REQUIRE(src.empty());
        REQUIRE(dst.size() == 4);
        REQUIRE(dst[0] == std::tuple{1, 2});
        REQUIRE(dst[1] == std::tuple{1, 2});
        REQUIRE(dst[2] == std::tuple{1, 2});
        REQUIRE(dst[3] == std::tuple{1, 2});
    }

    SECTION("initializer_list") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;
        ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});
    }

    SECTION("count copies") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;
        ts.assign(4, {1, 2});

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{1, 2});
        REQUIRE(ts[1] == std::tuple{1, 2});
        REQUIRE(ts[2] == std::tuple{1, 2});
        REQUIRE(ts[3] == std::tuple{1, 2});
    }

    SECTION("from sequence") {
        std::vector v = {
            std::tuple{0, 1},
            std::tuple{2, 3},
            std::tuple{4, 5},
            std::tuple{6, 7},
        };

        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;
        ts.assign(v.begin(), v.end());

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});
    }

    SECTION("assign initializer_list") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;
        ts.assign({
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        });

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});
    }
}

TEST_CASE("accessor", "[accessor]") {
    SECTION("at") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE(ts.at(0) == std::tuple{0, 1});
        REQUIRE(ts.at(1) == std::tuple{2, 3});
        REQUIRE(ts.at(2) == std::tuple{4, 5});
        REQUIRE(ts.at(3) == std::tuple{6, 7});
        REQUIRE_THROWS_AS(ts.at(4), std::out_of_range);
    }

    SECTION("operator[]") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});
    }

    SECTION("front back") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE(ts.front() == std::tuple{0, 1});
        REQUIRE(ts.back() == std::tuple{6, 7});
    }
}

TEST_CASE("iterator", "[iterator]") {
    SECTION("empty") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;
        REQUIRE(ts.begin() == ts.end());
        REQUIRE(ts.rbegin() == ts.rend());
    }

    SECTION("non empty") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE(ts.begin() != ts.end());
        REQUIRE(ts.rbegin() != ts.rend());
        REQUIRE(ts.rbegin().base() == ts.end());
        REQUIRE(ts.rend().base() == ts.begin());

        REQUIRE(std::next(ts.begin(), 4) == ts.end());
        REQUIRE(std::next(ts.rbegin(), 4) == ts.rend());
        REQUIRE(std::next(ts.rbegin(), 4).base() == ts.begin());
    }
}

TEST_CASE("capacity", "[capacity]") {
    SECTION("empty") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;
        REQUIRE(ts.empty());
        REQUIRE(ts.size() == 0);
        REQUIRE(ts.max_size() > 0);
    }

    SECTION("non empty") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };
        REQUIRE_FALSE(ts.empty());
        REQUIRE(ts.size() == 4);
        REQUIRE(ts.max_size() > 0);
    }

    SECTION("clear") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };
        ts.clear();
        REQUIRE(ts.size() == 0);
    }
}

TEST_CASE("insertion", "[insertion]") {
    SECTION("single") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        auto v   = decltype(ts)::value_type{3, 4};
        auto itr = ts.insert(std::next(ts.begin(), 2), v);
        REQUIRE(itr == std::next(ts.begin(), 2));

        itr = ts.insert(std::next(ts.begin(), 4), {5, 6});
        REQUIRE(itr == std::next(ts.begin(), 4));

        REQUIRE(ts.size() == 6);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{3, 4});
        REQUIRE(ts[3] == std::tuple{4, 5});
        REQUIRE(ts[4] == std::tuple{5, 6});
        REQUIRE(ts[5] == std::tuple{6, 7});
    }

    SECTION("count copies") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        auto itr = ts.insert(std::next(ts.begin(), 2), 3, {3, 4});
        REQUIRE(itr == std::next(ts.begin(), 2));

        REQUIRE(ts.size() == 7);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{3, 4});
        REQUIRE(ts[3] == std::tuple{3, 4});
        REQUIRE(ts[4] == std::tuple{3, 4});
        REQUIRE(ts[5] == std::tuple{4, 5});
        REQUIRE(ts[6] == std::tuple{6, 7});
    }

    SECTION("from sequence") {
        std::vector v = {
            std::tuple{2, 3},
            std::tuple{4, 5},
        };

        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {6, 7},
        };
        auto itr = ts.insert(std::next(ts.begin()), v.begin(), v.end());
        REQUIRE(itr == std::next(ts.begin()));

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});
    }

    SECTION("initializer_list") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {6, 7},
        };
        auto itr = ts.insert(
            std::next(ts.begin()),
            {
                std::tuple{2, 3},
                std::tuple{4, 5},
        }
        );
        REQUIRE(itr == std::next(ts.begin()));

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});
    }

    SECTION("emplace") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        auto itr = ts.emplace(std::next(ts.begin(), 2), 3, 4);
        REQUIRE(itr == std::next(ts.begin(), 2));

        REQUIRE(ts.size() == 5);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{3, 4});
        REQUIRE(ts[3] == std::tuple{4, 5});
        REQUIRE(ts[4] == std::tuple{6, 7});
    }
}

TEST_CASE("erase", "[erase]") {
    SECTION("single") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        auto itr = ts.erase(std::next(ts.begin()));
        REQUIRE(itr == std::next(ts.begin()));

        REQUIRE(ts.size() == 3);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{4, 5});
        REQUIRE(ts[2] == std::tuple{6, 7});
    }

    SECTION("range") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        auto itr = ts.erase(std::next(ts.begin(), 2), ts.end());
        REQUIRE(itr == ts.end());

        REQUIRE(ts.size() == 2);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
    }
}

TEST_CASE("tail insertion", "[insertion]") {
    SECTION("push_back") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;

        auto v = decltype(ts)::value_type{1, 2};
        ts.push_back(v);
        ts.push_back(decltype(ts)::value_type{3, 4});

        REQUIRE(ts.size() == 2);
        REQUIRE(ts[0] == std::tuple{1, 2});
        REQUIRE(ts[1] == std::tuple{3, 4});
    }

    SECTION("emplace_back") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts;

        auto ref = ts.emplace_back(1, 2);
        REQUIRE(ref == std::tuple{1, 2});

        REQUIRE(ts.size() == 1);
        REQUIRE(ts[0] == std::tuple{1, 2});
    }
}

TEST_CASE("tail erasure", "[erase]") {
    SECTION("pop_back") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        ts.pop_back();
        REQUIRE(ts.size() == 3);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
    }
}

TEST_CASE("swap", "[swap]") {
    SECTION("member") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> src =
                                                                        {
                                                                            {0, 1},
                                                                            {2, 3},
                                                                            {4, 5},
                                                                            {6, 7},
        },
                                                                    dst;

        src.swap(dst);

        REQUIRE(src.empty());
        REQUIRE(dst.size() == 4);
        REQUIRE(dst[0] == std::tuple{0, 1});
        REQUIRE(dst[1] == std::tuple{2, 3});
        REQUIRE(dst[2] == std::tuple{4, 5});
        REQUIRE(dst[3] == std::tuple{6, 7});
    }

    SECTION("free function") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> src =
                                                                        {
                                                                            {0, 1},
                                                                            {2, 3},
                                                                            {4, 5},
                                                                            {6, 7},
        },
                                                                    dst;

        swap(src, dst);

        REQUIRE(src.empty());
        REQUIRE(dst.size() == 4);
        REQUIRE(dst[0] == std::tuple{0, 1});
        REQUIRE(dst[1] == std::tuple{2, 3});
        REQUIRE(dst[2] == std::tuple{4, 5});
        REQUIRE(dst[3] == std::tuple{6, 7});
    }
}

TEST_CASE("extract", "[extract]") {
    flat_map::tied_sequence<std::vector<int>, std::vector<int>> src = {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    auto [k, v] = std::move(src).extract();
    REQUIRE(k == std::vector<int>{0, 2, 4, 6});
    REQUIRE(v == std::vector<int>{1, 3, 5, 7});
}

TEST_CASE("replace", "[replace]") {
    SECTION("valid") {
        auto k = std::vector<int>{0, 2, 4, 6};
        auto v = std::vector<int>{1, 3, 5, 7};

        flat_map::tied_sequence<std::vector<int>, std::vector<int>> src;
        src.replace(std::make_tuple(std::move(k), std::move(v)));

        {
            auto [k, v] = std::move(src).extract();
            REQUIRE(k == std::vector<int>{0, 2, 4, 6});
            REQUIRE(v == std::vector<int>{1, 3, 5, 7});
        }
    }

    SECTION("invalid") {
        auto k = std::vector<int>{0, 2, 4, 6, 8};
        auto v = std::vector<int>{1, 3, 5, 7};
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> src;

        REQUIRE_THROWS(src.replace(std::make_tuple(std::move(k), std::move(v))));

        k = std::vector<int>{0, 2, 4, 6};
        v = std::vector<int>{1, 3, 5, 7, 8};

        REQUIRE_THROWS(src.replace(std::make_tuple(std::move(k), std::move(v))));
    }
}

TEST_CASE("get_sequence", "[get_sequence]") {
    flat_map::tied_sequence<std::vector<int>, std::vector<int>> src = {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    REQUIRE(flat_map::get_sequence<0>(src) == std::vector<int>{0, 2, 4, 6});
    REQUIRE(flat_map::get_sequence<1>(src) == std::vector<int>{1, 3, 5, 7});
}

TEST_CASE("comparator", "[comparator]"){SECTION("traditional comparator"
){flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
      {0, 1},
      {2, 3},
      {4, 5},
      {6, 7},
  };

REQUIRE(ts == decltype(ts){{0, 1}, {2, 3}, {4, 5}, {6, 7}});
REQUIRE_FALSE(ts != decltype(ts){{0, 1}, {2, 3}, {4, 5}, {6, 7}});

REQUIRE_FALSE(ts == decltype(ts){{0, 1}, {2, 4}, {4, 5}, {6, 7}});
REQUIRE(ts != decltype(ts){{0, 1}, {2, 4}, {4, 5}, {6, 7}});

REQUIRE_FALSE(ts == decltype(ts){{0, 1}, {1, 3}, {4, 5}, {6, 7}});
REQUIRE(ts != decltype(ts){{0, 1}, {1, 3}, {4, 5}, {6, 7}});

REQUIRE_FALSE(ts < decltype(ts){{0, 1}, {2, 3}, {4, 5}, {6, 7}});
REQUIRE_FALSE(ts > decltype(ts){{0, 1}, {2, 3}, {4, 5}, {6, 7}});
REQUIRE(ts <= decltype(ts){{0, 1}, {2, 3}, {4, 5}, {6, 7}});
REQUIRE(ts >= decltype(ts){{0, 1}, {2, 3}, {4, 5}, {6, 7}});

REQUIRE(ts < decltype(ts){{0, 1}, {2, 4}, {4, 5}, {6, 7}});
REQUIRE_FALSE(ts > decltype(ts){{0, 1}, {2, 4}, {4, 5}, {6, 7}});
REQUIRE(ts <= decltype(ts){{0, 1}, {2, 4}, {4, 5}, {6, 7}});
REQUIRE_FALSE(ts >= decltype(ts){{0, 1}, {2, 4}, {4, 5}, {6, 7}});

REQUIRE_FALSE(ts < decltype(ts){{0, 1}, {2, 2}, {4, 5}, {6, 7}});
REQUIRE(ts > decltype(ts){{0, 1}, {2, 2}, {4, 5}, {6, 7}});
REQUIRE_FALSE(ts <= decltype(ts){{0, 1}, {2, 2}, {4, 5}, {6, 7}});
REQUIRE(ts >= decltype(ts){{0, 1}, {2, 2}, {4, 5}, {6, 7}});

REQUIRE_FALSE(ts < decltype(ts){{0, 1}, {1, 3}, {4, 5}, {6, 7}});
REQUIRE(ts > decltype(ts){{0, 1}, {1, 3}, {4, 5}, {6, 7}});
REQUIRE_FALSE(ts <= decltype(ts){{0, 1}, {1, 3}, {4, 5}, {6, 7}});
REQUIRE(ts >= decltype(ts){{0, 1}, {1, 3}, {4, 5}, {6, 7}});

REQUIRE(ts < decltype(ts){{0, 1}, {3, 3}, {4, 5}, {6, 7}});
REQUIRE_FALSE(ts > decltype(ts){{0, 1}, {3, 3}, {4, 5}, {6, 7}});
REQUIRE(ts <= decltype(ts){{0, 1}, {3, 3}, {4, 5}, {6, 7}});
REQUIRE_FALSE(ts >= decltype(ts){{0, 1}, {3, 3}, {4, 5}, {6, 7}});
}

#ifdef FLAT_MAP_HAS_THREE_WAY_COMPARISON
SECTION("three way comparison") {
    flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    REQUIRE((ts <=> decltype(ts){{0, 1}, {2, 3}, {4, 5}, {6, 7}}) ==
          std::weak_ordering::equivalent);

    REQUIRE((ts <=> decltype(ts){{0, 1}, {2, 4}, {4, 5}, {6, 7}}) ==
          std::weak_ordering::less);
    REQUIRE((ts <=> decltype(ts){{0, 1}, {2, 2}, {4, 5}, {6, 7}}) ==
          std::weak_ordering::greater);

    REQUIRE((ts <=> decltype(ts){{0, 1}, {3, 3}, {4, 5}, {6, 7}}) ==
          std::weak_ordering::less);
    REQUIRE((ts <=> decltype(ts){{0, 1}, {1, 3}, {4, 5}, {6, 7}}) ==
          std::weak_ordering::greater);
}
#endif
}

TEST_CASE("remove erase", "[erase]") {
    SECTION("erase") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        auto r = erase(ts, std::tuple{1, 2});
        REQUIRE(r == 0);
        REQUIRE(ts.size() == 4);

        r = erase(ts, std::tuple{2, 3});
        REQUIRE(r == 1);
        REQUIRE(ts.size() == 3);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{4, 5});
        REQUIRE(ts[2] == std::tuple{6, 7});
    }

    SECTION("erase") {
        flat_map::tied_sequence<std::vector<int>, std::vector<int>> ts = {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        auto r = erase_if(ts, [](auto vt) { return std::get<0>(vt) == 1; });
        REQUIRE(r == 0);
        REQUIRE(ts.size() == 4);

        r = erase_if(ts, [](auto vt) { return std::get<0>(vt) == 2; });
        REQUIRE(r == 1);
        REQUIRE(ts.size() == 3);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{4, 5});
        REQUIRE(ts[2] == std::tuple{6, 7});
    }
}

TEST_CASE("non contiguous", "[contiguous]") {
    SECTION("construct") {
        [[maybe_unused]] flat_map::tied_sequence<std::deque<int>, std::deque<int>> ts;
    }
}

TEST_CASE("allocator", "[allocator]") {
    SECTION("stateful allocator") {
        flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::deque<int, stateful_allocator<int>>>
            ts{stateful_allocator<int>{"state1"}, stateful_allocator<int>{"state2"}};

        auto alloc = ts.get_allocator();
        REQUIRE(alloc.get<0>().state == "state1");
        REQUIRE(alloc.get<1>().state == "state2");
    }

    SECTION("allocator forwarding") {
        flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::deque<int, stateful_allocator<int>>>
            ts{flat_map::forward_allocator(
                stateful_allocator<int>{"state1"},
                stateful_allocator<int>{"state2"}
            )};

        auto alloc = ts.get_allocator();
        REQUIRE(alloc.get<0>().state == "state1");
        REQUIRE(alloc.get<1>().state == "state2");
    }

    SECTION("allocator forwarding with count copies") {
        flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::deque<int, stateful_allocator<int>>>
            ts{
                4,
                {       1, 2},
                flat_map::forward_allocator(
                    stateful_allocator<int>{"state1"  },
                    stateful_allocator<int>{"state2"  }
                )
        };

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{1, 2});
        REQUIRE(ts[1] == std::tuple{1, 2});
        REQUIRE(ts[2] == std::tuple{1, 2});
        REQUIRE(ts[3] == std::tuple{1, 2});

        auto alloc = ts.get_allocator();
        REQUIRE(alloc.get<0>().state == "state1");
        REQUIRE(alloc.get<1>().state == "state2");
    }

    SECTION("allocator forwarding with count default-initialized") {
        flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::deque<int, stateful_allocator<int>>>
            ts{4,
               flat_map::forward_allocator(
                   stateful_allocator<int>{"state1"},
                   stateful_allocator<int>{"state2"}
               )};

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple<int, int>{});
        REQUIRE(ts[1] == std::tuple<int, int>{});
        REQUIRE(ts[2] == std::tuple<int, int>{});
        REQUIRE(ts[3] == std::tuple<int, int>{});

        auto alloc = ts.get_allocator();
        REQUIRE(alloc.get<0>().state == "state1");
        REQUIRE(alloc.get<1>().state == "state2");
    }

    SECTION("from sequence") {
        std::vector v = {
            std::tuple{0, 1},
            std::tuple{2, 3},
            std::tuple{4, 5},
            std::tuple{6, 7},
        };

        flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::deque<int, stateful_allocator<int>>>
            ts{v.begin(),
               v.end(),
               flat_map::forward_allocator(
                   stateful_allocator<int>{"state1"},
                   stateful_allocator<int>{"state2"}
               )};

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});

        auto alloc = ts.get_allocator();
        REQUIRE(alloc.get<0>().state == "state1");
        REQUIRE(alloc.get<1>().state == "state2");
    }

    SECTION("copy ctor") {
        flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::vector<int, stateful_allocator<int>>>
            src{
                4,
                {       1, 2},
                flat_map::forward_allocator(
                    stateful_allocator<int>{"state1"  },
                    stateful_allocator<int>{"state2"  }
                )
        };
        decltype(src) dst = {
            src,
            flat_map::forward_allocator(
                stateful_allocator<int>{"state3"},
                stateful_allocator<int>{"state4"}
            )};

        REQUIRE(src.begin() != dst.begin());
        REQUIRE(src.size() == 4);
        REQUIRE(src[0] == std::tuple{1, 2});
        REQUIRE(src[1] == std::tuple{1, 2});
        REQUIRE(src[2] == std::tuple{1, 2});
        REQUIRE(src[3] == std::tuple{1, 2});
        REQUIRE(dst.size() == 4);
        REQUIRE(dst[0] == std::tuple{1, 2});
        REQUIRE(dst[1] == std::tuple{1, 2});
        REQUIRE(dst[2] == std::tuple{1, 2});
        REQUIRE(dst[3] == std::tuple{1, 2});

        auto alloc = src.get_allocator();
        REQUIRE(alloc.get<0>().state == "state1");
        REQUIRE(alloc.get<1>().state == "state2");

        alloc = dst.get_allocator();
        REQUIRE(alloc.get<0>().state == "state3");
        REQUIRE(alloc.get<1>().state == "state4");
    }

    SECTION("move ctor") {
        flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::vector<int, stateful_allocator<int>>>
            src{
                4,
                {       1, 2},
                flat_map::forward_allocator(
                    stateful_allocator<int>{"state1"  },
                    stateful_allocator<int>{"state2"  }
                )
        };
        decltype(src) dst = {
            std::move(src),
            flat_map::forward_allocator(
                stateful_allocator<int>{"state3"},
                stateful_allocator<int>{"state4"}
            )};

        REQUIRE(src.begin() != dst.begin());
        REQUIRE(src.empty());
        REQUIRE(dst.size() == 4);
        REQUIRE(dst[0] == std::tuple{1, 2});
        REQUIRE(dst[1] == std::tuple{1, 2});
        REQUIRE(dst[2] == std::tuple{1, 2});
        REQUIRE(dst[3] == std::tuple{1, 2});

        auto alloc = src.get_allocator();
        REQUIRE(alloc.get<0>().state == "state1");
        REQUIRE(alloc.get<1>().state == "state2");

        alloc = dst.get_allocator();
        REQUIRE(alloc.get<0>().state == "state3");
        REQUIRE(alloc.get<1>().state == "state4");
    }

    SECTION("initializer_list") {
        flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::deque<int, stateful_allocator<int>>>
            ts = {
                {
                 {0, 1},
                 {2, 3},
                 {4, 5},
                 {6, 7},
                 },
                flat_map::forward_allocator(
                    stateful_allocator<int>{"state1" },
                    stateful_allocator<int>{"state2" }
                )
        };

        REQUIRE(ts.size() == 4);
        REQUIRE(ts[0] == std::tuple{0, 1});
        REQUIRE(ts[1] == std::tuple{2, 3});
        REQUIRE(ts[2] == std::tuple{4, 5});
        REQUIRE(ts[3] == std::tuple{6, 7});

        auto alloc = ts.get_allocator();
        REQUIRE(alloc.get<0>().state == "state1");
        REQUIRE(alloc.get<1>().state == "state2");
    }

    SECTION("nested") {
        flat_map::tied_sequence<flat_map::tied_sequence<
            std::vector<int, stateful_allocator<int>>,
            std::deque<int, stateful_allocator<int>>>>
            nts{flat_map::forward_allocator(flat_map::forward_allocator(
                stateful_allocator<int>{"state1"},
                stateful_allocator<int>{"state2"}
            ))};

        auto out = nts.get_allocator();
        auto in  = out.get<0>();
        REQUIRE(in.get<0>().state == "state1");
        REQUIRE(in.get<1>().state == "state2");
    }
}
