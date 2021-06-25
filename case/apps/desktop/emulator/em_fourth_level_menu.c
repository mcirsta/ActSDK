/*
 **      此文件用于更改keymap
 ** 把获取的按键值存入emu_cfg_info，然后调用emu_set_keymap()给模拟器发命令即可
 */
#include "emulator_scene.h"
#include "emulator_play.h"
#include "em_third_level_menu.h"
#include "em_fourth_level_menu.h"


extern scene_emulator_t* g_scene_emulator;
extern emu_config_info_t emu_cfg_info;
extern fusion_enter_e enter_type;


static void _get_key_name_from_keymap(int value, char* keyName);
static void _get_key_name_from_emu_tab(int id, char* keyName);
static bool _em_fourth_level_menu_exit(void);
static menu_t* _em_fourth_level_menu_init(void);
static bool _em_fourth_level_menu_deinit(void);
static int _menuitem_callback(menuview_t *list, menuview_sig_e signal);
static bool _dlg_common_exit(void);
static void _sure_modify_dlg_cb(dialog_t *dlg, widget_sig_dialog_e signal);
static void _sure_modify_dlg(string_resource_t *str_res);



#define KEYMAP_MAX_ITEM 9  //修改keymap设置有9个menuitem
menu_t *fourthLevelMenu = NULL;

/* fourth level menu item data define */
menuitem_t fourth_level_items[KEYMAP_MAX_ITEM];
int key_value[8];

/*
 ** 第四级菜单入口
 */
bool em_fourth_level_menu_enter(scene_emulator_t* my_scene)
{
    menu_t *cur_menu;
    fusion_effect_t effect;

    fusion_get_config(FUSION_TYPE_APP_CHANGE, enter_type, &effect); // FUSION_TYPE_COMMON
    fusion_set_effect(&effect);

    scene_fusion_set_first_frame(g_scene_emulator->scene_common.scene);

    my_scene->cur_menu = 4;
    cur_menu = _em_fourth_level_menu_init();
    menuview_set_menu(my_scene->menuview, cur_menu);
    menuview_load_style(my_scene->menuview, MENUVIEW_MENULIST_TEXT); //    MENUVIEW_OPTION
    menuview_show(my_scene->menuview);
    menuview_paint(my_scene->menuview);

    scene_fusion_set_last_frame(g_scene_emulator->scene_common.scene);
    fusion_display( NULL);

    return true;
}

/*
 ** 退出第四级菜单
 */
static bool _em_fourth_level_menu_exit(void)
{
    /* delete seleq menu */
    menuview_hide(g_scene_emulator->menuview);
    _em_fourth_level_menu_deinit();

    menuview_set_menu(g_scene_emulator->menuview, NULL); //将menuview的menu指针置空，避免野指针
    
    return true;
}

/*
 ** 初始化第四级菜单
 */
static menu_t* _em_fourth_level_menu_init(void)
{
    scene_t* scene;
    static char string[8][50];
    char temp[10];
    int num;

    /* initial menu	*/
    if (fourthLevelMenu == NULL)
    {
        /* get scene */
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            return false;
        }
        /* init items */
        memset(&fourth_level_items, 0, sizeof(fourth_level_items));

        _get_key_name_from_keymap((int) emu_cfg_info.btn_map[0], temp);
        sprintf(string[0], "%s    %s", "A", temp);

        _get_key_name_from_keymap((int) emu_cfg_info.btn_map[1], temp);
        sprintf(string[1], "%s    %s", "B", temp);

        _get_key_name_from_keymap((int) emu_cfg_info.btn_map[2], temp);
        sprintf(string[2], "%s    %s", "START", temp);

        _get_key_name_from_keymap((int) emu_cfg_info.btn_map[3], temp);
        sprintf(string[3], "%s    %s", "SELECT", temp);

        _get_key_name_from_keymap((int) emu_cfg_info.btn_map[4], temp);
        sprintf(string[4], "%s    %s", "LEFT", temp);

        _get_key_name_from_keymap((int) emu_cfg_info.btn_map[5], temp);
        sprintf(string[5], "%s    %s", "RIGHT", temp);

        _get_key_name_from_keymap((int) emu_cfg_info.btn_map[6], temp);
        sprintf(string[6], "%s    %s", "UP", temp);

        _get_key_name_from_keymap((int) emu_cfg_info.btn_map[7], temp);
        sprintf(string[7], "%s    %s", "DOWN", temp);

        for (num = 0; num < (KEYMAP_MAX_ITEM - 1); num++)
        {
            fourth_level_items[num].has_child = false;
            fourth_level_items[num].callback = (item_cb_t) _menuitem_callback;
            fourth_level_items[num].text = string[num];
        }
        fourth_level_items[8].has_child = false;
        fourth_level_items[8].callback = (item_cb_t) _menuitem_callback;
        fourth_level_items[8].text = "Save change";

        /* create menu */
        fourthLevelMenu = menu_create();
        if (fourthLevelMenu == NULL)
        {
            return false;
        }
        else
        {
            /* append items */
            for (num = 0; num < KEYMAP_MAX_ITEM; num++)
            {
                menu_append_item(fourthLevelMenu, &fourth_level_items[num]);
            }
        }

        /* init current index */
        fourthLevelMenu->total = KEYMAP_MAX_ITEM;
        fourthLevelMenu->offset = 0;
        fourthLevelMenu->current = 0;
        menu_set_cur_index(fourthLevelMenu, 0);
    }
    return fourthLevelMenu;
}

