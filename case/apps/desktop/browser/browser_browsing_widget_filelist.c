/********************************************************************************
 *                              USDK(1100)
 *                             Module: commonui
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>                <version >             <desc>
 *       binannan   2009-5-7 13:04:15     1.0                build this file
 *******************************************************************************/
/*!
 * \file     filelist.c
 * \brief    filelist control implementation
 * \author   binannan
 *******************************************************************************/
#include "include_case.h"
#include "include_psp.h"
#include "browser.h"
#include "browser_scene_browsing.h"
#include "browser_browsing_widget_filelist.h"
#include "browser_browsing_widget_option.h"
#include "browser_sty.h"

/*
 * 局部静态函数列表
 */
/*********************************************************************/
static void _bringup_content(int i);
static void _bringup_entry(entryop_e op);

static bool _entry_data_init(entry_list_t* entry);
static bool _entry_view_init(entry_list_t* entry);

static void _entry_unload(entry_list_t* entry);
static bool _entry_reload(entry_list_t* entry);
static bool _entry_deinit(entry_list_t* entry);

static void _entry_callback(menuview_t* entry_list, menuview_sig_e sig);

static const char* _entry_get_path(entry_list_t* entry);

static bool _content_init(int i);
static void _content_data_unload(content_list_t* content);
static bool _content_view_unload(content_list_t* content);

static void _content_deinit(content_list_t* content);

static void _content_push(void);
static void _content_pop(void);

static void _content_callback(menuview_t* content_list, menuview_sig_e sig);

static bool _content_cd(char* dir_path);
static bool _content_cdup(void);
static void _content_refresh(void);
static void _content_update_all(void);
static void _content_populate(void);
static int _content_populate_dirs(int number);
static int _content_populate_files(int number);

static bool _content_populate_dir_up(void);
static bool _content_populate_file_up(void);
static bool _content_populate_dir_down(void);
static bool _content_populate_file_down(void);

static bool _single_item_exec_fileop(void);

static bool _multi_item_exec_fileop(void);

static bool _multi_item_exec_fileop_normal(void);

static bool _multi_item_exec_fileop_searchmode(void);

static void _clear_menuitem(menuitem_t* item);
static void _clear_menu(menu_t* menu);

static void _locate_highlight_item(char* path_name);

static void _show_nofile(void);
static void  _show_nodisk(void);


static void _load_config(void);

/*********************************************************************/

FSEL_HANDLE g_fsel_handle = NULL;

bool g_file_op_success = false;

/*
 * 当执行了文件操作后，就意味着之前进入的每个目录的内容都有可能变化，所以在执行
 * content_cdup的时候，就需要刷新一下，而不能使用之前保存的数据了。这里设置一个
 * 变量来记录是否执行了文件操作。
 */
bool g_file_op_execed = false;


static filelist_t* s_filelist = NULL;

static dir_level_t* s_last_p_root = NULL;
static dir_level_t* s_last_p_cur_dir = NULL;
static bool s_last_select_mode = false;
static bool* s_last_select_flag = NULL;

static int s_last_global_offset = 0;
static int s_last_current = 0;

static FSEL_HANDLE s_last_fsel_handle = NULL;

static fsel_sortstyle_t s_fsel_sort_style = FSEL_NOSORTING;

/*
 * 当处于子目录搜索模式时，当前显示的文件的路径名需要临时获取，而不是使用当前所在的文件夹的路径名，
 * 这里指定一个全局的变量current_path_name来存放临时获取的某文件的路径名
 */
static char s_current_path_name[PATH_MAX];

static int s_entry_idx = -1;

static entryitem_e s_entryitems[3] =
{ ENTRY_NONE, ENTRY_NONE, ENTRY_NONE };

static int s_last_entry_idx = -1;
static entryitem_e s_last_entryitems[3] =
{ ENTRY_NONE, ENTRY_NONE, ENTRY_NONE };

static bool s_dialog_nodisk_visible = false;
static bool s_dialog_nofile_visible = false;

static unsigned int s_macro_pic_array[TYPE_DEFAULT + 1] =
{ PIC_FOLD, PIC_MUSIC, PIC_VIDEO, PIC_PHOTO, PIC_TXT, PIC_FILE, PIC_FILE };
static unsigned int s_macro_pic_sel_array[TYPE_DEFAULT + 1] =
{ PIC_FOLD_SEL, PIC_MUSIC_SEL, PIC_VIDEO_SEL, PIC_PHOTO_SEL, PIC_TXT_SEL, PIC_FILE_SEL, PIC_FILE_SEL };
static unsigned int s_macro_pich_array[TYPE_DEFAULT + 1] =
{ PIC_FOLDH, PIC_MUSICH, PIC_VIDEOH, PIC_PHOTOH, PIC_TXTH, PIC_FILEH, PIC_FILEH };
static unsigned int s_macro_pich_sel_array[TYPE_DEFAULT + 1] =
{ PIC_FOLDH_SEL, PIC_MUSICH_SEL, PIC_VIDEOH_SEL, PIC_PHOTOH_SEL, PIC_TXTH_SEL, PIC_FILEH_SEL, PIC_FILEH_SEL };
/*********************************************************************/

static void _bringup_content(int i)
{
    fusion_effect_t eff;
    fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);

    s_entry_idx = i;
    if ((s_entryitems[1] != ENTRY_NONE) || (s_entryitems[2] != ENTRY_NONE))
    {
        /*
         * displays the fusion just  when we enter the content  from the entry.
         */
        fusion_set_first_frame(NULL);
    }

    _entry_unload(s_filelist->data->entry);
    _content_init(i);
    _content_cd(NULL);

    s_filelist->list_inuse = CONTENT;
    browser_widget_filelist_paint(s_filelist);

    if ((s_entryitems[1] != ENTRY_NONE) || (s_entryitems[2] != ENTRY_NONE))
    {
        fusion_set_last_frame(NULL);

        fusion_display( NULL);
    }
}

static void _bringup_entry(entryop_e op)
{
    fusion_effect_t eff;
    fusion_get_config(FUSION_TYPE_COMMON, FUSION_BACKWARD, &eff);
    fusion_set_effect(&eff);

    _entry_data_init(s_filelist->data->entry);

    if ((s_entryitems[0] == ENTRY_NONE)  || (s_entryitems[1] != ENTRY_NONE))
    {
        _content_data_unload(s_filelist->data->content);
        _content_view_unload(s_filelist->data->content);

        _entry_view_init(s_filelist->data->entry);

        fusion_set_first_frame(NULL);

        s_filelist->list_inuse = ENTRY;
        browser_widget_filelist_paint(s_filelist);

        fusion_set_last_frame(NULL);

        fusion_display( NULL);
    }
    else
    {
        if (op == ENTRYOP_CD)
        {
            _content_data_unload(s_filelist->data->content);
            _content_view_unload(s_filelist->data->content);
            _bringup_content(0);
        }
        else if (op == ENTRYOP_CDUP)
        {
            browser_widget_filelist_exit(s_filelist);
        }
        else
        {
            print_err();
        }
    }
}

static bool _entry_data_init(entry_list_t* entry)
{
    int i;
    int idx = 0;

    menuitem_t internal;
    hotplugin_dev_info_t main_info;

    menuitem_t card;
    hotplugin_dev_info_t card_info;

    menuitem_t external;
    hotplugin_dev_info_t external_info;

    entry->entry_list_type = MENUVIEW_MENULIST_TEXT;

    for (i = 0; i < 3; i++)
    {
        s_entryitems[i] = ENTRY_NONE;
    }

    if (entry->entry_menu != NULL)
    {
        _clear_menu(entry->entry_menu);
        menu_delete(entry->entry_menu);
    }

    entry->entry_menu = menu_create();
    if (entry->entry_menu == NULL)
    {
        print_err("create menu_t failed.\n");
        return false;
    }

    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_LOCAL, &main_info);
    if (main_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        memset(&internal, 0, sizeof(menuitem_t));
        internal.has_child = true;
        internal.text_id = entry->str_int_mem->str_id;
        menu_append_item(entry->entry_menu, &internal);

        s_entryitems[idx] = ENTRY_INTERNAL;
        idx++;
    }
    else
    {

    }

    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
    if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        memset(&card, 0, sizeof(menuitem_t));
        card.has_child = true;
        card.text_id = entry->str_car_mem->str_id;
        menu_append_item(entry->entry_menu, &card);

        s_entryitems[idx] = ENTRY_CARD;
        idx++;
    }
    else
    {
    }

    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
    if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        memset(&external, 0, sizeof(menuitem_t));
        external.has_child = true;
        external.text_id = entry->str_ext_mem->str_id;
        menu_append_item(entry->entry_menu, &external);

        s_entryitems[idx] = ENTRY_EXTERNAL;
    }
    else
    {

    }

    set_single_shot_timer(1, (timer_proc) _show_nodisk, NULL);
    return true;
}

static bool _entry_view_init(entry_list_t* entry)
{

    entry->entry_list = menuview_create(entry->entry_list_type, _entry_callback);
    if (entry->entry_list == NULL)
    {
        print_err("create menuview_t failed.\n");
        return false;
    }
    s_filelist->canvas_id = entry->entry_list->canvas_id;
    menuview_set_menu(entry->entry_list, entry->entry_menu);
    if(s_entry_idx != -1)
    menu_set_cur_index(entry->entry_menu, s_entry_idx);

    return true;

}

static void _entry_unload(entry_list_t* entry)
{
    if (entry->entry_menu != NULL)
    {
        _clear_menu(entry->entry_menu);
        menu_delete(entry->entry_menu);
        entry->entry_menu = NULL;
    }

    if (entry->entry_list != NULL)
    {
        menuview_delete(entry->entry_list);
        entry->entry_list = NULL;
    }
}

static bool _entry_reload(entry_list_t* entry)
{
    _entry_unload(entry);
    _entry_data_init(entry);
    _entry_view_init(entry);
    return true;
}

static bool _entry_deinit(entry_list_t* entry)
{
    _entry_unload(entry);
    return true;

}

