/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_playlist.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_playlist.c
 * \brief    routine of photo playlist
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_PLAYLIST_H__
#define __PHOTO_PLAYLIST_H__

#include "photo.h"

/*
 * �򿪳�ʼ��playlist
 * ��������
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_init(photo_plist_mode_e mode);

/*
 * �ر�playlist
 * ��������
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_deinit(void);

/*
 * ��鲥���б��Ƿ��ʼ������
 * ��������
 * ����ֵ��true--�д���false-�޴���
 */
bool photo_playlist_check_init_error(void);

/*
 * ���Ŀ¼�£�������Ŀ¼�������ļ���playlist
 * ����
 *   dir: Ŀ¼ȫ·����
 *   device: �豸����SYSTEMDISK, CARD, EXTERNAL)
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_add_dir(const char *dir);

/*
 * ����ļ�ѡ�����е��ļ���playlist
 * ����
 *   dir: Ŀ¼ȫ·����
 *   device: �豸����SYSTEMDISK, CARD, EXTERNAL)
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_add_fsel(FSEL_HANDLE fsel);

/*
 * ��ӵ����ļ���playlist
 * ����
 *   fullpath: �ļ�ȫ·����
 *   device: �豸����SYSTEMDISK, CARD, EXTERNAL)
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_add_file(const char *fullpath);

/*
 * ���ѡ���ļ�����Ŀ¼�£�������Ŀ¼�������ļ���playlist
 * ����
 *   fullpath: �ļ�ȫ·����
 *   device: �豸����SYSTEMDISK, CARD, EXTERNAL)
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_add_file_dir(const char *fullpath);


/*
 * ��ȡ��ǰplaylist���ļ�����
 * ��������
 * ����ֵ����ǰplaylist���ļ�����
 */
int photo_playlist_get_num(void);

/*
 * ��ȡ�ļ���playlist�е�index
 * ����
 *   fullpath: �ļ�ȫ·����
 *   device: �豸����SYSTEMDISK, CARD, EXTERNAL)
 * ����ֵ
 *   -1: playlistû������ļ�
 *   >=0: �ļ���playlist�е�index
 */
int photo_playlist_get_file_index(const char *fullpath);

/*
 * ��ȡ�ļ���ϸ��Ϣ
 * ����
 *   info: �����ļ���Ϣ�ṹ��ָ�룬�����������ռ䣬ʹ����֮��������photo_playlist_free_detail_info
 *   start: ��ʼindex
 *   num: �ļ�����
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_get_detail_info(plist_detail_info_t *info, int start, int num);

/*
 * �ͷ��ļ���ϸ��Ϣ���ݣ������photo_playlist_get_detail_info�ɶԳ���
 * ����
 *   info: �ļ���Ϣ�ṹ��ָ��
 *   num: �ļ�����
 * ����ֵ����
 */
void photo_playlist_free_detail_info(plist_detail_info_t *info, int num);

/*
 * ��playlist��ɾ�����ɸ��ļ�
 * ����
 *   start: ��ʼindex
 *   num: �ļ�������-1��ɾ�������ļ�������startֵ
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_del_items(int start, int num);

/*
 * �����ļ�·����playlist��ɾ���ļ�
 * ����
 *   path: �ļ�·��
 * ����ֵ���ɹ�����true��ʧ�ܷ���false
 */
bool photo_playlist_del_item_path(const char *path);

#endif