/*
 ** 删除第四级菜单
 */
static bool _em_fourth_level_menu_deinit(void)
{
    /* delete menu */
    if (fourthLevelMenu != NULL)
    {
        /* create menu */
        menu_delete(fourthLevelMenu);
        fourthLevelMenu = NULL;
    }

    return true;
}

/*
 ** 第四级菜单的 menuitem 的回调函数
 */
static int _menuitem_callback(menuview_t *list, menuview_sig_e signal)
{
    return 0;
}

/*
 ** 第四级菜单的 menuview 处理按键事件和menuview显示更新
 */
bool set_keymap_proc_key(key_event_t * key_event)
{
    int id = 0;
    char key_name[10];
    static char str[8][50];
    char temp_str[20];

    memset(key_name, 0, sizeof(key_name));
    memset(temp_str, 0, sizeof(temp_str));     
    
    if ((key_event->type == KEY_TYPE_SHORT_UP) || (key_event->type == KEY_TYPE_LONG_UP) || (key_event->type
            == KEY_TYPE_HOLD_UP))
    {
        //return键，立即返回到上级菜单。
        if (key_event->val == KEY_RETURN)
        {
            _em_fourth_level_menu_exit();
            em_third_level_menu_enter(g_scene_emulator);
            return false;
        }
        
        //当焦点到达“save change”时，如果非“OK键”就不处理
        if ((fourthLevelMenu->current >= 8) && (key_event->val != KEY_OK))
        {
            return false;	
        }
        

        switch (key_event->val)
        {
            case KEY_VSUB:
            strcpy(key_name, "Vol-");
            break;

            case KEY_VADD:
            strcpy(key_name, "Vol+");
            break;

            case KEY_PREV:
            strcpy(key_name, "Prev");
            break;

            case KEY_NEXT:
            strcpy(key_name, "Next");
            break;

            case KEY_OK:
            strcpy(key_name, "Ok");
            break;

            case KEY_MENU:
            strcpy(key_name, "Menu");
            break;

            case KEY_CAMERA:
            strcpy(key_name, "Camera");
            break;

            case KEY_TVOUT:
            strcpy(key_name, "Tvout");
            break;

            case KEY_DELETE:
            strcpy(key_name, "delete");
            break;

            case KEY_PLAYBACK:
            strcpy(key_name, "playback");
            break;
            
            default:
            break;
        }

        if (fourthLevelMenu->current < (KEYMAP_MAX_ITEM - 1)) //0~7
        {
            id = fourthLevelMenu->current;
//            _get_key_name_from_emu_tab(id, str[id]);
//            if ( (strlen(key_name) + strlen(str[id])) <= sizeof(str[id]) )
//            {
//                sprintf(str[id], "%s    %s", str[id], key_name); //--- Klockwork 规范修改
//            }
            _get_key_name_from_emu_tab(id, temp_str);
            sprintf(str[id], "%s    %s", temp_str, key_name);
            
            
            fourth_level_items[id].text = str[id];
            menu_set_item(fourthLevelMenu, id, &fourth_level_items[id]); //menuitem_t *item
            key_value[id] = (int) (key_event->val);
        }

        fourthLevelMenu->current++; //焦点item下移
        if (fourthLevelMenu->current >= KEYMAP_MAX_ITEM)
        {
            fourthLevelMenu->current =  KEYMAP_MAX_ITEM;	
        }
        
        if (fourthLevelMenu->current < KEYMAP_MAX_ITEM)
        {
            menu_set_cur_index(fourthLevelMenu, fourthLevelMenu->current);
            menuview_paint(g_scene_emulator->menuview);
        }

        if (fourthLevelMenu->current == KEYMAP_MAX_ITEM) //id=8, do save change
        {
            fourthLevelMenu->current = 0;

            if (key_event->val == KEY_OK)
            {
                _sure_modify_dlg(g_scene_emulator->modify_str);
            }
        }
    }
    return true;
}

