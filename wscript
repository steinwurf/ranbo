#! /usr/bin/env python
# encoding: utf-8

APPNAME = "ranbo"
VERSION = "0.0.0"

import os


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


def docs(ctx):
    """Build the documentation in a virtualenv"""

    with ctx.create_virtualenv() as venv:

        # To update the requirements.txt just delete it - a fresh one
        # will be generated from test/requirements.in
        if not os.path.isfile("docs/requirements.txt"):
            venv.run("python -m pip install pip-tools")
            venv.run("pip-compile docs/requirements.in")

        venv.run("python -m pip install -r docs/requirements.txt")

        build_path = os.path.join(ctx.path.abspath(), "build", "site", "docs")

        venv.run(
            "giit clean . --build_path {}".format(build_path), cwd=ctx.path.abspath()
        )
        venv.run(
            "giit sphinx . --build_path {}".format(build_path), cwd=ctx.path.abspath()
        )


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
