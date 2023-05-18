etrace: A run-time tracing tool
===============================

These source files comprise a simple but efficient utility that allows the
display of a function call tree in a C/C++ program at run-time.

* ptrace.c - tracer linked with your application to enable run-time tracing
* etrace - a Python script that will perform the run-time tracing by displaying
           the function names as they are called

License
-------

This is free software placed in the public domain. Enjoy!

Pre-requisites
--------------

* gcc version 2.95 or later
* Python version 3.6 or later
* POSIX IPC primitives Python module: <https://pypi.org/project/posix-ipc>
* presence of the `nm` utility on your machine

Example using etrace
--------------------

There is an example in the [example](example) sub-dir. To compile it, do:

```shell
cd example
make
```

You should now have an executable called `crumble`. Running it should give
you a recipe for an apple crumble, with 1-second pauses between sentences
to demonstrate the run-time ability. Now let's get to this function call
tree:

To get the tree at run-time, open terminal and run:

```shell
make run
```

Data are exchanged between the running process you are trying to monitor and
the `etrace` script through a POSIX message queue with the maximum number of
messages set to 1 for tight synchronization between these two processes.

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

The code in `ptrace.c` simply redirects the information gathered at each call
to a POSIX message queue.

Now, if you examine closely what is sent to this message queue, you will see
that only the binary addresses of functions are printed out. To make the
link to true function names, you need to interpret the binary and make the
list of associations. There are many ways of doing this, and to be truly
platform-independent you should have a look at the BFD library from the GNU
project.

`etrace` implements a much simpler (but of course much less portable) solution
by using the `nm` and `c++filt` utilities to dump a list of all defined symbols
and associated addresses. The link between them and the pretty printing as a
function call tree is all done in the `etrace` script.

ptrace.c/etrace usage
---------------------

The `ptrace.c` module will look for a POSIX message queue named "/eTrace". If
no such queue can be found, nothing will be logged. You still get the penalty
associated to the two function calls per every function in your application,
but nothing else changes.

If the message queue can be opened for writing, log data will be send to it.

`etrace` needs to know the name of the executable you are trying to trace,
to retrieve the symbols (with `nm`). This is the only argument to the script.

How to use it in your code
--------------------------

To trace a stand-alone program, there is no need to #include any additional
file. Just compile you code with the `-finstrument-functions` gcc option and
link it against `ptrace.c` and `rt` library. This should do it.

Now when you want to trace at run-time (handy to see where the code breaks
in case of segfault), launch `etrace myprogram` in terminal.

Feedback
--------

Feedback is always welcome.
However, this code is not supported. This is only 80 lines of Python and
100 lines of C, so if you need additional features I'd say you are better
off on your own. :-)

N. Devillard
Wed May  9 15:29:04 CEST 2001

V. Chudnovsky
Mon Mar  8 2004
