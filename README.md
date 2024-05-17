etrace: A run-time tracing tool
===============================

The source file comprise a simple but efficient utility that allows the
display of a function call tree in a C/C++ program at run-time.

* etrace.c - tracer linked with your application to enable run-time tracing

License
-------

This is free software placed in the public domain. Enjoy!

Pre-requisites
--------------

* gcc version 2.95 or later
* linker with support for `--export-dynamic`

Example using etrace
--------------------

There is an example in the [example](example) sub-dir. To compile it, do:

```shell
cd example
make
```

You should now have executables called `crumble_c` and `crumble_cpp`. Running
them should give you a recipe for an apple crumble with 1-second pauses between
sentences to demonstrate the run-time ability. Now let's get to this function
call tree:

To get the tree at run-time, open terminal and run:

```shell
make run
```

You should see this:

```text
main
| Crumble_make_apple_crumble
| | Crumble_buy_stuff
| | | Crumble_buy
| | | Crumble_buy (total: 5 times)
| | Crumble_prepare_apples
| | | Crumble_skin_and_dice
| | Crumble_mix
| | Crumble_finalize
| | | Crumble_put
| | | Crumble_put (total: 2 times)
| | Crumble_cook
| | | Crumble_put
| | | Crumble_bake
```

How does it work?
-----------------

gcc 2.95 has a nifty feature called "instrument-functions", that adds a couple
of function calls to all functions in your code. Every time a function starts,
a function called `__cyg_profile_func_enter()` is called, and every time a
function exits, a function called `__cyg_profile_func_exit()` is called.

The code in `etrace.c` simply gets the address of currently entered function
and gets it's name from the dynamic symbol table. In case of C++ the name is
also demangled to be more readable.

How to use it in your code
--------------------------

To trace a stand-alone program, there is no need to #include any additional
file. Just compile you code with the `-finstrument-functions` and `-rdynamic`
gcc options and link it against `etrace.c`, `dl` and `rt` library.
This should do it.

Feedback
--------

Feedback is always welcome.
However, this code is not supported. This is only 100 lines of C, so if you
need additional features I'd say you are better off on your own. :-)

N. Devillard
Wed May  9 15:29:04 CEST 2001

V. Chudnovsky
Mon Mar  8 2004
