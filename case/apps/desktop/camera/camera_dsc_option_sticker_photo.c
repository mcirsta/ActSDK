/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 17:2   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dsc_option_sticker_photo.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               菜单项-大头贴
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dsc_option_sticker_photo.h"
#include "camera_dsc_config.h"
#include "camera_sty.h"
#include "camera_dsc_option.h"
#include "camera_dsc_ui.h"
#include "camera_dsc_control.h"
#include "camera_define.h"

static int s_decode_over_flag = 0;

static mmm_img_info_t s_sticker_dec_info[4];
static unsigned char *s_sticker_buf = NULL;

/* menu list data define */
menu_t *g_camera_dsc_option_sticker_photo = NULL;

/* menu max items */
#define CAMERA_DSC_OPTION_STICKER_PHOTO_MAX_ITEMS   4

static void _callback_one_frame(void);
static int _proc_items_do_sticker_photo(menuview_t *menuview, menuview_sig_e signal);
static bool _sticker_photo_decode_bitmap(const char *stkfullname, mmm_img_info_t *img_dec_info);
static bool _sticker_photo_tmp_check(const char *tmpfullname, const char *stkfullname, mmm_img_info_t *img_dec_info);
static bool _sticker_phtot_tmp_reader(const char *tmpfullname, mmm_img_info_t *img_dec_info);

/* 进入主菜单 */
bool camera_dsc_option_sticker_photo_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = camera_dsc_option_sticker_photo_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/* 退出主菜单 */
bool camera_dsc_option_sticker_photo_exit(menuview_t *menuview)
{
    //delete sticker_photo menu
    camera_dsc_option_sticker_photo_deinit();

    //return to option
    camera_dsc_option_enter(menuview, FUSION_BACKWARD);

    return true;
}

menu_t* camera_dsc_option_sticker_photo_init(void)
{
    scene_t* scene; //!< app scene
    resgroup_resource_t *group;
    string_resource_t *str_res[CAMERA_DSC_OPTION_STICKER_PHOTO_MAX_ITEMS] =
    { 0 };
    //picture_resource_t *g_cam_dsc_opt_dot;

    menuitem_t item =
    { 0 };
    int num;


    if ((g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value == 0xffff) \
        /*|| (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value < 1)\*/
        )
    {
        return NULL;
    }

    //initial menu
    if (g_camera_dsc_option_sticker_photo == NULL)
    {
        //get scene
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            print_err("cann't find app scene!");
            return NULL;
        }

        //load resource group
        group = (resgroup_resource_t *) get_scene_child(scene, DSC_STRGROUP_STICKER_PHOTO);
        str_res[0] = (string_resource_t *) get_resgroup_child(group, STR_CAM_OFF);
        str_res[1] = (string_resource_t *) get_resgroup_child(group, STR_PHOTO1);
        str_res[2] = (string_resource_t *) get_resgroup_child(group, STR_PHOTO2);
        str_res[3] = (string_resource_t *) get_resgroup_child(group, STR_PHOTO3);
        //init items
        memset(&item, 0, sizeof(item));

        //create menu
        g_camera_dsc_option_sticker_photo = menu_create();
        if (g_camera_dsc_option_sticker_photo == NULL)
        {
            print_err("cann't create menu!");
            return NULL;
        }
        else
        {
            // append items
            for (num = 0; num < (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value+1);\
                num++)
            {
                if(str_res[num] != NULL)
                {
                    item.text_id = (int) str_res[num]->str_id;
                }
                item.has_child = false;
                item.callback = (item_cb_t) _proc_items_do_sticker_photo;
                if (num == g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STICKERPHOTO].value)
                {
                    item.sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                    item.unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                }
                else
                {
                    item.sel_icon_id = 0;
                    item.unsel_icon_id = 0;
                }
                menu_append_item(g_camera_dsc_option_sticker_photo, &item);
            }
        }

        //init current index
        menu_set_cur_index(g_camera_dsc_option_sticker_photo, 0);
    }

    return g_camera_dsc_option_sticker_photo;
}

