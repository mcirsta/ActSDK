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

#include "photo_playlist.h"

static void *s_media_lib = NULL;
static void *s_photo_list = NULL;
static bool s_init_error = true;

static bool s_fsel_mode = true;
static FSEL_HANDLE  s_fsel = NULL;

bool photo_playlist_init(photo_plist_mode_e mode)
{
    plist_trivefield_t trive_field;
    char ext[CFG_MAX_BUFF_LENGTH];
    int ret;
    
    if(get_config(CFG_APP_SUPPORT_EXT_PHOTO, ext, CFG_MAX_BUFF_LENGTH) < 0)
    {
        print_err();
        return false;
    }

    if(true == s_fsel_mode)
    {
        if(NULL != s_fsel)
        {
            return true;
        }

        s_init_error = true;
        s_fsel = fsel_open();
        if(NULL == s_fsel)
        {
            print_err("fsel open error");
            return false;
        }
        fsel_setfilter(s_fsel, ext);
        fsel_sortstyle(s_fsel, FSEL_PINYIN);
        fsel_subdir_setting(s_fsel, FSEL_NOSUB, FSEL_FILENAME);
        s_init_error = false;
        if(get_config(CFG_PHOTO_NOWPLAY_PATH, ext, CFG_MAX_BUFF_LENGTH) > 0)
        {
            photo_playlist_add_file_dir(ext);
        }
        return true;
    }

    if((s_media_lib != NULL) && (s_photo_list != NULL))
    {
        return true;
    }

    s_init_error = true;

    /*设置数据库路径*/
    plist_set_rootdir(sys_get_main_disk_dir(DIR_SYSTEM_PLAYLIST));
    
    ret = plist_init();
    if(ret < 0)
    {
        print_err("plist init error!");
        return false;
    }

    if (-1 == plist_set_filetype_ext(IMAGE, ext))
    {
        return false;
    }

    s_media_lib = plist_open(NULL, MEDIALIB, NULL);
    if (NULL == s_media_lib)
    {
        print_err("photo media lib open failure!");
        return false;
    }

    memset(&trive_field, 0, sizeof(trive_field));
    trive_field.num = 1;
    trive_field.nfield[0] = FULLPATH;
    if(mode != PHOTO_PLIST_DEFAULT)
    {
        s_photo_list = plist_open(TBLNAME_PHOTO_SPECIAL, IMGRESERVE2, &trive_field);
    }
    else
    {
        s_photo_list = plist_open(TBLNAME_PHOTO_DEFAULT, IMGRESERVE1, &trive_field);
    }

    if (NULL == s_photo_list)
    {
        print_err("photo playlist open failure!");
        plist_close(s_media_lib);
        s_media_lib = NULL;
        return false;
    }

    s_init_error = false;
    return true;
}

bool photo_playlist_check_init_error(void)
{
    return s_init_error;
}

bool photo_playlist_deinit(void)
{
    if (NULL != s_media_lib)
    {
        plist_close(s_media_lib);
        s_media_lib = NULL;
    }

    if (NULL != s_photo_list)
    {
        plist_close(s_photo_list);
        s_photo_list = NULL;
        plist_quit();
    }

    if(NULL != s_fsel)
    {
        fsel_close(s_fsel);
    }

    return true;
}

bool photo_playlist_add_dir(const char *dir)
{
    int ret;
    if(true == s_fsel_mode)
    {
        ret = fsel_setworkdir(s_fsel, (char *)dir);
    }
    else
    {
        ret = plist_add_dir(s_photo_list, dir);
    }
    return (ret != -1);
}

bool photo_playlist_add_fsel(FSEL_HANDLE fsel)
{
    int ret;
    if(true == s_fsel_mode)
    {
        ret = fsel_setworkdir(s_fsel, (char *)fsel_getworkdir(fsel));
    }
    else
    {
        ret = plist_add_dirlist(s_photo_list, fsel, 0);
    }
    return (ret != -1);
}

bool photo_playlist_add_file(const char *fullpath)
{
    int ret;
    if(true == s_fsel_mode)
    {
        return false;
    }

    ret = plist_add_file(s_photo_list, fullpath);
    return (ret != -1);
}

