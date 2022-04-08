/*
 *  main.c - The simplest kernel module.
 */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/module.h> /* Needed by all modules */

#include <ranbo/rand48.h>
#include <ranbo/xoshiro256ss.h>
#include <ranbo/xoshiro64s.h>

MODULE_LICENSE("Closed - Steinwurf ApS");

static struct ranbo_rand48 r48;
static struct ranbo_xoshiro256ss xoshiro256ss;
static struct ranbo_xoshiro64s xoshiro64s;

static uint64_t rand48_random;

static uint64_t xoshiro256ss_random;

static uint64_t xoshiro64s_random;

int init_module(void)
{
    ranbo_rand48_set_seed(&r48, 0);
    ranbo_xoshiro256ss_set_seed(&xoshiro256ss, 0);
    ranbo_xoshiro64s_set_seed(&xoshiro64s, 0);
    rand48_random = ranbo_rand48_generate(&r48);
    xoshiro256ss_random = ranbo_xoshiro256ss_generate(&xoshiro256ss);
    xoshiro64s_random = ranbo_xoshiro64s_generate(&xoshiro64s);
    printk(KERN_INFO "Hello from rand48: %llu.\n", rand48_random);
    printk(KERN_INFO "Hello from xoshiro256ss: %llu.\n", xoshiro256ss_random);
    printk(KERN_INFO "Hello from xoshiro64s: %llu.\n", xoshiro64s_random);

    /*
     * A non 0 return means init_module failed; module can't be loaded.
     */
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Goodbye from ranbo.\n");
}
