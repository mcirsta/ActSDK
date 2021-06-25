/**
 * unit test framwork for USDK(mips, ucos-ii based)
 * Copyright (C) 2008            Actions-Semiconductor
 *
 * sangwei@actions-semi.com
 * 16-04-2008
 *
 */

/** @file
 * Remote interface with output to PC file.
 */
/** @addtogroup Remote
 * @{
 */

#ifndef CUNIT_REMOTE_H_SEEN
#define CUNIT_REMOTE_H_SEEN

#include "CUnit.h"
#include "TestDB.h"

#ifdef __cplusplus
extern "C"
{
#endif

    CU_EXPORT CU_ErrorCode CU_remote_run_tests(void);
CU_EXPORT CU_ErrorCode CU_remote_run_suite(CU_pSuite pSuite);
CU_EXPORT CU_ErrorCode CU_remote_run_test(CU_pSuite pSuite, CU_pTest pTest);
CU_EXPORT void CU_remote_show_failures(CU_pFailureRecord pFailure);

#ifdef __cplusplus
}
#endif
#endif  /*  CUNIT_REMOTE_H_SEEN  */
/** @} */
