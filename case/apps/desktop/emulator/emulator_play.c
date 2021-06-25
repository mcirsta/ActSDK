#include "emulator_sys_dep.h"
#include "emulator_scene.h"
#include "em_second_level_menu.h"
#include "emulator_play.h"

static void _err_info_dlg_cb(dialog_t *dlg, widget_sig_dialog_e signal);
static void _err_info_dlg(string_resource_t *str_res);
static bool _init_msgchannel(void);
static bool _deinit_msgchannel(void);
static bool _emulator_default_setting(emu_config_info_t *pcfg);
int _get_key_value_from_name(char *name);
char *_get_kayname_from_value(int value);


extern scene_emulator_t* g_scene_emulator;
extern int showSaveItem; /*�ڶ����˵���'�������'��item����ʾ��־*/
extern char saveFilePath[PATH_MAX];
extern int gameType;


emulator_core_queue_t emuqueue;
emu_state_t emu_state; /*ģ������ǰ״̬:EMU_STAT_STOP=0,EMU_STAT_RUNNING,EMU_STAT_PAUSE*/
emu_config_info_t emu_cfg_info; /* ��¼emulator״̬��disping mode,sound output and keymap */
int timer_id=0;

#define CFG_KEY_NAME_LEN  20

static const clk_domain_t s_clk_domain =
{ CLK_LOAD_MIN, CLK_LOAD_MAX};
static const clk_domain_t nes_clk_domain =
{ CLK_LOAD_MIN, 4};
static const clk_domain_t gbc_clk_domain =
{ CLK_LOAD_MIN, CLK_LOAD_HIGH_3}; //6


///* emulator Ĭ�ϵ� keymap */
//static int emu_key_map_default[8] =
//{KEY_TVOUT, /* a */
//KEY_CAMERA, /* b */
//KEY_MENU, /* start */
//KEY_OK, /* select */
//KEY_VSUB, /* left */
//KEY_VADD, /* right */
//KEY_PREV, /* up */
//KEY_NEXT, /* down */
//};



/*
 ** �� emulatorģ��������,�Լ���ʼ��
 */
bool open_emulator(void)
{
    /* �½�emulator ������Ϣ���� */
    _init_msgchannel();
    
    /* �½�emulator ���� */
    putenv("FILE=game_emu.app");
    execl("/mnt/sdisk/game_emu.app", "game_emu.app", 0);
    usleep(200000); //200ms
   
    /*emulator��[keymap],[display mode],[sound out]Ĭ������ */
    _emulator_default_setting(0);
    emu_state = EMU_STAT_STOP;
 
    return true;
}


/*
 ** �½�����ʼ��emulator��3����Ϣ����
 */
static bool _init_msgchannel(void)
{
    emutest_mq_create(&emuqueue.msgqueue, EMULATOR_SIMCOMMON_FIFO);
    emutest_mq_create(&emuqueue.synSemaphore, EMULATOR_SIMCOMMON_SYN);
    emutest_mq_create(&emuqueue.keymsg, EMULATOR_SIMCOMMON_KEYMSG);

    return true;
}

/*
 ** ɾ�� emulator��3����Ϣ����
 */
static bool _deinit_msgchannel(void)
{
    emutest_mq_destroy(&emuqueue.msgqueue);
    emutest_mq_destroy(&emuqueue.synSemaphore);
    emutest_mq_destroy(&emuqueue.keymsg); 
    
    return true;
}


/*
 ** ���ݼ������ֵõ���ֵ
 */
