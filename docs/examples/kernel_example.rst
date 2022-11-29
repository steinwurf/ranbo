Kernel Example
==============

This example illustrates how to write a simple "hello world" Linux kernel module,
that prints random numbers to the syslog. You can run it and type
"sudo dmesg | grep Hello" in the terminal, to see the output from the module.

The complete example is shown below.

.. literalinclude:: ../../examples/kernel/main.c
    :language: c
    :linenos:
