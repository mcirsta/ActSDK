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
 * \brief    ���Ժ궨��
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
     *      ���Ժ궨��
     * \note
     1)�ں�����ʼ�����鴫������ĺϷ���
     ��:
     int resetBufferSize(int nNewSize)
     {
     //����:�ı仺������С,
     //����:nNewSize �������³���
     //����ֵ:��������ǰ����
     //˵��:����ԭ��Ϣ���ݲ��� nNewSize<=0��ʾ���������
     assert(nNewSize >= 0);
     assert(nNewSize <= MAX_BUFFER_SIZE);
     ...
     }

     2)ÿ��assertֻ����һ������,��Ϊͬʱ����������ʱ,�������ʧ��,�޷�ֱ�۵��ж����ĸ�����ʧ��
     ����: assert(nOffset>=0 && nOffset+nSize<=m_nInfomationSize);
     ��: assert(nOffset >= 0);
     assert(nOffset+nSize <= m_nInfomationSize);

     3)����ʹ�øı价�������,��Ϊassertֻ��DEBUG����Ч,�����ô��,��ʹ�ó�������������ʱ��������
     ����: assert(i++ < 100)
     ������Ϊ�������������ִ��֮ǰi=100,��ô�������Ͳ���ִ�У���ôi++���������û��ִ�С�
     ��ȷ: assert(i < 100)
     i++;

     4)assert�ͺ�������Ӧ��һ��,���γ��߼����Ӿ��ϵ�һ�¸�

     5)�еĵط�,assert���ܴ�����������
     ---------------------------------------------------------
     ��switch���������Ҫ��default�Ӿ�����ʾ��Ϣ(Assert)��
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
