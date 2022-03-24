#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'template'
VERSION = '0.0.0'


def build(bld):

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_TEMPLATE_VERSION="{}"'.format(
            VERSION))

    bld.recurse('src/template')

    if bld.is_toplevel():

        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse('test')
        bld.recurse('examples')