int camera_dsc_option_sticker_photo_deinit(void)
{
    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value == 0xffff)
    {
        return true;
    }
    
    //delete menu
    if (g_camera_dsc_option_sticker_photo != NULL)
    {
        //create menu
        menu_delete(g_camera_dsc_option_sticker_photo);
        g_camera_dsc_option_sticker_photo = NULL;
    }

    return true;
}
static int _proc_items_do_sticker_photo(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item(cur_menu);
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            int cur_item_pos = menu_get_cur_index(cur_menu);
            menuitem_t *save_opt_item = menu_get_item(cur_menu, g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STICKERPHOTO].value);

            if (save_opt_item != NULL)
            {
                save_opt_item->sel_icon_id = 0;
                save_opt_item->unsel_icon_id = 0;
            }
            if (cur_item != NULL)
            {
                cur_item->sel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                cur_item->unsel_icon_id = (int) g_cam_dsc_opt_dot->pic_id;
                print_dbg(">>>> menu_list: %s clicked", cur_item->text);                
            }
            camera_dsc_sticker_photo_change(cur_item_pos);//切换大头贴

            camera_dsc_save_parameter(CAM_DSC_PARA_STICKER_PHOTO, cur_item_pos);
#if (MENU_SELECT_OK_HIDE == 1)
            camera_dsc_widget_option_exit(menuview, camera_dsc_option_sticker_photo_enter);
#else
            menuview_set_menu(menuview, cur_menu);
            menuview_paint(menuview);
#endif /* MENU_SELECT_OK_HIDE */
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dsc_option_sticker_photo_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}

static void _callback_one_frame(void)
{
    s_decode_over_flag = 1;
}

static bool _sticker_photo_decode_bitmap(const char *stkfullname, mmm_img_info_t *img_dec_info)
{
    int result;
    void * temphandle;
    void *afptr;
    mmm_img_file_info_t file_info;

    /*加载图片解码中间件动态库*/
    afptr = dlopen("a_codec.so", RTLD_LAZY);
    /*打开图片解码中间件，获取图片解码操作句柄*/

    if (afptr == NULL)
    {
        return false;
    }
    temphandle = mmm_id_open(NULL);
    if (temphandle == NULL)
    {
        goto open_err;
    }
    result = mmm_id_cmd(temphandle, MMM_SET_FILE, (unsigned int) stkfullname);
    if (result != 0)
    {
        goto mmm_cmd_err;
    }
    result = mmm_id_cmd(temphandle, MMM_GET_MEDIA_INFO, (unsigned int) &file_info);
    if (result != 0)
    {
        goto mmm_cmd_err;
    }
    img_dec_info->bpp = 2;
    img_dec_info->formate = MMM_RGB;
    img_dec_info->rect_width = file_info.width;
    img_dec_info->rect_height = file_info.height;
    /*若要同时解alpha值，buffer的大小需在普通解图大小的基础上再添加一个长*宽的大小 */
    #if 0
    if (img_dec_info->buf == NULL)
    {
        img_dec_info->buf = (char *) malloc(file_info.width * file_info.height * (img_dec_info->bpp + 1));
    }
    if (img_dec_info->buf == NULL)
    {
        goto mmm_cmd_err;
    }
    #else
    assert(file_info.width == 320);    
    assert(file_info.height == 240);
    
    if (s_sticker_buf == NULL)
    {
        s_sticker_buf = (char *) malloc(file_info.width * file_info.height * (img_dec_info->bpp + 1));
    }
    if (s_sticker_buf == NULL)
    {
        goto mmm_cmd_err;
    }
    else
    {
        memset(s_sticker_buf,0,sizeof(s_sticker_buf));
        img_dec_info->buf = s_sticker_buf;
    }
    
    #endif
    img_dec_info->len = file_info.width * file_info.height * (img_dec_info->bpp + 1);
    /*设置callback函数，当中间件解完图片时，调用该函数*/
    result = mmm_id_cmd(temphandle, MMM_SET_CALLBACK, (unsigned int) _callback_one_frame);
    if (result != 0)
    {
        goto mmm_cmd_err;
    }
    /*若要同时解alpha值，需调用该接口，对中间件进行设置 */
    result = mmm_id_cmd(temphandle, MMM_SET_ALPHA_DEC, 0);
    if (result != 0)
    {
        print_err("alpha decode err\n");
        goto mmm_cmd_err;
    }
    result = mmm_id_cmd(temphandle, MMM_IMG_DECODE, (unsigned int) img_dec_info);
    if (result != 0)
    {
        print_err("decode err\n");
        goto mmm_cmd_err;
    }

    while (1)
    {
        /*! 因为中间件的优先级低，app要让出一些cpu 给中间件*/
        usleep(1000* 500 );
        if (s_decode_over_flag != 0)
        {
            break;
        }
    }
    s_decode_over_flag = 0;
    print_info("decod over\n");
    mmm_id_close(temphandle, NULL);
    dlclose(afptr);
    return true;

    mmm_cmd_err:
    /*关闭中间件图片解码*/
    mmm_id_close(temphandle, NULL);

    open_err:
    /*卸载图片解码中间件动态库*/
    dlclose(afptr);
    print_err("decode sticker photo err = %s\n",stkfullname);
    return false;
}

