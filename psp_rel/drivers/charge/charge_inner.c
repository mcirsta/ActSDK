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
    /*�ö�ʱ�������е�ص���ʵʱ��⣬20090805��cccw*/
    p_jiffies = kernel_sym("jiffies");    
    init_timer(&battery_detect_timer);
    battery_detect_timer.function = (void *) act_get_battery;
    mod_timer(&battery_detect_timer, *p_jiffies + 450); /* first one delay 3s synpro with usb */
    bat_power=charge_get_battery_voltage();//���ⶨʱ���жϿ�ʼ���bat_powerǰ��Ӧ�û�ȡbat_power
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

void act_charge(void) /*0.5S��ѯ���*/
{
  // printf("\n  ch drv\n");
    unsigned int bat_tmp;
    if(charge_status ==CHARGE_STOP)
    {
        return;
    }
    bat_tmp = charge_get_battery_voltage();
    //printf("\n--%d-- BAT = 0x%x\n",__LINE__,bat_tmp);
    if (charge_status != CHARGE_NO_BATTERY) /*�����ǵ�ؼ��ĶϿ���ⲿ�֣�����0.5S����һ��*/
    {
      //printf("\n--%d-- BAT = 0x%x\n",__LINE__,bat_tmp);
        if (bat_tmp <= LI220) /*�����⵽��ص�ѹ<2.2V������Ϊ��ؿ��ر��Ͽ�*/
        {
            //printf("\n%d NO_BAT bat<2.2",__LINE__);
            charge_status = CHARGE_NO_BATTERY; /*�����״̬��ΪCHARGE_NO_BATTERY*/
            no_bat_cnt = 0; /*��no_bat_cnt��Ϊ0��ʹ�ú����ؿ��رպϵļ��*/
                                    /*����5��ѭ�����ٽ���(��ο�����պϵļ���ע��)*/
            charge_interval = 2; /*���������̵�ʱ������Ϊ1S*/
            charge_stop();
        }
        else /*�����ص�ѹ>2.2V*/
        {
            if (bat_tmp < last_bat_voltage) /* �����ص�ѹ���½� */
            {
                bat_voltage_fall++; /*�ۼƵ�ص�ѹ�½��Ĵ���*/
                //printf("\n bat_voltage_fall= %d ",bat_voltage_fall);
                if (bat_voltage_fall >= 5) /* �����ص�ѹ�½��Ĵ�������5�Σ�����Ϊ��ؿ��ر��Ͽ� */
                {
                    charge_status = CHARGE_NO_BATTERY;
                    no_bat_cnt = 0;
                    charge_interval = 2;
                    charge_stop();
                    //printf("\n%d NO_BAT bat fall",__LINE__);
                }
                last_bat_voltage = bat_tmp;
            }
        } /*��ضϿ�������*/
    }
    else
    {
        if (no_bat_cnt < 5)/*��no_bat_cntС��5(�ڼ�⵽��ؿ��ر��Ͽ�ʱ���˱�������Ϊ0)��*/
        /*�򲻼���ؿ����Ƿ���ϣ�Ŀ����Ϊ�˱ܿ��նϿ�����˲�䣬�����ϲ����ĵ�ѹ�Ե�ؼ���Ӱ��*/
        {
        }
        else if (no_bat_cnt == 5)/*��no_bat_cnt=5ʱ����ʼ����ؿ����Ƿ񱻺���*/
        {
            if (bat_tmp > LI380)/*��һ���ж�:����⵽��ص�ѹ����3.8V������Ϊ�����Ѿ�����*/
            {
                restart_charge_flag = 1;
                charge_stop();
                no_bat_cnt = 4;
                //printf("\n%d Bat back > LI380\n",__LINE__);
            }
            else /*����⵽��ص�ѹ����3.8V�����еڶ������жϣ����򿪳���·���ٲ�����ض˵�ѹ*/
            {
                charge_set_charging_current(CHARGE_CURRENT_50);
                charge_start();
              //printf("\n--%d-- charge_start()\n",__LINE__);
            }
        }
        else /*no_bat_cnt>5�����еڶ������ж�*/
        {
            if (bat_tmp < LI390) /*����⵽��ص�ѹС��3.9V�����ؿ����Ѿ������ϣ�������Ϊ��ؿ����Ա��Ͽ�*/
            {
                restart_charge_flag = 1;
                //printf("\n%d Bat back < LI390\n",__LINE__);
            }
            charge_stop();/*�رճ���·�����½��е�һ�����ж�*/
            no_bat_cnt = 4;
        }
  
        no_bat_cnt++;   
    }
    
  //  if ((charge_time_limit_count <= (charge_time_limit_config * 120)) || (!charge_time_limit_flag))
  //  {
  //    charge_count++;
  //    if ((charge_count % charge_interval) == 0)
  //    {
  //      charge_check(); /*���г��������̣����ݳ��״̬�Ĳ�ͬ��1S��5S����1��*/
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
        charge_check(); /*���г��������̣����ݳ��״̬�Ĳ�ͬ��1S��5S����1��*/
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
      if (bat_tmp >= max_charge_voltage) /*�����ǰ��ѹ�ﵽ������ѹ������Ϊ������������س��*/
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

    case CHARGE_TEMP_STATUS:/*��״̬����ʱ״̬��Ŀ��������"û���"��"����"�жϽ��*/
      charge_status = CHARGE_FAILED_OVERCHARGE;
      charge_interval = 10;
      break;
    case CHARGE_CHARGING:
      //printf("\n%d--enter ING \n",__LINE__);
      if (charge_is_charging() > 0) /*����·�Ƿ��ڳ��*/
      {
        faild_count = 0;
        //printf("\n%dBat=0x%x\n",__LINE__,bat_tmp);
        if (bat_tmp >= (max_charge_voltage + 0x01)) /*�����ǰ��ѹ�ﵽ������ѹ����ʼ������*/
        {
          //printf("\n%dbat>max_vol\n",__LINE__);
          charge_interval = 10;
          trickle_charge_time = 0;
          if (added_charge_time_flag > 0) /*����������Ѿ���ʼ*/
          {
            //printf("\nadded\n");
            added_charge_time_count = added_charge_time_count
                + charge_interval; /*�������������*/
            if (added_charge_time_count >= added_charge_time) /*������ʱ���ѵ���ֹͣ���*/
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
          else /*��������绹û��ʼ����������־added_charge_time_flag��1����ʼ������*/
          {
            //printf("\nadded st\n");
            added_charge_time_flag = 1;
            added_charge_time_count = 0;
            break;
          }
        }/*end if (bat_tmp >= max_charge_voltage)*/
        else /*��ǰ��ѹС��������ѹ*/
        {
          //printf("\nbat<max\n");
          if (charge_debounce > 0)
          {
            if (charge_get_charging_current() >= (charge_debounce - 1)) /*����������Ѿ��ﵽ�ϴμ�¼�ĳ��ʧ�ܵ�����
                                          �򽫳��ʧ�ܼ�¼���*/
            {
              charge_debounce = 0;
            }
          }
          //printf("\n%s--%d--Bat = %d\n", __FILE__, __LINE__,bat_tmp);
          if (bat_tmp <= LI300)/*����ǰ��ѹС��3V,��ʹ��50mA���*/
          {
            //printf("\ntrickle\n");
            charge_set_charging_current(CHARGE_CURRENT_50);
            trickle_charge_time++;
            if (trickle_charge_time >= CHARGE_TRICKLE_LIMIT) /*�����30���Ӻ󣬵�ص�ѹ��ȻС��3V������Ϊ�õ��ʧЧ*/
            {
              charge_status = CHARGE_FAILED_BATTERY;
              trickle_charge_time = 0;
              //printf("\nF_B\n");
            }
            charge_interval = 10;
            break;
          }
          else /*����ǰ��ѹ����3V*/
          {
            //printf("\n%dconstant\n",__LINE__);
            trickle_charge_time = 0;
            current_tmp = charge_get_charging_current();
            //printf("\n%dc=0x%x\n",__LINE__,current_tmp);
            if (current_tmp < max_charge_current) /*�����ǰ������С�����������������������*/
            {
              //printf("\nadd c\n");
              charge_set_charging_current((unsigned int) (current_tmp + 1));
              charge_interval = 2;/*��ѭ��ʱ���Ϊ1S���Ա���ٰѳ������������ֵ*/
              break;
            }
            else /*�����ǰ�������������������������°�ѭ��ʱ���Ϊ5S*/
            {
              //printf("\nkeep c\n");
              charge_interval = 10;
              break;
            }
          }

        }/*end ��ǰ��ѹС��������ѹ*/

      }/*end if (charge_is_charging())*/
      else /*������·û���ڳ��*/
      {
        //printf("\nch fail\n");
        if (charge_debounce == 0) /*���ǵ�һ�μ�⵽���ܳ�磬�����´򿪳���·��Ŀ�����ų���ʱ�Զ����ĸ���*/
        {
          //printf("\n%ddeb=0x%x\n",__LINE__,charge_debounce);
          charge_debounce = charge_get_charging_current() + 1;/*����ǰ������+1����charge_debounce��*/
          /*�´γ������������������charge_debounce-1,����Ϊ���β��ܳ��ʱֻ����Ϊ��ʱ�Ե��Ŷ�*/
          charge_stop();
          charge_status = CHARGE_MONITOR;/*�ٳ���һ�γ��*/
          added_charge_time_flag = 0;
          charge_interval = 10;
        }
        else /*���ڶ�����Ȼ��⵽����·���ܳ�磬����Ϊ������ĸ��������������㵱ǰ�ĳ�����*/
        {
          //printf("\n%ddeb=0x%x\n",__LINE__,charge_debounce);
          charge_debounce = 0;
          current_tmp = charge_get_charging_current();
          //printf("\n%dc=0x%x\n",__LINE__,current_tmp);
          if (current_tmp <= CHARGE_CURRENT_50) /*�����ǰ�������Ѿ�����50mA*/
          {
            faild_count++;
            if (faild_count >= 3) /*�Ƿ�����3��50mA�����ܳ�磬����ǣ�����Ϊ���ʧ��*/
            {
              //printf("\nf_count = %d\n",faild_count);
              //printf("\nfail 3t\n");
              faild_count = 0;
              //printf("\n%dBat=0x%x\n",__LINE__,bat_tmp);
              if (bat_tmp >= LI410) /*���ʧ��ʱ����ص�ѹ����4.1V(�����ǵ�Ȼ�ϲ����ĵ�ѹ)������Ϊ�ǹ��������û���*/
              {
                charge_stop();
                charge_status = CHARGE_TEMP_STATUS;/*�ѳ��״̬����Ϊ��ʱ״̬�����¸�ѭ��(��5���)��
                                  �����û��⵽�ǵ�ؿ��ر��Ͽ�������Ϊ�ǹ���*/
                charge_interval = 10;
                //printf("\n%d--over ch\n",__LINE__);
                break;
              }/*end if (bat_tmp >= LIBAT_VOLTAGE41)*/
              else /*���ʧ��ʱ����ص�ѹС��4.1V������Ϊ�ǳ����������������*/
              {
                charge_stop();
                charge_status = CHARGE_FAILED_OVERLOAD;
                charge_interval = 10;
                //printf("\nF_L\n");
                break;
              }
            }/*end if (faild_count >= 3)*/
            else /*û������3��50mA���ʧ�ܣ�����Ϊ��绹û��ʧ�ܣ������򿪳�������Գ��*/
            {
              //printf("\n50mA again\n");
              added_charge_time_flag = 0;
              charge_interval = 2;
              charge_stop();
              charge_status = CHARGE_MONITOR;
            }
          }/*end if (current_tmp <= CHARGE_CURRENT_50)*/
          else /*�����ǰ��������û�м���50mA�����С���������������Գ��*/
          {
            max_charge_current = current_tmp - 1;
            added_charge_time_flag = 0;
            charge_interval = 2;
            charge_stop();
            charge_status = CHARGE_MONITOR;
            break;
          }/*end �����ǰ��������û�м���50mA*/
        }/*end ���ڶ�����Ȼ��⵽����·���ܳ��*/
      }/*end ������·û���ڳ��*/
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
        else if (no_DC5V_debounce>10) //ȷ���ǳ���Դ�Ѿ��Ƴ�������
        {                                         //���ص����Ĳ�������ļ���쳣
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
        if (bat_tmp >= max_charge_voltage) /*�����ǰ��ѹ�ﵽ������ѹ������Ϊ������������س��*/
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
    
    case CHARGE_TEMP_STATUS:/*��״̬����ʱ״̬��Ŀ��������"û���"��"����"�жϽ��*/
        charge_status = CHARGE_FAILED_OVERCHARGE;
        charge_interval = 10;
        break;
    case CHARGE_CHARGING:
        //printf("\n%d--enter ING \n",__LINE__);
        if (charge_is_charging() > 0) /*����·�Ƿ��ڳ��*/
        {
            faild_count = 0;
            //printf("\n%dBat=0x%x\n",__LINE__,bat_tmp);
            if (bat_tmp >= (max_charge_voltage + 0x01)) /*�����ǰ��ѹ�ﵽ������ѹ����ʼ������*/
            {
                //printf("\n%dbat>max_vol\n",__LINE__);
                charge_interval = 10;
                trickle_charge_time = 0;
                if (added_charge_time_flag > 0) /*����������Ѿ���ʼ*/
                {
                  //printf("\nadded\n");
                    added_charge_time_count = added_charge_time_count
                        + charge_interval; /*�������������*/
                    if (added_charge_time_count >= added_charge_time) /*������ʱ���ѵ���ֹͣ���*/
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
                else /*��������绹û��ʼ����������־added_charge_time_flag��1����ʼ������*/
                {
                    //printf("\nadded st\n");
                    added_charge_time_flag = 1;
                    added_charge_time_count = 0;
                    break;
                }
            }/*end if (bat_tmp >= max_charge_voltage)*/
            else /*��ǰ��ѹС��������ѹ*/
            {
                //printf("\nbat<max\n");
                if (charge_debounce > 0)
                {
                    if (charge_get_charging_current() >= (charge_debounce - 1)) /*����������Ѿ��ﵽ��
                                                        �μ�¼�ĳ��ʧ�ܵ������򽫳��ʧ�ܼ�¼���*/
                    {
                        charge_debounce = 0;
                    }
                }
                //printf("\n%s--%d--Bat = %d\n", __FILE__, __LINE__,bat_tmp);
                if (bat_tmp <= LI300)/*����ǰ��ѹС��3V,��ʹ��50mA���*/
                {
                    //printf("\ntrickle\n");
                    charge_set_charging_current(CHARGE_CURRENT_50);
                    trickle_charge_time++;
                    if (trickle_charge_time >= CHARGE_TRICKLE_LIMIT) /*�����30���Ӻ�
                                                                                ��ص�ѹ��ȻС��3V������Ϊ�õ��ʧЧ*/
                    {
                        charge_status = CHARGE_FAILED_BATTERY;
                        trickle_charge_time = 0;
                        //printf("\nF_B\n");
                    }
                    charge_interval = 10;
                    break;
                }
                else /*����ǰ��ѹ����3V*/
                {
                    //printf("\n%dconstant\n",__LINE__);
                    trickle_charge_time = 0;
                    current_tmp = charge_get_charging_current();
                    //printf("\n%dc=0x%x\n",__LINE__,current_tmp);
                    if (current_tmp < max_charge_current) /*�����ǰ������С�����������������������*/
                    {
                        //printf("\nadd c\n");
                        charge_set_charging_current((unsigned int) (current_tmp + 1));
                        charge_interval = 2;/*��ѭ��ʱ���Ϊ1S���Ա���ٰѳ������������ֵ*/
                        break;
                    }
                    else /*�����ǰ�������������������������°�ѭ��ʱ���Ϊ5S*/
                    {
                        //printf("\nkeep c\n");
                        charge_interval = 10;
                        break;
                    }
                }
        
            }/*end ��ǰ��ѹС��������ѹ*/
        
        }/*end if (charge_is_charging())*/
        else /*������·û���ڳ��*/
        {
            //printf("\nch fail\n");
            if (charge_debounce == 0) /*���ǵ�һ�μ�⵽���ܳ�磬�����´򿪳���·��Ŀ�����ų���ʱ�Զ����ĸ���*/
            {
                //printf("\n%ddeb=0x%x\n",__LINE__,charge_debounce);
                charge_debounce = charge_get_charging_current() + 1;/*����ǰ������+1����charge_debounce��*/
                /*�´γ������������������charge_debounce-1,����Ϊ���β��ܳ��ʱֻ����Ϊ��ʱ�Ե��Ŷ�*/
                charge_stop();
                charge_status = CHARGE_MONITOR;/*�ٳ���һ�γ��*/
                added_charge_time_flag = 0;
                charge_interval = 10;
            }
            else /*���ڶ�����Ȼ��⵽����·���ܳ�磬����Ϊ������ĸ��������������㵱ǰ�ĳ�����*/
            {
                //printf("\n%ddeb=0x%x\n",__LINE__,charge_debounce);
                charge_debounce = 0;
                current_tmp = charge_get_charging_current();
                //printf("\n%dc=0x%x\n",__LINE__,current_tmp);
                if (current_tmp <= CHARGE_CURRENT_50) /*�����ǰ�������Ѿ�����50mA*/
                {
                    faild_count++;
                    
                            //if (faild_count >= 3) /*�Ƿ�����3��50mA�����ܳ�磬����ǣ�����Ϊ���ʧ��*/
                    if (faild_count <3) /*û������3��50mA���ʧ�ܣ�����Ϊ��绹û��ʧ�ܣ������򿪳�������Գ��*/
                    {
                        //printf("\n50mA again\n");
                        added_charge_time_flag = 0;
                        charge_interval = 2;
                        charge_stop();
                        charge_status = CHARGE_MONITOR;
                        break;
                    }       
                /*�Ƿ�����3��50mA�����ܳ�磬����ǣ�����Ϊ���ʧ��*/                   
                    //printf("\nf_count = %d\n",faild_count);
                    //printf("\nfail 3t\n");
                    faild_count = 0;
                    //printf("\n%dBat=0x%x\n",__LINE__,bat_tmp);
                    /*���ʧ��ʱ����ص�ѹ����4.1V(�����ǵ�Ȼ�ϲ����ĵ�ѹ)������Ϊ�ǹ��������û���*/
                    if (bat_tmp >= LI410) 
                    {
                        charge_stop();
                        charge_status = CHARGE_TEMP_STATUS;/*�ѳ��״̬����Ϊ��ʱ״̬�����¸�ѭ��(��5���)��
                                          �����û��⵽�ǵ�ؿ��ر��Ͽ�������Ϊ�ǹ���*/
                        charge_interval = 10;
                        //printf("\n%d--over ch\n",__LINE__);
                        break;
                    }/*end if (bat_tmp >= LIBAT_VOLTAGE41)*/
                    else /*���ʧ��ʱ����ص�ѹС��4.1V������Ϊ�ǳ����������������*/
                    {
                        charge_stop();
                        charge_status = CHARGE_FAILED_OVERLOAD;
                        charge_interval = 10;
                        //printf("\nF_L\n");
                        break;
                    }
                }/*end if (current_tmp <= CHARGE_CURRENT_50)*/
                else /*�����ǰ��������û�м���50mA�����С���������������Գ��*/
                {
                    max_charge_current = current_tmp - 1;
                    added_charge_time_flag = 0;
                    charge_interval = 2;
                    charge_stop();
                    charge_status = CHARGE_MONITOR;
                    break;
                }/*end �����ǰ��������û�м���50mA*/
            }/*end ���ڶ�����Ȼ��⵽����·���ܳ��*/
        }/*end ������·û���ڳ��*/
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

/*Ϊ�˶��QAC��5��Ƕ�����ƣ��Ѹò��ֶ���Ϊһ����ʵ��*/
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
//  if (charge_time_limit_count <= 720000) //�ü�����720000(100Сʱ)��ֹͣ�������Է�ֹ������ֵ��Χ�����¹���
//  {
//    charge_time_limit_count ++;
//  }
//}

