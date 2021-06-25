/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: get_drv_msg.c
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             �������ļ���
*******************************************************************************
*/ 
//shaofeng ���ӵ�psp�С� ���ӵ�actmem_api.a
#include "act_mem.h"
#include "include_psp.h"
#include "include_case.h"
#include "include_msg_server.h"



/* �Ƿ���Ҫ�������Ĺ���, �������Ҫ���ó�0, ������Լ����ռ� */
#define CFG_ADA_KEY_TONE_ENABLE 1



/* extern function, �������� */
static bool _get_key_tone_head( int fd, key_tone_head_t *head );
static bool _make_key_tone_data( const char *fullpath );
static bool _set_key_tone_by_fullpath( const char *fullpath );



/* define variable, �������� */
key_tone_param_t key_tone_param;
static int s_ada_device_fd = -1;


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       ������������ʼ��: ��ð�����ʹ��״̬, ���ð������ļ���ada����
*       config ��Ҫ���水�����ļ�·������Ч��־
* \param[in]    
* \retval       
*******************************************************************************/
void ada_para_init( void )
{
    int tmp;
    
    memset( &key_tone_param, 0x00, sizeof(key_tone_param) );
    key_tone_param.enable = 1;
    
    //��config�ж�ȡvalid״̬, ����ر��򲻽����κγ�ʼ������
    /* ��config�л����Ч״̬, ��ȡʧ������Ч������ */
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
*       ���������ļ�ͷ��Ϣ, ���ڴ��ݸ�ada��������. ���key_tone_head_t
* \param[in]    int: �������ļ����
* \param[out]   key_tone_head_t *: �������ļ�ͷ�ṹ��
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
    //��wav�ļ�ͷ
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
        //ֻȡKǰ�ĵ�λ
        
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
*       ���������ļ�����buffer��, ���ڴ��ݸ�ada��������
*       read file to buffer, which send to ada driver to play
* \param[in]    char *: �������ļ�ȫ·��
* \retval       
*******************************************************************************/
static bool _make_key_tone_data( const char *fullpath )
{
    bool result = true;
    int tmp;
    unsigned int file_len = 0;
    struct stat file_stat;

    /* ��֤�ļ��Ƿ����. �ȹرյ�ǰfd, �ٴ�fullpath�ļ� */
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
    //��ȡ�ļ�ͷʧ��, ������Ĭ��ֵ
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
    
    //���������ȼ�ȥ�ļ�ͷ, ��seek���ļ�ͷĩβ
    file_len = file_stat.st_size - WAV_FILE_HEAD_LENGTH;
    lseek( key_tone_param.fd, WAV_FILE_HEAD_LENGTH, SEEK_SET );
    print_dbg("filelen:0x%x", file_len);
    
    /* ֮ǰ�Ѿ������� */
    if (NULL != key_tone_param.data)
    {
        /* ֮ǰ������, ���ҳ��ȱ��, free����malloc */
        if (file_len > key_tone_param.len)
        {
            act_mem_free(key_tone_param.data);
            key_tone_param.data = NULL;
            
            //����������, uncache���ڴ�
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
        /* ֮ǰ������, ���ҳ��ȱ�С, ��Ȼʹ��֮ǰ��buffer */
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
    /* ֮ǰû������, malloc buffer */
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
    //�رհ������ļ����, �˺���֮ǰ������ֱ��return, ���������ܹر�
    close(key_tone_param.fd);
    key_tone_param.fd = -1;

    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       ���ð���������
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
    ��������ַ, ����ADA����ʹ��DMA�������ݱ����������ַ */
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
*       ���ð������ļ�ȫ·��, ������װ�ص�buffer��, �����䲥�Ų���
*       ���óɹ�valid=1, ����valid=0
* \param[in]    char *: �������ļ�ȫ·��
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
*       ���ð������򿪹ر�. ������setting�رհ���������
* \param[in]    int: 1 on; 0:off
* \retval       
*******************************************************************************/
bool ada_set_key_tone_on( int flag_on )
{
#if CFG_ADA_KEY_TONE_ENABLE > 0
    bool result = true;
    int tmp = -1;
    
    //��, ��Ҫ��ȡȫ·��
    if ( 1 == flag_on )
    {
        tmp = get_config( CFG_KEY_TONE_FULLPATH, key_tone_param.fullpath, sizeof(key_tone_param.fullpath) );
        if ( tmp < 0 )
        {
            print_warning("get_config CFG_KEY_TONE_FULLPATH failed\n");
            key_tone_param.on = 0;
            return false;
        }
        /* ���������ļ����õ�ada, ����ʧ������Ч������ */
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
    //�ر�
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
*       ���ð�����ʹ�����. ��ֹaudioӦ���ڰ�������ͻ, ������audioʱ����0, �˳�������1
*       set it enable or disable
* \param[in]    int: 1: ʹ��; 0: ��ֹ
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
*       ���Ű�����, ����һ�β���һ��
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

