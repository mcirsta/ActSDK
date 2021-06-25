/********************************************************************************
 *                              	NOYA 1100
 *                            Module: libc_sysindep_api
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lbkang     2008-07-25 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     ctype.h
 * \brief    ��ͷ�ļ������ĺ������ڶԵ����ַ����з����ת�����������к������
 *			��Ҫһ��int���͵��Ա�������ֵ����0��255֮�䣬����ΪEOF.
 * \author   lbkang
 * \version  version 1.0
 * \date  2008/07/30
 *******************************************************************************/
/*!\cond LIBC*/
/*!
 * \defgroup libc_char �ַ������ת��
 */
/*!\endcond*/

#ifndef __OS_CTYPE_H__
#define __OS_CTYPE_H__

/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */

/*!
 * \brief
 *      ��ԪcΪ��ĸ������ʱ���������ط�0ֵ�����򷵻�0��
 * \li  c: int�����Ա���
 * \ingroup libc_char
 */
#define isalnum(c) (((((c) | 0x20) - 'a') < 26U) || (((c) - '0') < 10U))

/*!
 * \brief
 *      ��ԪcΪ��ĸʱ���������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isalpha(c) ((('a' <= (c)) && ((c) <= 'z')) || (('A' <= (c)) && ((c) <= 'Z')))

/*!
 * \brief
 *      ��ԪcΪascii�ַ�ʱ���������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isascii(c) (((c) & ~0x7f) == 0)

/*!
 * \brief
 *      ת������cΪascii�ַ�����������ת�����ֵ��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define toascii(c) ((c) & 0x7f)

/*!
 * \brief
 *      ��ԪcΪ�ո񣨺��Ʊ����ʱ���������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isblank(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\v'))

/*!
 * \brief
 *      ��Ԫc�ǿ����ַ�ʱ���������ط�0�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define iscntrl(c) (((c) < 32) || ((c) == 127))

/*!
 * \brief
 *      ��Ԫ��ʮ��������ʱ���������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isdigit(c) (('0' <= (c)) && ((c) <= '9'))

/*!
 * \brief
 *      ��ԪcΪ���ո�֮����κοɴ�ӡ�ַ����������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isgraph(c) ('!' <= (c) && (c) <= '~')

/*!
 * \brief
 *      ����cΪСд��ĸʱ���������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define islower(c) (('a' <= (c)) && ((c) <= 'z'))

/*!
 * \brief
 *      cΪ��д��ĸʱ���������Ӧ��Сд��ĸ�����򷵻�c��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define tolower(c) (isupper(c) ? (((c) - 'A') + 'a') : (c))

/*!
 * \brief
 *      ��Ԫc�ǿɴ�ӡ�ַ�(���ո�)���������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isprint(c) (' ' <= (c) && (c) <= '~')

/*!
 * \brief
 *      ��ԪcΪ�հ��ַ����������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isspace(c) (((((unsigned int)(c)) - '\t') < 5U) || ((c) == ' '))

/*!
 * \brief
 *      ��ԪcΪ����ַ����������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define ispunct(c) (isprint(c) && !isalnum(c) && !isspace(c))

/*!
 * \brief
 *      ��ԪcΪ��д��ĸʱ���������ط�0ֵ�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isupper(c) (('A' <= (c)) && ((c) <= 'Z'))

/*!
 * \brief
 *      ��cΪСд��ĸʱ���������Ӧ�Ĵ�д��ĸ�����򷵻�c��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define toupper(c) (islower(c) ? (((c) - 'a') + 'A') : (c))

/*!
 * \brief
 *      ��ԪcΪʮ����������ʱ���������ط�0�����򷵻�0��
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define isxdigit(c)	((('0' <= (c)) && ((c) <= '9')) || (('a' <= (c)) && ((c) <= 'f')) \
                || (('A' <= (c)) && ((c) <= 'F')))

/*!
 * \brief
 *      ת��cΪСд��ĸ��Ӧ�ó���֤cΪ��д��ĸ���ȼ���tolower()
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define _tolower(c) ((c) - 'A' + 'a')

/*!
 * \brief
 *      ת��cΪ��д��ĸ��Ӧ�ó���֤cΪСд��ĸ���ȼ���toupper()
 * \li  c: int �����Ա���
 * \ingroup libc_char
 */
#define _toupper(c) ((c) - 'a' + 'A')

#endif/*end of __OS_CTYPE_H__*/