static void _entry_callback(menuview_t* entry_list, menuview_sig_e sig)
{
    menu_t* menu = entry_list->menu;
    menuitem_t* item = menu_get_cur_item(menu);

    print_warning("\noffset=%d, current=%d, total=%d, global_offset=%d, global_size=%d\n\n",
            menu->offset, menu->current,menu->total, menu->global_offset, menu->global_size);

    switch (sig)
    {
        case WIDGET_SIG_MENUVIEW_DOWN:
        {
            print_warning("WIDGET_SIG_MENUVIEW_DOWN\n");
        }
        break;

        case WIDGET_SIG_MENUVIEW_UP:
        {
            print_warning("WIDGET_SIG_MENUVIEW_UP\n");
        }
        break;

        case WIDGET_SIG_MENUVIEW_OK:
        {
            print_warning("WIDGET_SIG_MENUVIEW_OK\n");
            if(item != NULL)
            {
                _bringup_content(menu->current);
            }
        }

        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            print_warning("WIDGET_SIG_MENUVIEW_QUIT\n");
            browser_widget_filelist_exit(s_filelist);
        }
        break;

        case WIDGET_SIG_MENUVIEW_SHORT_MENU:
        {
            s_filelist->callback(s_filelist, WIDGET_SIG_FILELIST_SHORT_MENU);
        }

        default:
        break;
    }
}


static const char* _entry_get_path(entry_list_t* entry)
{
    int idx = menu_get_cur_index(entry->entry_menu);
    switch (s_entryitems[idx])
    {
        case ENTRY_INTERNAL:
        {
            return UDISK_ROOT;
        }
        case ENTRY_CARD:
        {
            return CARD_ROOT;
        }
        case ENTRY_EXTERNAL:
        {
            return EXTERNAL_ROOT;
        }
        default:
        {
            return NULL;
        }
    }
}


static bool _content_init(int i)
{
    int result = -1;
    content_list_t* content = s_filelist->data->content;

    if (content->p_mnavi == NULL)
    {
        content->p_mnavi = dlopen("mnavi.so", RTLD_LAZY);
        if (content->p_mnavi == NULL)
        {
            print_err("load mnavi.so failed.\n");
            goto mnavi_err;
        }
    }

    if (content->p_fsel == NULL)
    {

        content->p_fsel = fsel_open();
        if (content->p_fsel == NULL)
        {
            print_err("fsel_open failed.\n");
            goto fsel_err;
        }
        g_fsel_handle = content->p_fsel;

        fsel_subdir_setting(content->p_fsel, FSEL_NOSUB, FSEL_RESERVE);

        fsel_sortstyle(content->p_fsel, s_fsel_sort_style);

    }

    switch (s_entryitems[i])
    {
        case ENTRY_INTERNAL:
        {
            content->default_path_name = UDISK_ROOT"/";
        }
        break;
        case ENTRY_CARD:
        {
            content->default_path_name = CARD_ROOT"/";
        }
        break;
        case ENTRY_EXTERNAL:
        {
            content->default_path_name = EXTERNAL_ROOT"/";
        }
        break;

        default:
        break;
    }

    result = fsel_setworkdir(content->p_fsel, content->default_path_name);
    if (result != 0)
    {
        print_err("fsel_setworkdir failed.\n");
    }

    content->content_list_type = MENUVIEW_MENULIST_ICON;
    content->content_list = menuview_create(content->content_list_type, _content_callback);
    if (content->content_list == NULL)
    {
        print_err();
        goto fsel_err;
    }

    s_filelist->canvas_id = content->content_list ->canvas_id;

    return true;

    fsel_err: dlclose(content->p_mnavi);
    mnavi_err: return false;
}

static bool _content_view_unload(content_list_t* content)
{
    if (content->content_list != NULL)
    {
        menuview_delete(content->content_list);
        content->content_list = NULL;
        s_filelist->canvas_id = 0;
    }

    return true;
}

static void _content_data_unload(content_list_t* content)
{
    dir_level_t* p;
    dir_level_t* p_parent;

    for (p = content->p_cur_dir; p != NULL; p = p_parent)
    {
        p_parent = p->parent;

        if (p->item_group != NULL)
        {
            _clear_menu(p->item_group);
            menu_delete(p->item_group);
        }
        if (p->workdir != NULL)
        {
            free(p->workdir);
        }
        free(p);
    }

    if (content->p_fsel != NULL)
    {
        fsel_close(content->p_fsel);
        content->p_fsel = NULL;
    }

    content->p_root = NULL;
    content->p_cur_dir = NULL;

    content->select_flag = NULL;
    content->select_mode = false;
}

static void _content_deinit(content_list_t* content)
{
    _content_data_unload(content);
    _content_view_unload(content);

    if (content->p_fsel != NULL)
    {
        fsel_close(content->p_fsel);
        content->p_fsel = NULL;

    }
    if (content->p_mnavi != NULL)
    {
        dlclose(content->p_mnavi);
        content->p_mnavi = NULL;
    }
}

