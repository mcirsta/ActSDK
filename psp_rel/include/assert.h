/********************************************************************************
 *                              USDK 1100
 *                            Module: ASSERT
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-10-7 18:38     1.0             build this file
 ********************************************************************************/
/*!
 * \file     assert.h
 * \brief    断言宏定义
 * \author   hmwei
 * \version 1.0
 * \date  2008/10/7
 *******************************************************************************/
#ifndef __ASSERT_H__
#define __ASSERT_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*!
     * \brief
     *      断言宏定义
     * \note
     1)在函数开始处检验传入参数的合法性
     如:
     int resetBufferSize(int nNewSize)
     {
     //功能:改变缓冲区大小,
     //参数:nNewSize 缓冲区新长度
     //返回值:缓冲区当前长度
     //说明:保持原信息内容不变 nNewSize<=0表示清除缓冲区
     assert(nNewSize >= 0);
     assert(nNewSize <= MAX_BUFFER_SIZE);
     ...
     }

     2)每个assert只检验一个条件,因为同时检验多个条件时,如果断言失败,无法直观的判断是哪个条件失败
     不好: assert(nOffset>=0 && nOffset+nSize<=m_nInfomationSize);
     好: assert(nOffset >= 0);
     assert(nOffset+nSize <= m_nInfomationSize);

     3)不能使用改变环境的语句,因为assert只在DEBUG个生效,如果这么做,会使用程序在真正运行时遇到问题
     错误: assert(i++ < 100)
     这是因为如果出错，比如在执行之前i=100,那么这条语句就不会执行，那么i++这条命令就没有执行。
     正确: assert(i < 100)
     i++;

     4)assert和后面的语句应空一行,以形成逻辑和视觉上的一致感

     5)有的地方,assert不能代替条件过滤
     ---------------------------------------------------------
     在switch语句中总是要有default子句来显示信息(Assert)。
     int number = SomeMethod();

     switch(number)
     {
     case 1:
     Trace.WriteLine("Case 1:");
     break;
     case 2:
     Trace.WriteLine("Case 2:");
     break;
     default :
     Debug.Assert(false);
     break;
     }
     */
#pragma __PRQA_IGNORE_START__

#undef assert

#ifdef NDEBUG
#define assert(p)  	((void)0)
#else

#ifdef __KLOCWORK__

#define assert(e) do { \
	if(!(e)) { \
		abort(); \
	} \
} while (0)

#else

#define assert(e)	((e) ? ((void)0) : (printf("assert failed in file %s at line %d\n",__FILE__,__LINE__)))
#endif

#endif /* NDEBUG */

#pragma __PRQA_IGNORE_END__

#ifdef __cplusplus
}
#endif

#endif /*__ASSERT_H__*/
