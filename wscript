#! /usr/bin/env python
# encoding: utf-8

APPNAME = "ranbo"
VERSION = "2.0.0"

import os
import sys

from waflib.Build import BuildContext


def options(ctx):
    ctx.load("cmake")


def configure(ctx):

    ctx.load("cmake")

    if ctx.is_toplevel():
        ctx.cmake_configure()


def build(ctx):

    ctx.load("cmake")

    if ctx.is_toplevel():
        ctx.cmake_build()


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
