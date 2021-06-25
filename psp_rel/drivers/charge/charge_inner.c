/********************************************************************************
 *                        USDK130
 *                      the charge driver
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: charge.c
 * By  : 
 * Version: 1> v1.00     first version     2006-9-25 8:28
 * modified by  jianggaoping               2007-10-1 10:00
 *******************************************************************************
 */
 
/*!
* \file  charge.c
* \brief the charge drviver
* \author
* \par GENERAL DESCRIPTION:
*           the file finish the charge process and detect the system voltage 
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
*  \version 1.1
*  \date  2007/10/1
*/


#include "charge.h"
#include "charge_inner.h"
#include "charge_sys.h"


charge_status_t charge_status = CHARGE_STOP;

unsigned int charge_count = 0;
unsigned int charge_time_limit_count = 0;
unsigned char charge_interval = 10;
unsigned char no_bat_cnt = 0;
unsigned int retry_charge_cnt = 0;
unsigned char restart_charge_flag = 0;
unsigned int last_bat_voltage = 0x00, bat_voltage_fall = 0;

unsigned char charge_debounce = 0;
unsigned char added_charge_time_flag = 0;
unsigned int added_charge_time_count = 0;
unsigned int added_charge_time = 1200;

unsigned char faild_count = 0;
unsigned int trickle_charge_time = 0;

unsigned int max_charge_current = CHARGE_CURRENT_300;
unsigned int max_charge_voltage = LI420;

/*from charge_info.c*/
//timer_arg_t det_timer;
//unsigned char det_timerNo;
unsigned int max_voltage_config = 4200;
unsigned int ref_voltage_config = 1500;
unsigned int charge_time_limit_config = 60000;
unsigned char charge_time_limit_flag = 0;
unsigned char is_dc5v_on = 0;


struct timer_list battery_detect_timer;
unsigned long *p_jiffies;


int charge_dev_init(void)
{
    //charge_vram_param_init(void)
    /*用定时器来进行电池电量实时检测，20090805，cccw*/
    p_jiffies = kernel_sym("jiffies");    
    init_timer(&battery_detect_timer);
    battery_detect_timer.function = (void *) act_get_battery;
    mod_timer(&battery_detect_timer, *p_jiffies + 450); /* first one delay 3s synpro with usb */
    bat_power=charge_get_battery_voltage();//避免定时器中断开始检测bat_power前，应用获取bat_power
    bat_show=bat_power;        
    return 0;
}

int charge_dev_exit(void)
{
    del_timer_sync(&battery_detect_timer);    
    return 0;
}    

int charge_get_charge_status(charge_status_t * status_pt)
{
    if(status_pt == NULL)
    {
        return -1;
    }
    *status_pt = charge_status;
    return 0;
}

