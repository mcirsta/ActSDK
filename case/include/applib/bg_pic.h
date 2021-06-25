/*
 *******************************************************************************
 *                       NOYA1100
 *                (c) Copyright 2002-2008, Actions Co,Ld.
 *                        All Right Reserved
 * FileName: bg_pic.h
 * Description: 背景图文件相关定义
 * History:
 *       <author>    <time>          <version >      <desc>
 *       cheason     2009/06/03       1.0             建立该文件
 *******************************************************************************
 */

#ifndef __BG_PIC_H__
#define __BG_PIC_H__

/*!
 * 背景图文件头
 */
typedef struct
{
    unsigned long magic;
    unsigned long checksum;     /* 图片数据（不包含header）checksum */
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char reserved[3];
}bg_pic_header;

#define BG_PIC_MAGIC    0x25

/******************************************************************************/
/*!                    
* \par  Description:
*      保存背景图
* \param[in]    img_info -- 中间件图片解码信息结构体指针
* \param[in]    path -- 保存路径
* \param[in]    angle -- buffer的角度
* \retval       true -- 成功
* \retval       false -- 失败
* \par
*******************************************************************************/
bool set_user_bg_pic(mmm_img_info_t* img_info, const char *name_list, int angle);

/******************************************************************************/
/*!                    
* \par  Description:
*      恢复默认背景图
* \param[in] name_list -- 背景图文件名列表，以';'隔开
* \retval       true -- 成功
* \retval       false -- 失败
* \par
*******************************************************************************/
bool restore_default_bg_pic(const char *name_list);


/******************************************************************************/
/*!                    
* \par  Description:
*      获取用户自定义背景图
* \param[in]    name -- 背景图文件名
* \retval       true -- 成功
* \retval       false -- 失败
* \par
*******************************************************************************/
gui_bitmap_info_t *get_user_bg_pic(const char *name);


/******************************************************************************/
/*!                    
* \par  Description:
*      恢复所有开关机画面为默认状态
* \retval       true -- 成功
* \retval       false -- 失败
* \par
*******************************************************************************/
bool restore_default_all_animation(void);

#endif  /* #ifndef __BG_PIC_H__ */

