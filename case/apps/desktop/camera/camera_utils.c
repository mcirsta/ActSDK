/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_utils.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-11         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_utils.c
 * \brief    photo 通用函数
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/11
 *******************************************************************************/

#include "camera_main.h"

int file_num = 0;       //当前文件夹内的文件数
char tmp_path[PATH_MAX];

/*
 * 设置当前场景全部区域作为特效的第一帧 */
void scene_fusion_set_first_frame(scene_t *scene)
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_first_frame(&frame);
}

/*
 * 设置当前场景全部区域作为特效的最后一帧
 */
void scene_fusion_set_last_frame(scene_t *scene)
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_last_frame(&frame);
}

/*
 * Option菜单特效第一帧 */
void option_fusion_set_first_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *) info.pdata;//NULL;
    memset(frame.pdata, 0, (unsigned int)(frame.rect.width * frame.rect.height * 3));
    frame.alpha = true;//false;
    fusion_set_first_frame(&frame);
}

/*
 * Option菜单特效最后一帧
 */
void option_fusion_set_last_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *) info.pdata;
    frame.alpha = true;
    fusion_set_last_frame(&frame);
}

/******************************************************************************/
/*!                    
 * \par  Description:     check file num
*******************************************************************************/
static int _check_file_num(char *path)
{
    int ret;
    FSEL_HANDLE tmp_fp;
   
    tmp_fp = fsel_open();
    if (tmp_fp == NULL)
    {
        print_err();
        return -1;
    }
    ret = fsel_setworkdir(tmp_fp, path);
    if (ret != 0)
    {
        print_err();
        fsel_close(tmp_fp);
        return -1;
    }
    ret = fsel_setfilter(tmp_fp, DSC_EXT);
    if (ret != 0)
    {
        print_err();
        fsel_close(tmp_fp);
        return -1;
    }
    ret = fsel_getfilenum(tmp_fp,FSEL_FILENUM);
    return ret;
}

/******************************************************************************/
/*!                    
 * \par  Description:    make folder named like 20091226 under CAMERA folder,
 * \par  total file at each folder <= FILE_MAX
*******************************************************************************/
bool camera_check_folder(char *path)
{
    int ex_num = 1;
    int result;
    int num;
    DIR *fp;
    rtc_time_t time;
    rtc_date_t date;
    sys_get_rtc_date_time(&time, &date);

    snprintf(tmp_path, sizeof(tmp_path),"%s%04d%02d%02d",path,date.year, date.month, date.day);
    strcat(tmp_path, "/");
    fp = opendir(tmp_path);
    if(fp == NULL)
    {
        result = mkdir(tmp_path,0x777);
        if(result < 0)
        {
            print_err();
            return false;
        }
        strncpy(path, tmp_path, sizeof(tmp_path));
        file_num = 0;
        return true;
    }
    else
    {
        closedir(fp);
        num = _check_file_num(tmp_path);
        if((num != -1) && (num < FILE_MAX))
        {
            strncpy(path, tmp_path, sizeof(tmp_path));
            file_num = num;
            return true;
        }  
        while(1)
        {
            snprintf(tmp_path, sizeof(tmp_path),"%s%04d%02d%02d_%d",\
                     path, date.year, date.month, date.day, ex_num);
            strcat(tmp_path, "/");
            fp = opendir(tmp_path);
            if(fp == NULL)
            {
                result = mkdir(tmp_path,0x777);
                if(result < 0)
                {
                    print_err();
                    return false;
                }
                strncpy(path, tmp_path, sizeof(tmp_path));
                file_num = 0;
                return true;
            }
            else
            {
                closedir(fp);
                num = _check_file_num(tmp_path);
                if((num != -1) && (num < FILE_MAX))
                {
                    strncpy(path, tmp_path, sizeof(tmp_path));
                    file_num = num;
                    return true;
                }
                ex_num++;
            }
            if(ex_num>=999)
            {
                print_err("num=%d\n",num);
                return false;
            }
        } //end while(1)
    }
}


