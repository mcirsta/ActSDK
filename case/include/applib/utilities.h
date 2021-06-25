/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : utilities.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-6             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     utilities.h
 * \brief    application utilities  
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/

/*! 
 * \defgroup   applib_utilities    Ӧ�ó��ù��ߺ���
 */

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#ifdef ABS
#undef ABS
#endif

#pragma __PRQA_IGNORE_START__
#define ABS(x)   (((x) >= 0 ) ? (x) : ((-1) * (x)))
#pragma __PRQA_IGNORE_END__

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��16���Ƹ�ʽ��ӡһ��buffer������ 
 * \ingroup applib_utilities
 * \param[in] buf -- ����ӡbuffer���׵�ַ 
 * \param[in] size -- buffer�Ĵ�С(�ֽ�) 
 * \return  void     
 *******************************************************************************/
extern void print_buf(const char * buf, unsigned int size);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   ��һ�����������ο�ƽ��ֵ 
 * \ingroup applib_utilities
 * \param[in] a -- ����ƽ���� 
 * \return ��ƽ�����       
 *******************************************************************************/
extern unsigned short int_sqrt(unsigned long a);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   ��ȡһ��·�������ļ��� 
 * \ingroup applib_utilities
 * \param[in] filename -- ·���� 
 * \return  �ļ���ָ��       
 *******************************************************************************/
extern char *basename(const char *filename);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   ��ȡһ���ļ���·���� 
 * \ingroup applib_utilities
 * \param[in] path -- �ļ��� 
 * \return  �ļ�·����ָ��       
 *******************************************************************************/
extern char * dirname(char *path);

#endif /* #ifndef __UTILITIES_H__ */

