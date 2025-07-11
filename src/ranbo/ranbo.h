// Copyright (c) Steinwurf ApS 2020.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// https://www.steinwurf.com/licensing

#pragma once

#ifdef __KERNEL__
#include <linux/string.h>
#include <linux/types.h>
#else
#include <stdint.h>
#include <string.h>
#endif

#define STEINWURF_RANBO_VERSION "3.0.0"

#include "rand48.h"
#include "xoshiro256ss.h"
#include "xoshiro64s.h"
