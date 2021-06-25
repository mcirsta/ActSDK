/*!\cond LIBC*/
/****************************************************************************
 *                              usdk 1100
 *                            Module:libc_fs_dir
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      husanxi    2008-09-27 15:00     1.0             build this file
 ****************************************************************************/
/*!
 * \file     dirent.h
 * \brief    ���ļ�������C�⺯����Ŀ¼�������Ľṹ�嶨���Լ�����ԭ�͡�
 * \author   husanxi
 * \version 1.0
 * \date  2008/9/27
 ****************************************************************************/
#ifndef __DIRENT_H__
#define __DIRENT_H__

#include <libc/types.h>
#include <libc/pthread.h>
#include <libc/stdio.h>

#define FORWARD 0x01
#define BACKWARD 0x02
#define MAX_INT  0x7fffffff
#define  DIR_SET_FORWARD(s) ((s)->dd_flag |= FORWARD)
#define  DIR_CLEAR_FORWARD(s) ((s)->dd_flag &= ~FORWARD)
#define  DIR_SET_BACKWARD(s) ((s)->dd_flag|= BACKWARD)
#define  DIR_CLEAR_BACKWARD(s) ((s)->dd_flag &= ~BACKWARD)
#define  DIR_IS_FORWARD(s) ((s)->dd_flag & FORWARD)!=0
#define  DIR_IS_BACKWARD(s) ((s)->dd_flag & BACKWARD)!=0

/*!
 * \brief
 *      ��λ��λ��ʾ��Ŀ¼���Ӧ���ļ���ֻ����
 */
#define ATTR_READONLY       0x0001
/*!
 * \brief
 *      ��λ��λ��ʾ��Ŀ¼���Ӧ���ļ������ص�
 */
#define ATTR_HIDDEN         0x0002
/*!
 * \brief
 *      ��λ��λ��ʾ��Ŀ¼���Ӧ���ļ���ϵͳ�ļ�
 */
#define ATTR_SYSTEM         0x0004
/*!
 * \brief
 *      ��λ��λ��ʾ��Ŀ¼���Ӧ���
 */
#define ATTR_VOLUME         0x0008
/*!
 * \brief
 *      ��λ��λ��ʾ��Ŀ¼���Ӧ������Ŀ¼
 */
#define ATTR_DIRECTORY      0x0010
/*!
 * \brief
 *      ��λ��λ��ʾ��Ŀ¼���Ӧ�ļ�Ϊ�浵�ļ�
 */
#define ATTR_ARCHIVE        0x0020

/*!
 * \brief
 *      Ŀ¼���ṹ��������к���opendir���أ���Ϊreaddir��prevdir��Ŀ¼������
 *      �����룬�û�����������Ľṹ�������ֵ��
 */
typedef struct dirstream
{
    /*Ŀ¼�ļ����*/
    int dd_fd;

    /* Ŀ¼�����*/
    unsigned char dd_flag;

    /*Ŀ¼���ڻ����е�ƫ��*/
    size_t dd_nextloc;

    /* Ŀ¼������*/
    void *dd_buf;

    /*Ŀ¼����Ŀ¼�ļ��е�ƫ��*/
    off_t dd_nextoff;

    /* �������ܵ�Ŀ¼����Ŀ*/
    ssize_t dd_totalNum;

    /* ��ǰĿ¼���ڻ�������*/
    ssize_t dd_curNum;

    /* Ŀ¼������Ĵ�С*/
    ssize_t dd_max;

#ifdef LIBC_HAS_PTHREADS
/*�̻߳�����*/
pthread_mutex_t dd_lock;
#endif
} DIR;

/*!
 * \brief
 *      Ŀ¼��ṹ�嶨��
 */
typedef struct dirent
{
    /*! �ļ����к�*/
    unsigned int d_ino;

    /*! Ŀ¼ͷ�����ļ�Ŀ¼���ƫ����*/
    unsigned int d_off;

    /*! �ļ��޸�ʱ�� */
    unsigned int d_mtime;

    /*! �ü�¼���ܳ���,������¼ͷ���ļ���.32b����*/
    unsigned short int d_reclen;

    /*! �ļ��������ԣ�λ�������£�
     * \n ATTR_READONLY��λ:ֻ���ļ�
     * \n ATTR_HIDDEN��λ:�����ļ�
     * \n ATTR_SYSTEM��λ��ϵͳ�ļ�
     * \n ATTR_VOLUME��λ:����ļ�
     * \n ATTR_DIRECTORY��λ:Ŀ¼�ļ�
     * \n ATTR_ARCHIVE��λ:�浵�ļ� */
    unsigned char d_attrs;

    /*! �ļ�����Сд��ʾλ*/
    unsigned char d_lcase;

    /*! �ļ���,�ɱ䳤.����Ϊ = ��¼����(d_reclen) -
     offsetof(struct dirent, d_name)
     */
    char d_name[1000];
} dirent_t;

extern DIR *opendir(const char *name);
extern struct dirent *readdir(DIR *dir);
extern void seekdir(DIR *dir, off_t offset);
extern off_t telldir(DIR *dir);
extern void rewinddir(DIR *dir);
extern int closedir(DIR *dir);
extern int lastdir(DIR *fd);
extern struct dirent *prevdir(DIR *fd);
extern int reset2parentdir(DIR *fd);
/*!\endcond*/
#endif/*__DIRENT_H__*/