bool camera_dsc_sticker_photo_change(int cur_item)
{
    bool result;
    mmm_photo_sticker_t sticker_para;


    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value == 0xffff)
    {
        /*无大头贴功能取选项0*/
        cur_item = 0;
    }
    if(g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value < cur_item)
    {
        /*如果config中的数字大于可见列表数目*/
        // TODO:RECHECK
        cur_item = g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value;
    }
    switch (cur_item)
    {

        case 1:
        _sticker_phtot_tmp_reader(g_camera_dsc_sticker_cfgs[1].text\
        /*DEF_PATH_STICKER_TEMP1*/,  &s_sticker_dec_info[cur_item]);
        break;
        
        case 2:
        _sticker_phtot_tmp_reader(g_camera_dsc_sticker_cfgs[3].text\
        /*DEF_PATH_STICKER_TEMP2*/,  &s_sticker_dec_info[cur_item]);
        break;
        
        case 3:
        _sticker_phtot_tmp_reader(g_camera_dsc_sticker_cfgs[5].text\
        /*DEF_PATH_STICKER_TEMP3*/,  &s_sticker_dec_info[cur_item]);
        break;
          
        case 0:        
        default:
        break;
            
    }
    
    if (0 == cur_item)
    {
        sticker_para.width = 0;
        sticker_para.height = 0;
        sticker_para.photo_sticker_flag = 0;
        sticker_para.buf = NULL;
    }
    else
    {
        sticker_para.width = s_sticker_dec_info[cur_item].rect_width;
        sticker_para.height = s_sticker_dec_info[cur_item].rect_height;
        sticker_para.photo_sticker_flag = 1;
        //todo:if buf=null?
        assert(s_sticker_dec_info[cur_item].buf != NULL);
        sticker_para.buf = s_sticker_dec_info[cur_item].buf;
    }

    //api_number bug _sticker_photo_decode_bitmap(stkfullname);
    result = camera_dsc_ctr_set_photo_sticker(&sticker_para);

    return result;
}

static bool _sticker_photo_tmp_check(const char *tmpfullname, const char *stkfullname, mmm_img_info_t *img_dec_info)
{
    int fd = -1;
    bool ret = false;

    fd = open(tmpfullname, O_RDONLY);
    if(-1 == fd)
    {
        print_dbg("1st in have not (%s) file\n",stkfullname);

        ret = _sticker_photo_decode_bitmap(stkfullname, img_dec_info);

        if (ret == false)
        {
            print_err("decode file error: %s", stkfullname);
            goto out;
        }
        
        fd = open(tmpfullname, O_CREAT | O_WRONLY | O_TRUNC);
        if(-1 == fd)
        {
            print_err("create file error: %s", tmpfullname);
            ret = false;
            goto out;
        }
        write(fd, img_dec_info->buf, img_dec_info->len);
        ret = true;
    }
    ret = true;
    out:
    if(fd >= 0)
    {
        close(fd);
    }

    return ret;
}

