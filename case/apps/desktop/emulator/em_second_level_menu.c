#include "emulator_scene.h"
#include "emulator_play.h"
#include "em_first_level_menu.h"
#include "em_second_level_menu.h"
#include "em_third_level_menu.h"


extern scene_emulator_t* g_scene_emulator;
extern char romFullPath[PATH_MAX];
extern char oldRomFile[PATH_MAX];
extern char saveFilePath[PATH_MAX];
extern emu_state_t emu_state;
extern int start_byFileNavi;
extern fusion_enter_e enter_type;

static int _continue_game(void);
static void _quit_game_app(void);
static int _item_restart_proc(menuview_t *list, menuview_sig_e signal);
static int _item_load_proc(menuview_t *list, menuview_sig_e signal);
static int _item_save_proc(menuview_t *list, menuview_sig_e signal);
static int _item_setting_proc(menuview_t *list, menuview_sig_e signal);
static int _item_quit_proc(menuview_t *list, menuview_sig_e signal);

static menu_t* _em_second_level_menu_init(void);
static bool _em_second_level_menu_deinit(void);
static void _back_to_dirlist(void);

//错误处理对话框:当memory=8M时，不可以玩MD和SFC
static void _err_info_dlg_cb(dialog_t *dlg, widget_sig_dialog_e signal);
static void _err_info_dlg(string_resource_t *str_res);
static void _get_file_ext_arry(char *pFileName, char *pExt);
static int _is_memory_equal_8M(void);

menu_t *secondLevelMenu = NULL;
menuitem_t second_level_items[5];
/* 第一次进入，不显示"保存进度"，从游戏退出到第二级菜单时才显示 */
int showSaveItem = 0;

/*
 ** 第二级菜单入口
 */
bool em_second_level_menu_enter(scene_emulator_t* my_scene)
{
    menu_t *cur_menu;
    fusion_effect_t effect;

    fusion_get_config(FUSION_TYPE_APP_CHANGE, enter_type, &effect); //  FUSION_TYPE_COMMON
    fusion_set_effect(&effect);

    scene_fusion_set_first_frame(g_scene_emulator->scene_common.scene);

    my_scene->cur_menu = 2;
    cur_menu = _em_second_level_menu_init();
    menuview_set_menu(my_scene->menuview, cur_menu);
    menuview_load_style(my_scene->menuview, MENUVIEW_MENULIST_TEXT);
    menuview_show(my_scene->menuview);
    menuview_paint(my_scene->menuview);

    scene_fusion_set_last_frame(g_scene_emulator->scene_common.scene);
    fusion_display( NULL);

    //usleep(200000); //200ms    
    gui_screen_update();

    /*如果模拟器进程没有创建，那么就创建它*/
    emu_init();
    //menuview_paint(my_scene->menuview);
    return true;
}

/*
 ** 退出第二级菜单
 */
bool em_second_level_menu_exit(void)
{
    g_scene_emulator->cur_item02 = secondLevelMenu->current;

    /* delete seleq menu */
    menuview_hide(g_scene_emulator->menuview);
    _em_second_level_menu_deinit();

    menuview_set_menu(g_scene_emulator->menuview, NULL); //将menuview的menu指针置空，避免野指针
    
    return true;
}

/*
 ** 初始化第二级菜单
 */
static menu_t* _em_second_level_menu_init(void)
{
    scene_t* scene;
    //menuitem_t item;
    int num;

    /* initial menu	*/
    if (secondLevelMenu == NULL)
    {
        /* get scene */
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            //print_err("cann't find app scene!");
            return false;
        }
        /* init items */
        memset(&second_level_items, 0, sizeof(second_level_items));

        second_level_items[0].text_id = (int)g_scene_emulator->restart_str->str_id;
        second_level_items[0].has_child = false;
        second_level_items[0].callback = (item_cb_t) _item_restart_proc;

        second_level_items[1].text_id = (int)g_scene_emulator->load_str->str_id;
        second_level_items[1].has_child = false;
        second_level_items[1].callback = (item_cb_t) _item_load_proc;

        /* 第一次进入，不显示"保存进度"，从游戏退出到第二级菜单时才显示 */
        if (showSaveItem == 1)
        {
            second_level_items[2].text_id = (int)g_scene_emulator->save_str->str_id;
            second_level_items[2].has_child = false;
            second_level_items[2].callback = (item_cb_t) _item_save_proc;
        }

        second_level_items[2 + showSaveItem].text_id = (int)g_scene_emulator->set_str->str_id;
        second_level_items[2 + showSaveItem].has_child = true;
        second_level_items[2 + showSaveItem].callback = (item_cb_t) _item_setting_proc;

        second_level_items[3 + showSaveItem].text_id = (int)g_scene_emulator->quit_str->str_id;
        second_level_items[3 + showSaveItem].has_child = false;
        second_level_items[3 + showSaveItem].callback = (item_cb_t) _item_quit_proc;

        /* create menu */
        secondLevelMenu = menu_create();
        if (secondLevelMenu == NULL)
        {
            //print_err("cann't create menu!");
            return false;
        }
        else
        {
            /* append items */
            for (num = 0; num < (4 + showSaveItem); num++)
            {
                menu_append_item(secondLevelMenu, &second_level_items[num]);
            }
        }

        secondLevelMenu->total = 4 + showSaveItem;
        secondLevelMenu->offset = 0;
        
        /* 如果是新游戏，焦点item是第一个（restart） */
        if ((strcmp(oldRomFile, romFullPath) != 0) && (emu_state == EMU_STAT_STOP))
        {
            g_scene_emulator->cur_item02 = 0;
        }
        
        menu_set_cur_index(secondLevelMenu, g_scene_emulator->cur_item02);
    }
    return secondLevelMenu;
}

