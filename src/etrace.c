/**
 * @file etrace.c
 * @brief Simple add-on for printing call-trace in a human readable format.
 * @version 1.0
 *
 * This is free software placed in the public domain. Enjoy!
 *
 * This add-on requires executable to be compiled with instrumented functions
 * and to be linked with all symbols added to the dynamic symbol table. It can
 * be done with (preferable):
 *
 *   gcc -finstrument-functions -rdynamic ...
 *
 * or with:
 *
 *   gcc -finstrument-functions -Wl,-E ...
 *
 */

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#if __cplusplus
# include <cxxabi.h>
#endif

/**
 * Activate/deactivate tracing. Might be used
 * by the user to control tracing on runtime. */
static unsigned int active = 1;

/** Function called by every function event */
static void
__attribute__((__no_instrument_function__))
trace(void *this_fn, void *call_site, int level)
{

#if __cplusplus
	/** Buffer for demangle C++ names */
	static char *buffer = NULL;
	static size_t buffer_size = 0;
	if (buffer_size == 0) {
		buffer_size = 2048;
		buffer = static_cast<char *>(malloc(buffer_size));
	}
#endif

	/** Keep track of call nesting */
	static unsigned int indent = 0;
	indent += level;

	if (active == 0)
		return;
	if (level == -1)
		return;

#if defined(__thumb__)
	/* Convert thumb address to arm address */
	this_fn = (void *)((size_t)this_fn & ~1);
#endif

	Dl_info dl;
	if (dladdr(this_fn, &dl) == 0 || dl.dli_sname == NULL)
		return;

#if __cplusplus
	int status = 0;
	char *tmp = abi::__cxa_demangle(dl.dli_sname, buffer, &buffer_size, &status);
	if (tmp != NULL)
		dl.dli_sname = tmp;
#endif

	for (unsigned int i = 1; i < indent; i++)
		fprintf(stderr, "| ");
	fprintf(stderr, "%s\n", dl.dli_sname);

}

#if __cplusplus
extern "C" {
#endif

/** According to gcc documentation: called upon function entry */
void
__attribute__((__no_instrument_function__))
__cyg_profile_func_enter(void *this_fn, void *call_site)
{
	trace(this_fn, call_site, 1);
}

/** According to gcc documentation: called upon function exit */
void
__attribute__((__no_instrument_function__))
__cyg_profile_func_exit(void *this_fn, void *call_site)
{
	trace(this_fn, call_site, -1);
}

#if __cplusplus
}
#endif