void act_charge(void) /*0.5S查询入口*/
{
  // printf("\n  ch drv\n");
    unsigned int bat_tmp;
    if(charge_status ==CHARGE_STOP)
    {
        return;
    }
    bat_tmp = charge_get_battery_voltage();
    //printf("\n--%d-- BAT = 0x%x\n",__LINE__,bat_tmp);
    if (charge_status != CHARGE_NO_BATTERY) /*以下是电池检测的断开检测部分，部分0.5S运行一次*/
    {
      //printf("\n--%d-- BAT = 0x%x\n",__LINE__,bat_tmp);
        if (bat_tmp <= LI220) /*如果检测到电池电压<2.2V，则认为电池开关被断开*/
        {
            //printf("\n%d NO_BAT bat<2.2",__LINE__);
            charge_status = CHARGE_NO_BATTERY; /*将充电状态设为CHARGE_NO_BATTERY*/
            no_bat_cnt = 0; /*将no_bat_cnt设为0，使得后面电池开关闭合的检测*/
                                    /*空跑5个循环后再进行(请参考后面闭合的检测的注释)*/
            charge_interval = 2; /*充电调整流程的时间间隔设为1S*/
            charge_stop();
        }
        else /*如果电池电压>2.2V*/
        {
            if (bat_tmp < last_bat_voltage) /* 如果电池电压在下降 */
            {
                bat_voltage_fall++; /*累计电池电压下降的次数*/
                //printf("\n bat_voltage_fall= %d ",bat_voltage_fall);
                if (bat_voltage_fall >= 5) /* 如果电池电压下降的次数超过5次，则认为电池开关被断开 */
                {
                    charge_status = CHARGE_NO_BATTERY;
                    no_bat_cnt = 0;
                    charge_interval = 2;
                    charge_stop();
                    //printf("\n%d NO_BAT bat fall",__LINE__);
                }
                last_bat_voltage = bat_tmp;
            }
        } /*电池断开检测结束*/
    }
    else
    {
        if (no_bat_cnt < 5)/*若no_bat_cnt小于5(在检测到电池开关被断开时，此变量被置为0)，*/
        /*则不检测电池开关是否合上，目的是为了避开刚断开开关瞬间，电容上残留的电压对电池检测的影响*/
        {
        }
        else if (no_bat_cnt == 5)/*当no_bat_cnt=5时，开始检测电池开关是否被合上*/
        {
            if (bat_tmp > LI380)/*第一步判断:若检测到电池电压大于3.8V，则认为开关已经合上*/
            {
                restart_charge_flag = 1;
                charge_stop();
                no_bat_cnt = 4;
                //printf("\n%d Bat back > LI380\n",__LINE__);
            }
            else /*若检测到电池电压下于3.8V，进行第二步的判断，即打开充电电路，再测量电池端电压*/
            {
                charge_set_charging_current(CHARGE_CURRENT_50);
                charge_start();
              //printf("\n--%d-- charge_start()\n",__LINE__);
            }
        }
        else /*no_bat_cnt>5，进行第二步的判断*/
        {
            if (bat_tmp < LI390) /*若检测到电池电压小于3.9V，则电池开关已经被合上，否则认为电池开关仍被断开*/
            {
                restart_charge_flag = 1;
                //printf("\n%d Bat back < LI390\n",__LINE__);
            }
            charge_stop();/*关闭充电电路，重新进行第一步的判断*/
            no_bat_cnt = 4;
        }
  
        no_bat_cnt++;   
    }
    
  //  if ((charge_time_limit_count <= (charge_time_limit_config * 120)) || (!charge_time_limit_flag))
  //  {
  //    charge_count++;
  //    if ((charge_count % charge_interval) == 0)
  //    {
  //      charge_check(); /*运行充电调整流程，根据充电状态的不同，1S或5S运行1次*/
  //    }
  //  }
  //  else
  //  {
  //    charge_stop();
  //    charge_status = CHARGE_STOP;
  //    charge_interval = 10; 
  //  }
    charge_count++;
    if(charge_interval==0)
    {
        return;
    }
    if ((charge_count % charge_interval) == 0)
    {
        charge_check(); /*运行充电调整流程，根据充电状态的不同，1S或5S运行1次*/
    }
    return;
}

  
void charge_check(void)
{

    static unsigned int no_DC5V_debounce = 0;
#if 0    
    unsigned int bat_tmp, current_tmp;    
    bat_tmp = charge_get_battery_voltage();
#endif    
    bat_voltage_fall = 0;
    if (charge_detect_dc5v() > 0)
    {
        //printf("\n DC5V on \n");
        no_DC5V_debounce = 0;
        
        //    printf("\n\n--MAX V = 0x%x\n", max_charge_voltage);
        //    printf("\n--charge BAT = 0x%x\n", bat_tmp);
        //    printf("\n--added_time = 0x%x\n", added_charge_time);
        //    printf("\n--charge_status = 0x%x\n", charge_status);
        //    printf("\n--PMU_CHG = 0x%x\n", act_readl((unsigned int) PMU_CHG));
        //    printf("\n--PMU_LRADC = 0x%x\n", act_readl((unsigned int) PMU_LRADC));
        if (charge_status == CHARGE_NO_DC5V)
        {
            //printf("\n NO_DC5V to MON \n");
            charge_status = CHARGE_MONITOR;
        }
        
        if (restart_charge_flag == 1)
        {
            restart_charge_flag = 0;
            charge_status = CHARGE_MONITOR;
        }
#if 0   
    switch (charge_status)
    {
    case CHARGE_MONITOR:
      //printf("\n%d--enter MON \n",__LINE__);
      //printf("\n%dBat = 0x%x\n",__LINE__,bat_tmp);
      if (bat_tmp >= max_charge_voltage) /*如果当前电压达到最大充电电压，则认为电池已满，不必充电*/
      {
        printf("\n%d--FULL \n",__LINE__);
        charge_stop();
        charge_status = CHARGE_FULL;
        charge_interval = 10;
        break;
      }
      else
      {
        //printf("\n%dstart\n",__LINE__);
        added_charge_time_flag = 0;
        charge_interval = 10;
        charge_status = CHARGE_CHARGING;
        charge_set_charging_current(CHARGE_CURRENT_50);
        charge_start();
        break;
      }

    case CHARGE_TEMP_STATUS:/*该状态是暂时状态，目的是区分"没电池"和"过充"判断结果*/
      charge_status = CHARGE_FAILED_OVERCHARGE;
      charge_interval = 10;
      break;
    case CHARGE_CHARGING:
      //printf("\n%d--enter ING \n",__LINE__);
      if (charge_is_charging() > 0) /*充电电路是否在充电*/
      {
        faild_count = 0;
        //printf("\n%dBat=0x%x\n",__LINE__,bat_tmp);
        if (bat_tmp >= (max_charge_voltage + 0x01)) /*如果当前电压达到最大充电电压，开始额外充电*/
        {
          //printf("\n%dbat>max_vol\n",__LINE__);
          charge_interval = 10;
          trickle_charge_time = 0;
          if (added_charge_time_flag > 0) /*如果额外充电已经开始*/
          {
            //printf("\nadded\n");
            added_charge_time_count = added_charge_time_count
                + charge_interval; /*额外充电计数增加*/
            if (added_charge_time_count >= added_charge_time) /*额外充电时间已到，停止充电*/
            {
              added_charge_time_flag = 0;
              added_charge_time_count = 0;
              charge_status = CHARGE_MONITOR;
              charge_stop();
              //printf("\nadded f\n");
              break;
            }
            else
            {
              break;
            }
          }
          else /*如果额外充电还没开始，则额外充电标志added_charge_time_flag置1，开始额外充电*/
          {
            //printf("\nadded st\n");
            added_charge_time_flag = 1;
            added_charge_time_count = 0;
            break;
          }
        }/*end if (bat_tmp >= max_charge_voltage)*/
        else /*当前电压小于最大充电电压*/
        {
          //printf("\nbat<max\n");
          if (charge_debounce > 0)
          {
            if (charge_get_charging_current() >= (charge_debounce - 1)) /*如果充电电流已经达到上次记录的充电失败电流，
                                          则将充电失败记录清除*/
            {
              charge_debounce = 0;
            }
          }
          //printf("\n%s--%d--Bat = %d\n", __FILE__, __LINE__,bat_tmp);
          if (bat_tmp <= LI300)/*若当前电压小于3V,则使用50mA充电*/
          {
            //printf("\ntrickle\n");
            charge_set_charging_current(CHARGE_CURRENT_50);
            trickle_charge_time++;
            if (trickle_charge_time >= CHARGE_TRICKLE_LIMIT) /*若充电30分钟后，电池电压仍然小于3V，则认为该电池失效*/
            {
              charge_status = CHARGE_FAILED_BATTERY;
              trickle_charge_time = 0;
              //printf("\nF_B\n");
            }
            charge_interval = 10;
            break;
          }
          else /*若当前电压大于3V*/
          {
            //printf("\n%dconstant\n",__LINE__);
            trickle_charge_time = 0;
            current_tmp = charge_get_charging_current();
            //printf("\n%dc=0x%x\n",__LINE__,current_tmp);
            if (current_tmp < max_charge_current) /*如果当前充电电流小于最大充电电流，则增大充电电流*/
            {
              //printf("\nadd c\n");
              charge_set_charging_current((unsigned int) (current_tmp + 1));
              charge_interval = 2;/*将循环时间改为1S，以便快速把充电电流升到最大值*/
              break;
            }
            else /*如果当前充电电流等于最大充电电流，则重新把循环时间改为5S*/
            {
              //printf("\nkeep c\n");
              charge_interval = 10;
              break;
            }
          }

        }/*end 当前电压小于最大充电电压*/

      }/*end if (charge_is_charging())*/
      else /*若充电电路没有在充电*/
      {
        //printf("\nch fail\n");
        if (charge_debounce == 0) /*若是第一次检测到不能充电，则重新打开充电电路，目的是排除暂时性抖动的干扰*/
        {
          //printf("\n%ddeb=0x%x\n",__LINE__,charge_debounce);
          charge_debounce = charge_get_charging_current() + 1;/*将当前充电电流+1赋给charge_debounce，*/
          /*下次充电电流如果能正常升到charge_debounce-1,则认为本次不能充电时只是因为暂时性的扰动*/
          charge_stop();
          charge_status = CHARGE_MONITOR;/*再尝试一次充电*/
          added_charge_time_flag = 0;
          charge_interval = 10;
        }
        else /*若第二次仍然检测到充电电路不能充电，则认为充电器的负载能力不能满足当前的充电电流*/
        {
          //printf("\n%ddeb=0x%x\n",__LINE__,charge_debounce);
          charge_debounce = 0;
          current_tmp = charge_get_charging_current();
          //printf("\n%dc=0x%x\n",__LINE__,current_tmp);
          if (current_tmp <= CHARGE_CURRENT_50) /*如果当前充电电流已经减到50mA*/
          {
            faild_count++;
            if (faild_count >= 3) /*是否连续3次50mA都不能充电，如果是，则认为充电失败*/
            {
              //printf("\nf_count = %d\n",faild_count);
              //printf("\nfail 3t\n");
              faild_count = 0;
              //printf("\n%dBat=0x%x\n",__LINE__,bat_tmp);
              if (bat_tmp >= LI410) /*充电失败时，电池电压大于4.1V(可能是当然上残留的电压)，则认为是过充或者是没电池*/
              {
                charge_stop();
                charge_status = CHARGE_TEMP_STATUS;/*把充电状态设置为暂时状态，等下个循环(即5秒后)，
                                  如果还没检测到是电池开关被断开，则认为是过充*/
                charge_interval = 10;
                //printf("\n%d--over ch\n",__LINE__);
                break;
              }/*end if (bat_tmp >= LIBAT_VOLTAGE41)*/
              else /*充电失败时，电池电压小于4.1V，则认为是充电器负载能力不足*/
              {
                charge_stop();
                charge_status = CHARGE_FAILED_OVERLOAD;
                charge_interval = 10;
                //printf("\nF_L\n");
                break;
              }
            }/*end if (faild_count >= 3)*/
            else /*没有连续3次50mA充电失败，则认为充电还没有失败，继续打开充电器尝试充电*/
            {
              //printf("\n50mA again\n");
              added_charge_time_flag = 0;
              charge_interval = 2;
              charge_stop();
              charge_status = CHARGE_MONITOR;
            }
          }/*end if (current_tmp <= CHARGE_CURRENT_50)*/
          else /*如果当前充电电流还没有减到50mA，则减小充电电流，继续尝试充电*/
          {
            max_charge_current = current_tmp - 1;
            added_charge_time_flag = 0;
            charge_interval = 2;
            charge_stop();
            charge_status = CHARGE_MONITOR;
            break;
          }/*end 如果当前充电电流还没有减到50mA*/
        }/*end 若第二次仍然检测到充电电路不能充电*/
      }/*end 若充电电路没有在充电*/
      break;
      /*end case CHARGE_CHARGING:*/
    case CHARGE_NO_BATTERY:
      //printf("\n no_bat_cnt= %d\n",no_bat_cnt);

      break;
    case CHARGE_FAILED_OVERCHARGE:
    case CHARGE_FULL:
      if (bat_tmp < (max_charge_voltage - 0x3))
      {
        charge_status = CHARGE_MONITOR;
      }
      break;
    case CHARGE_FAILED_OVERLOAD:
      retry_charge_cnt = retry_charge_cnt + charge_interval;
      if (retry_charge_cnt > CHARGE_RETRY_TIME)
      {
        retry_charge_cnt = 0;
        charge_status = CHARGE_MONITOR;
      }
      break;
    case CHARGE_PAUSE:
    case CHARGE_STOP:
    case CHARGE_FAILED_BATTERY:
    default:
      charge_stop();
      break;
    } /*end switch (charge_staus)*/
#else
        state_machine();    
#endif    
    }/*end if (charge_is_dc5v_on())*/
    else
    {
        no_DC5V_debounce++;                 
        if (no_DC5V_debounce ==1)
        {

            charge_interval = 1;
            //printf("\nDC5V fail\n");
        }
        else if (no_DC5V_debounce>10) //确认是充电电源已经移除，而非
        {                                         //负载电流的波动引起的检测异常
            no_DC5V_debounce = 0;
            charge_interval = 2;
            charge_status = CHARGE_NO_DC5V;

        }
        else
        {
        	
        }
        charge_stop();
    }
}

