/*********************************************************************************
 *                       213x---gsensor driver
 *                (c) Copyright 2007-2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * FileName:data_buf.c    Author:yangrui  Date:2009/05/11
 * Description: This file contains functions that operate the  data buffer
 *              in gsensor module,such as read write and ioctl of the data buffer.
 * Others:
 * History:
 *      <author>    <time>       <version >    <desc>
 *      yangrui     2009/05/11     1.0         build this moudle
 *********************************************************************************/

#include "data_buf.h"
#include "gsensor_hw.h"
#include "gsensor.h"
#include "gsensor_sys.h"

#define GBUF_SIZE  10              /****size of data buffer*********/

static void _qstore(gsensor_data_t* ms_data);
static void _qcover(gsensor_data_t* ms_data);

atomic_t w_enable = ATOMIC_INIT(0);

static gsensor_data_t gdata_buf[GBUF_SIZE];
static int q_head = 0;
static int q_rear = 0;
static unsigned int data_num = 0; /*amount of datas in gdata_buf*/
static atomic_t gbuf_lock = ATOMIC_INIT(1);
static gsensor_data_t ms_data;
static int w_enable_copy; /*the copied value of w_enable*/
unsigned long w_interval = 100; /*the initial interval of writing gdata_buf*/

unsigned long * g_jiffies;
struct timer_list g_timer;

/*******************************************************************************
 * Function:       data_buf_write
 * Description:    write datas which are samplied from the sensor into the array of
 *                 gdata_buf
 * Input£º         void
 * Output£º        0:  success of writing operation
 *                 ENOLCK: unable of writing(the array of gdata_buf is locked)
 * Other£º
 *******************************************************************************/
int data_buf_write(void)
{
    int ret;
    /* if function data_buf_read is running, data_buf_write should return immediately*/
    if (atomic_read(&gbuf_lock) == 0)
    {
        return -ENOLCK;
    }

    ret = read_gsensor_data(&ms_data);
    if (ret != 0)
    {
        return ret;
    }

    if (data_num == (GBUF_SIZE))
    {
        _qcover(&ms_data);
    }
    else
    {
        _qstore(&ms_data);
    }

    return 0;
}

/*******************************************************************************
 * Function:       data_buf_read
 * Description:    reading datas in the array of gdata_buf. This function copies datas
 *                 from gdata_buf to user's buffer.
 * Input£º         buffer: the address of the user's buffer.
 *                 count: amount of chars that should be copied.
 * Output£º        amount of chars that have been copied.
 * Other£º
 *******************************************************************************/
ssize_t data_buf_read(char * buffer, size_t count)
{
    char * usr_buf;
    size_t i;
    usr_buf = buffer;

    atomic_set(&gbuf_lock, 0);

    if (data_num == 0) /* gdata_buf Îª¿Õ*/
    {
        atomic_set(&gbuf_lock, 1);
        return -ENODATA;
    }
    else
    {

        if (count > (data_num * sizeof(gsensor_data_t)))
        {
            count = data_num * sizeof(gsensor_data_t);
        }

        count = count / (sizeof(gsensor_data_t));

        for (i = 0; i < count; i++)
        {
            copy_to_user(usr_buf, (char *) (gdata_buf + q_head), sizeof(gsensor_data_t));
            usr_buf = usr_buf + sizeof(gsensor_data_t);
            q_head++;
            q_head = q_head % GBUF_SIZE;
            data_num--;
        }

        //printk("copied datas number is: %d\n", 16 * count);

        atomic_set(&gbuf_lock, 1);
        return (16 * count); /*  copied number*/

    }

}

/*******************************************************************************
 * Function:       data_buf_ioctl
 * Description:    control of writing gdata_buf enable/disable and set/get the writing
 *                 interval.
 * Input£º         cmd: writing gdata_buf enable/disable (GSENSOR_W_ENABLE: enable
 *                      GSENSOR_W_DISABLE: disable) and get/set writing interval.
 * Output£º        0: success of control;   EBADRQC: fail of control
 * Other£º         //
 *******************************************************************************/
int data_buf_ioctl(unsigned int cmd, unsigned long interval)
{
    int ret;
    w_enable_copy = atomic_read(&w_enable);

    switch (cmd)
    {
        case GSENSOR_DISABLE:
        if (w_enable_copy == 1)
        {
            atomic_set(&w_enable, 0);
            del_timer(&g_timer);
            ret = 0;
            return ret;
        }
        break;

        case GSENSOR_ENABLE:
        if (w_enable_copy == 0)
        {
            atomic_set(&w_enable, 1);
            g_timer.expires = *g_jiffies + msecs_to_jiffies((unsigned int) w_interval);
            add_timer(&g_timer);
            ret = 0;
            return ret;
        }
        break;

        case GSENSOR_SET_INTERVAL:
        if (interval < 10)
        {
            interval = 10;
        }

        interval = interval / 10;
        w_interval = interval * 10;
        g_timer.expires = *g_jiffies + msecs_to_jiffies((unsigned int) w_interval);
        ret = 0;
        return ret;               /*function return, so no break at the end of this case*/

        case GSENSOR_GET_INTERVAL:
        *(int *)interval = (int)w_interval;
        ret = 0;
        return ret; /*function return, so no break at the end of this case*/

        default:
        return -EINVAL;
    }
    return -EINVAL;
}

/*******************************************************************************
 * Function:       _qstore
 * Description:    store a date into gdata_buf. This function is used when gdata_buf
 *                 is non-full.
 * Input£º         gsensor_data_t: the data that will be stored.
 * Output£º        void
 * Other£º         //
 *******************************************************************************/
static void _qstore(gsensor_data_t* store_data)
{
    gdata_buf[q_rear].acc_x = store_data->acc_x;
    gdata_buf[q_rear].acc_y = store_data->acc_y;
    gdata_buf[q_rear].acc_z = store_data->acc_z;
    gdata_buf[q_rear].reserved = 0;
    q_rear++;
    q_rear = q_rear % GBUF_SIZE; /* loop back */
    data_num++;
}

/*******************************************************************************
 * Function:       _qcover
 * Description:    store a date into gdata_buf. This function is used when gdata_buf
 *                 is full. The newist data will cover the data which is stored into
 *                 into gdata_buf the earliest.
 * Input£º         gsensor_data_t: the data that will be stored.
 * Output£º        void
 * Other£º         //
 *******************************************************************************/
static void _qcover(gsensor_data_t* cover_data)
{
    gdata_buf[q_rear].acc_x = cover_data->acc_x;
    gdata_buf[q_rear].acc_y = cover_data->acc_y;
    gdata_buf[q_rear].acc_z = cover_data->acc_z;
    gdata_buf[q_rear].reserved = 0;
    q_head++;
    q_rear++;
    q_head = q_head % GBUF_SIZE;
    q_rear = q_rear % GBUF_SIZE;

}

/*******************************************************************************
* Function:       timer_handle
* Description:    Timer interrupt service function
* Input£º         void
* Output£º        void
* Other£º         
*******************************************************************************/
void timer_handle(unsigned long arg)
{
    data_buf_write();
    g_jiffies = kernel_sym("jiffies");
    mod_timer(&g_timer, *g_jiffies + msecs_to_jiffies((unsigned int)w_interval));
}

