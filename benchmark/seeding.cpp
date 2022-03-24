
#include <cstdint>
#include <cstring>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

#include <ranbo/xoshiro256ss.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

std::vector<uint64_t> generate_seeds(std::size_t number_of_seeds)
{
    srand(0);

    std::vector<uint64_t> seeds(number_of_seeds);

    for (std::size_t i = 0; i < number_of_seeds; i++)
    {
        seeds[i] = rand();
    }

    return seeds;
}

static void boost_mt_seed(benchmark::State& state, std::size_t number_of_seeds)
{
    boost::random::mt19937 generator;

    benchmark::DoNotOptimize(generator);

    auto seeds = generate_seeds(number_of_seeds);
    for (auto _ : state)
    {
        for (auto seed : seeds)
        {
            generator.seed((uint32_t)seed);
            benchmark::ClobberMemory();
        }
    }
}

static void xoshiro256ss_seed(benchmark::State& state,
                              std::size_t number_of_seeds)
{
    struct ranbo_xoshiro256ss generator;

    benchmark::DoNotOptimize(generator);

    auto seeds = generate_seeds(number_of_seeds);
    for (auto _ : state)
    {
        for (auto seed : seeds)
        {

            ranbo_xoshiro256ss_set_seed(&generator, seed);
            benchmark::ClobberMemory();
        }
    }
}

static void std_mt_seed(benchmark::State& state, std::size_t number_of_seeds)
{
    std::mt19937_64 generator;

    benchmark::DoNotOptimize(generator);

    auto seeds = generate_seeds(number_of_seeds);
    for (auto _ : state)
    {
        for (auto seed : seeds)
        {
            generator.seed(seed);
            benchmark::ClobberMemory();
        }
    }
}

static void BenchmarkArguments(benchmark::internal::Benchmark* b)
{
    b->Unit(benchmark::kNanosecond);
    b->Repetitions(10);
}

BENCHMARK_CAPTURE(boost_mt_seed, mersenne_boost, 100000)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(std_mt_seed, mersenne_std, 100000)->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(xoshiro256ss_seed, xoshiro256ss, 100000)
    ->Apply(BenchmarkArguments);

BENCHMARK_MAIN();
