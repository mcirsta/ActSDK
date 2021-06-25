/********************************************************************************
 *                              USDK(1100)
 *                             Module: commonui
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>                <version >             <desc>
 *       binannan   2009-11-03                 1.0                     add comments.
 *******************************************************************************/
/*!
 * \file     dirlist.c
 * \brief    dirlist control widget implementation
 * \author   binannan
 *******************************************************************************/
#include "include_case.h"

#include "dirlist_private.h"
#include "commonui_sty.h"

/*********************************************************************/
static void _bringup_content(int idx);

static void _bringup_entry(entryop_e op);

static void _entry_callback(menuview_t* entry_list, menuview_sig_e sig);

static bool _entry_data_init(entry_list_t* entry);

static bool _entry_view_init(entry_list_t* entry);

static bool _entry_unload(entry_list_t* entry);

static bool _entry_reload(entry_list_t* entry);

static bool _entry_deinit(entry_list_t* entry);

static void _content_callback(menuview_t* content_list, menuview_sig_e sig);

static bool _content_data_init(content_list_t*content , int idx);

static bool _content_view_init(content_list_t* content);

static bool _content_unload(content_list_t* content);

static bool _content_view_unload(content_list_t* content);

static bool _content_deinit(content_list_t* content);

static bool _content_cd(char* dir_path);

static bool _content_cdup(void);

static bool _content_refresh(void);

static bool _content_update_all(void);

static void _content_populate(void);

static int _content_populate_dirs(int number);

static bool _content_populate_dir_down(void);

static bool _content_populate_dir_up(void);

static int _content_populate_files(int number);

static bool _content_populate_file_down(void);

static bool _content_populate_file_up(void);

/* static bool _content_clear_path(void);  */

static menuview_type_e _find_list_type(dirlist_type_e dirlist_type);

static const char* _find_root_pathname(dirlist_type_e dirlist_type);

static void _check_external_storage(dirlist_t* dirlist);

static bool _is_dir_exist(const char *dir_path);

static bool _is_file_exist(const char* file_path);

static bool _is_sub_dir(const char* parent, const char* sub);

static void _load_configs(void);

static void _clear_menuitem(menuitem_t* item);

static void _clear_menu(menu_t* menu);

static void _show_nofile(void);

static void _show_nodisk(void);

static void _com_noop_dialog_cb(dialog_t * dialog, widget_sig_dialog_e signal);

static void _locate_highlight_item(char* path_name);

/*********************************************************************/

static dirlist_t* s_dirlist = NULL;

static bool s_in_create = false;

static bool s_decode_sig_emit_once = false;

static int s_entry_idx = -1;

static bool s_is_search_subdir = false;

static fsel_sortstyle_t s_fsel_sort_style = FSEL_NOSORTING; /* 文件选择器的排序类型*/

/*当处于子目录搜索模式时，当前显示的文件的路径名需要临时获取，而不是使用当前所在的文件夹的路径名，
 * 这里指定一个全局的变量current_path_name来存放临时获取的某文件的路径名
 */
static char s_current_path_name[PATH_MAX];

static char s_current_file_name[MAXNAMLEN];

static entryitem_e s_entryitems[3] =
{ ENTRY_NONE, ENTRY_NONE, ENTRY_NONE };

static bool s_is_dirlist_visible = true;

static bool s_external_storage_plugout = false;

/*
 * 这里，设置一个全局变量来记录dirlist是否刚刚调用过搜索接口，这样在
 *dirlist_show里面就能区分普通的文件播放界面返回时调用和设置搜索条件后返回时调用
 */

static bool s_search_string_just_set = false;

static dialog_t* s_dialog_nodisk = NULL;

static int s_bitmap_width = 0;
static int s_bitmap_height = 0;

/*********************************************************************/

static void _bringup_content(int idx)
{
    s_entry_idx = idx;
    _entry_unload(s_dirlist->data->entry);
    _content_data_init(s_dirlist->data->content,idx);
    _content_view_init(s_dirlist->data->content);
    _content_cd(NULL);
    s_dirlist->data->list_inuse = CONTENT;
    dirlist_paint(s_dirlist);
}

static void _bringup_entry(entryop_e op)
{
    _entry_data_init(s_dirlist->data->entry);

    /*
     * bringup_entry only called from the CONTENT, so if
     * there are more than one disk alive, we just stay in the ENTRY.
     */
    if ((s_entryitems[0] == ENTRY_NONE) || (s_entryitems[1] != ENTRY_NONE))
    {
        _content_unload(s_dirlist->data->content);
        _entry_view_init(s_dirlist->data->entry);

        s_dirlist->data->list_inuse = ENTRY;
        dirlist_paint(s_dirlist);
    }
    else
    {
        if (op == ENTRYOP_CD)
        {
            if (s_entryitems[0] != ENTRY_NONE)
            {
                _content_unload(s_dirlist->data->content);
                _bringup_content(0);
            }
        }
        else if(op == ENTRYOP_CDUP)
        {
            s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_QUIT);
        }
        else
        {
            print_err();
        }
    }
}

static void _entry_callback(menuview_t* entry_list, menuview_sig_e sig)
{
    menu_t* menu = entry_list->menu;
    menuitem_t* item = menu_get_cur_item(menu);
    print_dbg("\noffset=%d, current=%d, total=%d, global_offset=%d, global_size=%d\n\n",
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
            s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_QUIT);
        }
        break;

        default:
        break;
    }
}

static bool _entry_data_init(entry_list_t* entry)
{
    if (entry->entry_menu == NULL)
    {
        int i;
        int idx = 0;

        menuitem_t  internal;
        hotplugin_dev_info_t main_info;

        menuitem_t card;
        hotplugin_dev_info_t card_info;

        menuitem_t external;
        hotplugin_dev_info_t external_info;


        for(i =0; i < 3; i++)
        {
            s_entryitems[i] = ENTRY_NONE;
        }

        entry->entry_menu = menu_create();
        if (entry->entry_menu == NULL)
        {
            print_err("create menu_t failed.\n");
            return false;
        }

        /*TODO: there are 3 entries in the entry_list, internal, external flash and card.
         * the external and card entries should be hidden when not available
         */

        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_LOCAL, &main_info);
        if (main_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            memset(&internal, 0, sizeof(menuitem_t));

            internal.has_child = true;
            internal.text_id = entry->str_intmem->str_id;

            menu_append_item(entry->entry_menu, &internal);

            s_entryitems[idx] = ENTRY_INTERNAL;
            idx ++;
        }
        else
        {

        }

        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
        if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            memset(&card,0,sizeof(menuitem_t));
            card.has_child = true;
            card.text_id = entry->str_carmem->str_id;

            menu_append_item(entry->entry_menu, &card);

            s_entryitems[idx] = ENTRY_CARD;
            idx ++;
        }
        else
        {
        }

        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
        if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            memset(&external,0,sizeof(menuitem_t));
            external.has_child = true;
            external.text_id = entry->str_extmem->str_id;

            menu_append_item(entry->entry_menu, &external);

            s_entryitems[idx] = ENTRY_EXTERNAL;
        }
        else
        {
        }
    }

    set_single_shot_timer(1, (timer_proc)_show_nodisk, NULL);

    return true;
}

