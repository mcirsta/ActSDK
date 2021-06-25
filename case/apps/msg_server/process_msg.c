/*
 *******************************************************************************
 *                       NOYA1100
 *                (c) Copyright 2002-2008, Actions Co,Ld. 
 *                        All Right Reserved 
 * FileName: process_msg.c
 * Description:
 * History: 
 *       <author>    <time>          <version >      <desc>
 *       shaofeng    2009/03/11      1.0             �������ļ���
 *******************************************************************************
 */ 
#include "include_psp.h"
#include "include_case.h"
#include "include_msg_server.h"

static int s_axes_trans_matrix[3][3] = 
{
    {0,0,0},
    {0,0,0},
    {0,0,0},
};

typedef struct
{
    int x;
    int y;
    int z;
}axes_t;

inline static void axes_trans(axes_t * src, axes_t * dest);

/******************************************************************************/
/*!                    
 * \par  Description:
 *       ������ȥ������ȶ���ֵ, ת����appʶ���: ��ֵ+״̬
 * \param[in]    unsigned int: ��������ȡ�ļ�ֵ
 * \param[out]   input_gui_msg_t *: ת����appʶ��ļ�ֵ+״̬. ���ڷ��͸�gui, gui�ٷ��͸�����
 * \retval       
 *******************************************************************************/
bool process_key_event( unsigned int key_val_app, input_gui_msg_t *input_msg )
{
    static int          count = 0;          /* ��¼����type�ļ���ֵ */
    static unsigned int saved_key_val_app;  /* ���͸�APP�İ���ֵ, Ϊ�ܰ���ֵ */
    static key_type_e   saved_key_type;
    bool result = true;
    int tmp = 0;
    unsigned int upkey_val = 0;             /* ̧����İ���ֵ */

    print_dbg("key_val:0x%x", key_val_app);

    //�ж�ĳ����̧������: ԭ����, �¼�ֵû�е�
    upkey_val = saved_key_val_app & ~(key_val_app);
    saved_key_val_app = key_val_app;

    /* one or more key is up */
    if ( 0 != upkey_val )
    {
        input_msg->data.kmsg.val = upkey_val;

        switch ( saved_key_type )
        {
            case KEY_TYPE_DOWN :
            {
                input_msg->data.kmsg.type = KEY_TYPE_SHORT_UP;
                break;
            }

            case KEY_TYPE_LONG :
            {
                input_msg->data.kmsg.type = KEY_TYPE_LONG_UP;
                break;
            }
            
            case KEY_TYPE_HOLD :
            {
                input_msg->data.kmsg.type = KEY_TYPE_HOLD_UP;
                break;
            }
            
            default :
            {
                break;
            }

        }

        /* all key is up */
        /* ���һ������̧��ʱ, ��ռ�¼����type��count, ���������ఴ����down��Ϣ */
        if (0 == saved_key_val_app)
        {
            count = 0;
        }
    }
    else
    {
        /* one or more key is pressing */
        if ( 0 != saved_key_val_app )
        {
            input_msg->data.kmsg.val = saved_key_val_app;

            count++;
            if (1 == count)
            {
                input_msg->data.kmsg.type = KEY_TYPE_DOWN;
                saved_key_type = KEY_TYPE_DOWN;
            }
            else if (count == key_para.long_key_time)
            {
                input_msg->data.kmsg.type = KEY_TYPE_LONG;
                saved_key_type = KEY_TYPE_LONG;
            }
            else if (count > key_para.long_key_time)
            {
                tmp = count - key_para.long_key_time;
                if ( (key_para.hold_key_interval != 0) && ( (tmp % key_para.hold_key_interval) == 0 ) )
                {
                    input_msg->data.kmsg.type = KEY_TYPE_HOLD;
                    saved_key_type = KEY_TYPE_HOLD;
                }
                else
                {
                    result = false;
                }
            }
            else
            {
                result = false;
            }
        }
    }

    print_dbg("key_val:0x%x, key_type:0x%x", input_msg->data.kmsg.val, input_msg->data.kmsg.type);
    return result;
}


/******************************************************************************/
/*!                    
 * \par  Description:
 *       ����ĳ��������ӳ��״̬
 *       ���ݼ�ֵ�ҵ��䰴��ӳ��, ��������ӳ��ʹ��״̬
 * \param[in]    key_val_e: ��Ҫ����״̬�İ���ֵ
 * \param[in]    bool: ���õ�ʹ��״̬
 * \retval       
 *******************************************************************************/
bool key_mapping_set_status( key_val_e key, bool enable )
{
    int i;

    for ( i=0; i<MAX_NUM_KEY_MAP; i++ )
    {
        if ( key == key_para.key_map_list[i].key_val_src )
        {
            key_para.key_map_list[i].enable = enable;
        }
    }

    return true;
}


/******************************************************************************/
/*!                    
 * \par  Description:
 *       ����ӳ��ʵ�ֺ���
 *       ��key_map���ݽṹ����Ѱ�ұȽ�, һ���а�����Ϣ��map��������Ҫӳ���������Ϣ, ����ʼӳ��
 * \param[in]    input_gui_msg_t: ���͸�gui����Ϣ
 * \param[out]    input_gui_msg_t: ӳ���ֱ���޸Ĵ���Ĳ���, �ٷ��͸�gui����Ϣ
 * \retval       
 *******************************************************************************/