/*
 ** 删除第二级菜单
 */
static bool _em_second_level_menu_deinit(void)
{
    /* delete menu */
    if (secondLevelMenu != NULL)
    {
        menu_delete(secondLevelMenu);
        secondLevelMenu = NULL;
    }

    return true;
}

/*
 ** 以下是第二级菜单的各个item 的回调函数
 */
static int _item_restart_proc(menuview_t *list, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
#if OMIT_SFC_MD > 0
        /*错误处理对话框:当memory=8M时，不可以玩MD和SFC*/
        if(1 == _is_memory_equal_8M())
        {
            return 0;	
        }
#endif
//        if(strcmp(romFullPath, "") == 0)
//        {     
//            _err_info_dlg(g_scene_emulator->failopen_str);   
//            return 0;            
//        }
       
        return_to_game_wiodows();
        emu_play(romFullPath);        
        break;
     
        case WIDGET_SIG_MENUVIEW_QUIT:
        /* 恢复刚才暂停的游戏 */
        if (_continue_game() == 1)
        {
            break;
        }
        
        _quit_game_app();
        break;

        default:
        break;
    }
    return 0;
}

static int _item_load_proc(menuview_t *list, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
#if OMIT_SFC_MD > 0        
        /*错误处理对话框:当memory=8M时，不可以玩MD和SFC*/
        if(1 == _is_memory_equal_8M())
        {
            return 0;	
        }
#endif

        /*读取保存的进度信息文件路径*/
        //get_config(CFG_EMU_SAVE_FILE_PATH, saveFilePath, PATH_MAX);

        if(strcmp(saveFilePath, "") == 0)
        {    
            _err_info_dlg(g_scene_emulator->failopen_str);   
            return 0;            
        }
               
        return_to_game_wiodows();
        emu_load();
        break;
        
        case WIDGET_SIG_MENUVIEW_QUIT:
        /* 恢复刚才暂停的游戏 */
        if (_continue_game() == 1)
        {
            break;
        }
        
        _quit_game_app();
        break;

        default:
        break;
    }
    return 0;
}


static void _has_saved_dlg_cb(dialog_t *dlg, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        case WIDGET_SIG_DIALOG_CANCAL:
        default:
        
        /* 删除对话框 */
        if (g_scene_emulator->dlg != NULL)
        {
            dialog_delete(g_scene_emulator->dlg);
            g_scene_emulator->dlg = NULL; 
        }
        break;
    }
}

static int _item_save_proc(menuview_t *list, menuview_sig_e signal)
{
    dialog_init_t dlg_init;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        if (g_scene_emulator->dlg == NULL)
        {
            if (emu_save() != EMU_ERR_OK)
            {
                print_err("save rom fail ! \n");
                dlg_init.str_id = g_scene_emulator->save_err_str->str_id;
                memset(saveFilePath, 0, PATH_MAX); 
            }
            else
            {
                //printf("save rom successfully ! \n");
                dlg_init.str_id = g_scene_emulator->saved_str->str_id;
                strcpy(saveFilePath, romFullPath);
                
                /*保存游戏进度信息到config，便于后续载入进度时读取*/
                //if (set_config(CFG_EMU_SAVE_FILE_PATH, saveFilePath, PATH_MAX) < 0)
                //{
                //    print_err("set_config(CFG_EMU_SAVE_FILE_PATH) failed!");
                //}     
            }
            
            /* 保存成功对话框 */
            dlg_init.head = DIALOG_HEAD_INFO;  
            g_scene_emulator->dlg = dialog_create(&dlg_init, _has_saved_dlg_cb);
            if (g_scene_emulator->dlg != NULL)
            {
                dialog_paint(g_scene_emulator->dlg);
            }
        }
        break;

        
        case WIDGET_SIG_MENUVIEW_QUIT:
        /* 恢复刚才暂停的游戏 */
        if (_continue_game() == 1)
        {
            break;
        }
        
        _quit_game_app();
        break;

        default:
        break;
    }
    return 0;
}

