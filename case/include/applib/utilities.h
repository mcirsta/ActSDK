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
 * \defgroup   applib_utilities    应用常用工具函数
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
 *    按16进制格式打印一块buffer的内容 
 * \ingroup applib_utilities
 * \param[in] buf -- 欲打印buffer的首地址 
 * \param[in] size -- buffer的大小(字节) 
 * \return  void     
 *******************************************************************************/
extern void print_buf(const char * buf, unsigned int size);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   求一个整数的整形开平方值 
 * \ingroup applib_utilities
 * \param[in] a -- 被开平方数 
 * \return 开平方结果       
 *******************************************************************************/
extern unsigned short int_sqrt(unsigned long a);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   获取一个路径名的文件名 
 * \ingroup applib_utilities
 * \param[in] filename -- 路径名 
 * \return  文件名指针       
 *******************************************************************************/
extern char *basename(const char *filename);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   获取一个文件的路径名 
 * \ingroup applib_utilities
 * \param[in] path -- 文件名 
 * \return  文件路径名指针       
 *******************************************************************************/
extern char * dirname(char *path);

#endif /* #ifndef __UTILITIES_H__ */

