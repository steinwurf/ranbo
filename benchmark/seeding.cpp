
#include <cstdint>
#include <cstring>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

#include <ranbo/rand48.h>
#include <ranbo/xoshiro256ss.h>
#include <ranbo/xoshiro64s.h>

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

static void xoshiro256ss_seed(benchmark::State& state,
                              std::size_t number_of_seeds)
{
    struct ranbo_xoshiro256ss generator;

    benchmark::DoNotOptimize(generator);

    std::size_t bytes = 0;

    auto seeds = generate_seeds(number_of_seeds);
    for (auto _ : state)
    {
        for (auto seed : seeds)
        {

            ranbo_xoshiro256ss_set_seed(&generator, seed);
            benchmark::ClobberMemory();
            bytes += 8;
        }
    }
    state.SetBytesProcessed(bytes);
}

static void std_mt_seed(benchmark::State& state, std::size_t number_of_seeds)
{
    std::mt19937_64 generator;

    benchmark::DoNotOptimize(generator);

    std::size_t bytes = 0;

    auto seeds = generate_seeds(number_of_seeds);
    for (auto _ : state)
    {
        for (auto seed : seeds)
        {
            generator.seed(seed);
            benchmark::ClobberMemory();
            bytes += 8;
        }
    }
    state.SetBytesProcessed(bytes);
}

static void xoshiro64s_seed(benchmark::State& state,
                            std::size_t number_of_seeds)
{
    struct ranbo_xoshiro64s generator;

    benchmark::DoNotOptimize(generator);

    std::size_t bytes = 0;

    auto seeds = generate_seeds(number_of_seeds);
    for (auto _ : state)
    {
        for (auto seed : seeds)
        {

            ranbo_xoshiro64s_set_seed(&generator, seed);
            benchmark::ClobberMemory();
            bytes += 8;
        }
    }
    state.SetBytesProcessed(bytes);
}

static void rand48_seed(benchmark::State& state, std::size_t number_of_seeds)
{
    struct ranbo_rand48 rand48;

    benchmark::DoNotOptimize(rand48);

    std::size_t bytes = 0;

    auto seeds = generate_seeds(number_of_seeds);
    for (auto _ : state)
    {
        for (auto seed : seeds)
        {

            ranbo_rand48_set_seed(&rand48, seed);
            benchmark::ClobberMemory();
            bytes += 4;
        }
    }
    state.SetBytesProcessed(bytes);
}

static void BenchmarkArguments(benchmark::internal::Benchmark* b)
{
    b->Unit(benchmark::kNanosecond);
    b->Repetitions(10);
}

BENCHMARK_CAPTURE(std_mt_seed, mersenne_std, 100000)->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(xoshiro256ss_seed, xoshiro256ss, 100000)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(xoshiro64s_seed, xoshiro64s, 100000)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(rand48_seed, rand48, 100000)->Apply(BenchmarkArguments);

BENCHMARK_MAIN();