int _get_key_value_from_name(char *name)
{
    if (strcmp(name, "KEY_TVOUT") == 0)
    {
        return 	KEY_TVOUT;
    }
    if (strcmp(name, "KEY_CAMERA") == 0)
    {
        return 	KEY_CAMERA;
    }
    if (strcmp(name, "KEY_MENU") == 0)
    {
        return 	KEY_MENU;
    }    
    if (strcmp(name, "KEY_OK") == 0)
    {
        return 	KEY_OK;
    }    
    if (strcmp(name, "KEY_VSUB") == 0)
    {
        return 	KEY_VSUB;
    }
    if (strcmp(name, "KEY_VADD") == 0)
    {
        return 	KEY_VADD;
    }    
    if (strcmp(name, "KEY_PREV") == 0)
    {
        return 	KEY_PREV;
    }    
    if (strcmp(name, "KEY_NEXT") == 0)
    {
        return 	KEY_NEXT;
    }      
    if (strcmp(name, "KEY_DELETE") == 0)
    {
        return 	KEY_DELETE;
    } 
    if (strcmp(name, "KEY_PLAYBACK") == 0)
    {
        return 	KEY_PLAYBACK;
    }   
    return 0;     
}

/*
 ** ���ݼ�ֵ�õ�������
 */
char *_get_kayname_from_value(int value)
{
    switch (value)
    {
        case KEY_VSUB:
        return "KEY_VSUB";

        case KEY_VADD:
        return "KEY_VADD";

        case KEY_PREV:
        return "KEY_PREV";
        
        case KEY_NEXT:
        return "KEY_NEXT";

        case KEY_OK:
        return "KEY_OK";

        case KEY_MENU:
        return "KEY_MENU";

        case KEY_CAMERA:
        return "KEY_CAMERA";

        case KEY_TVOUT:
        return "KEY_TVOUT";

        case KEY_DELETE:
        return "KEY_DELETE";

        case KEY_PLAYBACK:
        return "KEY_PLAYBACK";
        
        default:
        break;
    }
    return 0; 	
}

/*
 ** emulatorĬ�����ã�sound��display mode�Լ�keymap
 */
