#! /usr/bin/env python
# encoding: utf-8

APPNAME = "ranbo"
VERSION = "0.0.0"


def build(bld):

    bld(
        includes="./src",
        export_includes="./src",
        name="ranbo_includes",
    )

    if bld.is_toplevel():

        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse("test")

        bld.recurse("examples")

        bld.recurse("benchmark")


def run_benchmarks(ctx):

    venv = ctx.create_virtualenv(name="virtualenv-plots", overwrite=False)

    # seeding benchmark
    venv.run(
        "build_current/benchmark/seeding --benchmark_out='benchmark_results/seeding.json' --benchmark_counters_tabular=true"
    )

    # generation benchmark
    venv.run(
        "build_current/benchmark/generation --benchmark_out='benchmark_results/generation.json' --benchmark_counters_tabular=true"
    )

    # seed and generate benchmark
    venv.run(
        "build_current/benchmark/generate_and_seed --benchmark_out='benchmark_results/generate_and_seed.json' --benchmark_counters_tabular=true"
    )
