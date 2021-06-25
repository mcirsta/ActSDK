/********************************************************************************
 *                        USDK130
 *                      the charge driver
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: batmonitor.c
 * By  :
 * Version: 1> v1.00     first version     2006-9-25 8:28
 * modified by  jianggaoping               2007-10-1 10:00
 *******************************************************************************
 */

/*!
 * \file  batmonitor.c
 * \brief  the driver of detect the  battery volatge and the charge status
 * \author
 * \par GENERAL DESCRIPTION:
 *           the file will get the battery voltage and the charge status
 * \par DECLARATIONS:
 *       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
 *
 *  \version 1.1
 *  \date  2007/10/1
 */

#include "charge_inner.h"
#include "charge.h"

#define USB_PORT_MODE_DEFAULT  0    //usb 接口返回状态
#define USB_PORT_MODE_B        1
#define USB_PORT_MODE_A        2
#define BATTERY_SLIDE_WIDTH 10
#define MAX_WAIT_TIME       4      
extern unsigned char is_dc5v_on ;
int (*charge_sys_info)(unsigned int value );
static int adapter_in_hold_jiffies=MAX_WAIT_TIME;
int (*charge_get_usb_port_mode)(void);
static unsigned char  prev_is_dc5v_on=0;
static unsigned char battery_glide[BATTERY_SLIDE_WIDTH];
static unsigned char glide_num = 0;
static unsigned char cur_insert = 0;

unsigned int bat_power = 0;
unsigned int bat_show = 0;


int bat_table[21][2] =
{
    {LI340,0},
    {LI345,5},
    {LI350,10},
    {LI355,15},
    {LI360,20},
    {LI360,25},
    {LI365,30},
    {LI370,35},
    {LI370,40},
    {LI375,45},
    {LI380,50},
    {LI380,55},
    {LI385,60},
    {LI385,65},
    {LI390,70},
    {LI395,75},
    {LI400,80},
    {LI405,85},
    {LI410,90},
    {LI415,95},
    {LI420,100}   
};


int lookup_bat_table(int index)
{
    int i = 0;
    int ret = 0;
    for(i =0;i<21;i++)
    {
        if (index <= bat_table[i][0])
        {
            ret = bat_table[i][1];
            break;
        }
    }
    if(ret == 0)
    {
        ret = 0xff;
    }    
    return ret;        
}

/*!
 * \breif adapter in decide whether to send the msg
 * \par  Description
 *       The function will be call when the adaptor and send related msg
 *
 * \parm[in]  null
 * \return  0
 * \ingroup  headbar
 *
 * \par
 */
static int send_adaptor_in_msg (void)
{
    int usb_connect_state;
    charge_get_usb_port_mode=kernel_sym("get_usb_port_mode");
    usb_connect_state=charge_get_usb_port_mode();
    if ( (usb_connect_state== USB_PORT_MODE_DEFAULT ) \
         || (usb_connect_state==USB_PORT_MODE_A))   //no line in or usb a line is inserted
    {
        charge_sys_info=kernel_sym("kerninfo_put_msg");
        
        charge_sys_info(SYS_ROUND_POWER_ADAPTER_IN);   
    //                printk("charge: SYS_ROUND_POWER_ADAPTER_IN is send!\n");          
    }
    else if(usb_connect_state== USB_PORT_MODE_B )    //USB b line is inserted
    {
                   //the charge start cammand is send by usb driver
    }
    else
    {
    }   
    return 0;
}



/*!
 * \breif get the voltage and the charge status
 * \par  Description
 *       The function will get the battery voltage and the charge status
 *
 * \parm[in]  null
 * \return  bat_power
 * \ingroup  headbar
 *
 * \par
 */


void act_get_battery(void)/*返回值：bit16---充电状态：0－无充电；1－充电*/
/*bit0~3:0~a,电压十分比。*/
/*输入参数为"BAT_MODE_REAL时,输出的是真实电量*/
/*输入参数为"BAT_MODE_SHOW时,输出的是非真实电量，是经过处理的电量，用于显示电量图标*/
{
//    OS_INT_SAVE_AREA
    static unsigned int charge_interval_time=50;
    unsigned int total_bat_val;
    unsigned int temp_bat_power;
    int ret,i;  

//    OS_ENTER_CRITICAL();

    charge_dc5v_check();

    if (!prev_is_dc5v_on)
    {
        if (is_dc5v_on==1)
        {
            if (charge_get_battery_voltage() <= LI360)   //need delay some time （about 600ms~1000ms）
            {                                            //allow the usb drv to detect the usb is inserted  
                adapter_in_hold_jiffies=-1;  
                charge_interval_time=25;           //sub the interval time        
            }
            else
            {
                send_adaptor_in_msg();        
            
            } 
        }
    }
    else
    {
        if (is_dc5v_on==0)
        {
            charge_sys_info= (int (*)(unsigned int))kernel_sym("kerninfo_put_msg");
            charge_sys_info(SYS_POWER_ADAPTER_OUT);     
            adapter_in_hold_jiffies=0;   

        } 
    }
    
    
    if (adapter_in_hold_jiffies<MAX_WAIT_TIME)
    {
        adapter_in_hold_jiffies++;
        if (is_dc5v_on==0)
        {
            adapter_in_hold_jiffies=MAX_WAIT_TIME;
            charge_interval_time=50;
                        
        }
        else if (adapter_in_hold_jiffies==(MAX_WAIT_TIME-1))  //750ms
        {
 
            charge_interval_time=50;
            send_adaptor_in_msg();          
        }
        else
        {
          
        }
    }

    
    prev_is_dc5v_on=is_dc5v_on; 
 
    temp_bat_power = (act_readl((unsigned int) PMU_LRADC) >> 16) & 0x3f;//0x20---3.3v,????//

    battery_glide[cur_insert] = (unsigned char)temp_bat_power;
    cur_insert++;
    if (glide_num < BATTERY_SLIDE_WIDTH)
    {
        glide_num++;
    }
    if (cur_insert >= BATTERY_SLIDE_WIDTH)
    {
        cur_insert = 0;
    }
    total_bat_val = 0;
    for (i = 0; i < glide_num; i++)
    {
        total_bat_val += battery_glide[i];
    }
    bat_power = (total_bat_val / glide_num);
    if ((bat_power < bat_show) || ((bat_power - bat_show) >= 2))
    {
        bat_show = bat_power;
    }
//    OS_EXIT_CRITICAL();
//    printf("act_get_battery\n");

    p_jiffies = kernel_sym("jiffies");
    mod_timer(&(battery_detect_timer), *p_jiffies + charge_interval_time); /* 0.03 second. */

    return 0;
}