static bool _emulator_default_setting(emu_config_info_t *pcfg)
{
    emu_ctrl_t msg;
    int value = 0;
    char key_name[CFG_KEY_NAME_LEN];
    int emu_key_map[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    /* ��������ģʽ������������ */
    if (get_config(CFG_EMU_SOUND_MODE, (char *)&value, sizeof(value)) < 0)
    {
        print_err("get_config(CFG_EMU_SOUND_MODE) failed!");
        value = 0;
    }       
    msg.type = EMU_SET_AUDIO;
    msg.msg.data[0] = (unsigned short)value;
    engine_cmd(&msg);
    emu_cfg_info.audio_enable = value;

    /* ������ʾģʽ��ȫ�����ȫ�� */
    if (get_config(CFG_EMU_DISP_MODE, (char *)&value, sizeof(value)) < 0)
    {
        print_err("get_config(CFG_EMU_DISP_MODE) failed!");
        value = 0;
    }    
    msg.type = EMU_SET_VIDEO;
    msg.msg.data[0] = (unsigned short)value; 
    engine_cmd(&msg);
    emu_cfg_info.video_mode = value;

    /* ���ð��� */
    get_config(CFG_EMU_KEY_A, key_name, CFG_KEY_NAME_LEN);
    emu_key_map[0] = _get_key_value_from_name(key_name);
    
    get_config(CFG_EMU_KEY_B, key_name, CFG_KEY_NAME_LEN);
    emu_key_map[1] = _get_key_value_from_name(key_name);
    
    get_config(CFG_EMU_KEY_START, key_name, CFG_KEY_NAME_LEN);
    emu_key_map[2] = _get_key_value_from_name(key_name); 
    
    get_config(CFG_EMU_KEY_SELECT, key_name, CFG_KEY_NAME_LEN);
    emu_key_map[3] = _get_key_value_from_name(key_name);     
    
    get_config(CFG_EMU_KEY_LEFT, key_name, CFG_KEY_NAME_LEN);
    emu_key_map[4] = _get_key_value_from_name(key_name); 
    
    get_config(CFG_EMU_KEY_RIGHT, key_name, CFG_KEY_NAME_LEN);
    emu_key_map[5] = _get_key_value_from_name(key_name);     
    
    get_config(CFG_EMU_KEY_UP, key_name, CFG_KEY_NAME_LEN);
    emu_key_map[6] = _get_key_value_from_name(key_name); 
    
    get_config(CFG_EMU_KEY_DOWN, key_name, CFG_KEY_NAME_LEN);
    emu_key_map[7] = _get_key_value_from_name(key_name);     
                   
    memcpy(msg.msg.pointer, emu_key_map, sizeof(emu_key_map));
    msg.type = EMU_SET_KEYMAP;
    engine_cmd(&msg);
    memcpy(emu_cfg_info.btn_map, emu_key_map, sizeof(emu_key_map));

    /* �践�ؼ����˼�Ҫ�������� */
    msg.type = EMU_SET_RETURNKEY;
    msg.msg.data[0] = KEY_RETURN;
    engine_cmd(&msg);

    return true;
}

/*
 *   EMU_PLAY: msg_t: type=EMU_PLAY, pointer: Ҫ����rom��·����,���msg.pointer
 *                   ֵΪ0, ��ô��ʾplay��ǰ��rom
 *                   sub_type=0, ��������, sub_type=1, �ӽ����ļ�����ĵط���ʼ
 *   EMU_CONINUE: ������ǰrom�Ĳ���(����֮ǰ����ͣ��)
 *   EMU_RESET:   ����ǰrom reset,���¿�ʼ
 *   EMU_EXIT:     �˳�ģ����
 *   EMU_SET:      �ı�����Ƶ���,�����Ƿ������������,��Ƶȫ������������С
 *  ����������Ϣ����Ҫ��Ӧ
 *  һ������:�����߷�������,Ȼ��ȴ���syn�ź�����,�ȴ�����
 */
int engine_cmd(emu_ctrl_t *pmsg)
{
    emu_ctrl_t msg;
 
    emutest_mq_post(&emuqueue.msgqueue, (char*)pmsg, sizeof(emu_ctrl_t));
    
    if ((pmsg->type == EMU_PLAY) || (pmsg->type == EMU_CONTINUE))
    {   	
        return (int)EMU_ERR_OK;	
    }
    else
    { 
        emutest_mq_pend(&emuqueue.synSemaphore, (char*)&msg, sizeof(msg));      
    }
 
    return msg.msg.data[0];
}

/******************************************************************************
 *        Timer��ʱ��
 *******************************************************************************/
unsigned int onTimer(void *para)
{
    int err;
    emu_ctrl_t msg;
	
    if (emu_state == EMU_STAT_RUNNING)
    {
        err = emutest_mq_pend_timeout(&emuqueue.synSemaphore, (char*)&msg, sizeof(msg), 100);
        if (err == -1) //err==-1 ��ʾtimeoutʱ������ʱ��û���յ�ģ��������Ϣ
        {
            return 0;	
        }
        else //�յ���ģ�����쳣��Ϣ�������� 
        {
            if (msg.msg.data[0] == EMU_ERR_OK) 
            {
                ; //û�д���
            }
            else if (msg.msg.data[0] == EMU_ERR_RETURN_REQUEST) //���ؼ�����Ϸ��Ҫ����
            {
                if (emu_state == EMU_STAT_RUNNING)
                {
                    emu_state = EMU_STAT_PAUSE;
                    return_to_ui_wiodows();
                }                    
            }
            else
            {
                showSaveItem = 0;
                return_to_ui_wiodows(); 
                if (msg.msg.data[0] == EMU_ERR_FILE_MISSSING)
                {
                    _err_info_dlg(g_scene_emulator->no_file_str);                	
                }
                else
                {
                    _err_info_dlg(g_scene_emulator->nosupp_str);  
                }      
                      	
                _deinit_msgchannel(); 
                usleep(1000); 	              	
                _init_msgchannel();
                emu_state = EMU_STAT_STOP;
            } 
            return 0;			    	
        }
    }
    return 0;
}

///*!\cond EMULATOR*/
///******************************************************************************/
///*!
// * \par  Description: ��Ϸ��playingʱ����Ӧ������Ȼ�������emulatorģ����
// * \param[in]   key_event: key_event_t *
// * \param[out]  none
// * \return      none
// * \retval
// * \ingroup      EMULATOR_FUNC
// * \par
// * \endcode*******************************************************************************/
//void deal_key_when_playing(key_event_t *key_event)
//{
//    emu_input_msg_t msg;
//
//    if (emu_state != EMU_STAT_RUNNING)
//    {
//        return;	
//    }
//    
//    if (key_event->val != KEY_RETURN)
//    {
//        switch (key_event->type)
//        {
//            /* msg.type=EMU_KEY_ACTION,msg.msg.data[0]=���µļ�ֵ, msg.msg.data[1]״̬(����1/̧��0)*/
//            case KEY_TYPE_DOWN: //����[1]
//            case KEY_TYPE_LONG:
//            case KEY_TYPE_HOLD:
//            msg.type = EMU_KEY_ACTION;
//            msg.msg.data[0] = key_event->val;
//            msg.msg.data[1] = 1;
//            emutest_mq_post(&emuqueue.keymsg, (char*)&msg, sizeof(msg));
//            break;
//
//            case KEY_TYPE_SHORT_UP: //̧��[0]
//            case KEY_TYPE_LONG_UP:
//            case KEY_TYPE_HOLD_UP:
//            msg.type = EMU_KEY_ACTION;
//            msg.msg.data[0] = key_event->val;
//            msg.msg.data[1] = 0;
//            emutest_mq_post(&emuqueue.keymsg, (char*)&msg, sizeof(msg));
//            break;
//
//            default:
//            break;
//        }
//    }
//    else /*����Ƿ��ؼ�[KEY_RETURN]*/
//    {
//        if (key_event->type == KEY_TYPE_SHORT_UP) // KEY_TYPE_DOWN
//        {
//            if (emu_state == EMU_STAT_RUNNING)
//            {
//            	emu_pause();
//                return_to_ui_wiodows();
//            }
//            else if (emu_state == EMU_STAT_PAUSE)
//            {
//                return_to_game_wiodows();
//            }
//            else
//            {
//            	;	
//            }
//        }
//    }
//}


void msg_key_server_enable(int enable)
{
    msg_apps_t msg;
    struct timespec timeout;
    msg_reply_t reply;
    bool result = 0;
    
    //֪ͨmsg_server ������������    
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;
    memset(&msg, 0, sizeof(msg_apps_t));
    msg.type = MSG_SET_KEY_SERVER_ENABLE_SYNC;
    memcpy(msg.content, &enable, sizeof(enable));    
    result = (int)send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, &timeout);
    if (false == result)
    {
        print_warning("send_sync_msg failed");
    }
}