static void _content_callback(menuview_t* content_list, menuview_sig_e sig)
{
    content_list_t* content = s_filelist->data->content;
    //    dir_level_t* p = content->p_cur_dir;

    menu_t* menu = content_list->menu;
    menuitem_t* item = menu_get_cur_item(menu);

    print_warning("\noffset=%d, current=%d, total=%d, global_offset=%d, global_size=%d\n\n",
            menu->offset, menu->current,menu->total, menu->global_offset, menu->global_size);

    switch (sig)
    {
        case WIDGET_SIG_MENUVIEW_DOWN:
        {
            print_warning("WIDGET_SIG_MENUVIEW_DOWN:\n");
        }
        break;

        case WIDGET_SIG_MENUVIEW_UP:
        {
            print_warning("WIDGET_SIG_MENUVIEW_UP:\n");
        }
        break;

        case WIDGET_SIG_MENUVIEW_OK:
        {
            print_warning("WIDGET_SIG_MENUVIEW_OK\n");

            if (content->select_mode == true)
            {
                if (item != NULL)
                {
                    /*
                     * 处于文件选择模式
                     */
                    file_type_e t = find_file_type(item->text);
                    print_dbg("cur = %d\t global_offset= %d\t\n",menu->current,menu->global_offset);

                    if (content->select_flag[menu->global_offset + menu->current] == true)
                    {
                        /*
                         * 当前item选中，切换为非选择
                         */
                        content->select_flag[menu->global_offset + menu->current] = false;
                        if (item->has_child == true)
                        {
                            item->sel_icon_id = content->pich_array[TYPE_FOLD]->pic_id;
                            item->unsel_icon_id = content->pic_array[TYPE_FOLD]->pic_id;
                        }
                        else
                        {
                            item->sel_icon_id = content->pich_array[t]->pic_id;
                            item->unsel_icon_id = content->pic_array[t]->pic_id;
                        }
                    }
                    else
                    {
                        /*
                         * 当前item非选中,切换为选中
                         */
                        content->select_flag[menu->global_offset + menu->current] = true;
                        if (item->has_child == true)
                        {
                            item->sel_icon_id = content->pich_sel_array[TYPE_FOLD]->pic_id;
                            item->unsel_icon_id = content->pic_sel_array[TYPE_FOLD]->pic_id;
                        }
                        else
                        {
                            item->sel_icon_id = content->pich_sel_array[t]->pic_id;
                            item->unsel_icon_id = content->pic_sel_array[t]->pic_id;
                        }
                    }
                    menuview_paint(content->content_list);
                }
            }
            else
            {
                /*
                 * 处于目录浏览模式
                 */
                if (item != NULL)
                {
                    if (item->has_child == true)
                    {
                        if (item->text != NULL)
                        {
                            _content_cd(item->text);
                            browser_widget_filelist_paint(s_filelist);
                        }
                    }
                    else
                    {
                        if (s_filelist->search_string != NULL)
                        {
                            int d = fsel_getfilenum(s_filelist->data->content->p_fsel,FSEL_DIRNUM);
                            memset(s_current_path_name, 0, PATH_MAX);
                            fsel_getfullpath(g_fsel_handle, s_current_path_name,
                                    d + menu->global_offset + menu->current + 1);
                        }
                        s_filelist->file_name = item->text;
                        s_filelist->callback(s_filelist, WIDGET_SIG_FILELIST_OK);
                    }
                }
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            print_warning("WIDGET_SIG_MENUVIEW_QUIT: \n");
            if (content->select_mode == true)
            {
                browser_widget_filelist_toggle_select_mode();
            }
            else if((s_filelist->search_string != NULL) && (s_last_fsel_handle == NULL))
            {
                browser_widget_filelist_enter_search_mode("");
                browser_widget_filelist_paint(s_filelist);
            }
            else
            {
                bool cduped = _content_cdup();
                if (cduped == true)
                {
                    /*
                     * 当执行过文件拷贝，移动后，普通的文件浏览返回上层时
                     * ，需要全部刷新，以避免目录内容有更新的情况。如果是再次进行
                     *文件操作，且中间有返回上层的操作，则不需要刷新目录，因为选择
                     *目标目录时，目录内容都是最新的。
                     */
                    if ( (g_file_op_execed == true) && (s_last_fsel_handle == NULL))
                    {
                        _content_refresh();

                        if (s_filelist->data->content->p_cur_dir->parent == NULL)
                        {
                            /*
                             * when we cd up to the root dir, we clear the file_op_execed flag.
                             */
                            g_file_op_execed = false;
                        }
                    }
                    browser_widget_filelist_paint(s_filelist);
                }
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        {
            print_warning("WIDGET_SIG_MENUVIEW_EXCEED_FIRST:\n");
            if (_content_populate_file_up() == false)
            {
                _content_populate_dir_up();
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        {
            print_warning("WIDGET_SIG_MENUVIEW_EXCEED_LAST:\n");
            if (_content_populate_dir_down() == false)
            {
                _content_populate_file_down();
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_UPDATE_ALL:
        {
            _content_update_all();
        }
        break;

        case WIDGET_SIG_MENUVIEW_SHORT_MENU:
        {
            if ((item != NULL) || (s_last_fsel_handle != NULL))
            {
                s_filelist->callback(s_filelist, WIDGET_SIG_FILELIST_SHORT_MENU);
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_DELETE:
        {
            if (item != NULL)
            {
                s_filelist->callback(s_filelist, WIDGET_SIG_FILELIST_DELETE);
            }
        }
        break;

        default:
        break;
    }

}

static bool _content_cd(char* dir_path)
{
    dir_level_t* p = NULL;
    content_list_t* content = s_filelist->data->content;

    FSEL_HANDLE fsel_handle = content->p_fsel;
    menuview_t* list_handle = content->content_list;

    const char* cwd = NULL;

    p = (dir_level_t*) malloc(sizeof(dir_level_t));
    if (p == NULL)
    {
        print_err("create dir_level_t failed.\n");
        return false;
    }

    if (dir_path != NULL)
    {
        int ret = fsel_cd(fsel_handle, dir_path);
        if ((s_filelist->search_string != NULL) && (ret == 0) && (s_last_fsel_handle == NULL))
        {
            browser_widget_filelist_enter_search_mode("");
        }
    }

    cwd = fsel_getworkdir(fsel_handle);
    if (cwd != NULL)
    {
        p->workdir = strdup(cwd);
        if (p->workdir != NULL)
        {
            strcpy(s_current_path_name, p->workdir);
        }
    }
    else
    {
        p->workdir = NULL;
    }

    p->dirs = fsel_getfilenum(fsel_handle, FSEL_DIRNUM);
    p->files = fsel_getfilenum(fsel_handle, FSEL_FILENUM);

    p->item_group = menu_create();
    p->item_group_size = menuview_get_lines(list_handle);

    if (content->mode == CONTENT_NORMAL)
    {
        menu_set_global_size(p->item_group, p->dirs + p->files);
    }
    else
    {
        menu_set_global_size(p->item_group, p->dirs);
    }

    menu_set_global_offset(p->item_group, 0);

    p->child = NULL;
    p->parent = NULL;
    if (content->p_root == NULL)
    {
        content->p_root = p;
    }
    if (content->p_cur_dir != NULL)
    {
        content->p_cur_dir->child = p;
        p->parent = content->p_cur_dir;
    }
    content->p_cur_dir = p;

    menuview_set_menu(content->content_list, content->p_cur_dir->item_group);

    _content_populate();

    if (s_last_fsel_handle == NULL)
    {
        set_single_shot_timer(1, (timer_proc) _show_nofile, NULL);
    }

    return true;
}

static bool _content_cdup(void)
{
    content_list_t* content = s_filelist->data->content;

    dir_level_t* p = content->p_cur_dir;

    if ((s_filelist->search_string != NULL) && (s_last_fsel_handle == NULL))
    {
        browser_widget_filelist_enter_search_mode("");
    }

    if (p->parent != NULL)
    {
        if (p->item_group != NULL)
        {
            _clear_menu(p->item_group);
            menu_delete(p->item_group);
        }
        if (p->workdir != NULL)
        {
            free(p->workdir);
        }
        content->p_cur_dir = p->parent;
        content->p_cur_dir->child = NULL;
        free(p);
        strcpy(s_current_path_name, content->p_cur_dir->workdir);
        menuview_set_menu(content->content_list, content->p_cur_dir->item_group);
        //fsel_setworkdir(content->p_fsel,content->p_cur_dir->workdir);
        fsel_cd(content->p_fsel, "..");
    }
    else if (is_sub_dir(content->default_path_name, p->workdir) == true)
    {
        char path_name[PATH_MAX];
        memset(path_name,0,PATH_MAX);
        strncpy(path_name,p->workdir,PATH_MAX -1 );

        if (p->item_group != NULL)
        {
            _clear_menu(p->item_group);
            menu_delete(p->item_group);
        }
        if (p->workdir != NULL)
        {
            free(p->workdir);
        }
        free(p);
        content->p_cur_dir = NULL;
        content->p_root = NULL;

        _content_cd("..");
        _locate_highlight_item(path_name);
    }
    else
    {
        _bringup_entry(ENTRYOP_CDUP);
        return false;
    }
    return true;
}

static void _content_refresh(void)
{
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;
    FSEL_HANDLE p_fsel = content->p_fsel;

    if(p == NULL)
    {
        return;
    }

    _clear_menu(p->item_group);
    menu_remove_all_items(p->item_group);

    content->p_cur_dir->dirs = fsel_getfilenum(p_fsel, FSEL_DIRNUM);
    content->p_cur_dir->files = fsel_getfilenum(p_fsel, FSEL_FILENUM);

    if (s_filelist->search_string == NULL)
    {
        /*
         * in copy/move mode, because we are selecting the destination directory,
         * we just count the dirs.
         */
        if ((content->mode == CONTENT_COPY) || (content->mode == CONTENT_MOVE))
        {
            content->p_cur_dir->files = 0;
            menu_set_global_size(p->item_group, p->dirs);
        }
        else
        {
            /*
             * in normal mode, we count the files and the dirs.
             */
            menu_set_global_size(p->item_group, p->dirs + p->files);
        }
    }
    else
    {
        /*
         *  in search mode, we just count the files.
         */
        content->p_cur_dir->dirs = 0;
        menu_set_global_size(p->item_group, 0 + p->files);
    }

    menu_set_global_offset(p->item_group, 0);

    _content_populate();
}

static void _content_update_all(void)
{
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;
    FSEL_HANDLE p_fsel = content->p_fsel;

    int dirs = 0;

    if(p == NULL)
    {
        return;
    }

    _clear_menu(p->item_group);
    menu_remove_all_items(p->item_group);

    p->files = fsel_getfilenum(p_fsel, FSEL_FILENUM);

    if(s_filelist->search_string == NULL)
    {
        p->dirs = fsel_getfilenum(p_fsel, FSEL_DIRNUM);
    }
    else
    {
        p->dirs = 0;
    }

    dirs = p->dirs - p->item_group->global_offset;

    if (dirs > 0)
    {
        p->dir_startno = p->item_group->global_offset + 1;
        p->dir_num = _content_populate_dirs(p->item_group_size);

        if (content->mode == CONTENT_NORMAL)
        {
            int remain = 0;
            p->file_startno = 1;
            p->file_num = 0;

            remain = p->item_group_size - p->dir_num;
            if (remain > 0)
            {
                p->file_num = _content_populate_files(remain);
            }
        }
    }
    else
    {
        p->dir_num = 0;
        p->dir_startno = p->dirs + 1;

        if (content->mode == CONTENT_NORMAL)
        {
            p->file_startno = (p->item_group->global_offset - p->dirs) + 1;
            p->file_num = _content_populate_files(p->item_group_size);
        }

    }
    browser_widget_filelist_paint(s_filelist);
}

static void _content_populate(void)
{
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;

    p->dir_startno = 1;
    p->dir_num = 0;

    p->file_startno = 1;
    p->file_num = 0;

    /*
     *  in search mode, we just count the files.
     */
    if ((s_filelist->search_string == NULL) || (s_last_fsel_handle != NULL))
    {
        p->dir_num = _content_populate_dirs(p->item_group_size);
    }

    if ((content->mode != CONTENT_COPY) && (content->mode != CONTENT_MOVE))
    {
        int remain = 0;

        remain = p->item_group_size - p->dir_num;
        if (remain > 0)
        {
            p->file_num = _content_populate_files(remain);
        }
    }
}

static int _content_populate_dirs(int number)
{
    int result = 0;
    int i = 0;
    int j = 0;
    fsel_filename_t* fsel_p = NULL;
    fsel_filename_t* fsel_tmp = NULL;
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;

    i = (((p->dirs - p->dir_startno) + 1) > number) ? number : ((p->dirs - p->dir_startno) + 1);
    if (i > 0)
    {
        fsel_p = (fsel_filename_t*) malloc(sizeof(fsel_filename_t) * (unsigned int) i);
        fsel_tmp = fsel_p;
        result = fsel_listdir(content->p_fsel, fsel_p, p->dir_startno, i, FSEL_POSITIVE);
    }
    for (j = 0; j < result; j++)
    {
        menuitem_t item;
        memset(&item, 0, sizeof(menuitem_t));

        item.text = strdup(fsel_tmp->name);

        if ((content->select_mode == true) && (content->select_flag[(p->dir_startno - 1) + j] == true))
        {
            item.sel_icon_id = content->pich_sel_array[TYPE_FOLD]->pic_id;
            item.unsel_icon_id = content->pic_sel_array[TYPE_FOLD]->pic_id;
        }
        else
        {
            item.sel_icon_id = content->pich_array[TYPE_FOLD]->pic_id;
            item.unsel_icon_id = content->pic_array[TYPE_FOLD]->pic_id;
        }

        item.has_child = true;
        menu_append_item(p->item_group, &item);
        fsel_tmp++;
    }

    if (fsel_p != NULL)
    {
        free(fsel_p);
    }
    return result;
}

static bool _content_populate_dir_down(void)
{
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;

    int result;
    int r;
    menuitem_t* first_dir_entry = NULL;

    if (p->dir_num == 0)
    {
        return false;
    }

    first_dir_entry = menu_get_item(p->item_group, 0);
    _clear_menuitem(first_dir_entry);
    menu_remove_first_item(p->item_group);

    r = p->dirs - (p->dir_startno - 1) - p->dir_num;
    if (r > 0)
    {
        fsel_filename_t fsel;
        menuitem_t item;

        result = fsel_listdir(content->p_fsel, &fsel, p->dir_startno + p->dir_num, 1, FSEL_POSITIVE);

        memset(&item, 0, sizeof(menuitem_t));

        item.text = strdup(fsel.name);

        if ((content->select_mode == true) && (content->select_flag[(p->dir_startno - 1) + p->dir_num] == true))
        {
            item.sel_icon_id = content->pich_sel_array[TYPE_FOLD]->pic_id;
            item.unsel_icon_id = content->pic_sel_array[TYPE_FOLD]->pic_id;
        }
        else
        {
            item.sel_icon_id = content->pich_array[TYPE_FOLD]->pic_id;
            item.unsel_icon_id = content->pic_array[TYPE_FOLD]->pic_id;
        }

        item.has_child = true;
        menu_append_item(p->item_group, &item);
        p->dir_startno++;
        return true;
    }
    else
    {
        //if(p->dir_startno < p->dirs)
        p->dir_startno++;
        p->dir_num--;
        return false;
    }
}

static bool _content_populate_dir_up(void)
{
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;

    int result;
    fsel_filename_t fsel;
    menuitem_t item;

    result = fsel_listdir(content->p_fsel, &fsel, p->dir_startno - 1, 1, FSEL_POSITIVE);

    memset(&item, 0, sizeof(menuitem_t));

    item.text = strdup(fsel.name);

    if ((content->select_mode == true) && (content->select_flag[p->dir_startno - 1 - 1] == true))
    {
        item.sel_icon_id = content->pich_sel_array[TYPE_FOLD]->pic_id;
        item.unsel_icon_id = content->pic_sel_array[TYPE_FOLD]->pic_id;
    }
    else
    {
        item.sel_icon_id = content->pich_array[TYPE_FOLD]->pic_id;
        item.unsel_icon_id = content->pic_array[TYPE_FOLD]->pic_id;
    }

    item.has_child = true;
    menu_prepend_item(p->item_group, &item);

    if (p->dir_num < p->item_group_size)
    {
        p->dir_num++;

    }
    else
    {
        menuitem_t* last_dir_entry = menu_get_item(p->item_group, p->item_group->total - 1);
        _clear_menuitem(last_dir_entry);
        menu_remove_last_item(p->item_group);
    }
    p->dir_startno--;
    return true;
}

static int _content_populate_files(int number)
{
    int result = 0;
    int i, j;
    fsel_filename_t* fsel_p = NULL;
    fsel_filename_t* fsel_tmp = NULL;
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;

    i = (((p->files - p->file_startno) + 1) > number) ? number : ((p->files - p->file_startno) + 1);
    if (i > 0)
    {
        fsel_p = (fsel_filename_t*) malloc(sizeof(fsel_filename_t) * (unsigned int) i);
        fsel_tmp = fsel_p;
        result = fsel_listfile(content->p_fsel, fsel_p, p->file_startno, i, FSEL_POSITIVE);
    }
    for (j = 0; j < result; j++)
    {
        menuitem_t item;
        file_type_e t;
        memset(&item, 0, sizeof(menuitem_t));

        item.text = strdup(fsel_tmp->name);

        t = find_file_type(fsel_tmp->name);

        if ((content->select_mode == true) && (content->select_flag[((p->dirs + p->file_startno) - 1) + j]))
        {
            item.sel_icon_id = content->pich_sel_array[t]->pic_id;
            item.unsel_icon_id = content->pic_sel_array[t]->pic_id;
        }
        else
        {
            item.sel_icon_id = content->pich_array[t]->pic_id;
            item.unsel_icon_id = content->pic_array[t]->pic_id;
        }

        item.has_child = false;
        menu_append_item(p->item_group, &item);
        fsel_tmp++;
    }

    if (fsel_p != NULL)
    {
        free(fsel_p);
    }
    return result;
}

static bool _content_populate_file_down(void)
{
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;

    int result;
    fsel_filename_t fsel;
    menuitem_t item;
    file_type_e t;

    result = fsel_listfile(content->p_fsel, &fsel, p->file_startno + p->file_num, 1, FSEL_POSITIVE);

    memset(&item, 0, sizeof(menuitem_t));

    item.text = strdup(fsel.name);

    t = find_file_type(fsel.name);

    if ((content->select_mode == true) && (content->select_flag[((p->dirs + p->file_startno) - 1) + p->file_num]))
    {
        item.sel_icon_id = content->pich_sel_array[t]->pic_id;
        item.unsel_icon_id = content->pic_sel_array[t]->pic_id;
    }
    else
    {
        item.sel_icon_id = content->pich_array[t]->pic_id;
        item.unsel_icon_id = content->pic_array[t]->pic_id;
    }

    item.has_child = false;
    menu_append_item(p->item_group, &item);

    if (p->file_num < p->item_group_size)
    {
        p->file_num++;

    }
    else
    {
        menuitem_t* first_file_entry = menu_get_item(p->item_group, 0);
        _clear_menuitem(first_file_entry);
        menu_remove_first_item(p->item_group);
        p->file_startno++;

    }
    return true;
}

static bool _content_populate_file_up(void)
{
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;

    int result;
    menuitem_t* last_file_entry = NULL;

    if (p->file_num == 0)
    {
        return false;
    }

    last_file_entry = menu_get_item(p->item_group, p->item_group->total - 1);
    _clear_menuitem(last_file_entry);
    menu_remove_last_item(p->item_group);

    if (p->file_startno > 1)
    {
        fsel_filename_t fsel;
        menuitem_t item;
        file_type_e t;

        result = fsel_listfile(content->p_fsel, &fsel, p->file_startno - 1, 1, FSEL_POSITIVE);

        memset(&item, 0, sizeof(menuitem_t));

        item.text = strdup(fsel.name);

        t = find_file_type(fsel.name);

        if ((content->select_mode == true) && (content->select_flag[((p->dirs + p->file_startno) - 1) - 1]))
        {
            item.sel_icon_id = content->pich_sel_array[t]->pic_id;
            item.unsel_icon_id = content->pic_sel_array[t]->pic_id;
        }
        else
        {
            item.sel_icon_id = content->pich_array[t]->pic_id;
            item.unsel_icon_id = content->pic_array[t]->pic_id;
        }

        item.has_child = false;
        menu_prepend_item(p->item_group, &item);
        p->file_startno--;
        return true;
    }
    else
    {
        p->file_num--;
        return false;
    }
}

static void _content_push(void)
{
    int i;

    content_list_t* content = s_filelist->data->content;
    s_last_p_root = content->p_root;
    s_last_p_cur_dir = content->p_cur_dir;
    s_last_select_mode = content->select_mode;
    s_last_select_flag = content->select_flag;


    content->p_root = NULL;
    content->p_cur_dir = NULL;

    content->select_flag = NULL;
    content->select_mode = false;


    for (i = 0; i < 3; i++)
    {
        s_last_entryitems[i] = s_entryitems[i];
    }

    s_last_entry_idx = s_entry_idx;

    s_last_global_offset = content->content_list->menu->global_offset;
    s_last_current = content->content_list->menu->current;

    s_last_fsel_handle = content->p_fsel;

    content->p_fsel= NULL;
}

static void _content_pop(void)
{
    content_list_t* content = s_filelist->data->content;
    content->p_root = s_last_p_root;
    s_last_p_root = NULL;
    content->p_cur_dir = s_last_p_cur_dir;
    s_last_p_cur_dir = NULL;
    content->select_mode = s_last_select_mode;
    s_last_select_mode = false;
    content->select_flag = s_last_select_flag;
    s_last_select_flag = NULL;

    s_last_global_offset = 0;
    s_last_current = 0;

    s_entry_idx = s_last_entry_idx;
    s_last_entry_idx = -1;

    content->p_fsel = s_last_fsel_handle;
    s_last_fsel_handle = NULL;

    g_fsel_handle = content->p_fsel;
}

filelist_t* browser_widget_filelist_create(filelist_cb_t callback)
{
    filelist_t* dl;
    bool b_entry_data_init = false;
    bool b_entry_view_init = false;

    char text[CFG_MAX_BUFF_LENGTH];
    char  browser_last_fullpath [PATH_MAX];
    char init_path [PATH_MAX];

    /*
     * malloc internal struct filelist_private_data_s variable.
     */

    dl = (filelist_t*) malloc(sizeof(filelist_t));
    if (dl == NULL)
    {
        print_err("malloc filelist_t failed.\n");
        return NULL;
    }
    memset(dl, 0, sizeof(filelist_t));

    dl->data = (filelist_private_data_t*) malloc(sizeof(filelist_private_data_t));
    if (dl->data == NULL)
    {
        print_err("malloc filelist_private_data_t failed.\n");
        free(dl);
        return NULL;
    }
    memset(dl->data, 0, sizeof(filelist_private_data_t));

    dl->data->entry = (entry_list_t*) malloc(sizeof(entry_list_t));
    if (dl->data->entry == NULL)
    {
        print_err("malloc entry_list_t failed.\n");
        free(dl->data);
        free(dl);
        return NULL;
    }
    memset(dl->data->entry, 0, sizeof(entry_list_t));

    dl->data->content = (content_list_t*) malloc(sizeof(content_list_t));
    if (dl->data->content == NULL)
    {
        print_err("malloc content_list_t failed.\n");
        free(dl->data->entry);
        free(dl->data);
        free(dl);
        return NULL;
    }
    memset(dl->data->content, 0, sizeof(content_list_t));

    s_filelist = dl;

    /*
     * Initialize internal variables
     */
    dl->callback = callback;
    dl->canvas_id = 0;
    dl->path_name = s_current_path_name;


    /*
     * load style.
     */
    if ( browser_widget_filelist_load_resource(dl) == false)
    {
        print_err("load resource failed.\n");
    }

    _load_config();

    if(g_resume_boot == true)
    {
        int ret  = -1;
        bool dir_exist = false;

        memset(text,0,CFG_MAX_BUFF_LENGTH);
        memset(browser_last_fullpath,0,PATH_MAX);
        memset(init_path,0,PATH_MAX);

        ret = get_config("browser_last_path", text, CFG_MAX_BUFF_LENGTH);
        if (ret != -1)
        {
            strncpy(browser_last_fullpath, text, PATH_MAX - 1);
        }

        strncpy(init_path, browser_last_fullpath, PATH_MAX - 1);
        dirname(init_path);

        dir_exist = is_dir_exist(init_path);
        if (dir_exist == false)
        {
            g_resume_boot = false;
        }
    }
/*
    if(resume_boot == true)
    {
        int idx;
        ret = get_config("browser_last_entry_idx", &idx,4);
        if (ret != -1)
        {
            entry_idx =  idx;
        }
    }
*/


    b_entry_data_init = _entry_data_init(dl->data->entry);
    if (b_entry_data_init == false)
    {
        print_err("init entry_list failed.\n");
        free(dl->data->entry);
        free(dl->data->content);
        free(dl->data);
        free(dl);
        return NULL;
    }

    b_entry_view_init = _entry_view_init(dl->data->entry);
    if (b_entry_view_init == false)
    {
        print_err("init entry_list failed.\n");
        _entry_unload(dl->data->entry);
        free(dl->data->entry);
        free(dl->data->content);
        free(dl->data);
        free(dl);
        return NULL;
    }

    dl->list_inuse = ENTRY;
    dl->content_mode = CONTENT_NORMAL;

    if (((s_entryitems[0] != ENTRY_NONE) && (s_entryitems[1] == ENTRY_NONE)) || (g_resume_boot == true))
    {
        if(g_resume_boot == true)
        {
            int ret = -1;
            int idx = 0;
            FSEL_HANDLE fsel_handle = NULL;

            _entry_unload(dl->data->entry);

            ret = get_config("browser_last_entry_idx", (char*)&idx,4);
            if (ret != -1)
            {
                if(s_entryitems[idx] != ENTRY_NONE)
                {
                    s_entry_idx =  idx;
                }
            }

            s_filelist->list_inuse = CONTENT;

            _content_init(s_entry_idx);

            fsel_handle = dl->data->content->p_fsel;

            fsel_setworkdir(fsel_handle,init_path);
            _content_cd(NULL);

            _locate_highlight_item(browser_last_fullpath);
        }
        else
        {
            _bringup_content(0);
        }
    }
    else
    {
        browser_widget_filelist_paint(dl);
    }
    return dl;
}

bool browser_widget_filelist_delete(filelist_t* filelist)
{
    if (filelist != NULL)
    {
        if (filelist->data != NULL)
        {
            if (filelist->data->entry != NULL)
            {
                _entry_deinit(filelist->data->entry);
                free(filelist->data->entry);
                filelist->data->entry = NULL;
            }

            if (filelist->data->content != NULL)
            {
                _content_deinit(filelist->data->content);
                free(filelist->data->content);
                filelist->data->content = NULL;
            }

            free(filelist->data);
            filelist->data = NULL;
        }

        free(filelist);
        filelist = NULL;
        s_filelist = NULL;
        return true;
    }
    else
    {
        return false;
    }

}

bool browser_widget_filelist_enterbackgroud(filelist_t* filelist)
{
    if (filelist->list_inuse == CONTENT)
    {
        /*
         _content_view_unload(filelist->data->content);
         */
        memset(s_current_path_name, 0, PATH_MAX);
        strcpy(s_current_path_name, filelist->data->content->p_cur_dir->workdir);
        strcat(s_current_path_name, filelist->file_name);
        set_config("browser_last_path", s_current_path_name, PATH_MAX - 1);

        set_config("browser_last_entry_idx",(char*)&s_entry_idx,4);
    }
    return true;
}

bool browser_widget_filelist_exit(filelist_t* filelist)
{
    filelist->callback(filelist, WIDGET_SIG_FILELIST_QUIT);
    return true;
}

void browser_widget_filelist_paint(filelist_t* filelist)
{
    switch (filelist->list_inuse)
    {
        case ENTRY:
        {
            menuview_paint(filelist->data->entry->entry_list);
        }
        break;
        case CONTENT:
        {
            menuview_paint(filelist->data->content->content_list);
        }
        break;

        default:
        break;
    }
}

void browser_widget_filelist_hide(filelist_t* filelist)
{
    switch (filelist->list_inuse)
    {
        case ENTRY:
        {
            menuview_hide(filelist->data->entry->entry_list);
        }
        break;
        case CONTENT:
        {
            menuview_hide(filelist->data->content->content_list);
        }
        break;

        default:
        break;
    }
}

void browser_widget_filelist_show(filelist_t* filelist)
{
    switch (filelist->list_inuse)
    {
        case ENTRY:
        {
            menuview_show(filelist->data->entry->entry_list);
            menuview_paint(filelist->data->entry->entry_list);
        }
        break;
        case CONTENT:
        {
            menuview_show(filelist->data->content->content_list);
            menuview_paint(filelist->data->content->content_list);
        }
        break;

        default:
        break;
    }
}

void browser_widget_filelist_proc_gui(filelist_t* filelist, gui_wm_message_t* pmsg)
{
    switch (filelist->list_inuse)
    {
        case ENTRY:
        {
            menuview_proc_gui(filelist->data->entry->entry_list, pmsg);
        }
        break;
        case CONTENT:
        {
            menuview_proc_gui(filelist->data->content->content_list, pmsg);
        }
        break;

        default:
        break;
    }
}

void browser_widget_filelist_proc_sys(filelist_t* filelist, msg_apps_t *pmsg)
{
    switch (pmsg->type)
    {
        case MSG_CARD_IN:
        {
            if (filelist->list_inuse == ENTRY)
            {
                _entry_reload(filelist->data->entry);
                browser_widget_filelist_paint(filelist);
            }
        }
        break;

        case MSG_CARD_OUT:
        {
            if (filelist->list_inuse == CONTENT)
            {
                if((s_entryitems[s_entry_idx] == ENTRY_CARD) || (s_last_entryitems[s_last_entry_idx] == ENTRY_CARD))
                {
                    filelist->callback(filelist, WIDGET_SIG_FILELIST_CONTENT_RELOAD);

                    if (s_last_entryitems[s_last_entry_idx] == ENTRY_CARD)
                    {
                        s_last_entryitems[s_last_entry_idx] = ENTRY_NONE;
                    }

                    _bringup_entry(ENTRYOP_CD);
                    if (g_file_op_execed == true)
                    {
                        g_file_op_execed = false;
                    }
                }
            }
            else //if (filelist->list_inuse == ENTRY)
            {
                //if(s_last_entryitems[s_last_entry_idx] == ENTRY_CARD)
                if (s_last_entry_idx != -1)
                {
                    filelist->callback(filelist, WIDGET_SIG_FILELIST_CONTENT_RELOAD);
                    _entry_unload(filelist->data->entry);
                    _bringup_entry(ENTRYOP_CD);
                }
                else
                {
                    _entry_reload(filelist->data->entry);
                }
                browser_widget_filelist_paint(filelist);
            }
        }
        break;

        case MSG_USB_HOST_DISK_IN:
        {
            if (filelist->list_inuse == ENTRY)
            {
                _entry_reload(filelist->data->entry);
                browser_widget_filelist_paint(filelist);
            }
        }
        break;
        case MSG_USB_HOST_DISK_OUT:
        {
            if (filelist->list_inuse == CONTENT)
            {
                if ((s_entryitems[s_entry_idx] == ENTRY_EXTERNAL) ||
                      (s_last_entryitems[s_last_entry_idx] == ENTRY_EXTERNAL))
                {
                    filelist->callback(filelist, WIDGET_SIG_FILELIST_CONTENT_RELOAD);

                    if (s_last_entryitems[s_last_entry_idx] == ENTRY_EXTERNAL)
                    {
                        s_last_entryitems[s_last_entry_idx] = ENTRY_NONE;
                    }
                    _bringup_entry(ENTRYOP_CD);
                    if (g_file_op_execed == true)
                    {
                        g_file_op_execed = false;
                    }
                }
            }
            else //if (filelist->list_inuse == ENTRY)
            {
                //if (s_last_entryitems[s_last_entry_idx] == ENTRY_EXTERNAL)
                if (s_last_entry_idx != -1)
                {
                    filelist->callback(filelist, WIDGET_SIG_FILELIST_CONTENT_RELOAD);
                    _entry_unload(filelist->data->entry);
                    _bringup_entry(ENTRYOP_CD);
                }
                else
                {
                    _entry_reload(filelist->data->entry);
                }
                browser_widget_filelist_paint(filelist);
            }
        }
        break;
        default:
        break;
    }
}

bool browser_widget_filelist_load_resource(filelist_t* filelist)
{
    scene_t* scene;
    resgroup_resource_t* dirlist_group;
    resgroup_resource_t* icons_group;
    file_type_e t;
    entry_list_t* entry = NULL;
    content_list_t* content = filelist->data->content;

    scene = sty_get_app_scene();
    if (scene == NULL)
    {
        print_err("call sty_get_common_scene failed.\n");
        return false;
    }

    content->str_nofile = (string_resource_t*) get_scene_child(scene, STR_NOFILE);

    content->str_no_matching_item = (string_resource_t*) get_scene_child(scene, STR_NO_MATCHING_ITEM);

    dirlist_group = (resgroup_resource_t*) get_scene_child(scene, ENTRY_RESGROUP);

    entry = filelist->data->entry;

    entry->str_int_mem = (string_resource_t*) get_resgroup_child(dirlist_group, STR_INT_MEM);
    entry->str_ext_mem = (string_resource_t*) get_resgroup_child(dirlist_group, STR_EXT_MEM);
    entry->str_car_mem = (string_resource_t*) get_resgroup_child(dirlist_group, STR_CAR_MEM);
    entry->str_nodisk = (string_resource_t*) get_resgroup_child(dirlist_group, STR_NO_DISK);


    icons_group = (resgroup_resource_t*) get_scene_child(scene, ICONS_RESGROUP);

    for (t = TYPE_FOLD; t < (TYPE_DEFAULT + 1); t++)
    {
        content->pic_array[t] = (picture_resource_t*) get_resgroup_child(icons_group, s_macro_pic_array[t]);
        content->pic_sel_array[t] = (picture_resource_t*) get_resgroup_child(icons_group, s_macro_pic_sel_array[t]);
        content->pich_array[t] = (picture_resource_t*) get_resgroup_child(icons_group, s_macro_pich_array[t]);
        content->pich_sel_array[t] = (picture_resource_t*) get_resgroup_child(icons_group, s_macro_pich_sel_array[t]);
    }

    return true;
}

bool browser_widget_filelist_load_style(filelist_t* filelist)
{
    switch (filelist->list_inuse)
    {
        case ENTRY:
        {
            menuview_load_style(filelist->data->entry->entry_list, filelist->data->entry->entry_list_type);
        }
        break;
        case CONTENT:
        {
            menuview_load_style(filelist->data->content->content_list, filelist->data->content->content_list_type);
            filelist->data->content->p_cur_dir->item_group_size = menuview_get_lines(
                    filelist->data->content->content_list);
            _content_refresh();
        }
        break;

        default:
        break;
    }
    return true;
}

int browser_widget_filelist_get_bitmap_width(filelist_t* filelist)
{
    if (filelist->data->content->content_list != NULL)
    {
        return menuview_get_bitmap_width(filelist->data->content->content_list);
    }
    else
    {
        return 0;
    }
}
int browser_widget_filelist_get_bitmap_height(filelist_t* filelist)
{
    if (filelist->data->content->content_list != NULL)
    {
        return menuview_get_bitmap_height(filelist->data->content->content_list);
    }
    else
    {
        return 0;
    }
}

void browser_widget_filelist_toggle_select_mode(void)
{
    int i;
    int index_current;
    int global_size;
    int global_offset;

    content_list_t* content = s_filelist->data->content;

    dir_level_t* p = content->p_cur_dir;
    print_dbg();

    if (content->select_mode == false)
    {
        content->select_flag = (bool*) malloc(sizeof(bool) * (unsigned int) (p->dirs + p->files));
        if (content->select_flag == NULL)
        {
            print_err("malloc content->select_flag failed.");
            return;
        }
        for (i = 0; i < (p->dirs + p->files); i++)
        {
            content->select_flag[i] = true;
        }
        content->select_mode = true;
        s_filelist->select_mode = true;
    }
    else
    {
        content->select_mode = false;
        s_filelist->select_mode = false;
        free(content->select_flag);
    }

    index_current = p->item_group->current;
    global_size = p->item_group->global_size;
    global_offset = p->item_group->global_offset;

    /*
     menu_delete(p->item_group);
     p->item_group = menu_create();
     */

    _clear_menu(p->item_group);
    menu_remove_all_items(p->item_group);

    _content_populate_dirs(p->dir_num);

    _content_populate_files(p->file_num);

    menu_set_cur_index(p->item_group, index_current);
    menu_set_global_size(p->item_group, global_size);
    menu_set_global_offset(p->item_group, global_offset);

    menuview_paint(content->content_list);

}

void browser_widget_filelist_enter_move_mode(void)
{
    content_list_t* content = s_filelist->data->content;
    content->mode = CONTENT_MOVE;

    _content_push();

    _bringup_entry(ENTRYOP_CD);
}

void browser_widget_filelist_enter_copy_mode(void)
{
    content_list_t* content = s_filelist->data->content;
    content->mode = CONTENT_COPY;

    _content_push();

    _bringup_entry(ENTRYOP_CD);
}

void browser_widget_filelist_exit_fileop_mode(void)
{
    content_list_t* content = s_filelist->data->content;
    entry_list_t* entry = s_filelist->data->entry;

    content->mode = CONTENT_NORMAL;

    _entry_unload(entry);

    if (s_filelist->list_inuse == CONTENT)
    {
        _content_data_unload(content);
    }
    else if (s_filelist->list_inuse == ENTRY)
    {
        content->content_list = menuview_create(content->content_list_type, _content_callback);
        if (content->content_list == NULL)
        {
            print_err();
            return;
        }
        s_filelist->canvas_id = content->content_list->canvas_id;
    }
    else
    {
        print_err();
    }

    _content_pop();
    menuview_set_menu(content->content_list, content->p_cur_dir->item_group);

    fsel_setworkdir(content->p_fsel, (char*) content->p_cur_dir->workdir);

    memset(s_current_path_name,0,PATH_MAX);
    strcpy(s_current_path_name,  content->p_cur_dir->workdir);

    _content_refresh();

    s_filelist->list_inuse = CONTENT;
    browser_widget_filelist_paint(s_filelist);
    print_dbg();

}

void browser_widget_filelist_exec_fileop(void)
{
    content_list_t* content = s_filelist->data->content;
    entry_list_t* entry = s_filelist->data->entry;

    bool result = true;

    char dest[PATH_MAX];

    s_filelist->in_file_op = true;
    s_filelist->replace_yes = false;

    if (s_last_select_mode == false)
    {
        result = _single_item_exec_fileop();
    }
    else
    {
        result = _multi_item_exec_fileop();
    }


    s_filelist->in_file_op = false;
    s_filelist->replace_yes = false;


    if(g_update_fsel == false)
    {
        g_update_fsel = true;
        if(s_filelist->list_inuse == CONTENT)
        {
            fsel_update(g_fsel_handle,content->p_cur_dir->workdir,FSEL_ADDDATA);
        }
        else
        {
            int idx = menu_get_cur_index(entry->entry_menu);

            memset(dest, 0, PATH_MAX);
            switch (s_entryitems[idx])
            {
                case ENTRY_INTERNAL:
                {
                    strcpy(dest, UDISK_ROOT);
                }
                break;
                case ENTRY_CARD:
                {
                    strcpy(dest, CARD_ROOT);
                }
                break;
                case ENTRY_EXTERNAL:
                {
                    strcpy(dest, EXTERNAL_ROOT);
                }
                default:
                {
                    print_err();
                    strcpy(dest, UDISK_ROOT);
                }
                break;
            }
            fsel_update(g_fsel_handle,dest,FSEL_ADDDATA);
        }

        fsel_update(g_fsel_handle, s_last_p_cur_dir->workdir, FSEL_DELDATA);
    }

    g_file_op_execed = true;

    g_file_op_success = result;
    set_single_shot_timer(1, (timer_proc) file_op_done, NULL);
}

static bool _single_item_exec_fileop(void)
{
    content_list_t* content = s_filelist->data->content;
    entry_list_t* entry = s_filelist->data->entry;

    char src[PATH_MAX];
    char dest[PATH_MAX];

    menuitem_t* selected_item = NULL;
    menuitem_t* current_item = NULL;

    memset(src, 0, PATH_MAX);

    selected_item = menu_get_cur_item(s_last_p_cur_dir->item_group);
    if (selected_item == NULL)
    {
        print_err();
        return false;
    }

    if (s_filelist->search_string == NULL)
    {
        strcpy(src, s_last_p_cur_dir->workdir);
        strcat(src, selected_item->text);
    }
    else
    {
        /*
         * 如果在搜索模式下，需要临时取得操作文件的全路径名称
         */
        int d = fsel_getfilenum(s_last_fsel_handle,FSEL_DIRNUM);
        memset(s_current_path_name, 0, PATH_MAX);
        fsel_getfullpath(s_last_fsel_handle, s_current_path_name,d + s_last_global_offset
                + s_last_current + 1);
        strcpy(src, s_current_path_name);
        strcat(src, selected_item->text);
    }

    memset(dest, 0, PATH_MAX);
    if (s_filelist->list_inuse == CONTENT)
    {
        strcpy(dest, content->p_cur_dir->workdir);

        current_item = menu_get_cur_item(content->p_cur_dir->item_group);
        /*
         * if current_item is NULL, that means the current directory's content is empty.
         * We just copy to this directory.
         */
        if (current_item != NULL)
        {
            strcat(dest, current_item->text);
        }
        strcat(dest, "/");
        strcat(dest, selected_item->text);
    }
    else //g_filelist->list_inuse == ENTRY
    {
        const char* path =_entry_get_path(entry);
        if(path != NULL)
        {
            strcpy(dest,path);
        }

        strcat(dest, "/");
        strcat(dest, selected_item->text);
    }

    /*
     print_dbg("the selected file's name is:%s-%s\t\n  the destination dir is:%s-%s\t\n ",
     last_p_cur_dir->workdir,selected_item->text,content->p_cur_dir->workdir,current_item->text);
     */
    print_dbg("src is: %s\t dest is: %s",src,dest);

    if (selected_item->has_child == true)
    {
        if (content->mode == CONTENT_MOVE)
        {
            int ret = move_dir(src, dest);
            if (ret != 0)
            {
                return false;
            }
        }
        else if (content->mode == CONTENT_COPY)
        {
            int ret = copy_dir(src, dest);
            if (ret != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else    //selected_item->has_child != true
    {
        if (content->mode == CONTENT_MOVE)
        {
            int ret = move_file(src, dest);
            if (ret != 0)
            {
                return false;
            }
        }
        else if (content->mode == CONTENT_COPY)
        {
            int ret = copy_file(src, dest);
            if (ret != 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}


static bool _multi_item_exec_fileop(void)
{
    bool result;

    g_update_fsel = true;   //avoid filenavi internal bug

    if (s_filelist->search_string == NULL)
    {
        result = _multi_item_exec_fileop_normal();
    }
    else /* 搜索模式*/
    {
        result = _multi_item_exec_fileop_searchmode();
    }
    return result;
}

static bool _multi_item_exec_fileop_normal(void)
{
    bool result = true;

    content_list_t* content = s_filelist->data->content;
    entry_list_t* entry = s_filelist->data->entry;

    char src[PATH_MAX];
    char dest[PATH_MAX];

    menuitem_t* current_item = NULL;

    fsel_filename_t* fsel_p = NULL;
    fsel_filename_t* fsel_tmp = NULL;

    int i, d, f;

    fsel_p = (fsel_filename_t*) malloc(sizeof(fsel_filename_t) * (unsigned int) (s_last_p_cur_dir->dirs
            + s_last_p_cur_dir->files));
    if (fsel_p == NULL)
    {
        print_err("malloc fsel_p failed.");
        result = false;
        return result;
    }

    fsel_tmp = fsel_p;

    d = fsel_listdir(s_last_fsel_handle, fsel_p, 1, s_last_p_cur_dir->dirs, FSEL_POSITIVE);

    for (i = 0; i < d; i++)
    {
        print_dbg(" dirpath is %s\t dirname is %s\t selectmode=%d\n",
                fsel_tmp->filepath,fsel_tmp->name,*(s_last_select_flag+i));
        if (*(s_last_select_flag + i) == true)
        {
            memset(src, 0, PATH_MAX);
            strcpy(src, fsel_tmp->filepath);
            strcat(src, fsel_tmp->name);

            memset(dest, 0, PATH_MAX);

            if (s_filelist->list_inuse == CONTENT)
            {
                strcpy(dest, content->p_cur_dir->workdir);

                current_item = menu_get_cur_item(content->p_cur_dir->item_group);
                /*
                 * if current_item is NULL, that means the current directory's content is empty.
                 * We just copy to this directory.
                 */
                if (current_item != NULL)
                {
                    strcat(dest, current_item->text);
                }
                strcat(dest, "/");
                strcat(dest, fsel_tmp->name);
            }
            else //g_filelist->list_inuse == ENTRY
            {
                const char* path =_entry_get_path(entry);
                if(path != NULL)
                {
                    strcpy(dest,path);
                }

                strcat(dest, "/");
                strncat(dest, fsel_tmp->name, (size_t)(PATH_MAX) - strlen(dest) - 1);
            }

            if (content->mode == CONTENT_MOVE)
            {
                int ret = move_dir(src, dest);
                if (ret != 0)
                {
                    result = false;
                    goto fileop_normal_failed;
                }
            }
            else if (content->mode == CONTENT_COPY)
            {
                int ret = copy_dir(src, dest);
                if (ret != 0)
                {
                    result = false;
                    goto fileop_normal_failed;
                }
            }
            else
            {
                result = false;
            }
        }
        fsel_tmp++;
    }

    f = fsel_listfile(s_last_fsel_handle, fsel_p + s_last_p_cur_dir->dirs, 1, s_last_p_cur_dir->files, FSEL_POSITIVE);

    for (i = 0; i < f; i++)
    {
        print_dbg(" filepath is %s\t filename is %s\t selectmode=%d\n",
                fsel_tmp->filepath,fsel_tmp->name,*(s_last_select_flag +d+i));
        if (*(s_last_select_flag + d + i) == true)
        {

            memset(src, 0, PATH_MAX);
            strcpy(src, fsel_tmp->filepath);
            strcat(src, fsel_tmp->name);

            memset(dest, 0, PATH_MAX);
            if (s_filelist->list_inuse == CONTENT)
            {
                strcpy(dest, content->p_cur_dir->workdir);

                current_item = menu_get_cur_item(content->p_cur_dir->item_group);
                /*
                 * if current_item is NULL, that means the current directory's content is empty.
                 * We just copy to this directory.
                 */
                if (current_item != NULL)
                {
                    strcat(dest, current_item->text);
                }
                strcat(dest, "/");
                strcat(dest, fsel_tmp->name);
            }
            else //g_filelist->list_inuse == ENTRY
            {
                const char* path =_entry_get_path(entry);
                if(path != NULL)
                {
                    strcpy(dest,path);
                }

                strcat(dest, "/");
                strncat(dest, fsel_tmp->name, (size_t)(PATH_MAX )- strlen(dest) - 1);
            }

            if (content->mode == CONTENT_MOVE)
            {
                int ret = move_file(src, dest);
                if (ret != 0)
                {
                    result = false;
                    goto fileop_normal_failed;
                }
            }
            else if (content->mode == CONTENT_COPY)
            {
                int ret = copy_file(src, dest);
                if (ret != 0)
                {
                    result = false;
                    goto fileop_normal_failed;
                }
            }
            else
            {
                result = false;
            }
        }
        fsel_tmp++;
    }

    fileop_normal_failed:
    free(fsel_p);

    return result;
}



static bool _multi_item_exec_fileop_searchmode(void)
{
    bool result = true;

    content_list_t* content = s_filelist->data->content;
    entry_list_t* entry = s_filelist->data->entry;

    char src[PATH_MAX];
    char dest[PATH_MAX];

    menuitem_t* current_item = NULL;

    fsel_filename_t* fsel_p = NULL;
    fsel_filename_t* fsel_tmp = NULL;

    int i, d, f;

    fsel_p = (fsel_filename_t*) malloc(sizeof(fsel_filename_t) * (unsigned int) (s_last_p_cur_dir->files));
    if (fsel_p == NULL)
    {
        print_err("malloc fsel_p failed.");
        result = false;
        return result;
    }
    fsel_tmp = fsel_p;
    f = fsel_listfile(s_last_fsel_handle, fsel_p, 1, s_last_p_cur_dir->files, FSEL_POSITIVE);

    d = fsel_getfilenum(s_last_fsel_handle,FSEL_DIRNUM);

    for (i = 0; i < f; i++)
    {
        print_dbg(" filepath is %s\t filename is %s\t selectmode=%d\n",
                fsel_tmp->filepath,fsel_tmp->name,*(s_last_select_flag+i));
        if (*(s_last_select_flag + i) == true)
        {

            memset(src, 0, PATH_MAX);
            /*
             * 如果在搜索模式下，需要临时取得操作文件的全路径名称
             */
            memset(s_current_path_name, 0, PATH_MAX);

            /*
             * 在搜索模式下，仅会显示文件。但实际上文件选择器也会将符合条件的
             * 目录列出。这里将目录数也考虑进去。
             * 注意：这里不同于搜索结果多选删除时采用的每次都针对第一个选中元素
             * 进行操作的做法，原因是移动和拷贝时，是使用的2个文件选择器句柄，这样
             * 当前句柄的数据就会保持不变。
             */
            fsel_getfullpath(s_last_fsel_handle, s_current_path_name, d + i + 1);
            strcpy(src, s_current_path_name);
            strncat(src, fsel_tmp->name, PATH_MAX - strlen(src) - 1);

            memset(dest, 0, PATH_MAX);
            if (s_filelist->list_inuse == CONTENT)
            {
                strcpy(dest, content->p_cur_dir->workdir);

                current_item = menu_get_cur_item(content->p_cur_dir->item_group);
                /*
                 * if current_item is NULL, that means the current directory's content is empty.
                 * We just copy to this directory.
                 */
                if (current_item != NULL)
                {
                    strcat(dest, current_item->text);
                }
                strcat(dest, "/");
                strcat(dest, fsel_tmp->name);
            }
            else //g_filelist->list_inuse == ENTRY
            {
                const char* path =_entry_get_path(entry);
                if(path != NULL)
                {
                    strcpy(dest,path);
                }
                strcat(dest, "/");
                strncat(dest, fsel_tmp->name, (size_t) (PATH_MAX) - strlen(dest) - 1);
            }

            if (content->mode == CONTENT_MOVE)
            {
                int ret = move_file(src, dest);
                if (ret != 0)
                {
                    result = false;
                    goto fileop_searchmode_failed;
                }
            }
            else if (content->mode == CONTENT_COPY)
            {
                int ret = copy_file(src, dest);
                if (ret != 0)
                {
                    result = false;
                    goto fileop_searchmode_failed;
                }
            }
            else
            {
                result = false;
            }
        }
        fsel_tmp++;
    }

    fileop_searchmode_failed:
    free(fsel_p);

    return result;
}


void browser_widget_filelist_exec_delete_fileop(void)
{
    bool result = true;
    content_list_t* content = s_filelist->data->content;

    char dest[PATH_MAX];

    fsel_filename_t* fsel_p = NULL;
    fsel_filename_t* fsel_tmp;

    content->mode = CONTENT_DELETE;
    if (content->select_mode == false)
    {

        menuitem_t* current_item = menu_get_cur_item(content->p_cur_dir->item_group);
        if (current_item == NULL)
        {
            print_err();
            goto exit_file_delete_op;
        }

        memset(dest, 0, PATH_MAX);

        if (s_filelist->search_string == NULL)
        {
            strcpy(dest, content->p_cur_dir->workdir);
            strcat(dest, current_item->text);
        }
        else
        {
            /*
             * 如果在搜索模式下，需要临时取得操作文件的全路径名称
             */
            int d = fsel_getfilenum(g_fsel_handle,FSEL_DIRNUM);
            memset(s_current_path_name, 0, PATH_MAX);
            fsel_getfullpath(g_fsel_handle, s_current_path_name, d + content->content_list->menu->global_offset
                    + content->content_list->menu->current + 1);
            strcpy(dest, s_current_path_name);
            strcat(dest, current_item->text);
        }

        if (current_item->has_child == true)
        {
            if (remove_dir_recursive(dest) != 0)
            {
                result = false;
            }
        }
        else
        {
            if (remove_file(dest) != 0)
            {
                result = false;
            }
        }
    }
    else /*  多选模式下*/
    {
        int i, d, f;

        g_update_fsel = true;   //avoid filenavi internal bug

        if (s_filelist->search_string == NULL)
        {
            /*
             * 非搜索模式
             */
            fsel_p = (fsel_filename_t*) malloc(sizeof(fsel_filename_t) * (unsigned int) (content->p_cur_dir->dirs
                    + content->p_cur_dir->files));
            if (fsel_p == NULL)
            {
                print_err("malloc fsel_p failed.");
                result = false;
                goto exit_file_delete_op;
            }
            fsel_tmp = fsel_p;

            //fsel_setworkdir(content->p_fsel, last_p_cur_dir->workdir);

            d = fsel_listdir(content->p_fsel, fsel_p, 1, content->p_cur_dir->dirs, FSEL_POSITIVE);

            for (i = 0; i < d; i++)
            {
                print_dbg(" dirpath is %s\t dirname is %s\t selectmode=%d\n",
                        fsel_tmp->filepath,fsel_tmp->name,*(content->select_flag+i));
                if (*(content->select_flag + i) == true)
                {
                    memset(dest, 0, PATH_MAX);
                    strcpy(dest, fsel_tmp->filepath);
                    strcat(dest, fsel_tmp->name);

                    if (remove_dir_recursive(dest) != 0)
                    {
                        result = false;
                        goto exit_file_delete_op;
                    }
                }
                fsel_tmp++;
            }

            f = fsel_listfile(content->p_fsel, fsel_p + content->p_cur_dir->dirs, 1, content->p_cur_dir->files,
                    FSEL_POSITIVE);

            for (i = 0; i < f; i++)
            {
                print_dbg(" filepath is %s\t filename is %s\t selectmode=%d\n",
                        fsel_tmp->filepath,fsel_tmp->name,*(content->select_flag +d+i));
                if (*(content->select_flag + d + i) == true)
                {
                    memset(dest, 0, PATH_MAX);
                    strcpy(dest, fsel_tmp->filepath);
                    strcat(dest, fsel_tmp->name);

                    if (remove_file(dest) != 0)
                    {
                        result = false;
                        goto exit_file_delete_op;
                    }

                }
                fsel_tmp++;
            }
        }
        else
        {
            /*
             * 搜索模式下
             */
            int unselected_files = 0;

            fsel_p = (fsel_filename_t*) malloc(sizeof(fsel_filename_t) * (unsigned int) (content->p_cur_dir->files));
            if (fsel_p == NULL)
            {
                print_err("malloc fsel_p failed.");
                result = false;
                goto exit_file_delete_op;
            }
            fsel_tmp = fsel_p;

            f = fsel_listfile(content->p_fsel, fsel_p, 1, content->p_cur_dir->files, FSEL_POSITIVE);

            d = fsel_getfilenum(content->p_fsel,FSEL_DIRNUM);

            for (i = 0; i < f; i++)
            {
                print_dbg(" filepath is %s\t filename is %s\t selectmode=%d\n",
                        fsel_tmp->filepath,fsel_tmp->name,*(content->select_flag +i));
                if (*(content->select_flag + i) == true)
                {
                    memset(dest, 0, PATH_MAX);

                    memset(s_current_path_name, 0, PATH_MAX);
                    /*
                     * 在搜索模式下，仅会显示文件。但实际上文件选择器也会将符合条件的
                     * 目录列出。这里将目录数也考虑进去。每次删除一个文件后，文件选择器
                     * 都会更新index值，所以应用只需要总是删除当前可以删除的第一个文件即可。
                     */
                    fsel_getfullpath(g_fsel_handle, s_current_path_name, d + 1 + unselected_files);
                    strcpy(dest, s_current_path_name);

                    strncat(dest, fsel_tmp->name, PATH_MAX - strlen(dest) - 1);

                    if (remove_file(dest) != 0)
                    {
                        result = false;
                        goto exit_file_delete_op;
                    }
                }
                else
                {
                    unselected_files++;
                }
                fsel_tmp++;
            }
        }
    }

    exit_file_delete_op:
    content->mode = CONTENT_NORMAL;

    if (g_update_fsel == false)
    {
        g_update_fsel = true;
        fsel_update(g_fsel_handle, (char*) content->p_cur_dir->workdir, FSEL_DELDATA);
    }

    if(fsel_p != NULL)
    {
        free(fsel_p);
    }

    g_file_op_success = result;
    set_single_shot_timer(1, (timer_proc) file_op_done, NULL);
}


void browser_widget_filelist_exit_delete_fileop_mode(void)
{
    content_list_t* content = s_filelist->data->content;
    dir_level_t* p = content->p_cur_dir;

    if (p == NULL)
    {
        return;
    }
    /*
     * if delete only one item successfully, we should update the menu data.
     */
    if (content->select_mode == false)
    {
        if (g_file_op_success == true)
        {
            int offset = p->item_group->global_offset;
            int current = p->item_group->current;

            p->item_group->global_size--;

            if ((offset + p->item_group->total) < (p->dirs + p->files))
            {
                /*
                 * if there are more items downwards.
                 */
                _content_update_all();
                p->item_group->current = current;
            }
            else if (offset > 0)
            {
                /*
                 * if there are more items upwards.
                 */
                p->item_group->global_offset = offset - 1;
                _content_update_all();
                p->item_group->current = current;
            }
            else
            {
                /*
                 * if there are no items downwards or upwards.
                 */
                _content_update_all();
                if (current <= (p->item_group->total - 1))
                {
                    p->item_group->current = current;
                }
                else
                {
                    p->item_group->current = current - 1;
                }
            }
        }
        else
        {
            /*
             * if delete only one item failed, we do nothing.
             */
        }
    }
    else
    {
        /*
         * if delete more than one item, we just refresh all.
         */
        _content_refresh();
    }
    browser_widget_filelist_paint(s_filelist);
}

void browser_widget_filelist_enter_search_mode(char* search_string)
{
    char* p = search_string;
    char* q = search_string;
    size_t str_len = 0;

    content_list_t* content = s_filelist->data->content;

    if (s_filelist->search_string != NULL)
    {
        free(s_filelist->search_string);
        s_filelist->search_string = NULL;
    }

    /*
     * trimming the leading & trailing blanks
     */
    if ((search_string != NULL) && (strlen(search_string) > 0))
    {
        p = search_string;
        q = search_string + strlen(search_string) - 1;

        while (*p == ' ')
        {
            p++;
        }

        while ((q >= p) && (*q == ' '))
        {
            q--;
        }

        if (q >= p)
        {
            str_len = (size_t)(q - p + 1 + 2);
            s_filelist->search_string = (char*) malloc(str_len + 1);
            if(s_filelist->search_string != NULL)
            {
                memset(s_filelist->search_string, 0, str_len + 1);
                strncpy(s_filelist->search_string, "*", 1);
                strncat(s_filelist->search_string, p, (size_t) (q - p + 1));
                strncat(s_filelist->search_string, "*", 1);

                if (s_filelist->is_search_subdir == true)
                {
                    fsel_subdir_setting(content->p_fsel, FSEL_SUB, FSEL_RESERVE);
                }
                else
                {
                    fsel_subdir_setting(content->p_fsel, FSEL_NOSUB, FSEL_RESERVE);
                }
                fsel_set_query(content->p_fsel, s_filelist->search_string, FSEL_QUERY);
            }
        }
    }
    else
    {
        fsel_subdir_setting(content->p_fsel, FSEL_NOSUB, FSEL_RESERVE);
        fsel_set_query(content->p_fsel, s_filelist->search_string, FSEL_CANCEL);
    }

    _content_refresh();
    //browser_widget_filelist_paint(g_filelist);
}


void browser_widget_filelist_show_no_matching_items(void)
{

    dialog_init_t init;
    init.head = DIALOG_HEAD_WARN;
    init.str_id = s_filelist->data->content->str_no_matching_item->str_id;

    dialog_modal(&init);

}


static void _show_nofile(void)
{

    if ((s_dialog_nofile_visible == true) || (s_dialog_nodisk_visible == true))
    {
        return;
    }

    if ((s_filelist->data->content != NULL) && (s_filelist->data->content->content_list != NULL)
            && (s_filelist->data->content->content_list->menu != NULL)
            && ((s_filelist->data->content->content_list->menu->total) == 0))
    {
        dialog_init_t init;
        init.head = DIALOG_HEAD_WARN;
        init.str_id = s_filelist->data->content->str_nofile->str_id;
        s_dialog_nofile_visible = true;
        dialog_modal(&init);
        s_dialog_nofile_visible = false;
    }
}

static void _show_nodisk(void)
{
    const app_info_t * app_info = NULL;

    if ((s_dialog_nodisk_visible == true)|| (s_dialog_nofile_visible == true))
    {
        return;
    }

    app_info = app_running_list_get_this_app_info();
    if((app_info != NULL) && (app_info->type != APP_TYPE_GUI))
    {
        print_warning(" console app no to deal");
        return;
    }

    if (s_entryitems[0] == ENTRY_NONE)
    {
        dialog_init_t init;
        init.head = DIALOG_HEAD_WARN;
        init.str_id = s_filelist->data->entry->str_nodisk->str_id;

        s_dialog_nodisk_visible = true;
        dialog_modal(&init);
        s_dialog_nodisk_visible = false;
    }
}

static void _load_config(void)
{
    int ret;
    char text[CFG_MAX_BUFF_LENGTH];

    memset(text, 0, CFG_MAX_BUFF_LENGTH);
    ret = get_config(CFG_SEARCH_FOLDER, text, CFG_MAX_BUFF_LENGTH);
    if ((ret != -1) && (strcmp(text, "on") == 0))
    {
        s_filelist->is_search_subdir = true;
    }
    else
    {
        s_filelist->is_search_subdir = false;
    }

    memset(text, 0, CFG_MAX_BUFF_LENGTH);
    ret = get_config(CFG_FSEL_SORT_STYLE, text, CFG_MAX_BUFF_LENGTH);
    if (ret != -1)
    {
        if (strcmp(text, "pinyin") == 0)
        {
            s_fsel_sort_style = FSEL_PINYIN;
        }
        else if (strcmp(text, "unicode") == 0)
        {
            s_fsel_sort_style = FSEL_BINARY;
        }
        else
        {
            s_fsel_sort_style = FSEL_NOSORTING;
        }
    }
}


static void _clear_menuitem(menuitem_t* item)
{
    if (item == NULL)
    {
        return;
    }
    if (item->text != NULL)
    {
        free(item->text);
    }
    if (item->bitmap != NULL)
    {
        if (item->bitmap->pdata != NULL)
        {
            free((void*)(item->bitmap->pdata));
        }
        free(item->bitmap);

    }
    if (item->info1 != NULL)
    {
        free(item->info1);
    }
    if (item->info2 != NULL)
    {
        free(item->info2);
    }
}

static void _clear_menu(menu_t* menu)
{
    int i;
    if (menu == NULL)
    {
        return;
    }
    for (i = 0; i < menu->total; i++)
    {
        _clear_menuitem(menu_get_item(menu, i));
    }
}

static void _locate_highlight_item(char* path_name)
{
    dir_level_t* p = s_filelist->data->content->p_cur_dir;
    int rc = fsel_getidbyname(s_filelist->data->content->p_fsel, path_name);

    /*
     *  We have to take 3 steps to position the current line:
     *  1. calculate the global_offset;
     *  2. call the content_update_all() which recalculate the correct itemgroup;
     *  3. position the right current line.
     */
    if (rc > p->item_group->total)
    {
        p->item_group->global_offset = rc - p->item_group->total;
        _content_update_all();
        p->item_group->current = p->item_group->total - 1;
    }
    else if (rc > 0)
    {
        p->item_group->global_offset = 0;
        _content_update_all();
        p->item_group->current = rc - 1;
    }
    else
    {
        p->item_group->global_offset = 0;
        _content_update_all();
        p->item_group->current = 0;
        print_err();
    }

    menuview_paint(s_filelist->data->content->content_list);
}