static bool _sticker_phtot_tmp_reader(const char *tmpfullname, mmm_img_info_t *img_dec_info)
{
    unsigned int size;
    int fd = -1;
    bool ret = false;

    fd = open(tmpfullname, O_RDONLY);
    if(-1 != fd)
    {
        print_dbg("have find (%s) file\n",tmpfullname);

        size = (unsigned int)lseek(fd, 0, SEEK_END);
        print_dbg("**size=%d,mult=%d\n",size,320*240);

        assert(size == 320*240*3);
        
        if (s_sticker_buf == NULL)
        {
            s_sticker_buf = (char *) malloc(size);
        }
        if (s_sticker_buf == NULL)
        {
            print_err("read sticker photo malloc fail");
            ret = false;
            goto out;
        }
        else
        {
            memset(s_sticker_buf,0,sizeof(s_sticker_buf));
            img_dec_info->buf = s_sticker_buf;
        }

        lseek(fd, 0, SEEK_SET);
        read(fd, img_dec_info->buf, size);
        //TODO:check how 2 chage use source img w,h
        img_dec_info->rect_width = 320;
        img_dec_info->rect_height = 240;
        //write(fdw, buff, size);
        ret = true;
    }
    out:
    if(fd >= 0)
    {
        close(fd);
    }

    return ret;
    
}
#if 0
bool camera_dsc_sticker_photo_decode_init(void)
{
    bool result;
    result = _sticker_photo_decode_bitmap(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STIKCER_FULLNAME_1].text\
    /*DEF_PATH_STICKER_1*/, &s_sticker_dec_info[1]);
    if (result == false)
    {
        return false;
    }
    result = _sticker_photo_decode_bitmap(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STIKCER_FULLNAME_2].text\
    /*DEF_PATH_STICKER_2*/, &s_sticker_dec_info[2]);
    if (result == false)
    {
        return false;
    }
    result = _sticker_photo_decode_bitmap(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STIKCER_FULLNAME_3].text\
    /*DEF_PATH_STICKER_3*/, &s_sticker_dec_info[3]);
    if (result == false)
    {
        return false;
    }
    return true;
}
#else
bool camera_dsc_sticker_photo_decode_init(void)
{
    bool result;
    int i = 0;

    if ((g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value == 0xffff) ||\
        (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value < 1))
    {
        /*如果没有大头贴仍然要继续运行*/
        return true;
    }

    //memset(&s_sticker_dec_info,0,(sizeof(mmm_img_info_t)*4));
    memset(&s_sticker_dec_info,0,sizeof(s_sticker_dec_info));
    
    for (i = 0; i < g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO].value; i++)
    {
        result = _sticker_photo_tmp_check(g_camera_dsc_sticker_cfgs[i*2+1].text, \
            g_camera_dsc_sticker_cfgs[i*2].text,\
             &s_sticker_dec_info[i+1]);
        if (result == false)
        {
            return false;
        }        
    }
    return true;
}
#endif
bool camera_dsc_sticker_photo_decode_deinit(void)
{
    #if 0
    if (s_sticker_dec_info[1].buf != NULL)
    {
        free(s_sticker_dec_info[1].buf);
        s_sticker_dec_info[1].buf = NULL;
    }
    if (s_sticker_dec_info[2].buf != NULL)
    {
        free(s_sticker_dec_info[2].buf);
        s_sticker_dec_info[2].buf = NULL;
    }
    if (s_sticker_dec_info[3].buf != NULL)
    {
        free(s_sticker_dec_info[3].buf);
        s_sticker_dec_info[3].buf = NULL;
    }
    
    #endif
    if (s_sticker_buf != NULL)
    {
        free(s_sticker_buf);
        s_sticker_buf = NULL;        
    }
    return true;
}

