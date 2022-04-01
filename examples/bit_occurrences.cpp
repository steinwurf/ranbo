#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <ranbo/xoshiro256ss.h>

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
    auto seed = 155;

    std::cout << "seed: " << seed << std::endl;
    std::vector<double> xoshiro256_count(64);
    std::vector<double> boost_mt_count(64);

    for (auto j = 0; j < 64; j++)
    {

        xoshiro256_count[j] = 0;

        boost_mt_count[j] = 0;
    }

    ranbo_xoshiro256ss x256generator;

    boost::random::mt19937 mt_generator;

    boost::random::uniform_int_distribution<uint64_t> dist;

    ranbo_xoshiro256ss_set_seed(&x256generator, seed);

    mt_generator.seed(seed);

    std::vector<uint64_t> xoshiro256_vector;
    std::vector<uint64_t> xoshiro64_vector;
    std::vector<uint64_t> mt_vector;

    xoshiro256_vector.reserve(iterations);
    xoshiro64_vector.reserve(iterations);
    mt_vector.reserve(iterations);

    for (auto i = 0; i < iterations; i++)
    {
        xoshiro256_vector.push_back(
            ranbo_xoshiro256ss_generate(&x256generator));
        mt_vector.push_back(dist(mt_generator));

        for (auto j = 0; j < 64; j++)
        {
            if (xoshiro256_vector.back() & (1ULL << j))
            {
                xoshiro256_count[j]++;
            }
            if (mt_vector.back() & (1ULL << j))
            {
                boost_mt_count[j]++;
            }
        }
    }
    std::cout << std::fixed;
    std::cout << std::setprecision(3);

    double xoshiro256_variance = calculate_variance(xoshiro256_count);

    double boost_mt_variance = calculate_variance(boost_mt_count);

    std::cout << "iterations = " << iterations << std::endl << std::endl;
    std::cout << "xoshiro256ss variance: " << xoshiro256_variance << std::endl;

    std::cout << "boost mt variance:     " << boost_mt_variance << std::endl
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
        << "boost mt max/min:     "
        << (*std::max_element(boost_mt_count.begin(), boost_mt_count.end()) /
            (double)iterations)
        << " "
        << (*std::min_element(boost_mt_count.begin(), boost_mt_count.end()) /
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