/*
 **	根据按键的值得到按键名字
 */
static void _get_key_name_from_keymap(int value, char* keyName)
{
    switch (value)
    {
        case KEY_TVOUT:
        strcpy(keyName, "Tvout");
        break;
        case KEY_CAMERA:
        strcpy(keyName, "Camera");
        break;
        case KEY_MENU:
        strcpy(keyName, "Menu");
        break;
        case KEY_OK:
        strcpy(keyName, "Ok");
        break;
        case KEY_VSUB:
        strcpy(keyName, "Vol-");
        break;
        case KEY_VADD:
        strcpy(keyName, "Vol+");
        break;
        case KEY_PREV:
        strcpy(keyName, "prev");
        break;
        case KEY_NEXT:
        strcpy(keyName, "Next");
        break;
        case KEY_DELETE:
        strcpy(keyName, "Delete");
        break;        
        case KEY_PLAYBACK:
        strcpy(keyName, "PlayBack");
        break;         
        default:
        break;
    }
}

/*
 **	根据id号从 emulator 按键顺序表中得到按键名字
 */
static void _get_key_name_from_emu_tab(int id, char* keyName)
{
    switch (id)
    {
        case 0:
        strcpy(keyName, "A");
        break;
        case 1:
        strcpy(keyName, "B");
        break;
        case 2:
        strcpy(keyName, "START");
        break;
        case 3:
        strcpy(keyName, "SELECT");
        break;
        case 4:
        strcpy(keyName, "LEFT");
        break;
        case 5:
        strcpy(keyName, "RIGHT");
        break;
        case 6:
        strcpy(keyName, "UP");
        break;
        case 7:
        strcpy(keyName, "DOWN");
        break;
        default:
        break;
    }
}



/*
 ** 确定按键修改对话框
 */
static void _sure_modify_dlg(string_resource_t *str_res)
{
    dialog_init_t dlg_init;

    dlg_init.head = DIALOG_HEAD_QUERY; 
    dlg_init.str_id = str_res->str_id;
    g_scene_emulator->dlg = dialog_create(&dlg_init, _sure_modify_dlg_cb);
    if (g_scene_emulator->dlg != NULL)
    {
        g_scene_emulator->dlg->total_button = 2;
        dialog_paint(g_scene_emulator->dlg);
    }
}

static void _sure_modify_dlg_cb(dialog_t *dlg, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK: 
        _dlg_common_exit();  
                 
        memcpy(emu_cfg_info.btn_map, key_value, sizeof(key_value));
        emu_set_keymap();
            
        _em_fourth_level_menu_exit();
        em_third_level_menu_enter(g_scene_emulator);
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        default:
        _dlg_common_exit();
            
        _em_fourth_level_menu_exit();
            
        em_third_level_menu_enter(g_scene_emulator);        
        break;
    }
}

/*
 **  dialog控件退出时的公共函数
 */
static bool _dlg_common_exit(void)
{
    bool result = true;

    if (g_scene_emulator->dlg != NULL)
    {
        result = dialog_delete(g_scene_emulator->dlg);
        g_scene_emulator->dlg = NULL;
    }
    return result;
}

