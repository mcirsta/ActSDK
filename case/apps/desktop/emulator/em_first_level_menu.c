#include "emulator_scene.h"
#include "emulator_play.h"
#include "em_second_level_menu.h"
#include "em_first_level_menu.h"


extern char game_kind[20];
extern scene_emulator_t* g_scene_emulator;
extern int showSaveItem;
extern emu_state_t emu_state;
extern fusion_enter_e enter_type;

static void _em_dirlist_cb(dirlist_t* dirlist, dirlist_sig_e sig);


menu_t *firstLevelMenu = NULL;
char romFullPath[PATH_MAX];
char oldRomFile[PATH_MAX];
char saveFilePath[PATH_MAX];

/*�����Ϸ�ļ���flash��deviceSel=0������deviceSel=1��host��deviceSel=2 */
int deviceSel = 0;

/*
 ***************** ������dirlist �ؼ���ʹ�� ***************
 ** The following routines used to
 ** show certain type game rom files. [dirlist widget]
 ** flag=0��ʾdirlist���ν��룬���軹ԭdirlist����
 **********************************************************
 */
bool em_dirlist_enter(scene_emulator_t* my_scene, int flag)
{
    dirlist_init_t dirlist_init;
    fusion_effect_t effect;

    fusion_get_config(FUSION_TYPE_APP_CHANGE, enter_type, &effect); // FUSION_TYPE_COMMON
    fusion_set_effect(&effect);

    scene_fusion_set_first_frame(g_scene_emulator->scene_common.scene);

    if (my_scene->dirlist == NULL)
    {
        my_scene->cur_menu = 1;
        dirlist_init.type = DIRLIST_GAME;
        dirlist_init.filter_string = game_kind;
        dirlist_init.top_path_name = NULL; 
        dirlist_init.init_path_name = NULL; //UDISK_GAME;

        if (my_scene->dirlist == NULL)
        {
            my_scene->dirlist = dirlist_create(&dirlist_init, _em_dirlist_cb);

            if (NULL == my_scene->dirlist)
            {
                return false;
            }
            dirlist_paint(my_scene->dirlist);
        }
    }
    else
    {
        dirlist_show(my_scene->dirlist);
        dirlist_paint(my_scene->dirlist);	
    }

    scene_fusion_set_last_frame(g_scene_emulator->scene_common.scene);
    fusion_display( NULL);

    return true;
}

bool em_dirlist_exit(dirlist_t *dirlist)
{
    bool result = true;

    if (dirlist != NULL)
    {
        result = dirlist_delete(dirlist);
        g_scene_emulator->dirlist = NULL;
    }

    return result;
}

static void _em_dirlist_cb(dirlist_t* dirlist, dirlist_sig_e sig)
{
    switch (sig)
    {
        case WIDGET_SIG_DIRLIST_OK:
        memset(oldRomFile, 0, ROM_FILE_FULL_PATH);
        strcpy(oldRomFile, romFullPath); 
        memset(romFullPath, 0, ROM_FILE_FULL_PATH);
        sprintf(romFullPath, "%s%s", dirlist->path_name, dirlist->file_name);
        dirlist_hide(dirlist);
        
        /* �Ƚ�saveFilePath=romFullPath����loadʱģ�����ڲ����жϣ�Ȼ�󷵻�load��� */
        memset(saveFilePath, 0, PATH_MAX);
        strcpy(saveFilePath, romFullPath);
        
        /*�ж�rom�ļ����ĸ��洢������*/
        if(strncmp(dirlist->path_name, CARD_ROOT, sizeof(CARD_ROOT)-1) == 0)
        {
            deviceSel = 1;	//card
        }
        else if(strncmp(dirlist->path_name, EXTERNAL_ROOT, sizeof(EXTERNAL_ROOT)-1) == 0)
        {
            deviceSel = 2;  //host
        }
        else
        {
            deviceSel = 0;  //flash
        }

        /* ����2��Ŀ¼ */
        emu_state = EMU_STAT_STOP;
        showSaveItem = 0;
        em_second_level_menu_enter(g_scene_emulator);
        break;

        case WIDGET_SIG_DIRLIST_QUIT:          
        /*�˳�ģ����*/
        emu_exit();
        sleep(1);
                       
        emulator_scene_quit();
        break;

        default:
        break;
    }
}