static bool _entry_view_init(entry_list_t* entry)
{
    entry->entry_list_type = MENUVIEW_MENULIST_TEXT;

    entry->entry_list = menuview_create(entry->entry_list_type, _entry_callback);
    if (entry->entry_list == NULL)
    {
        print_err("create menuview_t failed.\n");
        return false;
    }
    s_dirlist->canvas_id = entry->entry_list->canvas_id;

    menuview_set_menu(entry->entry_list, entry->entry_menu);

    if(s_entry_idx != -1)
    {
        menu_set_cur_index(entry->entry_menu, s_entry_idx);
    }
    return true;
}

static bool _entry_unload(entry_list_t* entry)
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
    return true;
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

static void _content_callback(menuview_t* content_list, menuview_sig_e sig)
{
    menu_t* menu = content_list->menu;
    menuitem_t* item = menu_get_cur_item(menu);

    if ((item != NULL) && (item->text != NULL))
    {
        strcpy(s_current_file_name, item->text);
    }

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
            if (item != NULL)
            {
                if (item->has_child == true)
                {
                    if (item->text != NULL)
                    {
                        _content_cd(item->text);
                        dirlist_paint(s_dirlist);
                    }
                }
                else
                {
                    /*
                     * TODO: when in subdirectory search mode, we should get the path
                     * from the  fsel_getfullpath call.
                     */
                    if (s_dirlist->data->content->search_string != NULL)
                    {
                        int d = fsel_getfilenum(s_dirlist->data->content->p_fsel,FSEL_DIRNUM);
                        memset(s_current_path_name, 0, PATH_MAX);
                        fsel_getfullpath(s_dirlist->data->content->p_fsel, s_current_path_name,
                                d + menu->global_offset + menu->current + 1);
                    }

                    s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_OK);
                }
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            print_warning("WIDGET_SIG_MENUVIEW_QUIT: \n");
            _content_cdup();
        }
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        {
            print_warning("WIDGET_SIG_MENUVIEW_EXCEED_FIRST:\n");
            if (_content_populate_file_up() == false)
            {
                _content_populate_dir_up();
            }
            s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_DECODE);
        }
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        {
            print_warning("WIDGET_SIG_MENUVIEW_EXCEED_LAST:\n");
            if (_content_populate_dir_down() == false)
            {
                _content_populate_file_down();
            }
            s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_DECODE);
        }
        break;
        case WIDGET_SIG_MENUVIEW_UPDATE_ALL:
        {
            print_warning("WIDGET_SIG_MENUVIEW_UPDATE_ALL:\n");
            _content_update_all();
            dirlist_paint(s_dirlist);
            s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_DECODE);
        }
        break;
        case WIDGET_SIG_MENUVIEW_SHORT_MENU:
        {
            print_warning("WIDGET_SIG_MENUVIEW_SHORT_MENU:\n");
            s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_SHORT_MENU);
        }
        break;
        case WIDGET_SIG_MENUVIEW_LONG_MENU:
        {
            print_warning("WIDGET_SIG_MENUVIEW_LONG_MENU:\n");
            s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_LONG_MENU);
        }
        break;

        default:
        break;
    }
}

static bool _content_data_init(content_list_t* content,int idx)
{
    int result = 0;

    if (content->p_mnavi == NULL)
    {
        content->p_mnavi = dlopen("mnavi.so", RTLD_NOW);
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
            goto fsel_open_err;
        }
        fsel_subdir_setting(content->p_fsel, FSEL_NOSUB, FSEL_RESERVE);

        fsel_sortstyle(content->p_fsel, s_fsel_sort_style);

        if ((s_dirlist->filter_string != NULL) && (strcmp(s_dirlist->filter_string, "") != 0))
        {
            fsel_setfilter(content->p_fsel, s_dirlist->filter_string);
        }
    }

    switch (s_entryitems[idx])
    {
        case ENTRY_INTERNAL:
        {
            content->default_top_path_name = UDISK_ROOT"/";
        }
        break;
        case ENTRY_CARD:
        {
            content->default_top_path_name = CARD_ROOT"/";
        }
        break;
        case ENTRY_EXTERNAL:
        {
            content->default_top_path_name = EXTERNAL_ROOT"/";
        }
        break;

        default:
        {
            print_err();
        }
        break;
    }

    result = fsel_setworkdir(content->p_fsel, content->default_top_path_name);
    if (result != 0)
    {
        print_err("fsel_setworkdir failed. root_pathname = %s\n",content->default_top_path_name);
    }

    return true;

    fsel_open_err: dlclose(content->p_mnavi);
    mnavi_err: return false;
}

static bool _content_view_init(content_list_t* content)
{
    content->content_list_type = _find_list_type(s_dirlist->type);

    if (content->content_list == NULL)
    {
        content->content_list = menuview_create(content->content_list_type, _content_callback);
        if (content->content_list == NULL)
        {
            print_err();
            return false;
        }
        else
        {
            s_bitmap_width = menuview_get_bitmap_width(content->content_list);
            s_bitmap_height = menuview_get_bitmap_height(content->content_list);

            s_dirlist->canvas_id = content->content_list->canvas_id;
            if((content->p_cur_dir != NULL) && (content->p_cur_dir->item_group != NULL))
            {
                menuview_set_menu(content->content_list, content->p_cur_dir->item_group);
            }
            return true;
        }
    }
    return false;
}


static bool _content_unload(content_list_t* content)
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

    if (content->filter_string != NULL)
    {
        free(content->filter_string);
        content->filter_string = NULL;
    }

    if (content->search_string != NULL)
    {
        free(content->search_string);
        content->search_string = NULL;
    }

    content->p_root = NULL;
    content->p_cur_dir = NULL;

    _content_view_unload(content);

    return true;
}

static bool _content_view_unload(content_list_t* content)
{
    if (content->content_list != NULL)
    {
        menuview_delete(content->content_list);
        content->content_list = NULL;
    }
    return true;
}

static bool _content_deinit(content_list_t* content)
{
    _content_unload(content);

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
    return true;
}

