/**
  @file		ptrace.c
  @author	N. Devillard, V. Chudnovsky
  @date		March 2004
  @version	$Revision: 2.0 $
  @brief	Add tracing capability to any program compiled with gcc.

  This module is only compiled when using gcc and tracing has been
  activated. It allows the compiled program to output messages whenever
  a function is entered or exited.

  To activate this feature, your version of gcc must support
  the -finstrument-functions flag.

  When using ptrace on a dynamic library, you must set the
  PTRACE_REFERENCE_FUNCTION macro to be the name of a function in the
  library. The address of this function when loaded will be the first
  line output to the trace file and will permit the translation of the
  other entry and exit pointers to their symbolic names. You may set
  the macro PTRACE_INCLUDE with any #include directives needed for
  that function to be accesible to this source file.

  The printed messages yield function addresses, not human-readable
  names. To link both, you need to get a list of symbols from the
  program. There are many (unportable) ways of doing that, see the
  'etrace' project on freshmeat for more information about how to dig
  the information.
*/

/*
	$Id: ptrace.c,v 1.1.1.1 2004-03-16 20:00:07 ndevilla Exp $
	$Author: ndevilla $
	$Date: 2004-03-16 20:00:07 $
	$Revision: 1.1.1.1 $
*/

#if (__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ > 95))

/*---------------------------------------------------------------------------
								Includes
 ---------------------------------------------------------------------------*/

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/*---------------------------------------------------------------------------
								Defines
 ---------------------------------------------------------------------------*/

#define TRACE_MQ_NAME "/eTrace"
#define TRACE_TAG_ENTER "enter"
#define TRACE_TAG_EXIT "exit"
#define TRACE_TAG_TERMINATE "END"
#define __NON_INSTRUMENT_FUNCTION__ __attribute__((__no_instrument_function__))

/*---------------------------------------------------------------------------
							Function codes
 ---------------------------------------------------------------------------*/

/** Message queue handle */
static mqd_t mq = -1;

/** Code segment offset */
static size_t offset;

/** Final trace close */
static void __NON_INSTRUMENT_FUNCTION__
gnu_ptrace_close(void)
{
	char buffer[128];
	sprintf(buffer, "%s %ld", TRACE_TAG_TERMINATE, (long)getpid());
	mq_send(mq, buffer, strlen(buffer), 0);
	mq_close(mq);
}

/** Trace initialization */
static int __NON_INSTRUMENT_FUNCTION__
gnu_ptrace_init(void)
{

	/* Open the message queue. If it does not exist, it means we have been
	 * launched not by the tracing script. In this case, we do not trace. */
	if ((mq = mq_open(TRACE_MQ_NAME, O_WRONLY)) == -1) {
		return -1;
	}

	FILE *f = fopen("/proc/self/maps", "r");
	fscanf(f, "%zx", &offset);
	fclose(f);

	atexit(gnu_ptrace_close);

	return 1;
}

/** Function called by every function event */
static void
__NON_INSTRUMENT_FUNCTION__
gnu_ptrace(const char *tag, void *p)
{
	static int first = 1;
	static int active = 0;

	if (first == 1) {
		active = gnu_ptrace_init();
		first = 0;
	}

	if (active == 1) {
		char buffer[128];
		sprintf(buffer, "%s %p", tag, (unsigned char *)p - offset);
		mq_send(mq, buffer, strlen(buffer), 0);
	}

	return;
}

#if __cplusplus
extern "C" {
#endif

/** According to gcc documentation: called upon function entry */
void
__NON_INSTRUMENT_FUNCTION__
__cyg_profile_func_enter(void *this_fn, void *call_site)
{
	gnu_ptrace(TRACE_TAG_ENTER, this_fn);
	(void)call_site;
}

/** According to gcc documentation: called upon function exit */
void
__NON_INSTRUMENT_FUNCTION__
__cyg_profile_func_exit(void *this_fn, void *call_site)
{
	gnu_ptrace(TRACE_TAG_EXIT, this_fn);
	(void)call_site;
}

#if __cplusplus
}
#endif

#endif
