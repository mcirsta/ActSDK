/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : drv_op_sys_event_monitor.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-6-18             v1.0             build this file 
 ********************************************************************************/

#include "sys_event_monitor.h"
#include "msg_server_config.h"

typedef struct evt_filter_list_s
{
    sys_evt_e type;
    struct evt_filter_list_s * next;
}evt_filter_list_t;

static evt_filter_list_t * s_evt_filter_list = NULL;

static int s_dev_fd = -1;
static bool _evt_filter_check(sys_evt_e type);
static bool _evt_filter_list_add(sys_evt_e type);
static bool _evt_filter_list_delete(sys_evt_e type);
static bool _evt_filter_list_clear(void);
static void _evt_filter_list_print(void);

bool sys_evt_monitor_open_dev( void )
{
    s_dev_fd = open(DEVICE_KERNINFO, O_RDWR);

    if (s_dev_fd < 0)
    {
        s_dev_fd = -1;
        print_err("open %s failed!", DEVICE_KERNINFO);
        return false;
    }

    if(strcasecmp(g_msg_server_cfgs[CFG_TYPE_UHOST_SUPPORT].text, "off") == 0)
    {
        _evt_filter_list_add(SYS_EVT_USB_A_IN);
        _evt_filter_list_add(SYS_EVT_USB_HOST_DISK_IN);
        _evt_filter_list_add(SYS_EVT_USB_HOST_DISK_READY);
        _evt_filter_list_add(SYS_EVT_USB_A_OUT);
        _evt_filter_list_add(SYS_EVT_USB_HOST_DISK_OUT);
    }

    if(strcasecmp(g_msg_server_cfgs[CFG_TYPE_CARD_SUPPORT].text, "off") == 0)
    {
        _evt_filter_list_add(SYS_EVT_CARD_IN);
        _evt_filter_list_add(SYS_EVT_CARD_READY);
        _evt_filter_list_add(SYS_EVT_CARD_OUT);
    }

    return true;
}

bool sys_evt_monitor_close_dev( void )
{
    _evt_filter_list_clear();

    if (-1 != s_dev_fd)
    {
        if(close(s_dev_fd) != 0)
        {
            print_err("close %s failed!", DEVICE_KERNINFO);
            return false;
        }
        else
        {
            return true;
        }
    }

    print_warning("%s has not opened!", DEVICE_KERNINFO);
    return false;
}

