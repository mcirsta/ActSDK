/*
 *******************************************************************************
 *                       NOYA1100
 *                (c) Copyright 2002-2008, Actions Co,Ld.
 *                        All Right Reserved
 * FileName: bg_pic.h
 * Description: ����ͼ�ļ���ض���
 * History:
 *       <author>    <time>          <version >      <desc>
 *       cheason     2009/06/03       1.0             �������ļ�
 *******************************************************************************
 */

#ifndef __BG_PIC_H__
#define __BG_PIC_H__

/*!
 * ����ͼ�ļ�ͷ
 */
typedef struct
{
    unsigned long magic;
    unsigned long checksum;     /* ͼƬ���ݣ�������header��checksum */
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char reserved[3];
}bg_pic_header;

#define BG_PIC_MAGIC    0x25

/******************************************************************************/
/*!                    
* \par  Description:
*      ���汳��ͼ
* \param[in]    img_info -- �м��ͼƬ������Ϣ�ṹ��ָ��
* \param[in]    path -- ����·��
* \param[in]    angle -- buffer�ĽǶ�
* \retval       true -- �ɹ�
* \retval       false -- ʧ��
* \par
*******************************************************************************/
bool set_user_bg_pic(mmm_img_info_t* img_info, const char *name_list, int angle);

/******************************************************************************/
/*!                    
* \par  Description:
*      �ָ�Ĭ�ϱ���ͼ
* \param[in] name_list -- ����ͼ�ļ����б���';'����
* \retval       true -- �ɹ�
* \retval       false -- ʧ��
* \par
*******************************************************************************/
bool restore_default_bg_pic(const char *name_list);


/******************************************************************************/
/*!                    
* \par  Description:
*      ��ȡ�û��Զ��屳��ͼ
* \param[in]    name -- ����ͼ�ļ���
* \retval       true -- �ɹ�
* \retval       false -- ʧ��
* \par
*******************************************************************************/
gui_bitmap_info_t *get_user_bg_pic(const char *name);


/******************************************************************************/
/*!                    
* \par  Description:
*      �ָ����п��ػ�����ΪĬ��״̬
* \retval       true -- �ɹ�
* \retval       false -- ʧ��
* \par
*******************************************************************************/
bool restore_default_all_animation(void);

#endif  /* #ifndef __BG_PIC_H__ */