static bool _content_cd(char* dir_path)
{
    content_list_t* content = s_dirlist->data->content;
    menuview_t* content_list = content->content_list;

    FSEL_HANDLE p_fsel = content->p_fsel;

    const char* cwd;

    dir_level_t* p = NULL;

    p = (dir_level_t*) malloc(sizeof(dir_level_t));
    if (p == NULL)
    {
        print_err("create dir_level_t failed.\n");
        return false;
    }
    memset(p,0,sizeof(dir_level_t));

    if (dir_path != NULL)
    {
        int ret = fsel_cd(p_fsel, dir_path);
        if(ret != 0  )
        {
            print_err(" cd to directory %s failed.",dir_path);
            free(p);
            return false;
        }
        /*
         * when change directory successfully, make the search invalid.
         */
        if (content->search_string != NULL)
        {
            dirlist_set_search_string(s_dirlist, "");
        }
    }

    cwd = fsel_getworkdir(p_fsel);
    if (cwd != NULL)
    {
        p->workdir = strdup(cwd);
        if (p->workdir != NULL)
        {
            strcpy(s_current_path_name, p->workdir);
        }
    }

    p->dirs = fsel_getfilenum(p_fsel, FSEL_DIRNUM);
    p->files = fsel_getfilenum(p_fsel, FSEL_FILENUM);

    p->item_group_size = menuview_get_lines(content_list);
    p->item_group = menu_create();

    menu_set_global_size(p->item_group, p->dirs + p->files);
    menu_set_global_offset(p->item_group, 0);

    /*
     * add p to the content's dir_level_t link list.
     */
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

    /*
     * bind the menuview and the menu
     */
    menuview_set_menu(content->content_list, content->p_cur_dir->item_group);

    _content_populate();

    set_single_shot_timer(1, (timer_proc)_show_nofile, NULL);

    /*
     * dirlist_create will call content_cd when client pass a init_pathname,
     * in the create process, we should not emit the WIDGET_SIG_DIRLIST_DECODE
     * because in this moment, client do not have the dirlist handle.
     */
    if (s_in_create == false)
    {
        s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_DECODE);
        s_decode_sig_emit_once = true;
    }
    return true;
}

static bool _content_cdup(void)
{
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;

    if (content->search_string != NULL)
    {
        dirlist_set_search_string(s_dirlist, "");
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
        fsel_cd(content->p_fsel, "..");
        s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_DECODE);
        dirlist_paint(s_dirlist);
    }
 /*
  *  the top_path_name restriction is turn off.
  *
    else if (s_dirlist->top_path_name != NULL)
    {
        bool in_top_sub_path = is_sub_dir(s_dirlist->top_path_name, p->workdir);
        if (p->item_group != NULL)
        {
            clear_menu(p->item_group);
            menu_delete(p->item_group);
        }
        if (p->workdir != NULL)
        {
            free(p->workdir);
        }
        free(p);
        content_handle->p_cur_dir = NULL;
        content_handle->p_root = NULL;
        if (in_top_sub_path == true)
        {
            content_cd("..");
            dirlist_paint(s_dirlist);
        }
        else
        {
            s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_QUIT);
        }
    }*/
    else if (_is_sub_dir(content->default_top_path_name, p->workdir) == true)
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
        dirlist_paint(s_dirlist);
    }
    else    /* content has reach the top directory: default_path_name*/
    {
        _bringup_entry(ENTRYOP_CDUP);
        return false;
    }
    return true;
}

/*!
 * 完整的刷新 content，将使用的menu的内部数据也清0
 * @return
 */
static bool _content_refresh(void)
{
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;
    FSEL_HANDLE p_fsel = content->p_fsel;

    _clear_menu(p->item_group);
    menu_remove_all_items(p->item_group);

    if (s_dirlist->data->content->search_string == NULL)
    {
        p->dirs = fsel_getfilenum(p_fsel, FSEL_DIRNUM);
    }
    else
    {
        p->dirs = 0;
    }

    p->files = fsel_getfilenum(p_fsel, FSEL_FILENUM);

    menu_set_global_size(p->item_group,  p->dirs + p->files);
    menu_set_global_offset(p->item_group, 0);

    _content_populate();
    return true;
}

/*!
 * 不清除menu的内部global数据，重新content。
 * 注意：content_update_all可能填入的数据可能起始于当前目录下项目的
 * 任意位置。 而content_refresh每次都是从开始处刷新
 * @return
 */
static bool _content_update_all(void)
{
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;
    int visible_dirs;
    int remain_items;

    _clear_menu(p->item_group);
    menu_remove_all_items(p->item_group);

    visible_dirs = p->dirs - p->item_group->global_offset;

    if (visible_dirs > 0)
    {
        p->dir_startno = p->item_group->global_offset + 1;
        p->dir_num = _content_populate_dirs(p->item_group_size);

        p->file_startno = 1;
        p->file_num = 0;

        remain_items = p->item_group_size - p->dir_num;
        if (remain_items > 0)
        {
            p->file_num = _content_populate_files(remain_items);
        }
    }
    else
    {
        p->dir_num = 0;
        p->dir_startno = p->dirs + 1;

        p->file_startno = (p->item_group->global_offset - p->dirs) + 1;
        p->file_num = _content_populate_files(p->item_group_size);
    }
    return true;
}

static void _content_populate(void)
{
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;
    int remain_items;

    p->dir_startno = 1;
    p->dir_num = 0;

    if (s_dirlist->data->content->search_string == NULL)
    {
        p->dir_num = _content_populate_dirs(p->item_group_size);
    }

    p->file_startno = 1;
    p->file_num = 0;

    remain_items = p->item_group_size - p->dir_num;
    if (remain_items > 0)
    {
        p->file_num = _content_populate_files(remain_items);
    }
}

