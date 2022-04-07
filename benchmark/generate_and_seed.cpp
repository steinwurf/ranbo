
#include <cstdint>
#include <cstring>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

#include <ranbo/rand48.h>
#include <ranbo/xoshiro256ss.h>
#include <ranbo/xoshiro64s.h>

static void xoshiro256ss_generate_and_seed(benchmark::State& state,
                                           std::size_t iterations)
{
    struct ranbo_xoshiro256ss generator;

    std::size_t bytes = 0;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            ranbo_xoshiro256ss_set_seed(&generator, i);

            auto random_number = ranbo_xoshiro256ss_generate(&generator);
            benchmark::DoNotOptimize(random_number);
            bytes += 8;
            benchmark::ClobberMemory();
        }
    }
    state.SetBytesProcessed(bytes);
}

static void std_mt_generate_and_seed(benchmark::State& state,
                                     std::size_t iterations)
{
    std::mt19937_64 generator;

    std::size_t bytes = 0;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            generator.seed(i);

            auto random_number = generator();
            benchmark::DoNotOptimize(random_number);
            bytes += 8;
            benchmark::ClobberMemory();
        }
    }
    state.SetBytesProcessed(bytes);
}

static void xoshiro64s_generate_and_seed(benchmark::State& state,
                                         std::size_t iterations)
{
    struct ranbo_xoshiro64s generator;

    std::size_t bytes = 0;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            ranbo_xoshiro64s_set_seed(&generator, i);

            auto random_number = ranbo_xoshiro64s_generate(&generator);
            benchmark::DoNotOptimize(random_number);

            bytes += 4;
            benchmark::ClobberMemory();
        }
    }

    state.SetBytesProcessed(bytes);
}

static void rand48_generate_and_seed(benchmark::State& state,
                                     std::size_t iterations)
{
    struct ranbo_rand48 rand48;

    std::size_t bytes = 0;

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            ranbo_rand48_set_seed(&rand48, i);

            auto random_number = ranbo_rand48_generate(&rand48);
            benchmark::DoNotOptimize(random_number);

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

std::size_t iterations = 5000U;

BENCHMARK_CAPTURE(std_mt_generate_and_seed, mersenne_std, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(xoshiro256ss_generate_and_seed, xoshiro256ss, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(xoshiro64s_generate_and_seed, xoshiro64s, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(rand48_generate_and_seed, rand48, iterations)
    ->Apply(BenchmarkArguments);

BENCHMARK_MAIN();