bool key_mapping( input_gui_msg_t *input_msg )
{
    bool result = true;
    int i = 0;

    if (INPUT_MSG_KEY == input_msg->type)
    {
        for (i=0; i<MAX_NUM_KEY_MAP; i++)
        {
            /* �ﵽ�û����һ��ӳ��, ���ߴﵽ���ӳ�����, �˳�*/
            if ( 0 == key_para.key_map_list[i].key_val_src )
            {
                break;
            }

            /* �ҵ���Ҫӳ��İ���, ����ӳ�� */
            if ( (true == key_para.key_map_list[i].enable) && \
                    (input_msg->data.kmsg.val == key_para.key_map_list[i].key_val_src) )
            {
                /* �˼�ֵ��ת�� */
                if (KEY_TYPE_ALL == key_para.key_map_list[i].key_type_src)
                {
                    input_msg->data.kmsg.val = key_para.key_map_list[i].key_val_dest;
                }
                /* �˼�ֵ��ĳ��״̬ ת���� ��һ��ֵ����һ״̬ */
                else if ( (key_para.key_map_list[i].key_type_src & input_msg->data.kmsg.type) != 0 )
                {
                    input_msg->data.kmsg.val = key_para.key_map_list[i].key_val_dest;
                    input_msg->data.kmsg.type = key_para.key_map_list[i].key_type_dest;
                }
                else
                {
                }                
            }
            else
            {
            }
        }// end for (i=0; ; i++)
    }// end if (INPUT_MSG_KEY == input_msg->type)

    return result;
}


/******************************************************************************/
/*!                    
 * \par  Description:
 *       ��ݼ�ӳ��ʵ�ֺ���
 *       ��key_shortcut_list���ݽṹ����Ѱ�ұȽ�, һ���а�����Ϣ��map��������Ҫӳ���������Ϣ, ����ʼӳ��
 * \param[in]    input_gui_msg_t: ���͸�gui����Ϣ
 * \retval       
 *******************************************************************************/
bool key_shortcut( input_gui_msg_t *input_msg )
{
    /*    print_dbg("input_msg.type:%d, keyval:0x%x, keytype:0x%x", \
                input_msg->type, input_msg->data.kmsg.val, input_msg->data.kmsg.type);
    */

    bool result = true;
    int i = 0;

    if (INPUT_MSG_KEY == input_msg->type)
    {
        for (i=0; i<MAX_NUM_KEY_SHORTCUT; i++)
        {
            /* �ﵽ�û����һ����ݼ�, ���ߴﵽ����ݼ�����, �˳�*/
            if ( 0 == key_para.key_shortcut_list[i].key_event.val )
            {
                break;
            }

            /* �ҵ���ݼ�, ���Ϳ�ݼ���Ӧ��Ϣ */
            if ( (input_msg->data.kmsg.val == key_para.key_shortcut_list[i].key_event.val) && \
                    (input_msg->data.kmsg.type == key_para.key_shortcut_list[i].key_event.type) )
            {
                post_broadcase_msg(key_para.key_shortcut_list[i].msg_type);
                break;
            }
        }// end for (i=0; ; i++)
    }// end if (INPUT_MSG_KEY == input_msg->type)

    return result;
}

static rotation_e s_rotaion = ROTATION_90;