static int _content_populate_dirs(int number)
{
    int result = 0;
    int visible_dirs;
    int j;

    fsel_filename_t* fsel_p = NULL;
    fsel_filename_t* fsel_tmp = NULL;
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;

    visible_dirs = (((p->dirs - p->dir_startno) + 1) > number) ? number : ((p->dirs - p->dir_startno) + 1);
    if (visible_dirs > 0)
    {
        fsel_p = (fsel_filename_t*) malloc(sizeof(fsel_filename_t) * (size_t)visible_dirs);
        if(fsel_p == NULL)
        {
            return 0;
        }
        fsel_tmp = fsel_p;
        result = fsel_listdir(content->p_fsel, fsel_p, p->dir_startno, visible_dirs, FSEL_POSITIVE);
    }
    for (j = 0; j < result; j++)
    {
        menuitem_t item;
        memset(&item, 0, sizeof(menuitem_t));

        item.text = strdup(fsel_tmp->name);
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
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;
    menuitem_t* p_item;
    int visible_dirs;

    if (p->dir_num == 0)
    {
        return false;
    }

    p_item = menu_get_item(p->item_group, 0);
    _clear_menuitem(p_item);
    menu_remove_first_item(p->item_group);

    visible_dirs = (p->dirs - (p->dir_startno - 1)) - p->dir_num;
    if (visible_dirs > 0)
    {
        fsel_filename_t fsel;
        menuitem_t item;

        fsel_listdir(content->p_fsel, &fsel, p->dir_startno + p->dir_num, 1, FSEL_POSITIVE);

        memset(&item, 0, sizeof(menuitem_t));

        item.text = strdup(fsel.name);

        item.has_child = true;

        menu_append_item(p->item_group, &item);

        p->dir_startno++;
        return true;
    }
    else
    {
        p->dir_startno++;
        p->dir_num--;
        return false;
    }
}

static bool _content_populate_dir_up(void)
{
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;

    fsel_filename_t fsel;
    menuitem_t item;

    fsel_listdir(content->p_fsel, &fsel, p->dir_startno - 1, 1, FSEL_POSITIVE);

    memset(&item, 0, sizeof(menuitem_t));

    item.text = strdup(fsel.name);
    item.has_child = true;

    menu_prepend_item(p->item_group, &item);

    if (p->dir_num < p->item_group_size)
    {
        p->dir_num++;
    }
    else
    {
        menuitem_t* i = menu_get_item(p->item_group, p->item_group->total - 1);
        _clear_menuitem(i);
        menu_remove_last_item(p->item_group);
    }
    p->dir_startno--;
    return true;
}

static int _content_populate_files(int number)
{
    int result = 0;
    int visible_files;
    int j;

    fsel_filename_t* fsel_p = NULL;
    fsel_filename_t* fsel_tmp = NULL;
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;

    visible_files = (((p->files - p->file_startno) + 1) > number) ? number : ((p->files - p->file_startno) + 1);
    if (visible_files > 0)
    {
        fsel_p = (fsel_filename_t*) malloc(sizeof(fsel_filename_t) * (size_t)visible_files);
        if(fsel_p == NULL)
        {
            return 0;
        }
        fsel_tmp = fsel_p;
        result = fsel_listfile(content->p_fsel, fsel_p, p->file_startno, visible_files, FSEL_POSITIVE);
    }

    for (j = 0; j < result; j++)
    {
        menuitem_t item;
        memset(&item, 0, sizeof(menuitem_t));

        item.text = strdup(fsel_tmp->name);
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
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;

    fsel_filename_t fsel;
    menuitem_t item;

    fsel_listfile(content->p_fsel, &fsel, p->file_startno + p->file_num, 1, FSEL_POSITIVE);

    memset(&item, 0, sizeof(menuitem_t));

    item.text = strdup(fsel.name);
    item.has_child = false;

    menu_append_item(p->item_group, &item);

    if (p->file_num < p->item_group_size)
    {
        p->file_num++;
    }
    else
    {
        menuitem_t* i = menu_get_item(p->item_group, 0);
        _clear_menuitem(i);
        menu_remove_first_item(p->item_group);
        p->file_startno++;
    }
    return true;
}

static bool _content_populate_file_up(void)
{
    content_list_t* content = s_dirlist->data->content;
    dir_level_t* p = content->p_cur_dir;
    menuitem_t* p_item;

    if (p->file_num == 0)
    {
        return false;
    }

    p_item = menu_get_item(p->item_group, p->item_group->total - 1);
    _clear_menuitem(p_item);
    menu_remove_last_item(p->item_group);

    if (p->file_startno > 1)
    {
        fsel_filename_t fsel;
        menuitem_t item;

        fsel_listfile(content->p_fsel, &fsel, p->file_startno - 1, 1, FSEL_POSITIVE);

        memset(&item, 0, sizeof(menuitem_t));

        item.text = strdup(fsel.name);
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

/*
 * 清理当前目录之上的所有父目录。
 * 在如下两种情况下需要：
 * A. 减少dirlist的内存占用到最少；
 * B. dirlist的父目录可能发生变化时。
 *
static bool _content_clear_path(void)
{
    content_list_t* content = s_dirlist->data->content;

    dir_level_t* p;
    dir_level_t* p_parent;

    p = content->p_cur_dir->parent;

    for (; p != NULL; p = p_parent)
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

    content->p_cur_dir->parent = NULL;
}
*/

static menuview_type_e _find_list_type(dirlist_type_e dirlist_type)
{
    menuview_type_e t;
    switch (dirlist_type)
    {
        case DIRLIST_MUSIC:
        {
            t = MENUVIEW_PICLIST_MUSIC;
        }
        break;
        case DIRLIST_VIDEO:
        {
            t = MENUVIEW_PICLIST_VIDEO;
        }
        break;
        case DIRLIST_PHOTO:
        {
            t = MENUVIEW_PICLIST_PICTURE;
        }
        break;
        case DIRLIST_EBOOK:
        {
            t = MENUVIEW_PICLIST_EBOOK;
        }
        break;
        case DIRLIST_DEFAULT:
        {
            t = MENUVIEW_FILELIST_TEXT;
        }
        break;
        default:
        {
            t = MENUVIEW_FILELIST_TEXT;
        }
        break;
    }
    return t;
}

static const char* _find_root_pathname(dirlist_type_e dirlist_type)
{
    const char* path = NULL;
    switch (dirlist_type)
    {
        case DIRLIST_MUSIC:
        {
            path = sys_get_main_disk_dir(DIR_MUSIC);
        }
        break;
        case DIRLIST_VIDEO:
        {
            path =  sys_get_main_disk_dir(DIR_VIDEO);
        }
        break;
        case DIRLIST_PHOTO:
        {
            path =  sys_get_main_disk_dir(DIR_PHOTO);
        }
        break;
        case DIRLIST_EBOOK:
        {
            path =  sys_get_main_disk_dir(DIR_EBOOK);
        }
        break;
        case DIRLIST_GAME:
        {
            path =  sys_get_main_disk_dir(DIR_GAME);
        }
        break;
        case DIRLIST_DEFAULT:
        {
            path =  sys_get_main_disk_dir(DIR_ROOT);
        }
        break;
        default:
        {
            path =  sys_get_main_disk_dir(DIR_ROOT);
        }
    }

    if(path == NULL)
    {
        path =  sys_get_main_disk_dir(DIR_ROOT);
    }
    return path;
}

static void _check_external_storage(dirlist_t* dirlist)
{
    if (dirlist == NULL)
    {
        return;
    }

    if ((dirlist->data->list_inuse == CONTENT) && (s_entryitems[s_entry_idx] == ENTRY_CARD))
    {
        hotplugin_dev_info_t card_info;
        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
        if (card_info.access != HOTPLUGIN_DEV_ACCESS_OK)
        {
            s_external_storage_plugout = true;
        }
    }
    else if ((dirlist->data->list_inuse == CONTENT) && (s_entryitems[s_entry_idx] == ENTRY_EXTERNAL))
    {
        hotplugin_dev_info_t external_info;
        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
        if (external_info.access != HOTPLUGIN_DEV_ACCESS_OK)
        {
            s_external_storage_plugout = true;
        }
    }
    else
    {
        s_external_storage_plugout = false;
    }
}

static bool _is_dir_exist(const char *dir_path)
{
    int ret = -1;
    struct stat s;
    ret = stat(dir_path, &s);
    if(ret == 0)
    {
        return S_ISDIR(s.st_mode);
    }
    else
    {
        return false;
    }
}

static bool _is_file_exist(const char* file_path)
{
    int ret = -1;
    struct stat s;
    ret = stat(file_path, &s);
    if(ret == 0)
    {
        return S_ISREG(s.st_mode);
    }
    else
    {
        return false;
    }
}

static bool _is_sub_dir(const char* parent, const char* sub)
{
    bool ret;
    char rel_parent[PATH_MAX];
    char rel_sub[PATH_MAX];
    size_t parent_len;
    size_t rel_parent_len;
    size_t sub_len;
    size_t rel_sub_len;

    if ((parent == NULL) || (sub == NULL))
    {
        return false;
    }

    if ((parent[0] != '/') || (sub[0] != '/'))
    {
        return false;
    }

    memset(rel_parent, 0, PATH_MAX);
    memset(rel_sub, 0, PATH_MAX);

    strncpy(rel_parent, parent, PATH_MAX);
    parent_len = strlen(parent);
    rel_parent_len = parent_len;

    strncpy(rel_sub, sub, PATH_MAX);
    sub_len = strlen(sub);
    rel_sub_len = sub_len;

    while (rel_parent[rel_parent_len - 1] == '/')
    {
        rel_parent[rel_parent_len - 1] = '\0';
        rel_parent_len--;
    }

    while (rel_sub[rel_sub_len - 1] == '/')
    {
        rel_sub[rel_sub_len - 1] = '\0';
        rel_sub_len--;
    }

    if (strncmp(rel_parent, rel_sub, rel_parent_len) != 0)
    {
        ret = false;
    }
    else
    {
        if (rel_sub[rel_parent_len] == '/')
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    return ret;
}

static void _load_configs(void)
{
    char text[CFG_MAX_BUFF_LENGTH];
    int ret;
    memset(text, 0, CFG_MAX_BUFF_LENGTH);
    ret = get_config(CFG_SEARCH_FOLDER, text, CFG_MAX_BUFF_LENGTH);
    if ((ret != -1) && (strcmp(text, "on") == 0))
    {
        s_is_search_subdir = true;
    }
    else
    {
        s_is_search_subdir = false;
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
            free(item->bitmap->pdata);
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


static void _show_nofile(void)
{
    if ((s_dirlist->data->content->content_list->menu->total) == 0)
    {
        dialog_init_t init;
        init.head = DIALOG_HEAD_WARN;
        init.str_id = s_dirlist->data->content->str_nofile->str_id;
        dialog_modal(&init);
    }
}

static void _show_nodisk(void)
{
    if (s_entryitems[0] == ENTRY_NONE)
    {
        dialog_init_t init;
        init.head = DIALOG_HEAD_WARN;
        init.str_id = s_dirlist->data->entry->str_nodisk->str_id;
        if(s_dialog_nodisk == NULL)
        {
            s_dialog_nodisk = dialog_create(&init, _com_noop_dialog_cb);
        }
    }
}

static void _com_noop_dialog_cb(dialog_t * dialog, widget_sig_dialog_e signal)
{
    print_dbg("signal:%d", signal);
    if(s_dialog_nodisk != NULL)
    {
        dialog_delete(s_dialog_nodisk);
        s_dialog_nodisk = NULL;
    }
}

static void _locate_highlight_item(char* path_name)
{
    dir_level_t* p = s_dirlist->data->content->p_cur_dir;
    int rc = fsel_getidbyname(s_dirlist->data->content->p_fsel, path_name);

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
}

dirlist_t* dirlist_create(dirlist_init_t* init, dirlist_cb_t callback)
{
    dirlist_t* dl;

    bool b_entry_data_init = false;
    bool b_entry_view_init = false;
    bool b_content_data_init = false;
    bool b_content_view_init = false;

    char* init_path = NULL;

    if (s_dirlist != NULL)
    {
        return s_dirlist;
    }

    /*
     * malloc internal struct dirlist_private_data_s variable.
     */
    dl = (dirlist_t*) malloc(sizeof(dirlist_t));
    if (dl == NULL)
    {
        print_err("malloc dirlist_t failed.\n");
        return NULL;
    }
    memset(dl, 0, sizeof(dirlist_t));

    dl->data = (dirlist_private_data_t*) malloc(sizeof(dirlist_private_data_t));
    if (dl->data == NULL)
    {
        print_err("malloc dirlist_private_data_t failed.\n");
        goto dl_err;
    }
    memset(dl->data, 0, sizeof(dirlist_private_data_t));

    dl->data->entry = (entry_list_t*) malloc(sizeof(entry_list_t));
    if (dl->data->entry == NULL)
    {
        print_err("malloc entry_list_t failed.\n");
        goto dl_err;
    }
    memset(dl->data->entry, 0, sizeof(entry_list_t));

    dl->data->content = (content_list_t*) malloc(sizeof(content_list_t));
    if (dl->data->content == NULL)
    {
        print_err("malloc content_list_t failed.\n");
        goto dl_err;
    }
    memset(dl->data->content, 0, sizeof(content_list_t));

    dl->data->list_inuse = NOTSET;

    s_dirlist = dl;

    /*
     * Initialize internal variables
     */
    dl->callback = callback;
    dl->canvas_id = 0;
    dl->type = init->type;
    dl->path_name = s_current_path_name;
    dl->file_name = s_current_file_name;

    /*
     * init_path_name determines what's the first directory the dirlist will reside in.
     */
    if (init->init_path_name != NULL)
    {
        /*
         *  If the init->init_path_name is a file's full pathname, get the path name part through
         *  the dirname(), save the path name to init_path.
         *  If the init->init_path_name is a directory's full pathname, just save it to
         *  dl->init_path_name and leave the init_path NULL.
         */
        bool is_file = _is_file_exist(init->init_path_name);
        if (is_file == true)
        {
            dl->init_path_name = strdup(init->init_path_name);
            init_path = strdup(init->init_path_name);
            init_path = dirname(init_path);
        }
        else
        {
            bool is_dir = _is_dir_exist(init->init_path_name);
            if (is_dir == true)
            {
                dl->init_path_name = strdup(init->init_path_name);
            }
        }
    }

    /*
     *  top_path_name determine what's the top directory the dirlist can reach.
     */
    if (init->top_path_name != NULL)
    {
        bool is_dir = _is_dir_exist(init->top_path_name);
        if (is_dir == true)
        {
            dl->top_path_name = strdup(init->top_path_name);
        }
    }

    if ((init->filter_string != NULL) && (strlen(init->filter_string) != 0))
    {
        dl->filter_string = strdup(init->filter_string);
    }

    /*
     * load style.
     */
    if (dirlist_load_style(dl) == false)
    {
        print_err("load style failed.\n");
    }

    /*
     * load configs.
     */
    _load_configs();

     /*
      * first we enter the entry list.
      */
    b_entry_data_init = _entry_data_init(dl->data->entry);
    b_entry_view_init = _entry_view_init(dl->data->entry);

    if ((b_entry_data_init == false) || (b_entry_view_init == false))
    {
        print_err("init entry failed.\n");
        goto dl_err;
    }
    dl->data->list_inuse = ENTRY;


    /*
     * if there are only one disk in entry list, we in turn jump into the deeper directory.
     */
    if (((s_entryitems[0] != ENTRY_NONE) && (s_entryitems[1] == ENTRY_NONE)) || ( dl->init_path_name !=NULL))
    {
        FSEL_HANDLE fsel_handle = NULL;

        _entry_unload(dl->data->entry);

        if (dl->init_path_name == NULL)
        {
            const char* root_pathname = _find_root_pathname(dl->type);
            if(root_pathname != NULL)
            {
                dl->init_path_name = strdup(root_pathname);
            }
        }

        s_entry_idx = 0;
        b_content_data_init = _content_data_init(dl->data->content,s_entry_idx);
        b_content_view_init = _content_view_init(dl->data->content);

        if ((b_content_data_init == false) ||( b_content_view_init == false))
        {
            print_err("init content failed.\n");
            goto dl_err;
        }

        fsel_handle = dl->data->content->p_fsel;
        if(dl->init_path_name != NULL)
        {
            if (init_path == NULL)
            {
                fsel_setworkdir(fsel_handle, dl->init_path_name);
            }
            else
            {
                fsel_setworkdir(fsel_handle, init_path);
            }
        }

        /*
         * put in_create to true to avoid unnecessary signal WIDGET_SIG_DIRLIST_DECODE
         * emit in following content_cd() call.
         */
        s_in_create = true;
        _content_cd(NULL);

        if (init_path != NULL)
        {
            _locate_highlight_item(dl->init_path_name);
            free(init_path);
        }

        dl->data->list_inuse = CONTENT;
    }
    s_in_create = false;

    return dl;

    dl_err: if (dl->data != NULL)
    {
        if (dl->data->content != NULL)
        {
            free(dl->data->content);
        }
        if (dl->data->entry != NULL)
        {
            free(dl->data->entry);
        }
        free(dl->data);
    }
    if (dl->filter_string != NULL)
    {
        free(dl->filter_string);
    }
    if (dl->top_path_name != NULL)
    {
        free(dl->top_path_name);
    }
    if (dl->init_path_name != NULL)
    {
        free(dl->init_path_name);
    }
    free(dl);
    return NULL;
}

bool dirlist_delete(dirlist_t* dirlist)
{
    if (dirlist == NULL)
    {
        return false;
    }

    if (dirlist->data != NULL)
    {
        if (dirlist->data->entry != NULL)
        {
            _entry_deinit(dirlist->data->entry);
            free(dirlist->data->entry);
            dirlist->data->entry = NULL;
        }

        if (dirlist->data->content != NULL)
        {
            _content_deinit(dirlist->data->content);
            free(dirlist->data->content);
            dirlist->data->content = NULL;
        }

        free(dirlist->data);
        dirlist->data = NULL;
    }
    if(dirlist->init_path_name != NULL)
    {
        free(dirlist->init_path_name);
        dirlist->init_path_name = NULL;
    }
    
    if(dirlist->top_path_name != NULL)
    {
        free(dirlist->top_path_name);
        dirlist->top_path_name = NULL;
    }
    
    if(dirlist->filter_string != NULL)
    {
        free(dirlist->filter_string);
        dirlist->filter_string = NULL;
    }

    free(dirlist);
    dirlist = NULL;
    s_dirlist = NULL;
    return true;
}

bool dirlist_paint(dirlist_t* dirlist)
{
    if (dirlist == NULL)
    {
        return false;
    }

    switch (dirlist->data->list_inuse)
    {
        case ENTRY:
        {
            menuview_paint(dirlist->data->entry->entry_list);
        }
        break;
        case CONTENT:
        {
            if (s_decode_sig_emit_once == false)
            {
                s_decode_sig_emit_once = true;
                s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_DECODE);
            }
            menuview_paint(dirlist->data->content->content_list);
        }
        break;

        default:
        break;
    }
    return true;
}

bool dirlist_proc_gui(dirlist_t* dirlist, gui_wm_message_t* pmsg)
{
    if (dirlist == NULL)
    {
        return false;
    }

    switch (dirlist->data->list_inuse)
    {
        case ENTRY:
        {
            menuview_proc_gui(dirlist->data->entry->entry_list, pmsg);
        }
        break;
        case CONTENT:
        {
            menuview_proc_gui(dirlist->data->content->content_list, pmsg);
        }
        break;

        default:
        break;
    }
    return true;
}

bool dirlist_proc_sys(dirlist_t* dirlist, msg_apps_t *pmsg)
{
    if (dirlist == NULL)
    {
        return false;
    }

    print_dbg("pmsg->type = %d",pmsg->type);
    switch (pmsg->type)
    {
        case MSG_CARD_IN:
        case MSG_CARD_OUT:
        {
            if ((dirlist->data->list_inuse == CONTENT) && (s_entryitems[s_entry_idx] == ENTRY_CARD))
            {
                if (s_is_dirlist_visible == true)
                {
                    _bringup_entry(ENTRYOP_CD);
                    break;
                }
                else
                {
                    s_external_storage_plugout = true;
                }
            }

            if (dirlist->data->list_inuse == ENTRY)
            {
                _entry_reload(dirlist->data->entry);
                dirlist_paint(dirlist);
            }
        }
        break;
        case MSG_USB_HOST_DISK_IN:
        case MSG_USB_HOST_DISK_OUT:
        {
            if ((dirlist->data->list_inuse == CONTENT) && (s_entryitems[s_entry_idx] == ENTRY_EXTERNAL))
            {
                if (s_is_dirlist_visible == true)
                {
                    _bringup_entry(ENTRYOP_CD);
                    break;
                }
                else
                {
                    s_external_storage_plugout = true;
                }
            }

            if (dirlist->data->list_inuse == ENTRY)
            {
                _entry_reload(dirlist->data->entry);
                dirlist_paint(dirlist);
            }
        }
        break;
        default:
        break;
    }
    return true;
}

bool dirlist_load_style(dirlist_t* dirlist)
{
    scene_t* scene;
    resgroup_resource_t* group;
    entry_list_t* entry;
    content_list_t* content;

    if (dirlist == NULL)
    {
        return false;
    }

    scene = sty_get_common_scene();
    if (scene == NULL)
    {
        print_err("call sty_get_common_scene failed.\n");
        return false;
    }

    group = (resgroup_resource_t*) get_scene_child(scene, DIRLIST_RESGROUP);

    entry = dirlist->data->entry;

    entry->str_intmem = (string_resource_t*) get_resgroup_child(group, STR_INTMEM);
    entry->str_extmem = (string_resource_t*) get_resgroup_child(group, STR_EXTMEM);
    entry->str_carmem = (string_resource_t*) get_resgroup_child(group, STR_CARMEM);
    entry->str_nodisk = (string_resource_t*) get_resgroup_child(group, STR_NODISK);

    content = dirlist->data->content;
    content->str_nofile = (string_resource_t*) get_resgroup_child(group, STR_NOFILE);

    switch (dirlist->data->list_inuse)
    {
        case ENTRY:
        {
            if (entry->entry_list != NULL)
            {
                menuview_load_style(entry->entry_list, entry->entry_list->type);
            }
        }
        break;

        case CONTENT:
        {
            if (content->content_list != NULL)
            {
                menuview_load_style(content->content_list,content->content_list->type);
                content->p_cur_dir->item_group_size = menuview_get_lines(content->content_list);
                _content_refresh();
                s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_DECODE);
            }
        }
        break;

        default:
        break;
    }
    return true;
}

int dirlist_get_bitmap_width(dirlist_t* dirlist)
{
    if(dirlist == NULL)
    {
        return  0;
    }
    return s_bitmap_width;
}
int dirlist_get_bitmap_height(dirlist_t* dirlist)
{
    if(dirlist == NULL)
    {
        return  0;
    }

    return s_bitmap_height;
}

gui_bitmap_info_t* dirlist_get_item_bitmap(dirlist_t* dirlist, int idx)
{
    content_list_t* content = dirlist->data->content;
    if ((content != NULL) && (content->p_cur_dir != NULL) && (content->p_cur_dir->item_group != NULL))
    {
        menuitem_t* i = menu_get_item(content->p_cur_dir->item_group, idx);
        if (i != NULL)
        {
            return i->bitmap;
        }
    }
    return NULL;
}

bool dirlist_set_item_bitmap(dirlist_t* dirlist, int idx, gui_bitmap_info_t* bitmap)
{
    content_list_t* content = dirlist->data->content;
    gui_bitmap_info_t* tmp = NULL;
    unsigned int pdata_size = 0;
    if (bitmap == NULL)
    {
        return false;
    }
    if ((content != NULL) && (content->p_cur_dir != NULL) && (content->p_cur_dir->item_group != NULL))
    {
        menuitem_t* i = menu_get_item(content->p_cur_dir->item_group, idx);
        if (i != NULL)
        {
            tmp = (gui_bitmap_info_t*) malloc(sizeof(gui_bitmap_info_t));
            if (tmp == NULL)
            {
                return false;
            }
            memcpy(tmp, bitmap, sizeof(gui_bitmap_info_t));
            pdata_size = tmp->xsize * tmp->ysize * tmp->bytesperpixel;
            tmp->pdata = (const unsigned char *) malloc(pdata_size);
            if (tmp->pdata == NULL)
            {
                free(tmp);
                return false;
            }
            memcpy((void*) tmp->pdata, (const void*) bitmap->pdata, pdata_size);
            i->bitmap = tmp;
            return true;
        }
    }
    return false;
}

bool dirlist_paint_bitmap(dirlist_t* dirlist, int idx)
{
    content_list_t* content = NULL;

    if (dirlist == NULL)
    {
        return false;
    }

    content = dirlist->data->content;

    if ((content != NULL) && (content->content_list != NULL))
    {
        menuview_paint_bitmap(content->content_list, idx);
        return true;
    }
    return false;
}

int dirlist_get_offset(dirlist_t *dirlist)
{
    content_list_t* content = NULL;
    if (dirlist == NULL)
    {
        return false;
    }

    content = dirlist->data->content;
    if ((content != NULL) && (content->p_cur_dir != NULL))
    {
        menu_t* menu = content->p_cur_dir->item_group;
        if (menu != NULL)
        {
            return menu->offset;
        }
    }
    return -1;
}

int dirlist_get_current(dirlist_t *dirlist)
{
    content_list_t* content = NULL;
    if (dirlist == NULL)
    {
        return false;
    }

    content = dirlist->data->content;
    if ((content != NULL) && (content->p_cur_dir != NULL))
    {
        menu_t* menu = content->p_cur_dir->item_group;
        if (menu != NULL)
        {
            return menu->current;
        }
    }
    return -1;
}

char* dirlist_get_item_name(dirlist_t* dirlist, int idx)
{
    content_list_t* content = NULL;
    if (dirlist == NULL)
    {
        return false;
    }

    content = dirlist->data->content;

    if ((content != NULL) && (content->p_cur_dir != NULL))
    {
        menu_t* menu = content->p_cur_dir->item_group;

        if (menu != NULL)
        {
            menuitem_t* item = NULL;

            if (content->search_string != NULL)
            {
                int d = fsel_getfilenum(content->p_fsel, FSEL_DIRNUM);
                memset(s_current_path_name, 0, PATH_MAX);
                fsel_getfullpath(content->p_fsel, s_current_path_name, d + menu->global_offset + idx + 1);
                //dirlist->path_name = current_path_name;
            }

            item = menu_get_item(menu, idx);
            if (item != NULL)
            {
                return item->text;
            }
        }
    }
    return NULL;
}

bool dirlist_is_item_dir(dirlist_t* dirlist, int idx)
{
    content_list_t* content = NULL;

    if (dirlist == NULL)
    {
        return false;
    }

    content = dirlist->data->content;
    if ((content != NULL) && (content->p_cur_dir != NULL))
    {
        menu_t* menu = content->p_cur_dir->item_group;
        if (menu != NULL)
        {
            menuitem_t* item = menu_get_item(menu, idx);
            if (item != NULL)
            {
                if (item->has_child == true)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return false;
}

int dirlist_get_total(dirlist_t* dirlist)
{
    content_list_t* content = NULL;
    if (dirlist == NULL)
    {
        return false;
    }

    content = dirlist->data->content;

    if ((content != NULL) && (content->p_cur_dir != NULL))
    {
        menu_t* menu = content->p_cur_dir->item_group;
        if (menu != NULL)
        {
            return menu->total;
        }
    }
    return -1;
}

int dirlist_get_global_size(dirlist_t *dirlist)
{
    content_list_t* content = NULL;

    if (dirlist == NULL)
    {
        return false;
    }

    content = dirlist->data->content;
    if ((content != NULL) && (content->p_cur_dir != NULL))
    {
        menu_t* menu = content->p_cur_dir->item_group;
        if (menu != NULL)
        {
            return menu->global_size;
        }
    }
    return -1;
}

int dirlist_get_global_offset(dirlist_t *dirlist)
{
    content_list_t* content = NULL;

    if (dirlist == NULL)
    {
        return false;
    }

    content = dirlist->data->content;
    if ((content != NULL) && (content->p_cur_dir != NULL))
    {
        menu_t* menu = content->p_cur_dir->item_group;
        if (menu != NULL)
        {
            return menu->global_offset;
        }
    }
    return -1;
}

bool dirlist_show(dirlist_t *dirlist)
{
    if (dirlist == NULL)
    {
        return false;
    }

    switch (dirlist->data->list_inuse)
    {
        case ENTRY:
        {
            //menuview_show(dirlist->data->entry->entry_list);
            _entry_reload(dirlist->data->entry);
        }
        break;
        case CONTENT:
        {
            /*
             * 首先检查外部存储设备是否拔出，且目前恰好dirlist的工作目录为
             * 此存储器中
             */
            _check_external_storage(dirlist);
            if (s_external_storage_plugout == true)
            {
                s_external_storage_plugout = false;
                _bringup_entry(ENTRYOP_CD);
                break;
            }

            _content_view_init(dirlist->data->content);

            /*
             * 如果是刚刚执行了搜索操作，则此时不需做任何动作
             */
            if (s_search_string_just_set == true)
            {
                s_search_string_just_set = false;
                dirlist->callback(dirlist, WIDGET_SIG_DIRLIST_DECODE);
                break;
            }
            else
            {
                /*
                 * XXX: 这里调用fsel_setworkdir的原因是，
                 * 当mnavi在执行fsel_getfullpath时，如果子目录搜索有效，则
                 * fsel内部的当前目录已经切换到了最后一次调用fsel_getfullpath所取得的目录。
                 * 这里需要重新设置一次当前目录，以确保后续的取数据的目录正确。
                 */
                fsel_setworkdir(s_dirlist->data->content->p_fsel, dirlist->data->content->p_cur_dir->workdir);
                strcpy(s_current_path_name, dirlist->data->content->p_cur_dir->workdir);
            }

            /*
             * 正常的show，一般情况下发生在文件播放界面返回时
             */
            if (dirlist->data->content->search_string != NULL)
            {
                /*
                 * 如果hide时，是搜索结果界面，则再次show时，清除搜索结果界面
                 */
                dirlist_set_search_string(s_dirlist, "");
                dirlist->callback(dirlist, WIDGET_SIG_DIRLIST_DECODE);
            }
            else
            {
                /*
                 * 如果是非搜索结果界面，则需重新更新一下当前显示条目，因为可能会出现文件删除情况
                 */
                char item_name[PATH_MAX];

                memset(item_name, 0, PATH_MAX);
                strcpy(item_name, s_current_path_name);
                strcat(item_name, dirlist->file_name);

                _content_refresh();

                if (_is_file_exist(item_name) == true)
                {
                    /*  如果文件存在，则需要定位到那个文件 */
                    _locate_highlight_item(item_name);
                }
                s_dirlist->callback(s_dirlist, WIDGET_SIG_DIRLIST_DECODE);
            }
        }
        break;

        default:
        break;
    }
    s_is_dirlist_visible = true;
    return true;
}

bool dirlist_hide(dirlist_t *dirlist)
{
    if (dirlist != NULL)
    {
        switch (dirlist->data->list_inuse)
        {
            case ENTRY:
            {
                //menuview_hide(dirlist->data->entry->entry_list);
                _entry_unload(dirlist->data->entry);
            }
            break;
            case CONTENT:
            {
                //menuview_hide(dirlist->data->content->content_list);
                _content_view_unload(dirlist->data->content);

                if (s_search_string_just_set == true)
                {
                    s_search_string_just_set = false;
                }
            }
            break;

            default:
            break;
        }
        s_is_dirlist_visible = false;
        return true;
    }
    else
    {
        return false;
    }
}

int dirlist_set_search_string(dirlist_t* dirlist, char* search_string)
{
    content_list_t* content;
    char* p = search_string;
    char* q = search_string;
    size_t str_len = 0;

    if (dirlist == NULL)
    {
        return -1;
    }

    content = dirlist->data->content;

    if (content->search_string != NULL)
    {
        free(content->search_string);
        content->search_string = NULL;
    }

    if((search_string != NULL) && (strlen(search_string) > 0))
    {
        /*
         * first trimming the leading & trailing blanks
         */
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
            content->search_string = (char*) malloc(str_len + 1);
            if(content->search_string != NULL)
            {
                memset(content->search_string, 0, str_len + 1);
                strncpy(content->search_string, "*", 1);
                strncat(content->search_string, p, (size_t) (q - p + 1));
                strncat(content->search_string, "*", 1);

                if (s_is_search_subdir == true)
                {
                    fsel_subdir_setting(content->p_fsel, FSEL_SUB, FSEL_RESERVE);
                }
                else
                {
                    fsel_subdir_setting(content->p_fsel, FSEL_NOSUB, FSEL_RESERVE);
                }

                fsel_set_query(content->p_fsel, content->search_string, FSEL_QUERY);

                s_search_string_just_set = true;
            }
        }
    }
    else
    {
        fsel_subdir_setting(content->p_fsel, FSEL_NOSUB, FSEL_RESERVE);

        fsel_set_query(content->p_fsel, content->search_string, FSEL_CANCEL);
    }

    _content_refresh();

    return (content->p_cur_dir->dirs + content->p_cur_dir->files);

}

FSEL_HANDLE dirlist_get_fsel_handle(dirlist_t* dirlist)
{
    if ((dirlist != NULL) && (dirlist->data != NULL) && (dirlist->data->content->p_fsel != NULL))
    {
        return dirlist->data->content->p_fsel;
    }
    else
    {
        return NULL;
    }
}