// �ص� UI ����
void return_to_ui_wiodows(void)
{
    rotation_e angle;
    bool on;
    msg_apps_t msg;

    /*[�ر�ע��]���˴��������㹻����ʱ������Ķ�����Ҫ��ģ������ȫpause��stop��*/
    usleep(100000);   
    angle = sty_get_view_rotation();
    sty_set_screen_rotation(angle);

    em_create_headbar();
    em_create_menuview();
    sty_draw_bitmap(g_scene_emulator->scene_common.handle_gui_dc, 
                    g_scene_emulator->bg_bitmap_info, 0, 0);       
    em_second_level_menu_enter(g_scene_emulator);
    gui_screen_update();
    
    sys_forbid_auto_standby(false); //ȡ����ֹʡ��ػ�
    sys_forbid_screensaver(false); //ȡ����ֹ��������

    //sys_set_key_tone_enable(1); //�򿪰�����

    
    /* �����Ϸʱ���Ƶ������*/ 
    if (gameType == 1) //nes
    {
        sys_pm_clear_clk_domain(&nes_clk_domain);
    }
    else if (gameType == 3) //gbc
    {
        sys_pm_clear_clk_domain(&gbc_clk_domain);
    }    
    else
    {
        sys_pm_clear_clk_domain(&s_clk_domain);
    }       

    // �������� G-sensor                     
    memset(&msg, 0, sizeof(msg_apps_t));    
    msg.type = MSG_SET_GSENSOR_ON;
    on = true;
    memcpy(msg.content, &on, sizeof(on));
    send_async_msg(APP_NAME_MSG_SERV, &msg);
    
    //֪ͨmsg_server ������������    
    msg_key_server_enable(1);     
}


