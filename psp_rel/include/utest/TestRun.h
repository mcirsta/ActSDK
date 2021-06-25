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
 *  Contains Interface to Run tests.
 *
 *  Aug 2001      Initial implementation. (AK)
 *
 *  09/Aug/2001   Contains generic run tests interface which can be used
 *                for any type of frontend interface framework. (AK)
 *
 *  24/Nov/2001   Added Handler for Group Initialization failure condition. (AK)
 *
 *  05-Aug-2004   New interface.  Since these should be internal functions,
 *                no support for deprecated version 1 names provided now,
 *                eliminated global variables for current test & suite,
 *                moved (renamed) _TestResult here from TestDB.h. (JDS)
 *
 *  05-Sep-2004   Added internal test interface. (JDS)
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
 *  Test run management functions (user interface).
 *  The TestRun module implements functions supporting the running
 *  of tests elements (suites and tests).  This includes functions for
 *  running suites and tests, retrieving the number of tests/suites run,
 *  and managing callbacks during the run process.
 *
 *  <P>The callback mechanism works as follows.  The CUnit runtime system
 *  supports the registering and calling of functions at the start and end
 *  of each test, when all tests are complete, and when a suite
 *  initialialization function returns an error.  This allows clients to
 *  perform actions associated with these events such as output formatting
 *  and reporting.</P>
 */
/** @addtogroup Framework
 * @{
 */

#ifndef CUNIT_TESTRUN_H_SEEN
#define CUNIT_TESTRUN_H_SEEN

#include "CUnit.h"
#include "CUError.h"
#include "TestDB.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* CU_PerformanceRecord definition */
/** Data type for holding assertion failure information (linked list). */
typedef struct CU_PerformanceRecord
{
    const char* strFuncName; /* Name of tested function */
    const char* strFileName; /* Name of file where performance occured */

    unsigned int time_start; /* start time of performance record */
    unsigned int time_end; /* end time of performance record */
    unsigned int time_diff; /* time difference, time_end - time_start */

    CU_pTest pTest; /* Test containing Performance */
    CU_pSuite pSuite; /* Suite containing test having failure. */

    struct CU_PerformanceRecord* pNext; /* Pointer to next record in linked list. */
    struct CU_PerformanceRecord* pPrev; /* Pointer to previous record in linked list. */
} CU_PerformanceRecord;
typedef struct CU_PerformanceRecord* CU_pPerformanceRecord; /* Pointer to CU_PerformanceRecord */

/* CU_InfoRecord definition */
typedef struct CU_InfoRecord
{
    unsigned int uiLineNumber; /**< Line number of failure. */
    const char* strFileName; /**< Name of file where failure occurred. */
    const char* strCondition; /**< Test condition which failed. */

    CU_pTest pTest; /**< Test containing failure. */
    CU_pSuite pSuite; /**< Suite containing test having failure. */

    int written_mark; /* if this info record is already written to file, this mark should set to 1, otherwise 0 */
    int infoValue;

    struct CU_InfoRecord* pNext; /**< Pointer to next record in linked list. */
    struct CU_InfoRecord* pPrev; /**< Pointer to previous record in linked list. */
} CU_InfoRecord;
typedef CU_InfoRecord* CU_pInfoRecord; /**< Pointer to CU_InfoRecord. */

/* CU_FailureRecord type definition. */
/** Data type for holding assertion failure information (linked list). */
typedef struct CU_FailureRecord
{
    unsigned int uiLineNumber; /**< Line number of failure. */
    const char* strFileName; /**< Name of file where failure occurred. */
    const char* strCondition; /**< Test condition which failed. */

    CU_pTest pTest; /**< Test containing failure. */
    CU_pSuite pSuite; /**< Suite containing test having failure. */

    int written_mark; /* if this failure record is already written to file, this mark should set to 1, otherwise 0 */

    struct CU_FailureRecord* pNext; /**< Pointer to next record in linked list. */
    struct CU_FailureRecord* pPrev; /**< Pointer to previous record in linked list. */
} CU_FailureRecord;
typedef CU_FailureRecord* CU_pFailureRecord; /**< Pointer to CU_FailureRecord. */

