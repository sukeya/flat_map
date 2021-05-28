// Copyright (c) 2021 Kohei Takahashi
// This software is released under the MIT License, see LICENSE.

#include "flat_map/flat_map.hpp"

#include <catch2/catch.hpp>
#include <iterator>

template <typename Key, typename T, typename Compare = std::less<Key>>
using FLAT_MAP = flat_map::flat_map<Key, T, Compare, CONTAINER<std::pair<Key, T>>>;

template <typename T>
struct wrap
{
    T value;
    wrap(T value) : value(value) {}
};

template <typename T> bool operator<(T lhs, wrap<T> rhs) { return lhs < rhs.value; }
template <typename T> bool operator<(wrap<T> lhs, T rhs) { return lhs.value < rhs; }

TEST_CASE("construction", "[construction]")
{
    SECTION("default construction")
    {
        FLAT_MAP<int, int> fm;
    }

    SECTION("iter construction")
    {
        std::vector<std::pair<int, int>> v =
        {
            {6, 7},
            {4, 5},
            {2, 3},
            {0, 1},
        };
        FLAT_MAP<int, int> fm{v.begin(), v.end()};

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("init list construction")
    {
        FLAT_MAP<int, int> fm =
        {
            {6, 7},
            {4, 5},
            {2, 3},
            {0, 1},
        };

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("copy construction")
    {
        FLAT_MAP<int, int> fm =
        {
            {6, 7},
            {4, 5},
            {2, 3},
            {0, 1},
        };
        auto* ptr = &*fm.begin();

        auto copy = fm;

        REQUIRE(ptr == &*fm.begin());
        REQUIRE(ptr != &*copy.begin());
        REQUIRE(copy.begin() != fm.begin());

        auto itr = copy.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == copy.end());
    }

    SECTION("move construction")
    {
        FLAT_MAP<int, int> fm =
        {
            {6, 7},
            {4, 5},
            {2, 3},
            {0, 1},
        };
        auto* ptr = &*fm.begin();

        auto move = std::move(fm);

        REQUIRE(ptr == &*move.begin());
        REQUIRE(move.begin() != fm.begin());
        REQUIRE(fm.begin() == fm.end());

        auto itr = move.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == move.end());
    }
}

TEST_CASE("assignment", "[assignment]")
{
    SECTION("copy assignment")
    {
        FLAT_MAP<int, int> fm =
        {
            {6, 7},
            {4, 5},
            {2, 3},
            {0, 1},
        };
        auto* ptr = &*fm.begin();

        FLAT_MAP<int, int> copy =
        {
            {1, 1},
        };
        copy = fm;

        REQUIRE(ptr == &*fm.begin());
        REQUIRE(ptr != &*copy.begin());
        REQUIRE(copy.begin() != fm.begin());

        auto itr = copy.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == copy.end());
    }

    SECTION("move assignment")
    {
        FLAT_MAP<int, int> fm =
        {
            {6, 7},
            {4, 5},
            {2, 3},
            {0, 1},
        };
        auto* ptr = &*fm.begin();

        FLAT_MAP<int, int> move =
        {
            {1, 1},
        };
        move = std::move(fm);

        REQUIRE(ptr == &*move.begin());
        REQUIRE(move.begin() != fm.begin());
        REQUIRE(fm.begin() == fm.end());

        auto itr = move.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == move.end());
    }
}

TEST_CASE("size", "[size]")
{
    SECTION("empty")
    {
        FLAT_MAP<int, int> fm;

        REQUIRE(fm.empty());
        REQUIRE(fm.size() == 0);
    }

    SECTION("non empty")
    {
        FLAT_MAP<int, int> fm =
        {
            {1, 1},
        };

        REQUIRE_FALSE(fm.empty());
        REQUIRE(fm.size() == 1);
    }

    SECTION("clear")
    {
        FLAT_MAP<int, int> fm =
        {
            {1, 1},
        };

        fm.clear();
        REQUIRE(fm.empty());
    }
}

TEST_CASE("equal_range", "[equal_range]")
{
    FLAT_MAP<int, int> const fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    SECTION("not found")
    {
        auto [first, last] = fm.equal_range(3);
        REQUIRE(first != fm.end());
        REQUIRE(first == last);
        REQUIRE(first->first == 4);
    }

    SECTION("found")
    {
        auto [first, last] = fm.equal_range(2);
        REQUIRE(first != fm.end());
        REQUIRE(std::next(first) == last);
        REQUIRE(first->first == 2);
    }
}