// �ص� dirlist UI ���� modify 2010-2-26
void return_to_dirlist(void)
{
    rotation_e angle;
    bool on;
    msg_apps_t msg;

    /*[�ر�ע��]���˴��������㹻����ʱ������Ķ�����Ҫ��ģ������ȫpause��stop��*/
    usleep(100000);   
    angle = sty_get_view_rotation();
    sty_set_screen_rotation(angle);

    em_create_headbar();
    em_create_menuview();
    sty_draw_bitmap(g_scene_emulator->scene_common.handle_gui_dc, 
                    g_scene_emulator->bg_bitmap_info, 0, 0);       
    em_dirlist_enter(g_scene_emulator, 1);
    gui_screen_update();
    
    sys_forbid_auto_standby(false); //ȡ����ֹʡ��ػ�
    sys_forbid_screensaver(false); //ȡ����ֹ��������

    //sys_set_key_tone_enable(1); //�򿪰�����

    
    /* �����Ϸʱ���Ƶ������*/ 
    if (gameType == 1) //nes
    {
        sys_pm_clear_clk_domain(&nes_clk_domain);
    }
    else if (gameType == 3) //gbc
    {
        sys_pm_clear_clk_domain(&gbc_clk_domain);
    }    
    else
    {
        sys_pm_clear_clk_domain(&s_clk_domain);
    }       

    // �������� G-sensor                     
    memset(&msg, 0, sizeof(msg_apps_t));    
    msg.type = MSG_SET_GSENSOR_ON;
    on = true;
    memcpy(msg.content, &on, sizeof(on));
    send_async_msg(APP_NAME_MSG_SERV, &msg);
    
    //֪ͨmsg_server ������������    
    msg_key_server_enable(1);     
}

// �ص� ��Ϸ ����
void return_to_game_wiodows(void)
{
    int hdc;
    bool on;
    msg_apps_t msg;
    
     /* ������Ϸ����ʱ����ϵͳ�������Ƶ�����õ���� */
    if (gameType == 1) //nes
    {
        sys_pm_set_clk_domain(&nes_clk_domain);
    }
    else if (gameType == 3) //gbc
    {
        sys_pm_set_clk_domain(&gbc_clk_domain);
    }    
    else
    {
        sys_pm_set_clk_domain(&s_clk_domain);
    } 
        

    em_delete_headbar();
    em_second_level_menu_exit();
    em_delete_menuview();

    hdc = g_scene_emulator->scene_common.handle_gui_dc;
    gui_dc_set_background_color(hdc, GUI_COLOR_BLACK);
    gui_dc_clear(hdc);
    sty_set_screen_rotation(ROTATION_0);

    sys_forbid_auto_standby(true); //��ֹʡ��ػ�
    sys_forbid_screensaver(true); //��ֹ��������

    //sys_set_key_tone_enable(0); //������ʹ�ܽ�ֹ�ӿ�

    // ֹͣ G-sensor
    memset(&msg, 0, sizeof(msg_apps_t));
    msg.type = MSG_SET_GSENSOR_ON;
    on = false;
    memcpy(msg.content, &on, sizeof(on));
    send_async_msg(APP_NAME_MSG_SERV, &msg);
    
    //֪ͨmsg_server ��ֹ��������    
    msg_key_server_enable(0);         
}