static int _item_setting_proc(menuview_t *list, menuview_sig_e signal)
{
    switch (signal)
    {
        /* 退出2级菜单,进入第三级菜单 */
        case WIDGET_SIG_MENUVIEW_OK:
        em_second_level_menu_exit();
        em_third_level_menu_enter(g_scene_emulator);
        break;
 
        case WIDGET_SIG_MENUVIEW_QUIT:
        /* 恢复刚才暂停的游戏 */
        if (_continue_game() == 1)
        {
            break;
        }
               
        _quit_game_app();
        break;

        default:
        break;
    }
    return 0;
}

static int _item_quit_proc(menuview_t *list, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /*退出模拟器*/
        _quit_game_app();
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        /* 恢复刚才暂停的游戏 */
        if (_continue_game() == 1)
        {
            break;
        }
                
        _quit_game_app();
        break;
        
        default:
        break;
    }
    return 0;
}

/*
 ** 恢复刚才暂停的游戏
 */
static int _continue_game(void)
{
    if (emu_state == EMU_STAT_PAUSE)
    {
        return_to_game_wiodows();
        emu_continue();
        return 1;
    }
    return 0;
}

/*
 ** 退出emulator应用
 */
static void _quit_game_app(void)
{  
    if (start_byFileNavi == 0)
    {
        _back_to_dirlist();
    }
    else
    {
        /*退出模拟器*/
        if (emu_exit() != 1)
        {
            //print_info("%s: %d: fail to exit simulator !\n", __FUNCTION__, __LINE__);
        }
        //sleep(1);
        _em_second_level_menu_deinit();
        emulator_scene_quit();
    }
}

/*
 ** 退回到第1级菜单
 */
static void _back_to_dirlist(void)
{
    em_second_level_menu_exit();
    em_dirlist_enter(g_scene_emulator, 1);
}


/*
** 错误处理对话框:当memory=8M时，不可以玩MD和SFC
*/
static void _err_info_dlg(string_resource_t *str_res)
{
    dialog_init_t dlg_init;

    dlg_init.head = DIALOG_HEAD_WARN;
    dlg_init.str_id = str_res->str_id;
    g_scene_emulator->dlg = dialog_create(&dlg_init, _err_info_dlg_cb);
    if (g_scene_emulator->dlg != NULL)
    {
        dialog_paint(g_scene_emulator->dlg);
    }
}

static void _err_info_dlg_cb(dialog_t *dlg, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        case WIDGET_SIG_DIALOG_CANCAL:
        default:
        
        /* 删除对话框 */
        if(g_scene_emulator->dlg != NULL)
        {
            dialog_delete(g_scene_emulator->dlg);
            g_scene_emulator->dlg = NULL; 
        }       
        break; 
    }
} 

/*
** 取得文件的后缀名
** 结果存放在函数传入的参数里
*/
static void _get_file_ext_arry(char *pFileName, char *pExt)
{
    int length = 0;
    int cnt = 0;
	
    if(pFileName == NULL )
    {
        *pExt = '\0';
        return ;
    }
	
    length = (int)strlen(pFileName);

    cnt = length - 1;
    while((pFileName[cnt] != '.') && (cnt > 0))
    {
        cnt--;
    }
	
    cnt++;
    for(; cnt<length; pExt++,cnt++)
    {
        if((pFileName[cnt] >= 65) && (pFileName[cnt] <= 90))
        {
            *pExt = pFileName[cnt] + 32; //如果是大写字母，就转化成小写字母
        }
        else
        {
            *pExt = pFileName[cnt];        	
        }
    }
    *pExt = '\0';
}



/*错误处理对话框:当memory=8M时，不可以玩MD和SFC*/
static int _is_memory_equal_8M(void)
{
    char kind[5];
    
    _get_file_ext_arry(romFullPath, kind);
    
    if (get_sdram_cap() == 8)
    {
        //如果是 MD和SFC 类型游戏 strcmp(kind,"smc|078|058")  smd|mdx|bin
        if (  (strcmp(kind,"smc")==0) || (strcmp(kind,"078")==0) || (strcmp(kind,"058")==0) 
           || (strcmp(kind,"smd")==0) || (strcmp(kind,"mdx")==0) || (strcmp(kind,"bin")==0)
        ) 
        {
            _err_info_dlg(g_scene_emulator->nosupp_str);
            return 1; 	
        } 
        else
        {
            return 0;	
        }          	
    }
    return 0;
}
