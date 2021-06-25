/*
 ********************************************************************************
 *                            Noya1100 --- �������д
 *                 Copyright(c) 2002-2009 Actions Semiconductor,
 *                        		All Rights Reserved.
 *
 * History:
 *       <author>    <time>      <version >       <desc>
 *       cheason     2009/02/20     1.0           build this file
 ********************************************************************************
 */

/*!
 * \file config_api.h
 * \brief  Ӧ��ȫ���������д
 * \author   cheason
 * \par GENERAL DESCRIPTION:
 *          ͨ����ѯ��д�������ļ�ʵ��Ӧ����ȫ��������Ķ�д
 * \par EXTERNALIZED FUNCTIONS:
 *          null
 * \version 1.0
 * \date 2009/02/20
 *
 */

#ifndef __CONFIG_API_H__
#define __CONFIG_API_H__

#include <libc/sys/limits.h>

/*! value����󳤶� */
#define CFG_MAX_BUFF_LENGTH PATH_MAX

/******************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ���ü���Ӧ��ֵ
 * \param[in]    key -- ���ü����ַ���ָ��
 * \param[out]   buff -- ֵ����ָ��
 * \param[in]    len -- ֵ�������󳤶�
 * \retval   �ɹ��򷵻�ʵ�ʶ�ȡ���ĳ��ȣ�ʧ���򷵻�-1.
 * \par
 * \code
 *       �������-1(����)�����ܵ����������
 *       -- key, buffΪ��: errno = EINVAL
 *       -- ʵ�ʵ�ֵ�Ȼ������󳤶ȴ�: errno = ENOMEM
 *       -- ��ֵ������: errno = CONFIG_KEY_NOT_FOUND
 * \endcode
 * \par Example:
 * \code
 *       char key[] = "VER";
 *       char value[30];
 *       int len = get_config(key, value, 30);
 *       if(len != -1)
 *       {
 *           printf("actual length is %d, value is: %s", len, value);
 *       }
 * \endcode
 *******************************************************************************/
int get_config(const char *key, char *buff, int len);

/******************************************************************************/
/*!
 * \par  Description:
 *     ���ü���ֵ
 * \param[in]    key -- ���ü����ַ���ָ��
 * \param[in]    buf -- ֵ����ָ��
 * \param[in]    len -- ���õ�ֵ�ĳ���
 * \retval   �ɹ��򷵻�0��ʧ���򷵻�-1.
 * \par
 * \code
 *       �������-1(����)�����ܵ����������
 *       -- key, buffΪ��: errno = EINVAL
 *       -- ���̿ռ䲻��: errno = ENOSPC
 *       -- ���õ�ֵ���ȳ����������ֵ: errno = CONFIG_EXCEED_MAX_LENGTH
 * \endcode
 * \par Example:
 * \code
 *       char key[] = "VER";
 *       char value[] = "ACTIONS_2009_02_20";
 *       if(set_config(key, value, strlen(value) + 1) != -1)
 *       {
 *           printf("set successfully!");
 *       }
 * \endcode
 *******************************************************************************/
int set_config(const char * key, const char * buf, int len);

/******************************************************************************/
/*!
 * \par  Description:
 *     �ָ���ֵĬ������
 * \param[in]    key -- �����ַ���ָ��
 * \retval   �ɹ��򷵻�0��ʧ���򷵻�-1.
 * \par
 * \code
 *       �������-1(����)�����ܵ����������
 *       -- keyΪ��: errno = EINVAL
 *       -- Ĭ�ϼ�ֵ������: errno = CONFIG_KEY_NOT_FOUND
 * \endcode
 * \par Example:
 * \code
 *       if(reset_config() != -1)
 *       {
 *           printf("reset successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int reset_config(const char *key);

/******************************************************************************/
/*!
 * \par  Description:
 *     �ָ����м�ֵ��Ĭ������
 * \retval   �ɹ��򷵻�0��ʧ���򷵻�-1.
 * \par Example:
 * \code
 *       if(reset_all_config() != -1)
 *       {
 *           printf("reset successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int reset_all_config(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     ���DRM HDS��
 * \param[in]    vram -- vram�豸�ڵ�(һ��Ϊ"/dev/vram")
 * \param[in]    path -- hds�ļ�·�� (һ��Ϊ"/mnt/sdisk/HDSFrame.hds")
 * \retval   �ɹ��򷵻�0��ʧ���򷵻�-1.
 * \par
 * \code
 *       �������-1(����)�����ܵ����������
 *       -- vram, pathΪ��: errno = EINVAL
 *       -- ���̿ռ䲻��: errno = ENOSPC
 * \endcode
 * \par Example:
 * \code
 *        int rc = clear_hds("/dev/vram", "/mnt/sdisk/HDSFrame.hds");
 *        if (rc < 0)
 *        {
 *            printf("Clear HDS region fail: %d\n", rc);
 *            goto fail;
 *        }
 * \endcode
 *******************************************************************************/
int clear_hds(const char *vram, const char *path);

/******************************************************************************/
/*!
 * \par  Description:
 *      �����ļ�У�飨У��ʧ��������ļ����𻵣�
 * \retval   У��ɹ��򷵻�0��ʧ���򷵻�-1.
 * \par Example:
 * \code
 *       if(!check_config_bin())
 *       {
 *           printf("check successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int check_config_bin(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     �����е�����ͬ����config.bin
 * \retval   �ɹ��򷵻�0��ʧ���򷵻�-1.
 * \par Example:
 * \code
 *       if(!sync_config())
 *       {
 *           printf("sync successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int sync_config(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     flash���д
 * \retval   �ɹ��򷵻�0��ʧ���򷵻�-1.
 * \par Example:
 * \code
 *       if(!nandflash_update())
 *       {
 *           printf("write back successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int nandflash_update(void);

#endif // ifndef __CONFIG_API_H__