void state_machine(void)
{
    unsigned int  current_tmp;  
    unsigned int bat_tmp;      
    
    bat_tmp = charge_get_battery_voltage();    
    
    switch (charge_status)
    {
    case CHARGE_MONITOR:
      //printf("\n%d--enter MON \n",__LINE__);
      //printf("\n%dBat = 0x%x\n",__LINE__,bat_tmp);
        if (bat_tmp >= max_charge_voltage) /*如果当前电压达到最大充电电压，则认为电池已满，不必充电*/
        {
            printf("\n%d--FULL \n",__LINE__);
            charge_stop();
            charge_status = CHARGE_FULL;
            charge_interval = 10;
            break;
        }
        else
        {
            //printf("\n%dstart\n",__LINE__);
            added_charge_time_flag = 0;
            charge_interval = 10;
            charge_status = CHARGE_CHARGING;
            charge_set_charging_current(CHARGE_CURRENT_50);
            charge_start();
            break;
        }
    
    case CHARGE_TEMP_STATUS:/*该状态是暂时状态，目的是区分"没电池"和"过充"判断结果*/
        charge_status = CHARGE_FAILED_OVERCHARGE;
        charge_interval = 10;
        break;
    case CHARGE_CHARGING:
        //printf("\n%d--enter ING \n",__LINE__);
        if (charge_is_charging() > 0) /*充电电路是否在充电*/
        {
            faild_count = 0;
            //printf("\n%dBat=0x%x\n",__LINE__,bat_tmp);
            if (bat_tmp >= (max_charge_voltage + 0x01)) /*如果当前电压达到最大充电电压，开始额外充电*/
            {
                //printf("\n%dbat>max_vol\n",__LINE__);
                charge_interval = 10;
                trickle_charge_time = 0;
                if (added_charge_time_flag > 0) /*如果额外充电已经开始*/
                {
                  //printf("\nadded\n");
                    added_charge_time_count = added_charge_time_count
                        + charge_interval; /*额外充电计数增加*/
                    if (added_charge_time_count >= added_charge_time) /*额外充电时间已到，停止充电*/
                    {
                        added_charge_time_flag = 0;
                        added_charge_time_count = 0;
                        charge_status = CHARGE_MONITOR;
                        charge_stop();
                        //printf("\nadded f\n");
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
                else /*如果额外充电还没开始，则额外充电标志added_charge_time_flag置1，开始额外充电*/
                {
                    //printf("\nadded st\n");
                    added_charge_time_flag = 1;
                    added_charge_time_count = 0;
                    break;
                }
            }/*end if (bat_tmp >= max_charge_voltage)*/
            else /*当前电压小于最大充电电压*/
            {
                //printf("\nbat<max\n");
                if (charge_debounce > 0)
                {
                    if (charge_get_charging_current() >= (charge_debounce - 1)) /*如果充电电流已经达到上
                                                        次记录的充电失败电流，则将充电失败记录清除*/
                    {
                        charge_debounce = 0;
                    }
                }
                //printf("\n%s--%d--Bat = %d\n", __FILE__, __LINE__,bat_tmp);
                if (bat_tmp <= LI300)/*若当前电压小于3V,则使用50mA充电*/
                {
                    //printf("\ntrickle\n");
                    charge_set_charging_current(CHARGE_CURRENT_50);
                    trickle_charge_time++;
                    if (trickle_charge_time >= CHARGE_TRICKLE_LIMIT) /*若充电30分钟后，
                                                                                电池电压仍然小于3V，则认为该电池失效*/
                    {
                        charge_status = CHARGE_FAILED_BATTERY;
                        trickle_charge_time = 0;
                        //printf("\nF_B\n");
                    }
                    charge_interval = 10;
                    break;
                }
                else /*若当前电压大于3V*/
                {
                    //printf("\n%dconstant\n",__LINE__);
                    trickle_charge_time = 0;
                    current_tmp = charge_get_charging_current();
                    //printf("\n%dc=0x%x\n",__LINE__,current_tmp);
                    if (current_tmp < max_charge_current) /*如果当前充电电流小于最大充电电流，则增大充电电流*/
                    {
                        //printf("\nadd c\n");
                        charge_set_charging_current((unsigned int) (current_tmp + 1));
                        charge_interval = 2;/*将循环时间改为1S，以便快速把充电电流升到最大值*/
                        break;
                    }
                    else /*如果当前充电电流等于最大充电电流，则重新把循环时间改为5S*/
                    {
                        //printf("\nkeep c\n");
                        charge_interval = 10;
                        break;
                    }
                }
        
            }/*end 当前电压小于最大充电电压*/
        
        }/*end if (charge_is_charging())*/
        else /*若充电电路没有在充电*/
        {
            //printf("\nch fail\n");
            if (charge_debounce == 0) /*若是第一次检测到不能充电，则重新打开充电电路，目的是排除暂时性抖动的干扰*/
            {
                //printf("\n%ddeb=0x%x\n",__LINE__,charge_debounce);
                charge_debounce = charge_get_charging_current() + 1;/*将当前充电电流+1赋给charge_debounce，*/
                /*下次充电电流如果能正常升到charge_debounce-1,则认为本次不能充电时只是因为暂时性的扰动*/
                charge_stop();
                charge_status = CHARGE_MONITOR;/*再尝试一次充电*/
                added_charge_time_flag = 0;
                charge_interval = 10;
            }
            else /*若第二次仍然检测到充电电路不能充电，则认为充电器的负载能力不能满足当前的充电电流*/
            {
                //printf("\n%ddeb=0x%x\n",__LINE__,charge_debounce);
                charge_debounce = 0;
                current_tmp = charge_get_charging_current();
                //printf("\n%dc=0x%x\n",__LINE__,current_tmp);
                if (current_tmp <= CHARGE_CURRENT_50) /*如果当前充电电流已经减到50mA*/
                {
                    faild_count++;
                    
                            //if (faild_count >= 3) /*是否连续3次50mA都不能充电，如果是，则认为充电失败*/
                    if (faild_count <3) /*没有连续3次50mA充电失败，则认为充电还没有失败，继续打开充电器尝试充电*/
                    {
                        //printf("\n50mA again\n");
                        added_charge_time_flag = 0;
                        charge_interval = 2;
                        charge_stop();
                        charge_status = CHARGE_MONITOR;
                        break;
                    }       
                /*是否连续3次50mA都不能充电，如果是，则认为充电失败*/                   
                    //printf("\nf_count = %d\n",faild_count);
                    //printf("\nfail 3t\n");
                    faild_count = 0;
                    //printf("\n%dBat=0x%x\n",__LINE__,bat_tmp);
                    /*充电失败时，电池电压大于4.1V(可能是当然上残留的电压)，则认为是过充或者是没电池*/
                    if (bat_tmp >= LI410) 
                    {
                        charge_stop();
                        charge_status = CHARGE_TEMP_STATUS;/*把充电状态设置为暂时状态，等下个循环(即5秒后)，
                                          如果还没检测到是电池开关被断开，则认为是过充*/
                        charge_interval = 10;
                        //printf("\n%d--over ch\n",__LINE__);
                        break;
                    }/*end if (bat_tmp >= LIBAT_VOLTAGE41)*/
                    else /*充电失败时，电池电压小于4.1V，则认为是充电器负载能力不足*/
                    {
                        charge_stop();
                        charge_status = CHARGE_FAILED_OVERLOAD;
                        charge_interval = 10;
                        //printf("\nF_L\n");
                        break;
                    }
                }/*end if (current_tmp <= CHARGE_CURRENT_50)*/
                else /*如果当前充电电流还没有减到50mA，则减小充电电流，继续尝试充电*/
                {
                    max_charge_current = current_tmp - 1;
                    added_charge_time_flag = 0;
                    charge_interval = 2;
                    charge_stop();
                    charge_status = CHARGE_MONITOR;
                    break;
                }/*end 如果当前充电电流还没有减到50mA*/
            }/*end 若第二次仍然检测到充电电路不能充电*/
        }/*end 若充电电路没有在充电*/
        break;
        /*end case CHARGE_CHARGING:*/
    case CHARGE_NO_BATTERY:
        //printf("\n no_bat_cnt= %d\n",no_bat_cnt);
    
        break;
    case CHARGE_FAILED_OVERCHARGE:
    case CHARGE_FULL:
        if (bat_tmp < (max_charge_voltage - 0x3))
        {
            charge_status = CHARGE_MONITOR;
        }
        break;
    case CHARGE_FAILED_OVERLOAD:
        retry_charge_cnt = retry_charge_cnt + charge_interval;
        if (retry_charge_cnt > CHARGE_RETRY_TIME)
        {
            retry_charge_cnt = 0;
            charge_status = CHARGE_MONITOR;
        }
        break;
    case CHARGE_PAUSE:
    case CHARGE_STOP:
    case CHARGE_FAILED_BATTERY:
    default:
        charge_stop();
        break;
    } /*end switch (charge_staus)*/  
    return;  
}

unsigned char charge_get_max_charge_voltage(void)
{ 
    unsigned int max_v=0,ref_v=0;
    
    max_v = max_voltage_config;
    ref_v = ref_voltage_config;
    max_v = max_v * 10;
    ref_v = ref_v * 10;
    if(ref_v ==0)
    {
        return 0;
    }
    max_v = max_v * (15000 / ref_v);
    max_v = ((max_v - 21000) / 385) +1;
    if (max_v > LIMAX)
    {
        max_v = LIMAX;
    }
    return max_v;
}


/*!
 *  \brief set the charge parameter
 *  \par  Description
 *        the function will set the parameter for charge process
 *
 *  \param[in]  vol---set the max voltage of battery
 *              current---set the max current of charge
 *              time-----set the time delay to avoid the invalid full ,the unit is minute
 *  \reval   1---set success
 *  \ingroup charge
 *
 *  \par
 */
//unsigned char act_set_max_param(unsigned int volt, unsigned int current,
//    unsigned char time)
//{
//  max_charge_voltage = volt;
//  max_charge_current = current;
//  added_charge_time = time * 120;
//  return 1;
//}
int act_set_max_param(charge_param_t   *para_pt)
{
    if(para_pt == NULL)
    {
        return -1;
    }
    max_charge_voltage = para_pt->max_voltage;
    max_charge_current = para_pt->max_current;
    added_charge_time = para_pt->added_time  * 120;
    return 0;
}


int act_get_max_param(charge_param_t   *para_pt)
{
    if(para_pt == NULL)
    {
        return -1;
    }
    para_pt->max_voltage = max_charge_voltage;
    para_pt->max_current = max_charge_current;
    para_pt->added_time  = added_charge_time / 120;
    return 0;
}

/*!
 *  \brief start or stop the charge
 *  \par  Description
 *        the function will start the charge or stop the battery charge
 *  \param[in]   type
 *               1--start charge
 *               0--pause  charge
 *  \reval   1---set success
 *  \ingroup charge
 *
 *  \par
 */
unsigned char act_set_pmu_charge(unsigned char type)
{
    if (type == 1)
    {
        charge_status = CHARGE_MONITOR;
        charge_disable_trickle();
        trickle_charge_time = 0;
        added_charge_time_count = 0;
        added_charge_time_flag = 0;
        charge_debounce = 0;
        charge_count = 0;
        last_bat_voltage = charge_get_battery_voltage();
    }
    else if (type == 0)
    {
        charge_status = CHARGE_STOP;
        trickle_charge_time = 0;
        added_charge_time_count = 0;
        added_charge_time_flag = 0;
        charge_debounce = 0;
        charge_stop();
    }
    else
    {
        return -1;
    }
  
    return 0;
}


void charge_vram_param_init(void)
{
//  vram_param_t param;
//  syscfg_t *syscfg = (void*)malloc(sizeof(syscfg_t));
//
//  param.pbuf = (void*)syscfg;
//  param.length = sizeof(syscfg_t);
//  param.offset = 0;
//  param.info_id = APP_INFO_PARTION;
//  param.info_subid = APP_SYSCFG;
//  vram_read(&param);
//
//  max_voltage_config = syscfg->charge_param.max_voltage;
//  ref_voltage_config = syscfg->charge_param.ref_voltage;
//  charge_time_limit_config = syscfg->charge_param.time_limit;
//  free(syscfg);
}

/*!
 * \brief  the charge driver init
 * \par    Description
 *         to init the charge driver include the usb init and detect the usb in or out
 *         meanwhile to apply a 500ms timer to detect the usb status and check the system
 *         voltage
 *
 *
 * \par
 */
//static int charge_init(void)
//{
//  int ret = 0;
//  timer_arg_t timer_arg;
//  unsigned char timerNo = 0;
//
//  charge_vram_param_init();
//  ret = usb_init();
//  if (ret == 1)
//  {
//    act_writel(act_readl((unsigned int) PMU_CTL) & 0x7fffffff, (unsigned int) PMU_CTL);
//    setVoltage(VDD175, VDD);
//    disChangeVoltage(1);
//    do_vbus_on();
//  }
//  else if (ret == -1)
//  {
//    //printf("usb init failed\n");
//  }
//  else
//  {
//    /* if usb out ,must open the low battery resert */
//    act_writel(act_readl((unsigned int) PMU_CTL) | (0x01 << 31), (unsigned int) PMU_CTL); 
//  }

//  timer_arg.timerHandler = (void *) usbCheck;
//  timer_arg.timerValue = 50;
//  timer_arg.type = TIMER_CYCLE;
//
//  det_timer.timerHandler = (void *) usb_check_again;
//  det_timer.timerValue = 0x0;
//  det_timer.type = TIMER_SINGLE;
//  det_timerNo = set_timer(&det_timer);
//  timerNo = set_timer(&timer_arg);
//  return ret;

//}

unsigned char act_set_charge_current(unsigned char level)
{
    //  charge_set_charging_current(level);
    return 0;
}

/*为了跺过QAC的5层嵌套限制，把该部分独立为一函数实现*/
static unsigned int count = 0;
void check_count(void)
{
    if (count == 0)
    {
        charge_disable_trickle();
        charge_set_charging_current(CHARGE_CURRENT_50);
        charge_start();
        count++;
    }
    else if (count < 4)
    {
        count++;
    }
    else if (count == 4)
    {
        if (charge_is_charging() > 0)
        {
            is_dc5v_on = 1;
        }
                    
        charge_stop();
        count++;
    }
    else if ( count < 10)
    {
        count++;
    }
    else // (count >= 20)
    {
        count = 0;
    }    
    return;
}

void charge_dc5v_check(void)
{ 

#ifdef CHARGE_ADAPTER_SUPPORT
    if (!is_dc5v_on)
    {
        if (charge_detect_dc5v() > 0)
        {
            if (charge_get_battery_voltage() <= LI360)
            {
                is_dc5v_on = 1;
                count = 0;
                charge_stop();
            }
            else
            {
                
                check_count();       
            }
        }
        else
        {
            is_dc5v_on = 0;
            count = 0;
          // charge_stop();     
        }
        
    }
    else
    {
        if (!charge_detect_dc5v())
        {
            is_dc5v_on = 0;
            count = 0;
        }
    }

#else
//  is_dc5v_on = act_usbConnect();  
    is_dc5v_on = 0;
#endif
    return;
}


unsigned char charge_is_dc5v_on(void)
{
    return is_dc5v_on;
}


/*!
 * \brief  the 500ms timer handle
 * \par  Description
 *         the function will check the usb and check the system voltage
 *
 * \param[in] null
 * \return null
 * \ingroup charge driver
 *  \par
 */
//void usbCheck(void)
//{
//  act_usb_check();
//  charge_dc5v_check();
//  charge_low_power_check();
//  if (charge_time_limit_count <= 720000) //让计数到720000(100小时)后，停止计数，以防止超出数值范围后重新归零
//  {
//    charge_time_limit_count ++;
//  }
//}

