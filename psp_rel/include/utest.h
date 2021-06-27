/********************************************************************************
 *                              USDK(1100)
 *                            Module: Unit Test Library
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>                            <version >             <desc>
 *       sangwei     2008-12-31   15:40     1.0                        build this file
 ********************************************************************************/
#ifndef __UTEST_H__
#define __UTEST_H__

#if (__TESTENV__==0)                        // 裸机环境单元测试
#include "./utest/setjmp.h"
#elif (__TESTENV__==1 || __TESTENV__==2)    // USDK环境单元测试
#include "./ucos/string.h"
#include "./ucos/setjmp.h"
#elif (__TESTENV__==3 || __TESTENV__==4)    // PC环境单元测试 LSDK环境单元测试
#include <string.h>
#include <setjmp.h>
#include <stdio.h>
#else
#error "__TESTENV__ have wrong value"
#endif /* end of (__TESTENV__==0) */

#include "assert.h"
//#include "./utest/remote.h"
#include "./utest/cunit_arch.h"

#endif /* end of __UTEST_H__ */
