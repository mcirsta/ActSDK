/********************************************************************************
 *                               1100
 *                            Module: fileselector Driver
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                          All Rights Reserved.
 *
 * History:
 *      <author>    <time>       <version >    <desc>
 *      lyxiang     2008/08/28     1100         build this file
 ********************************************************************************
 */

/*!
 * \file fileselector.h
 * \brief file selector api for 1100
 * \author   lyxiang
 * \par GENERAL DESCRIPTION:
 *  ���ļ�����Ŀ¼ѡ����ģ���ṩ��ȥ�����нӿں����ݽṹ
 *
 *     Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date 2008/08/28
 */

#ifndef __FILESELECTOR_H__
#define __FILESELECTOR_H__

#include <libc/stdio.h>
#include <libc/string.h>
#include <libc/unistd.h>
#include <libc/dirent.h>
#include <libc/sys/limits.h>
#include "act_errno.h"
//#include "types.h"

/*! \cond fsel_api*/

/*!
 * \brief
 * ���֧��Ŀ¼������
 */
#define FSEL_MAX_DIR_DEPTH 64

/*!
 * \brief
 * ���֧���ļ�������
 */
//#define FSEL_MAX_NAMELENGTH 1024

/*!
 * \brief
 * ������С
 */
#define  FSEL_SECTORSIZE  512

/*!
 * \brief
 * Ŀ¼ѡ�������
 */
typedef void * FSEL_HANDLE;

/*!
 *  \brief
 *  �ļ�/Ŀ¼�ṹ����
 *\li FSEL_FILENAME:�ļ�
 *\li FSEL_DIRNAME:Ŀ¼
 *\li FSEL_DIRNAME:both,��ҪӦ������Ŀ¼����ʱ������Χ�Ŀ���
 */
typedef enum
{
    FSEL_FILENAME = 1,
    FSEL_DIRNAME = 2,
    FSEL_RESERVE = 3,
} fsel_filenametype_t;

/*!
 * \brief
 *  �ļ�/Ŀ¼���ṹ.
 */
typedef struct
{
    /*! Ŀ¼/�ļ��ṹ��*/
    fsel_filenametype_t type;
    /*! �ļ���*/
    char name[PATH_MAX];
    /*!�ļ���·����Ϣ*/
    char *filepath;//�ļ���·����ע��:��ʹ��fsel_listfile��fsel_listdir��ȡ�ļ�ʱ�����ܻ�ȡ���ļ���·��
} fsel_filename_t;

/*!
 *  \brief
 *  ���������ж�
 *\li FSEL_NOSORTING:������
 *\li FSEL_PINYIN:ƴ������
 *\li FSEL_BINARY:ASC������
 */
typedef enum
{
    FSEL_NOSORTING = 0, //������
    FSEL_PINYIN = 1, //ƴ������
    FSEL_BINARY = 2,
} fsel_sortstyle_t;

/*!
 *  \brief
 *  �����ļ���Ч�ļ��ķ�Χ����
 *\li FSEL_SUB:������Χ����Ŀ¼
 *\li FSEL_NOSUB:��������Ŀ¼
 */
typedef enum
{
    FSEL_SUB = 1,
    FSEL_NOSUB = 2,
} fsel_subdir_t;

/*!
 *  \brief
 * �ļ�/Ŀ¼���͵��趨
 *\li FSEL_FILENUM:��ǰĿ¼����Ч�ļ�����
 *\li FSEL_DIRNUM:��ǰĿ¼��Ŀ¼����
 */
typedef enum
{
    FSEL_FILENUM = 1,
    FSEL_DIRNUM = 2,
} fsel_itemtype_t;

/*!
 *  \brief
 * �趨��ȡ�ļ�/Ŀ¼��˳��
 *\li FSEL_POSITIVE:˳���ȡ�ļ�/Ŀ¼
 *\li FSEL_CONVERSE:�����ȡ�ļ�/Ŀ¼
 */
typedef enum
{
    FSEL_POSITIVE = 0,
    FSEL_CONVERSE = 1,
} fsel_listtype_t;

/*!
 *  \brief
 * �������ݿ��м�¼���ļ�/Ŀ¼��
 *\li ADDDATA:�����ļ�/Ŀ¼
 *\li DELDATA:ɾ���ļ�/Ŀ¼
 */
typedef enum
{
    FSEL_ADDDATA = 0,
    FSEL_DELDATA = 1,
} fsel_update_mode;

/*!
 *  \brief
 * �趨���ݿ��ѯģʽ
 *\li FSEL_CANCEL:ȡ�����ݿ��ѯ
 *\li FSEL_SUBDIR:��ѯ��ǰĿ¼����Ŀ¼
 *\li FSEL_CURRENTDIR:��ѯ��ǰĿ¼
 */
typedef enum
{
    FSEL_CANCEL = 0,
    FSEL_QUERY = 1,
} fsel_queryset_t;

/*! \cond fsel_api*/

FSEL_HANDLE fsel_open(void);

int fsel_close(FSEL_HANDLE Handle);

int fsel_setworkdir(FSEL_HANDLE Handle, char *pDirName);

const char * fsel_getworkdir(FSEL_HANDLE Handle);

int fsel_setfilter(FSEL_HANDLE Handle, char* Ext);

int fsel_getfilenum(FSEL_HANDLE Handle, fsel_itemtype_t itemtype);

int fsel_listfile(FSEL_HANDLE Handle, fsel_filename_t *file_name, int startno,
        int num, fsel_listtype_t type);

int fsel_listdir(FSEL_HANDLE Handle, fsel_filename_t *file_name, int startno,
        int num, fsel_listtype_t type);

int fsel_cd(FSEL_HANDLE Handle, char*pDirName);

int fsel_sortstyle(FSEL_HANDLE Handle, fsel_sortstyle_t style);

int fsel_getidbyname(FSEL_HANDLE Handle, char * pFullpath);

int fsel_update(FSEL_HANDLE Handle, char *path, fsel_update_mode flag);

int fsel_next_fileinfo(FSEL_HANDLE Handle, fsel_filename_t *file_name,
        fsel_subdir_t flag);

int fsel_prev_fileinfo(FSEL_HANDLE Handle, fsel_filename_t *file_name,
        fsel_subdir_t flag);

int fsel_set_query(FSEL_HANDLE Handle, char *pchar, fsel_queryset_t flag);

int fsel_subdir_setting(FSEL_HANDLE Handle, fsel_subdir_t flag,
        fsel_filenametype_t type);

int fsel_getfullpath(FSEL_HANDLE Handle, char* path, int num);

/*! \endcond*/

#endif
