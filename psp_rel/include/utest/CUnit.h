/**
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2001            Anil Kumar
 *  Copyright (C) 2004,2005,2006  Anil Kumar, Jerry St.Clair
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *  Contains ASSERT Macro definitions.
 *
 *  09/Aug/2001   ASSERT definitions. (AK)
 *
 *  12/Mar/2003   New Assert definitions. (AK)
 *
 *  27/Jul/2003   Modified ASSERT_XXX Macro definitions. (AK)
 *
 *  15-Jul-2004   New interface, changed action on assert failure to not
 *                return, provided _FATAL versions of assertions to return
 *                from test function on failure. (JDS)
 *
 *  01-Sep-2004   Modified assertions for setjmp/longjmp mechanism of
 *                aborting test runs, added CU_FAIL and CU_PASS macros. (JDS)
 *
 *  07-May-2005   Added CU_ prefix to remaining CUnit defines (BOOL, TRUE,
 *                FALSE, MAX_...).  Added CU_UNREFERENCED_PARAMETER() define. (JDS)
 */

/*
 * unit test framwork for USDK(mips, ucos-ii based)
 * Copyright (C) 2008            Actions-Semiconductor
 *
 * sangwei@actions-semi.com
 * 10-04-2008
 *
 */

/** @file
 * Basic CUnit include file for user and system code.
 * Defines macros for assertions for use in user test cases.
 * Basic system macro definitions also appear here.
 */
/** @addtogroup Framework
 * @{
 */

#ifndef CUNIT_CUNIT_H_SEEN
#define CUNIT_CUNIT_H_SEEN

/** CUnit version number. */
#define CU_VERSION "2.1-0"

/*  Max string lengths for names (includes terminating NULL. */
/** Maximum length of a test name string. */
#define CU_MAX_TEST_NAME_LENGTH     128
/** Maximim length of a suite name string. */
#define CU_MAX_SUITE_NAME_LENGTH	128

/* Global type Definitions to be used for boolean operators. */
#ifndef CU_BOOL
/** Boolean type for CUnit use. */
#define CU_BOOL int
#endif

#ifndef CU_TRUE
/** Boolean TRUE for CUnit use. */
#define CU_TRUE 1
#endif

#ifndef CU_FALSE
/** Boolean FALSE for CUnit use. */
#define CU_FALSE	0
#endif

#ifdef WIN32
#  ifdef CU_DLL
#    ifdef CU_BUILD_DLL
#      define CU_EXPORT __declspec(dllexport)
#    else
#      define CU_EXPORT __declspec(dllimport)
#    endif
#  else
#    define CU_EXPORT
#  endif
#else
#  define CU_EXPORT
#endif

#include "CUError.h"
#include "TestDB.h"   /* not needed here - included for user convenience */
#include "TestRun.h"  /* not needed here - include (after BOOL define) for user convenience */

/* set mount dir only be useful in PC environment and NFS environment */
#define CU_SET_MOUNT_DIR(strMountDir) \
    { CU_set_mount_dir(strMountDir); }

/* Force data write to log */
#define CU_FLUSH \
  { CU_flush(); }

/* Record the start time of running a function */
#define CU_TIMER_START(strFunc) \
  { CU_performanceTimerStart(strFunc, __FILE__); }

/* Record the end time of running a function */
#define CU_TIMER_END \
  { CU_performanceTimerEnd(); }

/* Just print an information */
#define CU_INFO(msg) \
  { CU_info(2, __LINE__, (msg), __FILE__, CU_FALSE); }

/** Record a pass condition without performing a logical test. */
#define CU_PASS(msg) \
  { CU_info(1, __LINE__, (msg), __FILE__, CU_FALSE); }

/** Simple assertion.
 *  Reports failure with no other action.
 */