/*
** ��ʼ��ģ����
*/
void emu_init(void)
{
    rotation_e angle;

    if (g_scene_emulator->thread_exist == 0)
    {
        /*�����һ�ν�����Ϸ������simulator���̣�����ʼ��ģ����*/
        open_emulator();
        g_scene_emulator->thread_exist = 1;

        /*����ģ�������̺�ģ������ı���Ļ��ʾ�������UI����������������˲���*/
        angle = sty_get_view_rotation();
        if ((angle == ROTATION_90) || (angle == ROTATION_270))
        {
            sty_set_screen_rotation(ROTATION_90);
        }
        else
        {
            sty_set_screen_rotation(ROTATION_0);
        }
        
        /*������ʱ����playingʱ���emulator���صĴ�����Ϣ*/
        timer_id = set_timer(30, onTimer, (void *) 0); //100
    }
    else //�����Ϸ�����Ѵ��ڣ������¿�ʼһ����Ϸromʱ����Ҫ�����Ϣ����
    {
        _deinit_msgchannel(); 
        usleep(1000); 	              	
        _init_msgchannel();        	
    }
}

// ��ͣ�������ʼ
int emu_continue(void)
{
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;

    msg.type = EMU_CONTINUE;
    msg.sub_type = 0; //��������
    result = engine_cmd(&msg);
    emu_state = EMU_STAT_RUNNING;
 
    return result;
}

// ���´�ͷ��ʼ����
int emu_reset(void)
{
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;

    msg.type = EMU_RESET;
    msg.sub_type = 0;
    result = engine_cmd(&msg);
    if (result == EMU_ERR_OK)
    {
        emu_state = EMU_STAT_RUNNING;
    }

    return result;
}

// ��ȡ���Ⱥ󲥷�:EMU_PLAY_MODE_LOADSTAT
int emu_load(void)
{
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;

    if (showSaveItem == 0)
    {
        /*�ڶ����˵���'�������'��item����ʾ��־*/
        showSaveItem = 1;
    }
    
    memset(&msg, 0, sizeof(emu_ctrl_t));
    //strcpy(msg.msg.pointer, romFullPath);
    strcpy(msg.msg.pointer, saveFilePath);
    msg.type = EMU_PLAY;
    msg.sub_type = 1; /* �ӽ����ļ�����ĵط���ʼ */
    result = engine_cmd(&msg);
    emu_state = EMU_STAT_RUNNING;

    return result;
}

/*
 *  ����ĳ��rom�ļ�
 *  romname: Ϊȫ·����,���ݸ��ļ�ϵͳ�ӿڴ�
 *           ���msg.pointerֵΪ0, ��ô��ʾplay��ǰ��rom
 *  playMode: RESTART / RESET / LOADSTAT
 */
int emu_play(const char *romname)
{
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;

    if (showSaveItem == 0)
    {
        /*�ڶ����˵���'�������'��item����ʾ��־*/
        showSaveItem = 1;
    }
    emu_state = EMU_STAT_RUNNING;
    
    memset(&msg, 0, sizeof(emu_ctrl_t));
    strcpy(msg.msg.pointer, romname);
    msg.type = EMU_PLAY; /*EMU_PLAY_MODE_RESTART: ��ͷ��ʼ����*/
    msg.sub_type = 0; /*��������*/
    result = engine_cmd(&msg);
  
    return result;
}

// ��ͣ��Ϸ 
int emu_pause(void)
{
    emu_ctrl_t msg,retMsg;
    int result = EMU_ERR_OK;

    msg.type = EMU_KEY_ACTION;
    msg.msg.data[0] = KEY_RETURN;
    msg.msg.data[1] = 0; /*����*/
    
    emutest_mq_post(&emuqueue.keymsg, (char*)&msg, sizeof(msg));

    result = emutest_mq_pend(&emuqueue.synSemaphore, (char*)&retMsg, sizeof(retMsg));	

    emu_state = EMU_STAT_PAUSE;

    return retMsg.msg.data[0];
}

// ������Ϸ����
int emu_save(void)
{
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;

    memset(&msg, 0, sizeof(emu_ctrl_t));
    msg.type = EMU_SAVE_STATE;
    result = engine_cmd(&msg);
    return result;
}