TEST_CASE("equal_range with transparent", "[equal_range]")
{
    FLAT_MAP<int, int, std::less<>> const fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    SECTION("not found")
    {
        auto [first, last] = fm.equal_range(wrap{3});
        REQUIRE(first != fm.end());
        REQUIRE(first == last);
        REQUIRE(first->first == 4);
    }

    SECTION("found")
    {
        auto [first, last] = fm.equal_range(wrap{2});
        REQUIRE(first != fm.end());
        REQUIRE(std::next(first) == last);
        REQUIRE(first->first == 2);
    }
}

TEST_CASE("lower_bound", "[lower_bound]")
{
    FLAT_MAP<int, int> const fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    SECTION("not equal")
    {
        auto itr = fm.lower_bound(3);
        REQUIRE(itr != fm.end());
        REQUIRE(itr->first == 4);
    }

    SECTION("equal")
    {
        auto itr = fm.lower_bound(2);
        REQUIRE(itr != fm.end());
        REQUIRE(itr->first == 2);
    }

    SECTION("not found")
    {
        auto itr = fm.lower_bound(9);
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("lower_bound with transparent", "[lower_bound]")
{
    FLAT_MAP<int, int, std::less<>> const fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    SECTION("not equal")
    {
        auto itr = fm.lower_bound(wrap{3});
        REQUIRE(itr != fm.end());
        REQUIRE(itr->first == 4);
    }

    SECTION("equal")
    {
        auto itr = fm.lower_bound(wrap{2});
        REQUIRE(itr != fm.end());
        REQUIRE(itr->first == 2);
    }

    SECTION("not found")
    {
        auto itr = fm.lower_bound(wrap{9});
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("upper_bound", "[upper_bound]")
{
    FLAT_MAP<int, int> const fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    SECTION("not equal")
    {
        auto itr = fm.upper_bound(3);
        REQUIRE(itr != fm.end());
        REQUIRE(itr->first == 4);
    }

    SECTION("equal")
    {
        auto itr = fm.upper_bound(2);
        REQUIRE(itr != fm.end());
        REQUIRE(itr->first == 4);
    }

    SECTION("not found")
    {
        auto itr = fm.upper_bound(9);
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("upper_bound with transparent", "[upper_bound]")
{
    FLAT_MAP<int, int, std::less<>> const fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    SECTION("not equal")
    {
        auto itr = fm.upper_bound(wrap{3});
        REQUIRE(itr != fm.end());
        REQUIRE(itr->first == 4);
    }

    SECTION("equal")
    {
        auto itr = fm.upper_bound(wrap{2});
        REQUIRE(itr != fm.end());
        REQUIRE(itr->first == 4);
    }

    SECTION("not found")
    {
        auto itr = fm.upper_bound(wrap{9});
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("accessor", "[accessor]")
{
    FLAT_MAP<int, int> fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    SECTION("found")
    {
        auto itr = fm.find(2);
        REQUIRE(itr != fm.end());
        REQUIRE(std::distance(fm.begin(), itr) == 1);
        REQUIRE(itr->first == 2);
    }

    SECTION("not found")
    {
        auto itr = fm.find(3);
        REQUIRE(itr == fm.end());
    }

    SECTION("counting")
    {
        REQUIRE(fm.count(2) == 1);
        REQUIRE(fm.count(3) == 0);
    }

    SECTION("contains")
    {
        REQUIRE(fm.contains(4));
        REQUIRE_FALSE(fm.contains(5));
    }

    SECTION("found on at")
    {
        auto& value = fm.at(2);
        REQUIRE(value == 3);
    }

    SECTION("not found on at")
    {
        REQUIRE_THROWS_AS(fm.at(3), std::out_of_range);
    }

    SECTION("found on op[]")
    {
        auto& value = fm[2];
        REQUIRE(value == 3);
    }

    SECTION("not found on op[]")
    {
        auto& value = fm[3];
        REQUIRE(fm.size() == 5);
        REQUIRE(value == int{});

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == int{});
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("accessor with transparent", "[accessor]")
{
    FLAT_MAP<int, int, std::less<>> fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    SECTION("found")
    {
        auto itr = fm.find(wrap{2});
        REQUIRE(itr != fm.end());
        REQUIRE(std::distance(fm.begin(), itr) == 1);
        REQUIRE(itr->first == 2);
    }

    SECTION("not found")
    {
        auto itr = fm.find(wrap{3});
        REQUIRE(itr == fm.end());
    }

    SECTION("counting")
    {
        REQUIRE(fm.count(2) == 1);
        REQUIRE(fm.count(3) == 0);
    }

    SECTION("contains")
    {
        REQUIRE(fm.contains(wrap{4}));
        REQUIRE_FALSE(fm.contains(wrap{5}));
    }
}

TEST_CASE("insertion", "[insertion]")
{
    SECTION("insert")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto [itr, inserted] = fm.insert({8, 9});
            REQUIRE(inserted);
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto [itr, inserted] = fm.insert({3, 4});
            REQUIRE(inserted);
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto [itr, inserted] = fm.insert({2, 5});
            REQUIRE_FALSE(inserted);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert with helpful hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.insert(fm.end(), {8, 9}); // 7
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto itr = fm.insert(std::next(fm.begin(), 2), {3, 4}); // 1
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.insert(std::next(fm.begin(), 1), {2, 5}); // 4
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert with annoying hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.insert(std::next(fm.begin()), {5, 6}); // 5
            REQUIRE(itr->first == 5);
            REQUIRE(itr->second == 6);
            REQUIRE(std::distance(fm.begin(), itr) == 3);
        }

        {
            auto itr = fm.insert(fm.end(), {3, 4}); // 8
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.insert(std::next(fm.begin(), 3), {1, 2}); // 2
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 1);
            REQUIRE(itr->second == 2);
            REQUIRE(std::distance(fm.begin(), itr) == 1);
        }

        {
            auto itr = fm.insert(std::next(fm.begin(), 2), {2, 5}); // 3
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        {
            auto itr = fm.insert(std::next(fm.begin()), {6, 9}); // 6
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 6);
            REQUIRE(itr->second == 7);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 2);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 5);
        REQUIRE(itr->second == 6);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert range")
    {
        std::vector<std::pair<int, int>> v =
        {
            {3, 9},
            {1, 3},
            {6, 4},
            {1, 2},
        };
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        fm.insert(v.begin(), v.end());

        REQUIRE(fm.size() == 6);

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert initializer_list")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        fm.insert({
            {3, 9},
            {1, 3},
            {6, 4},
            {1, 2},
        });

        REQUIRE(fm.size() == 6);

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert sorted range")
    {
        std::vector<std::pair<int, int>> v =
        {
            {1, 3},
            {1, 2},
            {3, 9},
            {6, 4},
        };
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        fm.insert_sorted(v.begin(), v.end());

        REQUIRE(fm.size() == 6);

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert sorted initializer_list")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        fm.insert_sorted({
            {1, 3},
            {1, 2},
            {3, 9},
            {6, 4},
        });

        REQUIRE(fm.size() == 6);

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("erase", "[erase]")
{
    SECTION("erase by key")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE(fm.erase(5) == 0);
        REQUIRE(fm.size() == 4);

        REQUIRE(fm.erase(2) == 1);
        REQUIRE(fm.size() == 3);

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("node", "[insertion][erase]")
{
    SECTION("extract")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        auto node = fm.extract(5);
        REQUIRE(fm.size() == 4);
        REQUIRE_FALSE(node.value.has_value());

        node = fm.extract(2);
        REQUIRE(fm.size() == 3);
        REQUIRE(node.value.has_value());
        REQUIRE(node.value->first == 2);
        REQUIRE(node.value->second == 3);

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    using node_type = FLAT_MAP<int, int>::node_type;

    SECTION("insert")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto [itr, inserted, node] = fm.insert(node_type{{8, 9}});
            REQUIRE(inserted);
            REQUIRE_FALSE(node.value.has_value());
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto [itr, inserted, node] = fm.insert(node_type{{3, 4}});
            REQUIRE(inserted);
            REQUIRE_FALSE(node.value.has_value());
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto [itr, inserted, node] = fm.insert(node_type{{2, 5}});
            REQUIRE_FALSE(inserted);
            REQUIRE(node.value->first == 2);
            REQUIRE(node.value->second == 5);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert with helpful hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.insert(fm.end(), node_type{{8, 9}}); // 7
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto itr = fm.insert(std::next(fm.begin(), 2), node_type{{3, 4}}); // 1
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.insert(std::next(fm.begin(), 1), node_type{{2, 5}}); // 4
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert with annoying hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.insert(std::next(fm.begin()), node_type{{5, 6}}); // 5
            REQUIRE(itr->first == 5);
            REQUIRE(itr->second == 6);
            REQUIRE(std::distance(fm.begin(), itr) == 3);
        }

        {
            auto itr = fm.insert(fm.end(), node_type{{3, 4}}); // 8
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.insert(std::next(fm.begin(), 3), node_type{{1, 2}}); // 2
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 1);
            REQUIRE(itr->second == 2);
            REQUIRE(std::distance(fm.begin(), itr) == 1);
        }

        {
            auto itr = fm.insert(std::next(fm.begin(), 2), node_type{{2, 5}}); // 3
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        {
            auto itr = fm.insert(std::next(fm.begin()), node_type{{6, 9}}); // 6
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 6);
            REQUIRE(itr->second == 7);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 2);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 5);
        REQUIRE(itr->second == 6);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("insert or assign", "[insertion]")
{
    SECTION("insert or assign")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto [itr, inserted] = fm.insert_or_assign(8, 9);
            REQUIRE(inserted);
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto [itr, inserted] = fm.insert_or_assign(3, 4);
            REQUIRE(inserted);
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto [itr, inserted] = fm.insert_or_assign(2, 5);
            REQUIRE_FALSE(inserted);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 5);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert or assign with helpful hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.insert_or_assign(fm.end(), 8, 9); // 7
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto itr = fm.insert_or_assign(std::next(fm.begin(), 2), 3, 4); // 1
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.insert_or_assign(std::next(fm.begin(), 1), 2, 5); // 4
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 5);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("insert or assign with annoying hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.insert_or_assign(std::next(fm.begin()), 5, 6); // 5
            REQUIRE(itr->first == 5);
            REQUIRE(itr->second == 6);
            REQUIRE(std::distance(fm.begin(), itr) == 3);
        }

        {
            auto itr = fm.insert_or_assign(fm.end(), 3, 4); // 8
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.insert_or_assign(std::next(fm.begin(), 3), 1, 2); // 2
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 1);
            REQUIRE(itr->second == 2);
            REQUIRE(std::distance(fm.begin(), itr) == 1);
        }

        {
            auto itr = fm.insert_or_assign(std::next(fm.begin(), 2), 2, 5); // 3
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 5);
        }

        {
            auto itr = fm.insert_or_assign(std::next(fm.begin()), 6, 9); // 6
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 6);
            REQUIRE(itr->second == 9);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 2);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 5);
        REQUIRE(itr->second == 6);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("emplace insertion", "[insertion]")
{
    SECTION("emplace")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto [itr, inserted] = fm.emplace(8, 9);
            REQUIRE(inserted);
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto [itr, inserted] = fm.emplace(3, 4);
            REQUIRE(inserted);
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto [itr, inserted] = fm.emplace(2, 5);
            REQUIRE_FALSE(inserted);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("emplace with helpful hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.emplace_hint(fm.end(), 8, 9); // 7
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto itr = fm.emplace_hint(std::next(fm.begin(), 2), 3, 4); // 1
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.emplace_hint(std::next(fm.begin(), 1), 2, 5); // 4
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("emplace with annoying hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.emplace_hint(std::next(fm.begin()), 5, 6); // 5
            REQUIRE(itr->first == 5);
            REQUIRE(itr->second == 6);
            REQUIRE(std::distance(fm.begin(), itr) == 3);
        }

        {
            auto itr = fm.emplace_hint(fm.end(), 3, 4); // 8
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.emplace_hint(std::next(fm.begin(), 3), 1, 2); // 2
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 1);
            REQUIRE(itr->second == 2);
            REQUIRE(std::distance(fm.begin(), itr) == 1);
        }

        {
            auto itr = fm.emplace_hint(std::next(fm.begin(), 2), 2, 5); // 3
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        {
            auto itr = fm.emplace_hint(std::next(fm.begin()), 6, 9); // 6
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 6);
            REQUIRE(itr->second == 7);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 2);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 5);
        REQUIRE(itr->second == 6);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("try emplace")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto [itr, inserted] = fm.try_emplace(8, 9);
            REQUIRE(inserted);
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto [itr, inserted] = fm.try_emplace(3, 4);
            REQUIRE(inserted);
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto [itr, inserted] = fm.try_emplace(2, 5);
            REQUIRE_FALSE(inserted);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("try emplace with helpful hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.try_emplace(fm.end(), 8, 9); // 7
            REQUIRE(itr->first == 8);
            REQUIRE(itr->second == 9);
            REQUIRE(std::distance(fm.begin(), itr) == 4);
        }

        {
            auto itr = fm.try_emplace(std::next(fm.begin(), 2), 3, 4); // 1
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.try_emplace(std::next(fm.begin(), 1), 2, 5); // 4
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr->first == 8);
        REQUIRE(itr->second == 9);
        ++itr;
        REQUIRE(itr == fm.end());
    }

    SECTION("try emplace with annoying hint")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        {
            auto itr = fm.try_emplace(std::next(fm.begin()), 5, 6); // 5
            REQUIRE(itr->first == 5);
            REQUIRE(itr->second == 6);
            REQUIRE(std::distance(fm.begin(), itr) == 3);
        }

        {
            auto itr = fm.try_emplace(fm.end(), 3, 4); // 8
            REQUIRE(fm.size() == 6);
            REQUIRE(itr->first == 3);
            REQUIRE(itr->second == 4);
            REQUIRE(std::distance(fm.begin(), itr) == 2);
        }

        {
            auto itr = fm.try_emplace(std::next(fm.begin(), 3), 1, 2); // 2
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 1);
            REQUIRE(itr->second == 2);
            REQUIRE(std::distance(fm.begin(), itr) == 1);
        }

        {
            auto itr = fm.try_emplace(std::next(fm.begin(), 2), 2, 5); // 3
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 2);
            REQUIRE(itr->second == 3);
        }

        {
            auto itr = fm.try_emplace(std::next(fm.begin()), 6, 9); // 6
            REQUIRE(fm.size() == 7);
            REQUIRE(itr->first == 6);
            REQUIRE(itr->second == 7);
        }

        auto itr = fm.begin();
        REQUIRE(itr->first == 0);
        REQUIRE(itr->second == 1);
        ++itr;
        REQUIRE(itr->first == 1);
        REQUIRE(itr->second == 2);
        ++itr;
        REQUIRE(itr->first == 2);
        REQUIRE(itr->second == 3);
        ++itr;
        REQUIRE(itr->first == 3);
        REQUIRE(itr->second == 4);
        ++itr;
        REQUIRE(itr->first == 4);
        REQUIRE(itr->second == 5);
        ++itr;
        REQUIRE(itr->first == 5);
        REQUIRE(itr->second == 6);
        ++itr;
        REQUIRE(itr->first == 6);
        REQUIRE(itr->second == 7);
        ++itr;
        REQUIRE(itr == fm.end());
    }
}

