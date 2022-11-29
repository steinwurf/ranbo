#! /usr/bin/env python
# encoding: utf-8

APPNAME = "ranbo"
VERSION = "1.0.1"

import os
import sys

from waflib.Build import BuildContext


def options(opt):

    if opt.is_toplevel():

        opts = opt.add_option_group("Kernel options")

        opts.add_option(
            "--kernel_example",
            action="store_true",
            default=False,
            help="Enable kernel example build",
        )

        opts.add_option(
            "--kernel_path",
            default=None,
            dest="kernel_path",
            help="Set the path to the Linux kernel sources",
        )


def build(bld):

    bld(includes="src", export_includes="src", name="ranbo_includes")

    if bld.is_toplevel():

        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse("test")

        bld.recurse("examples")

        bld.recurse("benchmark")

        if bld.get_tool_option("kernel_example"):
            bld.recurse("examples/kernel")


class ReleaseContext(BuildContext):
    cmd = "prepare_release"
    fun = "prepare_release"


def prepare_release(ctx):
    """Prepare a release."""

    # Rewrite versions
    with ctx.rewrite_file(filename="src/ranbo/ranbo.h") as f:

        pattern = r'#define STEINWURF_RANBO_VERSION "\d+\.\d+\.\d+"'
        replacement = '#define STEINWURF_RANBO_VERSION "{}"'.format(VERSION)

        f.regex_replace(pattern=pattern, replacement=replacement)

    # Rewrite versions
    with ctx.rewrite_file(filename="library.json") as f:

        pattern = r'"version": "\d+\.\d+\.\d+",'
        replacement = '"version": "{}",'.format(VERSION)

        f.regex_replace(pattern=pattern, replacement=replacement)


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
