#include <benchmark/benchmark.h>
#include <deque>
#include <flat_map/flat_map.hpp>
#include <map>
#include <random>
#include <unordered_map>
#include <vector>

static std::mt19937 rng_state{};

template <typename C>
static void BM_construct_by_iterator(benchmark::State& state) {
    std::vector<std::pair<int, int>> v;

    for (auto _ : state) {
        state.PauseTiming();
        v.resize(state.range(0));
        for (auto& [k, v] : v) {
            k = std::uniform_int_distribution<int>{}(rng_state);
            v = std::uniform_int_distribution<int>{}(rng_state);
        }
        state.ResumeTiming();

        C fm(v.begin(), v.end());
        benchmark::DoNotOptimize(fm.begin());
        benchmark::ClobberMemory();
    }
}
BENCHMARK_TEMPLATE(BM_construct_by_iterator, std::map<int, int>)->Range(4, 1 << 18);
BENCHMARK_TEMPLATE(BM_construct_by_iterator, std::unordered_map<int, int>)->Range(4, 1 << 18);
BENCHMARK_TEMPLATE(BM_construct_by_iterator, flat_map::flat_map<int, int>)->Range(4, 1 << 18);
BENCHMARK_TEMPLATE(BM_construct_by_iterator, flat_map::flat_map<int, int, std::less<int>, std::deque<std::pair<int, int>>>)
    ->Range(4, 1 << 18);

BENCHMARK_MAIN();
