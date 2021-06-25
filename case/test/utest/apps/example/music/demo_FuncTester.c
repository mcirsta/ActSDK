/********************************************************************************
*                              usdk130
*                            Module: testdrv unit test
*                 Copyright(c) 2001-2008 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>          <time>                  <version >         <desc>
*       jiancailiu     2008-05-20 11:00        1.0              build this file
********************************************************************************/
/*!
* \file     name_FuncTester.c
* \brief    name func cases
* \author   jiancailiu
* \par      GENERAL DESCRIPTION:
*               examle unit test with driver, one function test here
* \par      EXTERNALIZED FUNCTIONS:
*               driver "testdrv"
*
*      Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
*
* \version 1.0
* \date  2008/5/20
*******************************************************************************/
#include "demo_utest.h"



/******************************************************************************/
/*!
* \par  Description:
*	  function for suite init
* \param[in]    none
* \param[out]   none
* \return       the result
* \retval           0 sucess
* \retval           1 failed
* \ingroup      module name
* \par          exmaple code
* \code
*               list example code in here
* \endcode
*******************************************************************************/
int suite_testvalue_init(void)
{

	/* suite初始化 */

	return 0;   /* 返回1此整个siute将不被执行 */
}

/******************************************************************************/
/*!
* \par  Description:
*	  function for suite clean
* \param[in]    none
* \param[out]   none
* \return       the result
* \retval           0 sucess
* \retval           1 failed
* \ingroup      module name
* \par          exmaple code
* \code
*               list example code in here
* \endcode
*******************************************************************************/
int suite_testvalue_clean(void)
{

	/* suite清除 */

	return 0;   /* 返回1会给出警告信息 */
}

/******************************************************************************/
/*!
* \par  Description:
*	  function for test init
* \param[in]    none
* \param[out]   none
* \return       the result
* \retval           0 sucess
* \retval           1 failed
* \ingroup      module name
* \par          exmaple code
* \code
*               list example code in here
* \endcode
*******************************************************************************/
int test_value_init(void)
{
	/* test初始化 */

	return 0;   /* 返回1此整个test将不被执行 */
}

/******************************************************************************/
/*!
* \par  Description:
*	  function for test clean
* \param[in]    none
* \param[out]   none
* \return       the result
* \retval           0 sucess
* \retval           1 failed
* \ingroup      module name
* \par          exmaple code
* \code
*               list example code in here
* \endcode
*******************************************************************************/
int test_value_clean(void)
{
	/* test清除 */

	return 0;   /* 返回1会给出警告信息 */
}

/******************************************************************************/
/*!
* \par  Description:
*	  function for test cases
* \param[in]    none
* \param[out]   none
* \return       none
* \retval           0 sucess
* \retval           1 failed
* \ingroup      module name
* \par          exmaple code
* \code
*               list example code in here
* \endcode
*******************************************************************************/
static void test_value_usual(void)
{
    int a = 3;
    int b = 1;
    CU_ASSERT_TRUE(test_value(a, b));
}

static void test_value_unusual(void)
{
    int a = 1;
    int b = 3;
    CU_ASSERT_TRUE(!test_value(a, b));
}


/*!
 *    cases in tests \n
 *
 */
CU_TestInfo test_value_func[] = {
{ "cases_test_value_usual", test_value_usual, test_value_init, test_value_clean },
{ "cases_test_value_unusual", test_value_unusual, test_value_init, test_value_clean },
	CU_TEST_INFO_NULL,
};