bool process_gsensor_event( gsensor_data_t * gs_data)
{
    msg_apps_t msg; 
    rotation_e cur_rotaion = s_rotaion;
    unsigned short resultant_acc = 0;
    static bool deal_shake = false;

    /* acceleration values in g_sensor self coordinate system */
    axes_t axes_g_sensor = {0};

    /* acceleration values in screen coordinate system */
    axes_t axes_scr = {0};

    axes_g_sensor.x = gs_data->acc_x;
    axes_g_sensor.y = gs_data->acc_y;
    axes_g_sensor.z = gs_data->acc_z;

    /* transform acceleration values in g_sensor self coordinate system  to screen coordinate system */
    axes_trans(&axes_g_sensor, &axes_scr);

    resultant_acc = int_sqrt((unsigned long)( \
                (gs_data->acc_x * gs_data->acc_x) \
                + (gs_data->acc_y * gs_data->acc_y) \
                + (gs_data->acc_z * gs_data->acc_z)) );

    print_dbg("======gsensor:(%d,%d,%d)-->%d", axes_g_sensor.x, axes_g_sensor.y, axes_g_sensor.z, resultant_acc);
    print_dbg("======screen view:(%d,%d,%d)-->%d", axes_scr.x, axes_scr.y, axes_scr.z, resultant_acc);

    if(resultant_acc >(unsigned int)g_msg_server_cfgs[CFG_TYPE_GSENSOR_SHAKE_THRESHOLD].value)
    {
        if(deal_shake == false)
        {
            if(strcasecmp(g_msg_server_cfgs[CFG_TYPE_GSENSOR_SHAKE_SUPPORT].text, "on") == 0)
            {
                msg.type = MSG_GSENSOR_SHAKED;

                print_dbg("+SHAKED+:(%d,%d,%d)-->%d", axes_g_sensor.x, axes_g_sensor.y, axes_g_sensor.z, resultant_acc);
                print_dbg("broadcast msg: MSG_GSENSOR_SHAKED");
                broadcast_msg(&msg);
            }

            deal_shake = true;
        }
    }
    else if(resultant_acc < 1000)
    {
        deal_shake = false;
    }
    else
    {
    }

    if(ABS(axes_scr.z) > 800)
    {
        /*ƽ��*/
    }
    else
    {
        if( (axes_scr.y > 0) && (ABS(axes_scr.y) > 800) )
        {
            print_dbg("++++++ view angle:0 ++++++++");
            cur_rotaion = ROTATION_0;

        }
        else if( (axes_scr.x < 0) && (ABS(axes_scr.x) > 800) )
        {
            print_dbg("++++++ view angle:90 ++++++++");
            cur_rotaion = ROTATION_90;
        }
        else if( (axes_scr.y < 0) && (ABS(axes_scr.y) > 800) )
        {
            print_dbg("++++++ view angle:180 ++++++++");
            cur_rotaion = ROTATION_180;
        }
        else if( (axes_scr.x > 0) && (ABS(axes_scr.x) > 800) )
        {
            print_dbg("++++++ view angle:270 ++++++++");
            cur_rotaion = ROTATION_270;
        }
        else
        {
        }
    }

    if(cur_rotaion != s_rotaion)
    {
        if(deal_shake == false)
        {
            if(strcasecmp(g_msg_server_cfgs[CFG_TYPE_SCR_MODE].text, "Auto") == 0)
            {
                s_rotaion = cur_rotaion;
                msg.type = MSG_GSENSOR_ROTATION_CHANGED;
                memcpy(msg.content, &s_rotaion, sizeof(cur_rotaion));

                print_info("broadcast msg: MSG_GSENSOR_ROTATION_CHANGED");
                broadcast_msg(&msg);
            }
        }
    }

    return true;
}

rotation_e gsensor_get_rotation(void)
{
    return s_rotaion;
}

bool init_g_sensor_axes_trans_matrix(void)
{
    char text[3][128]; 
    int i = 0;

    memset(text, 0, sizeof(text));

    if(get_config(CFG_SCREEN_X, text[0], 128) < 0)
    {
        print_err("get_config(CFG_SCREEN_X) failed!");
        return false;
    }

    if(get_config(CFG_SCREEN_Y, text[1], 128) < 0)
    {
        print_err("get_config(CFG_SCREEN_Y) failed!");
        return false;
    }

    if(get_config(CFG_SCREEN_Z, text[2], 128) < 0)
    {
        print_err("get_config(CFG_SCREEN_Z) failed!");
        return false;
    }

#pragma __PRQA_IGNORE_START__
    /* �˴���ʹ��if else �ṹ����ʱ����QAC��飬�Ա�����������ȳ���5�ľ��� */

    for( i = 0; i < 3; i++)
    {
        if((strcasecmp(text[i], "+g_sensor_x") == 0) || (strcasecmp(text[i], "g_sensor_x") == 0))
        {
            s_axes_trans_matrix[i][0] = 1;
        }
        else if(strcasecmp(text[i], "-g_sensor_x") == 0)
        {
            s_axes_trans_matrix[i][0] = -1;
        }
        else if((strcasecmp(text[i], "+g_sensor_y") == 0) || (strcasecmp(text[i], "g_sensor_y") == 0))
        {
            s_axes_trans_matrix[i][1] = 1;
        }
        else if(strcasecmp(text[i], "-g_sensor_y") == 0)
        {
            s_axes_trans_matrix[i][1] = -1;
        }
        else if((strcasecmp(text[i], "+g_sensor_z") == 0) || (strcasecmp(text[i], "g_sensor_z") == 0))
        {
            s_axes_trans_matrix[i][2] = 1;
        }
        else if(strcasecmp(text[i], "-g_sensor_z") == 0)
        {
            s_axes_trans_matrix[i][2] = -1;
        }
        else
        {
            s_axes_trans_matrix[i][0] = 1;
        }
    }

#pragma __PRQA_IGNORE_END__

    return true;
}

inline static void axes_trans(axes_t * src, axes_t * dest)
{
    dest->x = (s_axes_trans_matrix[0][0] * src->x) + (s_axes_trans_matrix[0][1] * src->y) 
        + (s_axes_trans_matrix[0][2] * src->z) ;

    dest->y = (s_axes_trans_matrix[1][0] * src->x) + (s_axes_trans_matrix[1][1] * src->y) 
        + (s_axes_trans_matrix[1][2] * src->z) ;

    dest->z = (s_axes_trans_matrix[2][0] * src->x) + (s_axes_trans_matrix[2][1] * src->y) 
        + (s_axes_trans_matrix[2][2] * src->z) ;
}
