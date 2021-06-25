/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: get_drv_msg.c
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#include "include_case.h"
#include "include_msg_server.h"


/* define variable, 变量定义 */
key_para_t  key_para = 
{
    0, 0,
    {
        {KEY_MAP1},
        {KEY_MAP2},
        {KEY_MAP3},
        {KEY_MAP4},
        {KEY_MAP5},
        {KEY_MAP6},
        {KEY_MAP7},
        {KEY_MAP8},
        {KEY_MAP9},
        {KEY_MAP10}
    },
    {
        {KEY_SHORTCUT1},
        {KEY_SHORTCUT2},
        {KEY_SHORTCUT3},
        {KEY_SHORTCUT4},
        {KEY_SHORTCUT5}
    }
};


static int s_key_device_fd = -1;




void key_para_init( void )
{
    key_para.long_key_time      = TIME_KEY_LONG;
    key_para.hold_key_interval  = TIME_KEY_HOLD;
}


void key_device_open( void )
{
    s_key_device_fd = open(DEVICE_KEY, O_RDWR, 0);
    if (s_key_device_fd < 0)
    {
        print_err("Error: open key device\n\n");
    }
}



void key_device_close( void )
{
    if (-1 != s_key_device_fd)
    {
        close(s_key_device_fd);
    }
}



/******************************************************************************/
/*!                    
* \par  Description:
*       从驱动获取键值, 此键值驱动未经过去抖处理, 是驱动采集的原始数据
* \param[out]   unsigned int: 
* \retval       
*******************************************************************************/
bool key_get_drv_keyval(unsigned int *key_val_drv)
{
    int tmp;
    unsigned int tmp_key_drv = 0;

    if (-1 == s_key_device_fd)
    {
        print_warning("key device isn't open\n");
        return false;
    }
    
    tmp = read(s_key_device_fd, &tmp_key_drv, sizeof(tmp_key_drv));
    if ((unsigned int)tmp != sizeof(tmp_key_drv))
    {
        return false;
    }
    else
    {
        *key_val_drv = tmp_key_drv;
        return true;
    }
}



/******************************************************************************/
/*!                    
* \par  Description:
*       获取锁键状态
* \param[out]   int: 
* \retval       
*******************************************************************************/
bool key_get_keylock_status( int *is_key_lock )
{
    bool result = true;
    int tmp;
    
    if (-1 == s_key_device_fd)
    {
        print_warning("key device isn't open\n");
        return false;
    }

    tmp = ioctl(s_key_device_fd, KEY_GET_LOCKKEY_STATU, (int)is_key_lock);
    if (tmp < 0)
    {
        print_warning("ioctl KEY_GET_LOCKKEY_STATU err\n\n");
        result = false;
    }
    else
    {
        result = true;
    }
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       将从驱动获取的键值进行去抖处理
* \param[in]   int: 想要读取的个数
* \param[out]   unsigned int: 保存经过去抖的键值
* \retval       int: 实际读取的个数
*******************************************************************************/
int key_get_no_jitter_keyval(int num, unsigned int *p_keyval)
{
    static unsigned int saved_keyval_jitter[CNT_DEBOUNCE_KEYVAL+1];
    static int position_save_key = 0;
    int result = 0;
    int i;
    bool tmp;
    bool is_jitter;
    unsigned int keyval_drv_jitter = 0;
    unsigned int keyval_no_jitter = 0;
    
    while(1)
    {
        is_jitter = false;
        
        tmp = key_get_drv_keyval( &keyval_drv_jitter );
        //读空了驱动的queue, 退出
        if( false == tmp )
        {
            break;
        }
        
        //读到0为全部按键都抬起
        if ( 0 == keyval_drv_jitter )
        {
            memset( saved_keyval_jitter, 0x00, sizeof(saved_keyval_jitter) );
            keyval_no_jitter = 0;
        }
        //读到其他按键, 做去抖处理
        else
        {
            //将从驱动获取的带有抖动的键值, 保存起来用于去抖处理
            saved_keyval_jitter[position_save_key] = keyval_drv_jitter;
            position_save_key++;
            if ( position_save_key >= CNT_DEBOUNCE_KEYVAL )
            {
                position_save_key = 0;
            }

            //比较从驱动获取的带有抖动的键值, 判断是否为稳定的
            for ( i=0; i<(CNT_DEBOUNCE_KEYVAL-1); i++ )
            {
                if ( saved_keyval_jitter[i] != saved_keyval_jitter[i+1] )
                {
                    is_jitter = true;
                    break;
                }
            }
            //如果是抖动, 不保存, 继续读下一个
            if ( true == is_jitter )
            {
                continue;
            }

            //最终获取的稳定键值, 发送给ap. 同时将saved_keyval_jitter设置成0, 用于下次去抖
            keyval_no_jitter = saved_keyval_jitter[0];
        }
        
        //将去抖后的键值保存到输出参数
        *(p_keyval + result) = keyval_no_jitter;
        
        result++;
        if ( result >= num )
        {
            break;
        }
    }
    
    return result;
}

