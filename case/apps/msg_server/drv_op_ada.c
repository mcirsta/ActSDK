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
//shaofeng 增加到psp中。 增加的actmem_api.a
#include "act_mem.h"
#include "include_psp.h"
#include "include_case.h"
#include "include_msg_server.h"



/* 是否需要按键音的功能, 如果不需要设置成0, 宏编译节约代码空间 */
#define CFG_ADA_KEY_TONE_ENABLE 1



/* extern function, 函数声明 */
static bool _get_key_tone_head( int fd, key_tone_head_t *head );
static bool _make_key_tone_data( const char *fullpath );
static bool _set_key_tone_by_fullpath( const char *fullpath );



/* define variable, 变量定义 */
key_tone_param_t key_tone_param;
static int s_ada_device_fd = -1;


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       按键音参数初始化: 获得按键音使能状态, 设置按键音文件到ada驱动
*       config 中要保存按键音文件路径和有效标志
* \param[in]    
* \retval       
*******************************************************************************/
void ada_para_init( void )
{
    int tmp;
    
    memset( &key_tone_param, 0x00, sizeof(key_tone_param) );
    key_tone_param.enable = 1;
    
    //从config中读取valid状态, 如果关闭则不进行任何初始化操作
    /* 从config中获得有效状态, 获取失败则无效按键音 */
    tmp = get_config( CFG_KEY_TONE_ON, (char *)&key_tone_param.on, sizeof(key_tone_param.on) );
    if ( tmp < 0 )
    {
        key_tone_param.on = 0;
        print_warning("get config CFG_KEY_TONE_VALID failed");
        return;
    }
    
    ada_set_key_tone_enable( key_tone_param.enable );
    ada_set_key_tone_on( key_tone_param.on );
}



void ada_device_open( void )
{
    int vol;
    s_ada_device_fd = open(DEVICE_ADA, O_RDWR, 0);
    if (s_ada_device_fd < 0)
    {
        print_err("\n\n\n");
    }
    get_config(CFG_VOICE_VOLUM, (char*) &vol, 4);
    ada_set_volume(vol);
}



void ada_device_close( void )
{
    if (-1 != s_ada_device_fd)
    {
        close(s_ada_device_fd);
    }
}


