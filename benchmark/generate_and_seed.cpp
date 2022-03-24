
#include <cstdint>
#include <cstring>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

#include <ranbo/xoshiro256ss.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

static void boost_mt_generate_and_seed(benchmark::State& state,
                                       std::size_t iterations)
{
    boost::random::mt19937 generator;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            generator.seed(i);
            for (std::size_t j = 0; j < 10U; j++)
            {
                benchmark::DoNotOptimize(generator());
            }
        }
    }
}

static void xoshiro256ss_generate_and_seed(benchmark::State& state,
                                           std::size_t iterations)
{
    struct ranbo_xoshiro256ss generator;

    ranbo_xoshiro256ss_set_seed(&generator, 0);

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            ranbo_xoshiro256ss_set_seed(&generator, i);
            for (std::size_t j = 0; j < 10U; j++)
            {
                benchmark::DoNotOptimize(
                    ranbo_xoshiro256ss_generate(&generator));
            }
        }
    }
}

static void std_mt_generate_and_seed(benchmark::State& state,
                                     std::size_t iterations)
{
    std::mt19937_64 generator;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            generator.seed(i);
            for (std::size_t j = 0; j < 10U; j++)
            {
                benchmark::DoNotOptimize(generator());
            }
        }
    }
}

static void BenchmarkArguments(benchmark::internal::Benchmark* b)
{
    b->Unit(benchmark::kNanosecond);
    b->Repetitions(10);
}

std::size_t iterations = 10000U;

BENCHMARK_CAPTURE(boost_mt_generate_and_seed, mersenne_boost, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(std_mt_generate_and_seed, mersenne_std, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(xoshiro256ss_generate_and_seed, xoshiro256ss, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_MAIN();
