/*******************************************************************************
 *                              NOYA1100
 *                            Module: drv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-11          1.0             build this file 
*******************************************************************************/
/*!
 * \file     key_inner.c
 * \brief    实现按键扫描
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/11
*******************************************************************************/

/*本模块的头文件*/

#include "key_inner.h"
#include "key_drv.h"
#include "key_buf_queue.h"
#include "ucos/jiffies.h"
#include "key_sys.h"
#include "key_config.h"

/*记录驱动被打开几次*/
static int count_module_open =0;
/*定时器：定时扫描按键 */
static struct timer_list timer_key_scan;
/* 系统定时器 时间参数 */
static unsigned long *p_jiffies;

static int last_scan_matrix_val =0; /*记录上次有效的scan matrix value*/
static unsigned int time_cnt_scan_matrix = 0;/*计数器*/

static unsigned int temp_key_buf[KEY_MESSAGE_BUF_SIZE];
static int hold_state = 0x0;    /* 0x0: unholded; 0x1: holded;*/

static volatile int has_save_all_up = 1;

static int key_scan_handler(unsigned long arg)
{
    int key_val_all=0;
    int key_val_adc=0;
    int key_val_scan_matrix=0;
    int key_val_single_gpio=0;
    int key_val_gpio_matrix=0;
    
    
    key_val_adc = get_adc_keyval();
    
    key_val_single_gpio = get_single_gpio_keyval();
    
    if(key_val_single_gpio==KEY_NULL)
    {
        key_val_gpio_matrix = get_gpio_matrix_keyval();
    }
    
    key_val_scan_matrix = get_scan_matrix_keyval();
      
    key_val_all = key_val_adc | key_val_single_gpio |key_val_gpio_matrix|last_scan_matrix_val;
    
    //printf("key_val_adc = %x\n",key_val_adc);
    //printf("key_val_single_gpio = %x\n",key_val_single_gpio);
    //printf("key_val_gpio_matrix = %x\n",key_val_gpio_matrix);
    //printf("key_val_scan_matrix = %x  last_scan_matrix_val=%x\n",key_val_scan_matrix,last_scan_matrix_val);
    if(key_val_all !=0)
    {
       // printf("key_val_all = %x\n",key_val_all);
    }
        
    if (( KEY_LOCK & key_val_all ) != 0)
    {
        hold_state = 1;
        key_val_all &= ~KEY_LOCK;/* hold键值通过hold_state变量进行标志, 不通过键值发送*/
    }
    else
    {
        hold_state = 0;
    }
    
    if ( KEY_NULL != key_val_all )
    {
        has_save_all_up = key_val_all;
        qstore( (unsigned int)key_val_all );
    }
    else
    {
        if(KEY_NULL !=has_save_all_up)
        {
            qstore_up(KEY_NULL);
            has_save_all_up = KEY_NULL;    
        }
    }
    
    /* KEY_SCANE_TIME_INTERVAL * 10ms. */
    mod_timer(&(timer_key_scan), *p_jiffies + KEY_SCANE_TIME_INTERVAL);
    
    
    return 0;
}


/******************************************************************************/
/*!
* \par  Description:
*     create detect keyup timer
* \param[in]   none
* \return      
*******************************************************************************/
static int _create_key_scan_timer( void )
{
    init_timer( &timer_key_scan );
    timer_key_scan.function = (void(*)(unsigned long))key_scan_handler;
    
    mod_timer( &(timer_key_scan), *p_jiffies + KEY_SCANE_TIME_INTERVAL );
    
    return 0;
}


/******************************************************************************/
/*!
* \par  Description:
*     delete detect keyup timer
* \param[in]   none
* \return      
*******************************************************************************/
static int _delete_key_scan_timer( void )
{
    del_timer_sync( &timer_key_scan );
    
    return 0;
}


void key_init(void)
{
    key_scan_matrix_init();
    key_adc_init();    
    key_single_gpio_init();
    key_gpio_matrix_init(); 
}


void key_exit(void)
{
    key_scan_matrix_exit();
    key_adc_exit();    
    key_single_gpio_exit();
    key_gpio_matrix_exit(); 
}



/******************************************************************************/
/*!                    
* \par  Description:
* we simulate a queue in a arrar buffer, since it's small size, we just shift left  
* when we read the key messages.
* \param[in]   char * buffer, dest. addr of reading
* \param[in]   size_t cnt, the number of byte to read 
* \return      the result  
*******************************************************************************/
int act_buttons_read( char * buffer, size_t num_read_byte)
{
    unsigned int num_saved_key_byte;
    unsigned int num_read_4byte, i ;
    
    if ( is_empty() == 1 ) 
    {
        return -EAGAIN;
    }
    
    num_saved_key_byte = sizeof_key_buf() * (sizeof(unsigned int));
    if ( num_read_byte > num_saved_key_byte )
    {
        num_read_byte = num_saved_key_byte;
    }
    
    num_read_4byte = num_read_byte/( sizeof (unsigned int) );
    if( 0 == num_read_4byte )
    {
        return 0;
    }
    
    for( i=0; i<num_read_4byte; i++ )
    {              
        temp_key_buf[i] = qretrieve();
    }
    copy_to_user(buffer, temp_key_buf, num_read_byte);
    
    return (int)( num_read_4byte * (sizeof(unsigned int)) );
}


/******************************************************************************/
/*!                    
* \par  Description:
*    act_buttons_write,write data to driver,reserve not use  
* \param[in]   none
* \return      the result  
*******************************************************************************/
ssize_t act_buttons_write(void)
{
    return 0;
}


/******************************************************************************/
/*!                    
* \par  Description:
*   act_buttons_ioctl,  ioctl with applications ,reserve ,not use  
* \param[in]   none
* \return      the result  
*******************************************************************************/
int act_buttons_ioctl(unsigned int cmd, unsigned long arg)
{
 
    switch(cmd) 
    {
    case KEY_GET_LOCKKEY_STATU:
        put_user( hold_state, (unsigned long *)arg );
        break;
        
    default:
        return -EINVAL;
    }
     
    return 0;
}


/******************************************************************************/
/*!                    
* \par  Description:
*   init key module ,read key map throgh vram infterface  
* \param[in]   none
* \return      the result  
*******************************************************************************/
int act_buttons_open(void)
{
    count_module_open ++;
    if( count_module_open > 1 )
    {
        return 0;
    }
    p_jiffies = kernel_sym( "jiffies" );
    
    _create_key_scan_timer();
    
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     this function is called when close key driver,
*     realse irq isr and relase some important resource 
* \param[in]   c  para3
* \return      the result  
*******************************************************************************/
int act_buttons_release(void)
{
    count_module_open--;
    if ( count_module_open > 0 )
    {
        return 0;
    }
    
    _delete_key_scan_timer();
    
    return 0;
}

