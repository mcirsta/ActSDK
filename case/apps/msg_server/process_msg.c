/*
 *******************************************************************************
 *                       NOYA1100
 *                (c) Copyright 2002-2008, Actions Co,Ld. 
 *                        All Right Reserved 
 * FileName: process_msg.c
 * Description:
 * History: 
 *       <author>    <time>          <version >      <desc>
 *       shaofeng    2009/03/11      1.0             建立该文件。
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
 *       将经过去抖后的稳定键值, 转换成app识别的: 键值+状态
 * \param[in]    unsigned int: 从驱动读取的键值
 * \param[out]   input_gui_msg_t *: 转换成app识别的键值+状态. 用于发送给gui, gui再发送给焦点
 * \retval       
 *******************************************************************************/
bool process_key_event( unsigned int key_val_app, input_gui_msg_t *input_msg )
{
    static int          count = 0;          /* 记录按键type的计数值 */
    static unsigned int saved_key_val_app;  /* 发送给APP的按键值, 为总按键值 */
    static key_type_e   saved_key_type;
    bool result = true;
    int tmp = 0;
    unsigned int upkey_val = 0;             /* 抬起键的按键值 */

    print_dbg("key_val:0x%x", key_val_app);

    //判断某个键抬起条件: 原先有, 新键值没有的
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
        /* 最后一个按键抬起时, 清空记录按键type的count, 否则会产生多按键的down消息 */
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
 *       设置某个按键的映射状态
 *       根据键值找到其按键映射, 并设置其映射使能状态
 * \param[in]    key_val_e: 需要设置状态的按键值
 * \param[in]    bool: 设置的使能状态
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
 *       按键映射实现函数
 *       在key_map数据结构体中寻找比较, 一旦有按键消息在map数据中需要映射成其他消息, 即开始映射
 * \param[in]    input_gui_msg_t: 发送给gui的消息
 * \param[out]    input_gui_msg_t: 映射后直接修改传入的参数, 再发送给gui的消息
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
            /* 达到用户最后一个映射, 或者达到最大映射个数, 退出*/
            if ( 0 == key_para.key_map_list[i].key_val_src )
            {
                break;
            }

            /* 找到需要映射的按键, 进行映射 */
            if ( (true == key_para.key_map_list[i].enable) && \
                    (input_msg->data.kmsg.val == key_para.key_map_list[i].key_val_src) )
            {
                /* 此键值的转换 */
                if (KEY_TYPE_ALL == key_para.key_map_list[i].key_type_src)
                {
                    input_msg->data.kmsg.val = key_para.key_map_list[i].key_val_dest;
                }
                /* 此键值的某种状态 转换成 另一键值的另一状态 */
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
 *       快捷键映射实现函数
 *       在key_shortcut_list数据结构体中寻找比较, 一旦有按键消息在map数据中需要映射成其他消息, 即开始映射
 * \param[in]    input_gui_msg_t: 发送给gui的消息
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
            /* 达到用户最后一个快捷键, 或者达到最大快捷键个数, 退出*/
            if ( 0 == key_para.key_shortcut_list[i].key_event.val )
            {
                break;
            }

            /* 找到快捷键, 发送快捷键对应消息 */
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
        /*平放*/
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
    /* 此处需使用if else 结构，暂时屏蔽QAC检查，以避免控制语句深度超过5的警告 */

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