/* CU_RunSummary type definition. */
/** Data type for holding statistics and assertion failures for a test run. */
typedef struct CU_RunSummary
{
    unsigned int nSuitesRun; /**< Number of suites completed during run. */
    unsigned int nSuitesFailed; /**< Number of suites containing failed assertions. */
    unsigned int nSuitesInitFailed; /**< Number of suites for which initialization failed. */
    unsigned int nSuitesExitFailed; /**< Number of suites for which exit failed. */
    unsigned int nTestsRun; /**< Number of tests completed during run. */
    unsigned int nTestsFailed; /**< Number of tests containing failed assertions. */
    unsigned int nTestsInitFailed; /**< Number of tests for which initialization failed. */
    unsigned int nTestsExitFailed; /**< Number of tests for which exit failed. */
    unsigned int nAsserts; /**< Number of assertions tested during run. */
    unsigned int nAssertsFailed; /**< Number of failed assertions. */
    unsigned int nFailureRecords; /**< Number of failure records generated. */
    unsigned int nInfoRecords; /**< Number of info records generated. */
} CU_RunSummary;
typedef CU_RunSummary* CU_pRunSummary; /**< Pointer to CU_RunSummary. */

typedef void (*CU_TestInitFailureMessageHandler)(const CU_pTest pTest,
        const CU_pSuite pSuite);
typedef void (*CU_TestCleanupFailureMessageHandler)(const CU_pTest pTest,
        const CU_pSuite pSuite);

/* Type Definitions for Message Handlers. */
/** Message handler called at the start of a test.
 *  The parameters are the test and suite being run.
 *  The test run is considered in progress when the
 *  message handler is called.  Neither pTest nor
 *  pSuite may be null.
 */
typedef void (*CU_TestStartMessageHandler)(const CU_pTest pTest,
        const CU_pSuite pSuite);
/** Message handler called at the completion of a test.
 *  The parameters are the test and suite being run, plus
 *  a pointer to the first failure record applicable to
 *  this test.  If the test did not have any assertion
 *  failures, pFailure will be NULL.
 *  The test run is considered in progress when the
 *  message handler is called.
 */
typedef void (*CU_TestCompleteMessageHandler)(const CU_pTest pTest,
        const CU_pSuite pSuite, const CU_pFailureRecord pFailure,
        const CU_pInfoRecord pInfo);
/** Message handler called at the completion of a test run.
 *  The parameter is a pointer to the linked list holding
 *  the failure records for the test run.
 *  The test run is considered completed when the
 *  message handler is called.
 */
typedef void (*CU_AllTestsCompleteMessageHandler)(
        const CU_pFailureRecord pFailure, const CU_pPerformanceRecord pRecord);

/** Message handler called when a suite initializer fails.
 *  The test run is considered in progress when the
 *  message handler is called.
 */
typedef void (*CU_SuiteInitFailureMessageHandler)(const CU_pSuite pSuite);

/** Message handler called when a suite cleanup function fails.
 *  The test run is considered in progress when the
 *  message handler is called.
 */
typedef void (*CU_SuiteCleanupFailureMessageHandler)(const CU_pSuite pSuite);

/** Message handler called when in anytime wanted to flush test asserts */
typedef void (*CU_FlushHandler)(const CU_pFailureRecord pFailure,
        const CU_pInfoRecord pInfo);

/** Message handler called when user want to change mount dir */
typedef void (*CU_MountDirHandler)(char* CU_mountdir);

/* Get/Set functions for Message Handlers. */
CU_EXPORT void CU_set_test_start_handler(
        CU_TestStartMessageHandler pTestStartMessage);
CU_EXPORT void CU_set_test_complete_handler(
        CU_TestCompleteMessageHandler pTestCompleteMessage);
CU_EXPORT void CU_set_all_test_complete_handler(
        CU_AllTestsCompleteMessageHandler pAllTestsCompleteMessage);
CU_EXPORT void CU_set_test_init_failure_handler(
        CU_TestInitFailureMessageHandler pTestInitFailureMessage);
CU_EXPORT void CU_set_test_cleanup_failure_handler(
        CU_TestCleanupFailureMessageHandler pTestCleanupFailureMessage);
CU_EXPORT void CU_set_suite_init_failure_handler(
        CU_SuiteInitFailureMessageHandler pSuiteInitFailureMessage);