bool photo_playlist_add_file_dir(const char *fullpath)
{
    char pdir[MAX_PATH_LENGTH];

    if(false == get_parent_directory(pdir, MAX_PATH_LENGTH, fullpath))
    {
        return false;
    }

    if(true == s_fsel_mode)
    {
        return fsel_setworkdir(s_fsel, pdir);
    }
    else
    {
        return photo_playlist_add_dir(pdir);
    }
}

int photo_playlist_get_num(void)
{
    int ret = 0;

    if(true == s_fsel_mode)
    {
        ret = fsel_getfilenum(s_fsel, FSEL_FILENUM);
    }
    else
    {
        ret = plist_get_item_num(s_photo_list);
    }

    if (-1 == ret)
    {
        ret = 0;
    }

    return ret;
}

int photo_playlist_get_file_index(const char *fullpath)
{
    int ret;
    if(true == s_fsel_mode)
    {
        ret = fsel_getidbyname(s_fsel, (char *)fullpath) - 1;
    }
    else
    {
        ret = plist_name_to_index(s_photo_list, fullpath, 0);
    }

    return ret;
}

bool photo_playlist_get_detail_info(plist_detail_info_t *info, int start, int num)
{
    int ret = 0, i, j;
    fsel_filename_t filename;
    char fullpath[PATH_MAX];

    if(true == s_fsel_mode)
    {
        for(i = 0; i < num; i++)
        {
            ret = fsel_listfile(s_fsel, &filename, (i + start) + 1, 1, FSEL_POSITIVE);
            if(ret < 0)
            {
                ret = -1;
                break;
            }
            ret = fsel_getfullpath(s_fsel, fullpath, (i + start) + 1);
            //print_err("\nindex:%d, path:%s, filename:%s\n", (i + start) + 1, fullpath, filename.name);
            snprintf(fullpath, PATH_MAX, "%s%s", fullpath, filename.name);
            info[i].fullpath = strdup(fullpath);
            info[i].filename = strdup(filename.name);
            //print_err("\nfullpath:%s\n", info[i].fullpath);
        }
        
        if(-1 == ret)
        {
            for(j = 0; j < i; j++)
            {
                free(info[j].fullpath);
                free(info[j].filename);
            }
        }
    }
    else
    {
        ret = plist_get_items_to_mallocbuf(s_photo_list, info, start, num, false);
        if(0 == ret)
        {
            ret = -1;
        }
    }

    if (-1 == ret)
    {
        return false;
    }
    return true;
}

void photo_playlist_free_detail_info(plist_detail_info_t *info, int num)
{
    int i;

    if(true == s_fsel_mode)
    {
        for(i = 0; i < num; i++)
        {
            free(info[i].fullpath);
            free(info[i].filename);
        }
    }
    else
    {
        plist_free_buf(info, num);
    }
}

bool photo_playlist_del_items(int start, int num)
{
    int ret = -1, i;
    plist_detail_info_t info;

    if(-1 == num)
    {
        if(true == s_fsel_mode)
        {
            return true;
        }
        start = 0;
        num = photo_playlist_get_num();
    }

    if(true == s_fsel_mode)
    {
        for(i = 0; i < num; i++)
        {
            if(!photo_playlist_get_detail_info(&info, i + start, 1))
            {
                break;
            }
            ret = fsel_update(s_fsel, info.fullpath, FSEL_DELDATA);
            photo_playlist_free_detail_info(&info, 1);
        }
    }
    else
    {
        ret = plist_del_items(s_photo_list, start, num);
    }
    return (ret != -1);
}

bool photo_playlist_del_item_path(const char *path)
{
    int ret = -1, i = 0;
    
    if(true == s_fsel_mode)
    {
        ret = fsel_update(s_fsel, (char *)path, FSEL_DELDATA);
    }
    else
    {
        i = photo_playlist_get_file_index(path);
        if(i >= 0)
        {
            ret = plist_del_items(s_photo_list, i, 1);
        }
    }
    return (ret != -1);
}