#define CU_ASSERT(value) \
  { CU_assertImplementation((value), __LINE__, #value, __FILE__, CU_FALSE); }

/** Simple assertion.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_FATAL(value) \
  { CU_assertImplementation((value), __LINE__, #value, __FILE__, CU_TRUE); }
#define CU_ASSERT_FATAL1(value, cbFunc, ptr) \
  { CU_assertImplementation1((value), __LINE__, #value, __FILE__, CU_TRUE, cbFunc, ptr); }

/** Simple assertion.
 *  Reports failure with no other action.
 */
#define CU_TEST(value) \
  { CU_assertImplementation((value), __LINE__, #value, __FILE__, CU_FALSE); }

/** Simple assertion.
 *  Reports failure and causes test to abort.
 */
#define CU_TEST_FATAL(value) \
  { CU_assertImplementation((value), __LINE__, #value, __FILE__, CU_TRUE); }
#define CU_TEST_FATAL1(value, cbFunc, ptr) \
  { CU_assertImplementation1((value), __LINE__, #value, __FILE__, CU_TRUE, cbFunc, ptr); }

/** Record a failure without performing a logical test. */
#if 0
#define CU_FAIL(msg) \
  { CU_assertImplementation(CU_FALSE, __LINE__, ("CU_FAIL(" #msg ")"), __FILE__, CU_FALSE); }
#endif
#define CU_FAIL(msg) \
  { CU_info(0, __LINE__, (msg), __FILE__, CU_FALSE); }

/** Record a failure without performing a logical test, and abort test. */
#define CU_FAIL_FATAL(msg) \
  { CU_assertImplementation(CU_FALSE, __LINE__, ("CU_FAIL_FATAL(" #msg ")"), __FILE__, CU_TRUE); }
#define CU_FAIL_FATAL1(msg, cbFunc, ptr) \
  { CU_assertImplementation1(CU_FALSE, __LINE__, ("CU_FAIL_FATAL(" #msg ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that value is CU_TRUE.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_TRUE(value) \
  { CU_assertImplementation((value), __LINE__, ("CU_ASSERT_TRUE(" #value ")"), __FILE__, CU_FALSE); }

/** Asserts that value is CU_TRUE.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_TRUE_FATAL(value) \
  { CU_assertImplementation((value), __LINE__, ("CU_ASSERT_TRUE_FATAL(" #value ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_TRUE_FATAL1(value, cbFunc, ptr) \
  { CU_assertImplementation1((value), __LINE__, ("CU_ASSERT_TRUE_FATAL(" #value ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that value is CU_FALSE.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_FALSE(value) \
  { CU_assertImplementation(!(value), __LINE__, ("CU_ASSERT_FALSE(" #value ")"), __FILE__, CU_FALSE); }

/** Asserts that value is CU_FALSE.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_FALSE_FATAL(value) \
  { CU_assertImplementation(!(value), __LINE__, ("CU_ASSERT_FALSE_FATAL(" #value ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_FALSE_FATAL1(value, cbFunc, ptr) \
  { CU_assertImplementation1(!(value), __LINE__, ("CU_ASSERT_FALSE_FATAL(" #value ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that actual == expected.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_EQUAL(actual, expected) \
  { CU_assertImplementation(((actual) == (expected)), __LINE__, ("CU_ASSERT_EQUAL(" #actual "," #expected ")"), __FILE__, CU_FALSE); }

/** Asserts that actual == expected.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_EQUAL_FATAL(actual, expected) \
  { CU_assertImplementation(((actual) == (expected)), __LINE__, ("CU_ASSERT_EQUAL_FATAL(" #actual "," #expected ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_EQUAL_FATAL1(actual, expected, cbFunc, ptr) \
  { CU_assertImplementation1(((actual) == (expected)), __LINE__, ("CU_ASSERT_EQUAL_FATAL(" #actual "," #expected ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that actual != expected.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_NOT_EQUAL(actual, expected) \
  { CU_assertImplementation(((actual) != (expected)), __LINE__, ("CU_ASSERT_NOT_EQUAL(" #actual "," #expected ")"), __FILE__, CU_FALSE); }

/** Asserts that actual != expected.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_NOT_EQUAL_FATAL(actual, expected) \
  { CU_assertImplementation(((actual) != (expected)), __LINE__, ("CU_ASSERT_NOT_EQUAL_FATAL(" #actual "," #expected ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_NOT_EQUAL_FATAL1(actual, expected, cbFunc, ptr) \
  { CU_assertImplementation1(((actual) != (expected)), __LINE__, ("CU_ASSERT_NOT_EQUAL_FATAL(" #actual "," #expected ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that pointers actual == expected.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_PTR_EQUAL(actual, expected) \
  { CU_assertImplementation(((void*)(actual) == (void*)(expected)), __LINE__, ("CU_ASSERT_PTR_EQUAL(" #actual "," #expected ")"), __FILE__, CU_FALSE); }

/** Asserts that pointers actual == expected.
 * Reports failure and causes test to abort.
 */
#define CU_ASSERT_PTR_EQUAL_FATAL(actual, expected) \
  { CU_assertImplementation(((void*)(actual) == (void*)(expected)), __LINE__, ("CU_ASSERT_PTR_EQUAL_FATAL(" #actual "," #expected ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_PTR_EQUAL_FATAL1(actual, expected, cbFunc, ptr) \
  { CU_assertImplementation1(((void*)(actual) == (void*)(expected)), __LINE__, ("CU_ASSERT_PTR_EQUAL_FATAL(" #actual "," #expected ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that pointers actual != expected.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_PTR_NOT_EQUAL(actual, expected) \
  { CU_assertImplementation(((void*)(actual) != (void*)(expected)), __LINE__, ("CU_ASSERT_PTR_NOT_EQUAL(" #actual "," #expected ")"), __FILE__, CU_FALSE); }

/** Asserts that pointers actual != expected.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_PTR_NOT_EQUAL_FATAL(actual, expected) \
  { CU_assertImplementation(((void*)(actual) != (void*)(expected)), __LINE__, ("CU_ASSERT_PTR_NOT_EQUAL_FATAL(" #actual "," #expected ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_PTR_NOT_EQUAL_FATAL1(actual, expected, cbFunc, ptr) \
  { CU_assertImplementation1(((void*)(actual) != (void*)(expected)), __LINE__, ("CU_ASSERT_PTR_NOT_EQUAL_FATAL(" #actual "," #expected ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that pointer value is NULL.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_PTR_NULL(value) \
  { CU_assertImplementation((NULL == (void*)(value)), __LINE__, ("CU_ASSERT_PTR_NULL(" #value")"), __FILE__, CU_FALSE); }

/** Asserts that pointer value is NULL.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_PTR_NULL_FATAL(value) \
  { CU_assertImplementation((NULL == (void*)(value)), __LINE__, ("CU_ASSERT_PTR_NULL_FATAL(" #value")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_PTR_NULL_FATAL1(value, cbFunc, ptr) \
  { CU_assertImplementation1((NULL == (void*)(value)), __LINE__, ("CU_ASSERT_PTR_NULL_FATAL(" #value")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that pointer value is not NULL.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_PTR_NOT_NULL(value) \
  { CU_assertImplementation((NULL != (void*)(value)), __LINE__, ("CU_ASSERT_PTR_NOT_NULL(" #value")"), __FILE__, CU_FALSE); }

/** Asserts that pointer value is not NULL.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_PTR_NOT_NULL_FATAL(value) \
  { CU_assertImplementation((NULL != (void*)(value)), __LINE__, ("CU_ASSERT_PTR_NOT_NULL_FATAL(" #value")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_PTR_NOT_NULL_FATAL1(value, cbFunc, ptr) \
  { CU_assertImplementation1((NULL != (void*)(value)), __LINE__, ("CU_ASSERT_PTR_NOT_NULL_FATAL(" #value")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that string actual == expected.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_STRING_EQUAL(actual, expected) \
  { CU_assertImplementation(!(strcmp((const char*)(actual), (const char*)(expected))), __LINE__, ("CU_ASSERT_STRING_EQUAL(" #actual ","  #expected ")"), __FILE__, CU_FALSE); }

/** Asserts that string actual == expected.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_STRING_EQUAL_FATAL(actual, expected) \
  { CU_assertImplementation(!(strcmp((const char*)(actual), (const char*)(expected))), __LINE__, ("CU_ASSERT_STRING_EQUAL_FATAL(" #actual ","  #expected ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_STRING_EQUAL_FATAL1(actual, expected, cbFunc, ptr) \
  { CU_assertImplementation1(!(strcmp((const char*)(actual), (const char*)(expected))), __LINE__, ("CU_ASSERT_STRING_EQUAL_FATAL(" #actual ","  #expected ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that string actual != expected.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_STRING_NOT_EQUAL(actual, expected) \
  { CU_assertImplementation((strcmp((const char*)(actual), (const char*)(expected))), __LINE__, ("CU_ASSERT_STRING_NOT_EQUAL(" #actual ","  #expected ")"), __FILE__, CU_FALSE); }

/** Asserts that string actual != expected.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_STRING_NOT_EQUAL_FATAL(actual, expected) \
  { CU_assertImplementation((strcmp((const char*)(actual), (const char*)(expected))), __LINE__, ("CU_ASSERT_STRING_NOT_EQUAL_FATAL(" #actual ","  #expected ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_STRING_NOT_EQUAL_FATAL1(actual, expected, cbFunc, ptr) \
  { CU_assertImplementation1((strcmp((const char*)(actual), (const char*)(expected))), __LINE__, ("CU_ASSERT_STRING_NOT_EQUAL_FATAL(" #actual ","  #expected ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that string actual == expected with length specified.
 *  The comparison is limited to count characters.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_NSTRING_EQUAL(actual, expected, count) \
  { CU_assertImplementation(!(strncmp((const char*)(actual), (const char*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_NSTRING_EQUAL(" #actual ","  #expected "," #count ")"), __FILE__, CU_FALSE); }

/** Asserts that string actual == expected with length specified.
 *  The comparison is limited to count characters.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_NSTRING_EQUAL_FATAL(actual, expected, count) \
  { CU_assertImplementation(!(strncmp((const char*)(actual), (const char*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_NSTRING_EQUAL_FATAL(" #actual ","  #expected "," #count ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_NSTRING_EQUAL_FATAL1(actual, expected, count, cbFunc, ptr) \
  { CU_assertImplementation1(!(strncmp((const char*)(actual), (const char*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_NSTRING_EQUAL_FATAL(" #actual ","  #expected "," #count ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that string actual != expected with length specified.
 *  The comparison is limited to count characters.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_NSTRING_NOT_EQUAL(actual, expected, count) \
  { CU_assertImplementation((strncmp((const char*)(actual), (const char*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_NSTRING_NOT_EQUAL(" #actual ","  #expected "," #count ")"), __FILE__, CU_FALSE); }

/** Asserts that string actual != expected with length specified.
 *  The comparison is limited to count characters.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_NSTRING_NOT_EQUAL_FATAL(actual, expected, count) \
  { CU_assertImplementation((strncmp((const char*)(actual), (const char*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_NSTRING_NOT_EQUAL_FATAL(" #actual ","  #expected "," #count ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_NSTRING_NOT_EQUAL_FATAL1(actual, expected, count, cbFunc, ptr) \
  { CU_assertImplementation1((strncmp((const char*)(actual), (const char*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_NSTRING_NOT_EQUAL_FATAL(" #actual ","  #expected "," #count ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that memory block actual == expected.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_MEMORY_EQUAL(actual, expected, count) \
  { CU_assertImplementation(!(memcmp((void *)(actual), (void*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_STRING_EQUAL(" #actual ","  #expected ")"), __FILE__, CU_FALSE); }

/** Asserts that memory block actual == expected.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_MEMORY_EQUAL_FATAL(actual, expected, count) \
  { CU_assertImplementation(!(memcmp((void *)(actual), (void*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_STRING_EQUAL_FATAL(" #actual ","  #expected ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_MEMORY_EQUAL_FATAL1(actual, expected, count, cbFunc, ptr) \
  { CU_assertImplementation1(!(memcmp((void *)(actual), (void*)(expected), (int)(count))), __LINE__, ("CU_ASSERT_STRING_EQUAL_FATAL(" #actual ","  #expected ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that memory block actual == expected.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_MEMORY_NOT_EQUAL(actual, expected, count) \
  { CU_assertImplementation(((memcmp((void *)(actual), (void*)(expected), (int)(count)))==0?0:1), __LINE__, ("CU_ASSERT_STRING_EQUAL(" #actual ","  #expected ")"), __FILE__, CU_FALSE); }

/** Asserts that memory block actual == expected.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_MEMORY_NOT_EQUAL_FATAL(actual, expected, count) \
  { CU_assertImplementation(((memcmp((void *)(actual), (void*)(expected), (int)(count)))==0?0:1), __LINE__, ("CU_ASSERT_STRING_EQUAL_FATAL(" #actual ","  #expected ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_MEMORY_NOT_EQUAL_FATAL1(actual, expected, count, cbFunc, ptr) \
  { CU_assertImplementation1(((memcmp((void *)(actual), (void*)(expected), (int)(count)))==0?0:1), __LINE__, ("CU_ASSERT_STRING_EQUAL_FATAL(" #actual ","  #expected ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that double actual == expected within the specified tolerance.
 *  If actual is within granularity of expected, the assertion passes.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_DOUBLE_EQUAL(actual, expected, granularity) \
  { CU_assertImplementation(((fabs((double)(actual) - (expected)) <= fabs((double)(granularity)))), __LINE__, ("CU_ASSERT_DOUBLE_EQUAL(" #actual ","  #expected "," #granularity ")"), __FILE__, CU_FALSE); }

/** Asserts that double actual == expected within the specified tolerance.
 *  If actual is within granularity of expected, the assertion passes.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_DOUBLE_EQUAL_FATAL(actual, expected, granularity) \
  { CU_assertImplementation(((fabs((double)(actual) - (expected)) <= fabs((double)(granularity)))), __LINE__, ("CU_ASSERT_DOUBLE_EQUAL_FATAL(" #actual ","  #expected "," #granularity ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_DOUBLE_EQUAL_FATAL1(actual, expected, granularity, cbFunc, ptr) \
  { CU_assertImplementation1(((fabs((double)(actual) - (expected)) <= fabs((double)(granularity)))), __LINE__, ("CU_ASSERT_DOUBLE_EQUAL_FATAL(" #actual ","  #expected "," #granularity ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

/** Asserts that double actual != expected within the specified tolerance.
 *  If actual is within granularity of expected, the assertion fails.
 *  Reports failure with no other action.
 */
#define CU_ASSERT_DOUBLE_NOT_EQUAL(actual, expected, granularity) \
  { CU_assertImplementation(((fabs((double)(actual) - (expected)) > fabs((double)(granularity)))), __LINE__, ("CU_ASSERT_DOUBLE_NOT_EQUAL(" #actual ","  #expected "," #granularity ")"), __FILE__, CU_FALSE); }

/** Asserts that double actual != expected within the specified tolerance.
 *  If actual is within granularity of expected, the assertion fails.
 *  Reports failure and causes test to abort.
 */
#define CU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(actual, expected, granularity) \
  { CU_assertImplementation(((fabs((double)(actual) - (expected)) > fabs((double)(granularity)))), __LINE__, ("CU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(" #actual ","  #expected "," #granularity ")"), __FILE__, CU_TRUE); }
#define CU_ASSERT_DOUBLE_NOT_EQUAL_FATAL1(actual, expected, granularity, cbFunc, ptr) \
  { CU_assertImplementation1(((fabs((double)(actual) - (expected)) > fabs((double)(granularity)))), __LINE__, ("CU_ASSERT_DOUBLE_NOT_EQUAL_FATAL(" #actual ","  #expected "," #granularity ")"), __FILE__, CU_TRUE, cbFunc, ptr); }

#endif  /*  CUNIT_CUNIT_H_SEEN  */

/** @} */
