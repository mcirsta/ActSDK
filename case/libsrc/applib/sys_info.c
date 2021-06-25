/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : sys_info.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-07-06             v1.0             build this file 
 ********************************************************************************/

#include <include_case.h>
#include <include_psp.h>

typedef struct
{
    charge_voltage_t voltage;
}power_threshold_map_t;

static power_threshold_map_t s_power_threshold_map[] =
{
    { LI350 },
    { LI360 },
    { LI360 },
    { LI360 },
    { LI360 },
    { LI360 },
    { LI380 },
};

bool sys_get_hotplugin_dev_info(hotplugin_dev_type_e dev, hotplugin_dev_info_t * info)
{
    int dev_fd; 
    kerninfo_dev_info_t dev_info = {0};
    get_dev_info_arg_t get_dev_info_arg = {0};

    if(dev == HOTPLUGIN_DEV_TYPE_CARD)
    {
        get_dev_info_arg.dev_id = DEV_ID_CARD;
    }
    else if(dev == HOTPLUGIN_DEV_TYPE_UHOST)
    {
        get_dev_info_arg.dev_id = DEV_ID_USB_HOST;
    }
    else if(dev == HOTPLUGIN_DEV_TYPE_LOCAL)
    {
        get_dev_info_arg.dev_id = DEV_ID_LOCAL;
    }
    else
    {
        return false;
    }

    get_dev_info_arg.dev_info = &dev_info;

    if(info == NULL)
    {
        print_err("input parameter(info) can not be a NULL pointer!");
        return false;
    }

    dev_fd =  open(DEVICE_KERNINFO, O_RDONLY);

    if (dev_fd < 0)
    {
        print_err("open %s failed!", DEVICE_KERNINFO);
        return false;
    }

    if(ioctl(dev_fd, GET_DEV_INFO, ( int )&get_dev_info_arg) != 0)
    {
        print_err("get dev info failed!");
        close(dev_fd);
        return false;
    }

    if(dev_info.exist_flags == DEV_IN)
    {
        info->insert = HOTPLUGIN_DEV_IN;
    }
    else if(dev_info.exist_flags == DEV_READY)
    {
        info->insert = HOTPLUGIN_DEV_READY;
    }
    else if(dev_info.exist_flags == DEV_OUT)
    {
        info->insert = HOTPLUGIN_DEV_OUT;
    }
    else
    {
        info->insert = HOTPLUGIN_DEV_ERR;
    }

    if(dev_info.access_flags == DEV_ACCESS_OK)
    {
        info->access = HOTPLUGIN_DEV_ACCESS_OK;
    }
    else
    {
        info->access = HOTPLUGIN_DEV_ACCESS_ERR;
    }

    close(dev_fd);
    return true;
}

bool sys_power_threshold_check(power_threshold_type_e type)
{
    hw_status_t hw_status;
    struct timespec timeout;
    msg_apps_t  msg = {0};
    msg_reply_t reply = {0};
    charge_voltage_t cur_volate; 

    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    msg.type = MSG_GET_HW_STATUS_SYNC;

    if(send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, &timeout) == false)
    {
        print_err("send_sync_msg failed");
        return false;
    }

    memcpy(&hw_status, reply.content, sizeof(hw_status_t));

    if( (PLUG_IN == hw_status.usb) || (true == hw_status.adapter) )
    {
        cur_volate = LI420;
    }
    else
    {
        cur_volate = hw_status.voltage;
    }

    if( cur_volate >= s_power_threshold_map[type].voltage)
    {
        return true;
    }
    
    return false; 
}

hotplugin_dev_type_e sys_get_main_disk(void)
{
    int val = 0;

    if(get_config(CFG_NANDBASE, (char *)&val, sizeof(val)) < 0)
    {
        print_err("get_config(CFG_NANDBASE) error!");

        print_info("main disk is %d", val);
        return HOTPLUGIN_DEV_TYPE_NONE;
    }

    if(val == 1)
    {
        val = (int)HOTPLUGIN_DEV_TYPE_LOCAL;

        if(set_config(CFG_MAIN_DISK, (char *)&val, sizeof(val)) < 0)
        {
            print_err("set_config(CFG_MAIN_DISK, %d) error!", val);
        }

        print_info("main disk is %d", val);
        return (hotplugin_dev_type_e)val;
    }
    else
    {
        if(get_config(CFG_MAIN_DISK, (char *)&val, sizeof(val)) < 0)
        {
            print_err("get_config(CFG_MAIN_DISK) error!");

            val = HOTPLUGIN_DEV_TYPE_NONE;
            if(set_config(CFG_MAIN_DISK, (char *)&val, sizeof(val)) < 0)
            {
                print_err("set_config(CFG_MAIN_DISK, %d) error!", val);
            }
        }

        print_info("main disk is %d", val);
        return (hotplugin_dev_type_e)val;
    }
}

bool sys_set_main_disk(hotplugin_dev_type_e type)
{
    if(set_config(CFG_MAIN_DISK, (char *)&type, sizeof(type)) < 0)
    {
        print_err("set_config(CFG_MAIN_DISK, %d) error!", type);
        return false;
    }

    print_info("main disk is %d", type);
    return true; 
}