CU_EXPORT void CU_set_suite_cleanup_failure_handler(
        CU_SuiteCleanupFailureMessageHandler pSuiteCleanupFailureMessage);
CU_EXPORT void CU_set_flush_handler(CU_FlushHandler pFlushHandler);

CU_EXPORT CU_TestStartMessageHandler CU_get_test_start_handler(void);
CU_EXPORT CU_TestCompleteMessageHandler CU_get_test_complete_handler(void);
CU_EXPORT CU_AllTestsCompleteMessageHandler CU_get_all_test_complete_handler(void);
CU_EXPORT CU_TestInitFailureMessageHandler CU_get_test_init_failure_handler(void);
CU_EXPORT CU_TestCleanupFailureMessageHandler CU_get_test_cleanup_failure_handler(void);
CU_EXPORT CU_SuiteInitFailureMessageHandler CU_get_suite_init_failure_handler(void);
CU_EXPORT CU_SuiteCleanupFailureMessageHandler CU_get_suite_cleanup_failure_handler(void);
CU_EXPORT CU_FlushHandler CU_get_flush_handler(void);

/* Functions for running registered tests and suites. */
CU_EXPORT CU_ErrorCode CU_run_all_tests(void);
CU_EXPORT CU_ErrorCode CU_run_suite(CU_pSuite pSuite);
CU_EXPORT CU_ErrorCode CU_run_test(CU_pSuite pSuite, CU_pTest pTest);

/* Functions for getting information about the previous test run. */
CU_EXPORT unsigned int CU_get_number_of_suites_run(void);
CU_EXPORT unsigned int CU_get_number_of_suites_failed(void);
CU_EXPORT unsigned int CU_get_number_of_tests_run(void);
CU_EXPORT unsigned int CU_get_number_of_tests_failed(void);
CU_EXPORT unsigned int CU_get_number_of_asserts(void);
CU_EXPORT unsigned int CU_get_number_of_successes(void);
CU_EXPORT unsigned int CU_get_number_of_failures(void);
CU_EXPORT unsigned int CU_get_number_of_failure_records(void);
CU_EXPORT CU_pFailureRecord CU_get_failure_list(void);
CU_EXPORT CU_pRunSummary CU_get_run_summary(void);

/* Functions for internal & testing use. */
CU_EXPORT CU_pSuite CU_get_current_suite(void);
CU_EXPORT CU_pTest CU_get_current_test(void);
CU_EXPORT CU_BOOL CU_is_test_running(void);
CU_EXPORT void CU_clear_previous_results(void);

/* Assert-like information function, added by zhangyh */
int CU_info(int infoValue, unsigned int uiLine, char strCondition[],
        char strFile[], CU_BOOL bFatal);

/* Assertion implementation function. */
CU_EXPORT CU_BOOL CU_assertImplementation(CU_BOOL bValue,
        unsigned int uiLine,
        char strCondition[],
        char strFile[],
        CU_BOOL bFatal);

typedef void (* cbFunc)(void *ptr);
CU_EXPORT CU_BOOL CU_assertImplementation1(CU_BOOL bValue,
        unsigned int uiLine,
        char strCondition[],
        char strFile[],
        CU_BOOL bFatal,
        cbFunc cbfunc, void *ptr);

#ifdef CUNIT_BUILD_TESTS
void test_exit(int status);
void CU_set_current_suite(CU_pSuite pSuite);
void CU_set_current_test(CU_pTest pTest);
CU_pFailureRecord CU_get_last_failure(void);

void add_info(CU_pInfoRecord* ppInfo,
        CU_pRunSummary pRunSummary,
        unsigned int uiLineNumber,
        char szCondition[],
        char szFileName[],
        CU_pSuite pSuite,
        CU_pTest pTest,
        int infoValue);

void add_failure(CU_pFailureRecord* ppFailure,
        CU_pRunSummary pRunSummary,
        unsigned int uiLineNumber,
        char szCondition[],
        char szFileName[],
        CU_pSuite pSuite,
        CU_pTest pTest);
void clear_previous_results(CU_pRunSummary pRunSummary, CU_pFailureRecord* ppFailure);
#endif /* end of CUNIT_BUILD_TESTS */

#ifdef __cplusplus
}
#endif
#endif  /*  CUNIT_TESTRUN_H_SEEN  */
/** @} */