sys_evt_e sys_evt_monitor_get_evt(void)
{
    int sys_msg;
    sys_evt_e sys_evt;

    if (-1 == s_dev_fd)
    {
        print_warning("%s has not opened!", DEVICE_KERNINFO);
        return SYS_EVT_ERR;
    }

    if(ioctl(s_dev_fd, GET_SYS_MSG, (int)&sys_msg) != 0)
    {
        if(errno != ENOMSG)
        {
            print_err("get sys msg failed!");
            return SYS_EVT_ERR;
        }

        return SYS_EVT_NO_EVT;
    }

    switch(sys_msg)
    {

        case SYS_MSG_USB_A_IN :
        {
            print_info("USB A IN!");
            sys_evt = SYS_EVT_USB_A_IN;
            break;
        }
        case SYS_MSG_ADD_DEV_MSC :
        {
            print_info("USB host disk in!");
            sys_evt = SYS_EVT_USB_HOST_DISK_IN;
            break;
        }
        case SYS_MSG_DEV_MSC_READY :
        {
            print_info("USB host disk ready!");
            sys_evt = SYS_EVT_USB_HOST_DISK_READY;
            break;
        }
        case SYS_MSG_USB_A_OUT :
        {
            print_info("USB A OUT!");
            sys_evt = SYS_EVT_USB_A_OUT;
            break;
        }
        case SYS_MSG_REMOVE_DEV_MSC :
        {
            print_info("USB host disk out!" );
            sys_evt = SYS_EVT_USB_HOST_DISK_OUT;
            break;
        }
        case SYS_MSG_USB_B_IN :
        {
            print_info("USB B IN!" );
            sys_evt = SYS_EVT_USB_B_IN;
            break;
        }
        case SYS_MSG_USB_B_OUT :
        {
            print_info("USB B OUT!" );
            sys_evt = SYS_EVT_USB_B_OUT;
            break;
        }
        case SYS_MSG_CARD_IN :
        {
            print_info("CARD IN!" );
            sys_evt = SYS_EVT_CARD_IN;
            break;
        }
        case SYS_MSG_CARD_READY :
        {
            print_info("SYS_MSG_CARD_READY" );
            sys_evt = SYS_EVT_CARD_READY;
            break;
        }
        case SYS_MSG_CARD_ERR:
        {
            print_info("CARD INIT ERR!" );
            sys_evt = SYS_EVT_CARD_ERR;
            break;
        }
        case SYS_MSG_CARD_OUT :
        {
            print_info("CARD OUT!" );
            sys_evt = SYS_EVT_CARD_OUT;
            break;
        }
        case SYS_MSG_ALARM_IN :
        {
            print_info("ALARM IN!" );
            sys_evt = SYS_EVT_ALARM_IN;
            break;
        }
        case SYS_ROUND_POWER_ADAPTER_IN :
        {
            print_info("ROUND_POWER_ADAPTER IN!" );
            sys_evt = SYS_EVT_ROUND_POWER_ADAPTER_IN;
            break;
        }
        case SYS_POWER_ADAPTER_OUT :
        {
            print_info("POWER_ADAPTER OUT!" );
            sys_evt = SYS_EVT_POWER_ADAPTER_OUT;
            break;
        }
        default :
        {
            sys_evt = SYS_EVT_NO_EVT;
            break;
        }
    }

    if(_evt_filter_check(sys_evt) == true)
    {
        sys_evt = SYS_EVT_NO_EVT;
    }

    return sys_evt;
}

static bool _evt_filter_check(sys_evt_e type)
{
    evt_filter_list_t * node = s_evt_filter_list;

    while(node != NULL) 
    {
        if(node->type == type)
        {
            return true;
        }

        node = node->next;
    }

    return false;
}

static bool _evt_filter_list_add(sys_evt_e type)
{
    evt_filter_list_t * new_node;

    if(_evt_filter_check(type) == true)
    {
        return true;
    }

    new_node = (evt_filter_list_t *) malloc ( sizeof(evt_filter_list_t));
    if( new_node == NULL )
    {
        print_err("malloc failed!");
        return false;
    }

    new_node->type = type;
    new_node->next = s_evt_filter_list;
    s_evt_filter_list = new_node;
    //_evt_filter_list_print();

    return true;
}

static bool _evt_filter_list_delete(sys_evt_e type)
{
    evt_filter_list_t * cur_node = s_evt_filter_list;
    evt_filter_list_t * prev_node = s_evt_filter_list;

    while(cur_node != NULL) 
    {
        if(cur_node->type == type)
        {
            if(prev_node == cur_node)
            {
                s_evt_filter_list = s_evt_filter_list->next;
                prev_node = s_evt_filter_list;
                free(cur_node);
                cur_node = s_evt_filter_list;
            }
            else
            {
                prev_node->next = cur_node->next;
                free(cur_node);
                cur_node = prev_node->next;
            }
        }
        else
        {
            prev_node = cur_node;
            cur_node = cur_node->next;
        }
    }

    //_evt_filter_list_print();

    return true;
}

static bool _evt_filter_list_clear(void)
{
    evt_filter_list_t * node;

    while(s_evt_filter_list != NULL) 
    {
        node = s_evt_filter_list;
        s_evt_filter_list = s_evt_filter_list->next;
        free(node);
    }

    //_evt_filter_list_print();

    return true;
}

static void _evt_filter_list_print(void)
{
    evt_filter_list_t * node = s_evt_filter_list;

    printf("\n##### evt filter list #####\n");
    while(node != NULL) 
    {
        printf("\tevt type:%d\n", node->type);
        node = node->next;
    }
}