/******************************************************************************/
/*!                    
* \par  Description:
*       将按键音文件头信息, 用于传递给ada驱动播放. 详见key_tone_head_t
* \param[in]    int: 按键音文件句柄
* \param[out]   key_tone_head_t *: 按键音文件头结构体
* \retval       
*******************************************************************************/
static bool _get_key_tone_head( int fd, key_tone_head_t *head )
{
    bool result = true;
    char buffer_wav_head[WAV_FILE_HEAD_LENGTH];
    int tmp_sample_rate = 0;
    short tmp_channel = 0;
    int tmp;
    
    /* get key tone file head info */
    tmp = read( fd, buffer_wav_head, sizeof(buffer_wav_head) );
    if (tmp < 0)
    {
        return false;
    }
    //是wav文件头
    if ( 0 == strncmp(buffer_wav_head, "RIFF", 4) )
    {
        memcpy( &tmp_channel, &buffer_wav_head[0x16], 2 );
        if ( 2 == tmp_channel )
        {
            head->channle = ADA_STEREO;
        }
        else
        {
            head->channle = ADA_MONO;
        }
        
        memcpy( &tmp_sample_rate, &buffer_wav_head[0x18], 2 );
        tmp_sample_rate /= 1000;
        //只取K前的单位
        
        switch( tmp_sample_rate )
        {
            case 8:
            {
                head->sample_rate = ACT_SAMPLE_8K;
                break;
            }

            case 11:
            {
                head->sample_rate = ACT_SAMPLE_11K;
                break;
            }

            case 12:
            {
                head->sample_rate = ACT_SAMPLE_12K;
                break;
            }

            case 16:
            {
                head->sample_rate = ACT_SAMPLE_16K;
                break;
            }

            case 22:
            {
                head->sample_rate = ACT_SAMPLE_22K;
                break;
            }

            case 24:
            {
                head->sample_rate = ACT_SAMPLE_24K;
                break;
            }

            case 32:
            {
                head->sample_rate = ACT_SAMPLE_32K;
                break;
            }

            case 44:
            {
                head->sample_rate = ACT_SAMPLE_44K;
                break;
            }

            case 48:
            {
                head->sample_rate = ACT_SAMPLE_48K;
                break;
            }

            case 96:
            {
                head->sample_rate = ACT_SAMPLE_96K;
                break;
            }

            default :
            {
                head->sample_rate = ACT_SAMPLE_16K;
                break;
            }
        }
    }
    else
    {
        result = false;
    }
    
    return result;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       将按键音文件读到buffer中, 用于传递给ada驱动播放
*       read file to buffer, which send to ada driver to play
* \param[in]    char *: 按键音文件全路径
* \retval       
*******************************************************************************/
static bool _make_key_tone_data( const char *fullpath )
{
    bool result = true;
    int tmp;
    unsigned int file_len = 0;
    struct stat file_stat;

    /* 验证文件是否存在. 先关闭当前fd, 再打开fullpath文件 */
    if ( key_tone_param.fd > 0 )
    {
        close(key_tone_param.fd);
        key_tone_param.fd = 0;
    }
    key_tone_param.fd = open(fullpath, O_RDONLY, 0);
    if (key_tone_param.fd <= 0)
    {
        print_err("open file:%s, faild", fullpath);
        goto MAKE_KEY_TONE_DATA_ERR;
    }
    
    result = _get_key_tone_head( key_tone_param.fd, &key_tone_param.head );
    //读取文件头失败, 则设置默认值
    if ( false == result )
    {
        key_tone_param.head.channle = ADA_MONO;
        key_tone_param.head.sample_rate = ACT_SAMPLE_16K;
    }
    
    /* get key tone file len */
    tmp = fstat( key_tone_param.fd, &file_stat );
    if (tmp < 0)
    {
        goto MAKE_KEY_TONE_DATA_ERR;
    }
    
    //按键音长度减去文件头, 并seek到文件头末尾
    file_len = file_stat.st_size - WAV_FILE_HEAD_LENGTH;
    lseek( key_tone_param.fd, WAV_FILE_HEAD_LENGTH, SEEK_SET );
    print_dbg("filelen:0x%x", file_len);
    
    /* 之前已经有数据 */
    if (NULL != key_tone_param.data)
    {
        /* 之前有数据, 并且长度变大, free后再malloc */
        if (file_len > key_tone_param.len)
        {
            act_mem_free(key_tone_param.data);
            key_tone_param.data = NULL;
            
            //申请连续的, uncache的内存
            key_tone_param.data = act_mem_allocate( file_len, MEM_CONTINUOUS|UNCACHE_MEM );
            if (NULL == key_tone_param.data)
            {
                print_err("malloc_dma %d bytes faild", file_len);
                goto MAKE_KEY_TONE_DATA_ERR;
            }
            
            /* get key tone file data */
            memset(key_tone_param.data, 0x00, file_len);
            tmp = read(key_tone_param.fd, key_tone_param.data, file_len);
            print_dbg("read len:0x%x", tmp);
            if (tmp <= 0)
            {
                goto MAKE_KEY_TONE_DATA_ERR;
            }
        }
        /* 之前有数据, 并且长度变小, 仍然使用之前的buffer */
        else
        {
            /* get key tone file data */
            memset(key_tone_param.data, 0x00, file_len);
            tmp = read(key_tone_param.fd, key_tone_param.data, file_len);
            print_dbg("read len:0x%x", tmp);
            if (tmp <= 0)
            {
                goto MAKE_KEY_TONE_DATA_ERR;
            }
        }
    }
    /* 之前没有数据, malloc buffer */
    else
    {
        key_tone_param.data = act_mem_allocate( file_len, MEM_CONTINUOUS|UNCACHE_MEM );
        if (NULL == key_tone_param.data)
        {
            print_err("malloc_dma %d bytes faild", file_len);
            result = false;
            goto MAKE_KEY_TONE_DATA_ERR;
        }
        
        /* get key tone file data */
        memset(key_tone_param.data, 0x00, file_len);
        tmp = read(key_tone_param.fd, key_tone_param.data, file_len);
        print_dbg("read len:0x%x", tmp);
        if (tmp <= 0)
        {
            goto MAKE_KEY_TONE_DATA_ERR;
        }
    }
    
    MAKE_KEY_TONE_DATA_ERR:
    key_tone_param.len = file_len;
    //关闭按键音文件句柄, 此函数之前出错不能直接return, 否则句柄不能关闭
    close(key_tone_param.fd);
    key_tone_param.fd = -1;

    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       设置按键音参数
*       include: set ada param, and key tone buffer address length
* \param[in]    
* \retval       
*******************************************************************************/
bool ada_set_key_tone_param( void )
{
    int tmp;
    char *physical_addr = NULL;
    
    if ( (NULL == key_tone_param.data ) || (key_tone_param.len == 0) )
    {
        return false;
    }
    
    ioctl(s_ada_device_fd, DACIO_GET_PCM_SYNC, 0);  

    ioctl(s_ada_device_fd, DACIO_SET_PCM_RATE,    key_tone_param.head.sample_rate); 
    ioctl(s_ada_device_fd, DACIO_SET_PCM_CHANNEL, key_tone_param.head.channle); 
    ioctl(s_ada_device_fd, DACIO_SET_PCM_BITS,    32); 
    
    /* get the data's physical address, for ada driver must use physical add
    获得物理地址, 传给ADA驱动使用DMA搬送数据必须是物理地址 */
    physical_addr = (char *)( (unsigned int)key_tone_param.data & ~(0xE << 28));
    tmp = ioctl(s_ada_device_fd, DACIO_SET_PCM_ADDR , (int)physical_addr);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        return false;
    }
    tmp = ioctl(s_ada_device_fd, DACIO_SET_PCM_LEN, (int)key_tone_param.len);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        return false;
    }
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       设置按键音文件全路径, 并将其装载到buffer中, 设置其播放参数
*       设置成功valid=1, 否则valid=0
* \param[in]    char *: 按键音文件全路径
* \retval       
*******************************************************************************/
static bool _set_key_tone_by_fullpath( const char *fullpath )
{
#if CFG_ADA_KEY_TONE_ENABLE > 0
    bool result = true;

    key_tone_param.on = 1;
    
    if (-1 == s_ada_device_fd)
    {
        print_warning("ada device isn't open\n");
        key_tone_param.on = 0;
        return false;
    }
    
    /* read file data to key_tone_para.data, and get this file data length */
    result = _make_key_tone_data( fullpath );
    if ( result == false )
    {
        print_err("\n\n");
        key_tone_param.on = 0;
        return false;
    }

    result = ada_set_key_tone_param();
    if ( result == false )
    {
        print_err("\n\n");
        key_tone_param.on = 0;
        return false;
    }

    return result;
#else
    return false;
#endif
}

/******************************************************************************/
/*!                    
* \par  Description:
*       设置按键音打开关闭. 仅用于setting关闭按键音功能
* \param[in]    int: 1 on; 0:off
* \retval       
*******************************************************************************/
bool ada_set_key_tone_on( int flag_on )
{
#if CFG_ADA_KEY_TONE_ENABLE > 0
    bool result = true;
    int tmp = -1;
    
    //打开, 需要读取全路径
    if ( 1 == flag_on )
    {
        tmp = get_config( CFG_KEY_TONE_FULLPATH, key_tone_param.fullpath, sizeof(key_tone_param.fullpath) );
        if ( tmp < 0 )
        {
            print_warning("get_config CFG_KEY_TONE_FULLPATH failed\n");
            key_tone_param.on = 0;
            return false;
        }
        /* 将按键音文件设置到ada, 设置失败则无效按键音 */
        result = _set_key_tone_by_fullpath( key_tone_param.fullpath );
        if (false == result)
        {
            key_tone_param.on = 0;
            print_err("set_key_tone_by_fullpath:%s failed", key_tone_param.fullpath);
            return false;
        }
        
        key_tone_param.on     = 1;
        ada_set_key_tone_enable( key_tone_param.enable );
    }
    //关闭
    else
    {
        key_tone_param.on     = 0;
        result = ada_set_key_tone_enable( key_tone_param.enable );
    }
    
    return result;
#else
    return false;
#endif
}

/******************************************************************************/
/*!                    
* \par  Description:
*       设置按键音使能与否. 防止audio应用于按键音冲突, 当进入audio时设置0, 退出后设置1
*       set it enable or disable
* \param[in]    int: 1: 使能; 0: 禁止
* \retval       
*******************************************************************************/
bool ada_set_key_tone_enable( int flag_enable )
{
#if CFG_ADA_KEY_TONE_ENABLE > 0
    bool result = true;
    int tmp;

    if (-1 == s_ada_device_fd)
    {
        print_warning("ada device isn't open\n");
        return false;
    }
    
    //wait sync, then set
    ioctl(s_ada_device_fd, DACIO_GET_PCM_SYNC, 0);  
    tmp = ioctl(s_ada_device_fd, DACIO_PCM_PLAY_ON , flag_enable);
    if ( tmp < 0 )
    {
        key_tone_param.enable = 0;
        print_warning("\n\n");
        result = false;
    }
    else
    {
        key_tone_param.enable = flag_enable;
        result = true;
    }

    return result;
#else
    return false;
#endif
}


/******************************************************************************/
/*!                    
* \par  Description:
*       播放按键音, 调用一次播放一次
*       play key tone
* \param[in]    
* \retval       
*******************************************************************************/
bool ada_set_key_tone_play( void )
{
#if CFG_ADA_KEY_TONE_ENABLE > 0
    bool result = true;
    int tmp;

    if (-1 == s_ada_device_fd)
    {
        print_warning("ada device isn't open\n");
        return false;
    }
    if ( (0 == key_tone_param.enable) || (0 == key_tone_param.on) )
    {
        return false;
    }

    tmp = ioctl(s_ada_device_fd, DACIO_SET_PCM_PLAY , 0);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    else
    {
        result = true;
    }

    return result;
#else
    return false;
#endif
}


//shaofeng s0 mute??
bool ada_set_volume( int volume )
{
    bool result = true;
    int tmp;

    if (-1 == s_ada_device_fd)
    {
        print_warning("ada device isn't open\n");
        return false;
    }
    if ( (volume > MAX_HARDWARE_VOLUME) || (volume < 0) )
    {
        print_warning("ada device isn't open\n");
        return false;
    }

    tmp = ioctl(s_ada_device_fd, DACIO_SET_VOLUME, volume);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    else
    {
        result = true;
    }

    return result;
}


bool ada_get_volume( int *volume )
{
    bool result = true;
    int tmp;

    if (-1 == s_ada_device_fd)
    {
        print_warning("ada device isn't open\n");
        return false;
    }

    tmp = ioctl(s_ada_device_fd, DACIO_GET_VOLUME, (int)volume);
    if ( tmp < 0 )
    {
        print_warning("\n\n");
        result = false;
    }
    else
    {
        result = true;
    }

    if ( (*volume > MAX_HARDWARE_VOLUME) || (*volume < 0) )
    {
        print_warning("ada device isn't open\n");
        return false;
    }
    
    return result;
}