TEST_CASE("swap", "[swap]")
{
    SECTION("member method")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        FLAT_MAP<int, int> dst;

        fm.swap(dst);

        REQUIRE(fm.empty());
        REQUIRE(dst.size() == 4);
    }

    SECTION("free function")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        FLAT_MAP<int, int> dst;

        swap(fm, dst);

        REQUIRE(fm.empty());
        REQUIRE(dst.size() == 4);
    }
}

TEST_CASE("erase_if", "[erase_if]")
{
    FLAT_MAP<int, int> fm =
    {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7},
    };

    auto n = erase_if(fm, [](auto& kvp) { return kvp.first < 4; });
    REQUIRE(n == 2);
    REQUIRE(fm.size() == 2);

    auto itr = fm.begin();
    REQUIRE(itr->first == 4);
    REQUIRE(itr->second == 5);
    ++itr;
    REQUIRE(itr->first == 6);
    REQUIRE(itr->second == 7);
    ++itr;
    REQUIRE(itr == fm.end());
}

TEST_CASE("comparison", "[comparison]")
{
    SECTION("traditional comparator")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE(fm == FLAT_MAP<int, int>{{0, 1}, {2, 3}, {4, 5}, {6, 7}});
        REQUIRE_FALSE(fm != FLAT_MAP<int, int>{{0, 1}, {2, 3}, {4, 5}, {6, 7}});

        REQUIRE_FALSE(fm == FLAT_MAP<int, int>{{0, 1}, {2, 4}, {4, 5}, {6, 7}});
        REQUIRE(fm != FLAT_MAP<int, int>{{0, 1}, {2, 4}, {4, 5}, {6, 7}});

        REQUIRE_FALSE(fm == FLAT_MAP<int, int>{{0, 1}, {1, 3}, {4, 5}, {6, 7}});
        REQUIRE(fm != FLAT_MAP<int, int>{{0, 1}, {1, 3}, {4, 5}, {6, 7}});

        REQUIRE_FALSE(fm < FLAT_MAP<int, int>{{0, 1}, {2, 3}, {4, 5}, {6, 7}});
        REQUIRE_FALSE(fm > FLAT_MAP<int, int>{{0, 1}, {2, 3}, {4, 5}, {6, 7}});
        REQUIRE(fm <= FLAT_MAP<int, int>{{0, 1}, {2, 3}, {4, 5}, {6, 7}});
        REQUIRE(fm >= FLAT_MAP<int, int>{{0, 1}, {2, 3}, {4, 5}, {6, 7}});

        REQUIRE(fm < FLAT_MAP<int, int>{{0, 1}, {2, 4}, {4, 5}, {6, 7}});
        REQUIRE_FALSE(fm > FLAT_MAP<int, int>{{0, 1}, {2, 4}, {4, 5}, {6, 7}});
        REQUIRE(fm <= FLAT_MAP<int, int>{{0, 1}, {2, 4}, {4, 5}, {6, 7}});
        REQUIRE_FALSE(fm >= FLAT_MAP<int, int>{{0, 1}, {2, 4}, {4, 5}, {6, 7}});

        REQUIRE_FALSE(fm < FLAT_MAP<int, int>{{0, 1}, {2, 2}, {4, 5}, {6, 7}});
        REQUIRE(fm > FLAT_MAP<int, int>{{0, 1}, {2, 2}, {4, 5}, {6, 7}});
        REQUIRE_FALSE(fm <= FLAT_MAP<int, int>{{0, 1}, {2, 2}, {4, 5}, {6, 7}});
        REQUIRE(fm >= FLAT_MAP<int, int>{{0, 1}, {2, 2}, {4, 5}, {6, 7}});

        REQUIRE_FALSE(fm < FLAT_MAP<int, int>{{0, 1}, {1, 3}, {4, 5}, {6, 7}});
        REQUIRE(fm > FLAT_MAP<int, int>{{0, 1}, {1, 3}, {4, 5}, {6, 7}});
        REQUIRE_FALSE(fm <= FLAT_MAP<int, int>{{0, 1}, {1, 3}, {4, 5}, {6, 7}});
        REQUIRE(fm >= FLAT_MAP<int, int>{{0, 1}, {1, 3}, {4, 5}, {6, 7}});

        REQUIRE(fm < FLAT_MAP<int, int>{{0, 1}, {3, 3}, {4, 5}, {6, 7}});
        REQUIRE_FALSE(fm > FLAT_MAP<int, int>{{0, 1}, {3, 3}, {4, 5}, {6, 7}});
        REQUIRE(fm <= FLAT_MAP<int, int>{{0, 1}, {3, 3}, {4, 5}, {6, 7}});
        REQUIRE_FALSE(fm >= FLAT_MAP<int, int>{{0, 1}, {3, 3}, {4, 5}, {6, 7}});
    }

#ifdef FLAT_MAP_HAS_THREE_WAY_COMPARISON
    SECTION("three way comparison")
    {
        FLAT_MAP<int, int> fm =
        {
            {0, 1},
            {2, 3},
            {4, 5},
            {6, 7},
        };

        REQUIRE((fm <=> FLAT_MAP<int, int>{{0, 1}, {2, 3}, {4, 5}, {6, 7}}) == std::weak_ordering::equivalent);
        REQUIRE((fm <=> FLAT_MAP<int, int>{{0, 1}, {2, 4}, {4, 5}, {6, 7}}) == std::weak_ordering::less);
        REQUIRE((fm <=> FLAT_MAP<int, int>{{0, 1}, {2, 2}, {4, 5}, {6, 7}}) == std::weak_ordering::greater);
    }
#endif
}
