#include <algorithm>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include <ranbo/rand48.h>
#include <ranbo/xoshiro256ss.h>
#include <ranbo/xoshiro64s.h>

double calculate_variance(std::vector<double> counts)
{
    auto mean = 0.50;

    std::transform(counts.begin(), counts.end(), counts.begin(),
                   [mean](double x) { return (x - mean) * (x - mean); });

    auto variance = std::accumulate(counts.begin(), counts.end(), 0.0) /
                    static_cast<double>(counts.size() - 1);
    return variance;
}

void run(int iterations)
{
    auto seed = 0;

    std::cout << "seed: " << seed << std::endl;
    std::vector<double> xoshiro256_count(64);
    std::vector<double> xoshiro64_count(64);
    std::vector<double> rand48_count(64);

    for (auto j = 0; j < 64; j++)
    {

        xoshiro256_count[j] = 0;

        xoshiro64_count[j] = 0;

        rand48_count[j] = 0;
    }

    ranbo_xoshiro256ss x256generator;

    ranbo_rand48 rand48;

    ranbo_xoshiro64s x64generator;

    ranbo_xoshiro256ss_set_seed(&x256generator, seed);

    ranbo_xoshiro64s_set_seed(&x64generator, seed);

    ranbo_rand48_set_seed(&rand48, (uint32_t)seed);

    std::vector<uint64_t> xoshiro256_vector;
    std::vector<uint64_t> xoshiro64_vector;
    std::vector<uint64_t> rand48_vector;

    xoshiro256_vector.reserve(iterations);
    xoshiro64_vector.reserve(iterations);
    rand48_vector.reserve(iterations);

    for (auto i = 0; i < iterations; i++)
    {
        xoshiro256_vector.push_back(
            ranbo_xoshiro256ss_generate(&x256generator));
        xoshiro64_vector.push_back(
            ((uint64_t)ranbo_xoshiro64s_generate(&x64generator) << 32) |
            (uint64_t)ranbo_xoshiro64s_generate(&x64generator));
        rand48_vector.push_back(
            ((uint64_t)ranbo_rand48_generate(&rand48) << 32) |
            (uint64_t)ranbo_rand48_generate(&rand48));

        for (auto j = 0; j < 64; j++)
        {
            if (xoshiro256_vector.back() & (1ULL << j))
            {
                xoshiro256_count[j]++;
            }
            if (xoshiro64_vector.back() & (1ULL << j))
            {
                xoshiro64_count[j]++;
            }
            if (rand48_vector.back() & (1ULL << j))
            {
                rand48_count[j]++;
            }
        }

        seed++;

        ranbo_xoshiro256ss_set_seed(&x256generator, seed);

        ranbo_xoshiro64s_set_seed(&x64generator, seed);

        ranbo_rand48_set_seed(&rand48, (uint32_t)seed);
    }
    std::cout << std::fixed;
    std::cout << std::setprecision(3);

    double xoshiro256_variance = calculate_variance(xoshiro256_count);

    double xoshiro64_variance = calculate_variance(xoshiro64_count);

    double rand48_variance = calculate_variance(rand48_count);

    std::cout << "iterations = " << iterations << std::endl << std::endl;
    std::cout << "xoshiro256ss variance: " << xoshiro256_variance << std::endl;

    std::cout << "xoshiro64s variance:  " << xoshiro64_variance << std::endl
              << std::endl;

    std::cout << "rand48 variance:     " << rand48_variance << std::endl
              << std::endl;

    std::cout << "xoshiro256ss max/min: "
              << (*std::max_element(xoshiro256_count.begin(),
                                    xoshiro256_count.end())) /
                     (double)iterations
              << " "
              << (*std::min_element(xoshiro256_count.begin(),
                                    xoshiro256_count.end()) /
                  (double)iterations)
              << std::endl;

    std::cout
        << "xoshiro64s max/min:  "
        << (*std::max_element(xoshiro64_count.begin(), xoshiro64_count.end())) /
               (double)iterations
        << " "
        << (*std::min_element(xoshiro64_count.begin(), xoshiro64_count.end()) /
            (double)iterations)
        << std::endl;

    std::cout << "rand48 max/min:     "
              << (*std::max_element(rand48_count.begin(), rand48_count.end()) /
                  (double)iterations)
              << " "
              << (*std::min_element(rand48_count.begin(), rand48_count.end()) /
                  (double)iterations)
              << std::endl
              << std::endl;

    std::cout << std::endl;
    return;
}

int main()
{
    run(1000);
    run(10000);
    run(100000);
    run(1000000);
    run(10000000);

    return 0;
}