// �˳�ģ����
int emu_exit(void)
{
    int status;
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;
    
    if (g_scene_emulator->thread_exist == 1)
    {
        if (emu_state == EMU_STAT_RUNNING)
        {
            emu_pause();
        }
    
        memset(&msg, 0, sizeof(emu_ctrl_t));	
        msg.type = EMU_EXIT;
        result = engine_cmd(&msg);
        emu_state = EMU_STAT_STOP;
        g_scene_emulator->thread_exist = 0;

    
        while( wait(&status) <= 0 )
        {
            usleep(10000);
        }
    
        kill_timer(timer_id);
        _deinit_msgchannel();        
    }
       
    return result;
}

/*
 ** ��������ģʽ������ �� ����
 */
int emu_set_sound(int value)
{
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;

    memset(&msg, 0, sizeof(emu_ctrl_t));
    msg.type = EMU_SET_AUDIO;
    msg.msg.data[0] = (unsigned short)value;
    result = engine_cmd(&msg);

    if (set_config(CFG_EMU_SOUND_MODE, (char *)&value, sizeof(value)) < 0)
    {
        print_err("set_config(CFG_EMU_SOUND_MODE) failed!");
    } 
    
    return result;
}

/*
 ** ������ʾģʽ��ȫ�� �� ��ȫ��
 */
int emu_set_disp_mode(int value)
{
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;

    memset(&msg, 0, sizeof(emu_ctrl_t));
    msg.type = EMU_SET_VIDEO;
    msg.msg.data[0]=(unsigned short)value;
    result = engine_cmd(&msg);
    
    if (set_config(CFG_EMU_DISP_MODE, (char *)&value, sizeof(value)) < 0)
    {
        print_err("set_config(CFG_EMU_DISP_MODE) failed!");
    }     
    
    return result;
}

/*
 ** �������ð���
 */
int emu_set_keymap(void)
{
    emu_ctrl_t msg;
    int result = EMU_ERR_OK;
    char *key_name;


    memset(&msg, 0, sizeof(emu_ctrl_t));
    memcpy(msg.msg.pointer, emu_cfg_info.btn_map, sizeof(emu_cfg_info.btn_map));
    msg.type = EMU_SET_KEYMAP;
    result = engine_cmd(&msg);    

    key_name = _get_kayname_from_value(emu_cfg_info.btn_map[0]);    
    set_config(CFG_EMU_KEY_A, key_name, CFG_KEY_NAME_LEN);
    
    key_name = _get_kayname_from_value(emu_cfg_info.btn_map[1]);
    set_config(CFG_EMU_KEY_B, key_name, CFG_KEY_NAME_LEN);
    
    key_name = _get_kayname_from_value(emu_cfg_info.btn_map[2]);
    set_config(CFG_EMU_KEY_START, key_name, CFG_KEY_NAME_LEN);
    
    key_name = _get_kayname_from_value(emu_cfg_info.btn_map[3]);
    set_config(CFG_EMU_KEY_SELECT, key_name, CFG_KEY_NAME_LEN); 
    
    key_name = _get_kayname_from_value(emu_cfg_info.btn_map[4]);   
    set_config(CFG_EMU_KEY_LEFT, key_name, CFG_KEY_NAME_LEN);
    
    key_name = _get_kayname_from_value(emu_cfg_info.btn_map[5]);
    set_config(CFG_EMU_KEY_RIGHT, key_name, CFG_KEY_NAME_LEN);  
    
    key_name = _get_kayname_from_value(emu_cfg_info.btn_map[6]);  
    set_config(CFG_EMU_KEY_UP, key_name, CFG_KEY_NAME_LEN);
    
    key_name = _get_kayname_from_value(emu_cfg_info.btn_map[7]);
    set_config(CFG_EMU_KEY_DOWN, key_name, CFG_KEY_NAME_LEN);    
    
    return result;
}


//������Ի���
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
        
        /* ɾ���Ի��� */
        if(g_scene_emulator->dlg != NULL)
        {
            dialog_delete(g_scene_emulator->dlg);
            g_scene_emulator->dlg = NULL; 
        }       
        break; 
    }
} 
