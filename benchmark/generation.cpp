
#include <cstdint>
#include <cstring>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

#include <ranbo/xoshiro256ss.h>
#include <ranbo/xoshiro64s.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

static void boost_mt_generate(benchmark::State& state, std::size_t iterations)
{
    boost::random::mt19937 generator;

    std::size_t bytes = 0;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            benchmark::DoNotOptimize(generator());
            bytes += 4;
            benchmark::ClobberMemory();
        }
    }

    state.SetBytesProcessed(bytes);
}

static void xoshiro256ss_generate(benchmark::State& state,
                                  std::size_t iterations)
{
    struct ranbo_xoshiro256ss generator;

    std::size_t bytes = 0;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            benchmark::DoNotOptimize(ranbo_xoshiro256ss_generate(&generator));
            bytes += 8;
            benchmark::ClobberMemory();
        }
    }
    state.SetBytesProcessed(bytes);
}

static void std_mt_generate(benchmark::State& state, std::size_t iterations)
{
    std::mt19937_64 generator;

    std::size_t bytes = 0;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            benchmark::DoNotOptimize(generator());
            bytes += 8;
            benchmark::ClobberMemory();
        }
    }
    state.SetBytesProcessed(bytes);
}

static void xoshiro64s_generate(benchmark::State& state, std::size_t iterations)
{
    struct ranbo_xoshiro64s generator;

    std::size_t bytes = 0;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            benchmark::DoNotOptimize(ranbo_xoshiro64s_generate(&generator));
            bytes += 4;
            benchmark::ClobberMemory();
        }
    }
    state.SetBytesProcessed(bytes);
}

static void BenchmarkArguments(benchmark::internal::Benchmark* b)
{
    b->Unit(benchmark::kNanosecond);
    b->Repetitions(10);
}

std::size_t iterations = 100000U;

BENCHMARK_CAPTURE(boost_mt_generate, mersenne_boost, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(std_mt_generate, mersenne_std, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(xoshiro256ss_generate, xoshiro256ss, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(xoshiro64s_generate, xoshiro64s, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_MAIN();
