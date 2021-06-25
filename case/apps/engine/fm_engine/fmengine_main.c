/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : fmengine_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-30          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     fmengine_main.c
 * \brief    routine of fm radio engine
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/30
 *******************************************************************************/


#include "include_case.h"
#include "fmengine_main.h"

static void* s_applib_lib = NULL;
static int s_fd = -1;
static int s_fd_ada = -1;
static eg_seek_info_t s_auto_seek_info;/*�ñ������ڱ����Զ���̨ʱ���ѵ�̨��*/
static engine_seek_t s_engine_seek; /*�Զ���̨ʱ���п��Ʊ���״̬����ֵ��*/
struct radio_seek_info_t s_seek_info; /*��Ҫ���뵽������*/
static engine_state_e s_engine_state = FM_PLAYING;/*ȫ�ֱ������ڼ�¼��ǰFM����״̬*/
static engine_state_e s_app_expect_state = FM_PLAYING;/*��¼UI���õ�����״̬*/


static void _once_seek_timer(void);

static bool _lib_init(void);

static bool _lib_deinit(void);


static bool _lib_init(void)
{
    s_applib_lib = dlopen("applib.so",RTLD_LAZY);
    if(s_applib_lib == NULL)
    {
        print_err("load applib.so failed.");
        return false;
    }
    return true;
}

static bool _lib_deinit(void)
{
    if(s_applib_lib != NULL)
    {
        dlclose(s_applib_lib);
        s_applib_lib = NULL;
    }
    return true;
}

/*!
 * ������
 * ��������
 * ����ֵ��true--�򿪳ɹ���false--��ʧ��
 */
static bool fm_engine_open(void)
{
    adc_input_t  input_value;
    adc_gain_t gain_value;

    s_fd = open(DEVICE_FM, O_RDWR);
    s_fd_ada = open(DEVICE_ADA, O_RDWR);

    if((s_fd == -1) || (s_fd_ada == -1))
    {
        return false;
    }

   /* legality value */
    input_value.mic = 0;
    input_value.fm = 1;
    input_value.linein = 0;
    input_value.mix = 0;
    ioctl(s_fd_ada, ADCIO_SET_INPUT, (unsigned long)&input_value);

   /* legality value */
    gain_value.mic_gain = 7;
    gain_value.fm_gain = 7;
    gain_value.linein_gain = 7;
    gain_value.adc_gain = 3;
    ioctl(s_fd_ada, ADCIO_SET_GAIN, (unsigned long)&gain_value);

    ioctl(s_fd_ada, DACIO_SET_MUTE, 0);
    s_engine_seek.seek_over_flg = true;
    return true;
}

/*!
 * �ر�����
 * ��������
 * ����ֵ����
 */
static void fm_engine_close(void)
{
    adc_input_t  input_value;


    if(s_fd_ada != -1)
    {
        input_value.mic = 0;
        input_value.fm = 0;
        input_value.linein = 0;
        input_value.mix = 0;
        ioctl(s_fd_ada, ADCIO_SET_INPUT, (unsigned long)&input_value);/*���*/
        ioctl(s_fd_ada, ADCIO_SET_MONITOR, 0);  /*�رռ���*/      
        close(s_fd_ada);
        s_fd_ada = -1;
    }
    if (s_fd != -1)
    {
        close(s_fd);
        s_fd = -1;
    }
    sys_forbid_auto_standby(false);
}

/*!
 * ��������Ƿ����
 * �������������*value = 1--���߲���,0--����δ����
 * ����ֵ��0-��ȡ�ɹ���-1-��ȡʧ��
 */
static inline int fm_engine_get_antenna(unsigned long *value)
{
    int ret = -1;
    ioctl(s_fd, RADIO_GET_ANTENNA, (unsigned long) value);

    if(1 == *value)
        ret = 0;
    return ret; 
}

/*!
 * ����FM����
 * ��������
 * ����ֵ��0-���óɹ���-1-����ʧ��
 */
static inline int fm_engine_play(void)
{
    sys_forbid_auto_standby(true);
    s_engine_state = FM_PLAYING;
    /*��fm pa����������̨������������һ�𲥷ţ��ڸ�pa��,��ʱ�������*/
    ioctl(s_fd_ada, ADCIO_SET_MONITOR, 1);
    return ioctl(s_fd, RADIO_SET_PLAY, 0);
}

/*!
 * ����FM����
 * ��������
 * ����ֵ��0-���óɹ���-1-����ʧ��
 */
static inline int fm_engine_stop(void)
{
    sys_forbid_auto_standby(false);
    ioctl(s_fd_ada, ADCIO_SET_MONITOR, 0);
    s_engine_state = FM_STOPPED;
    return ioctl(s_fd, RADIO_SET_STOP, 0);
}

/*!
 * ��ȡFM�������ԣ�����������������
 * ����(�������value ֵָ��  = 1-��������0-������
 * ����ֵ��0-��ȡ�ɹ���-1-��ȡʧ��
 */
static inline int fm_engine_get_stereo(unsigned long *value)
{
    return ioctl(s_fd, RADIO_GET_STEREO_MODE, (unsigned long) value);
}

/*!
 * ��ȡ��ǰFM״̬��Ϣ
 * �����������������FM״̬��Ϣ�ṹ��ָ��
 * ����ֵ��0-��ȡ�ɹ���-1-��ȡʧ��
 */
static inline int fm_engine_get_info(struct radio_info_t *info)
{
    return ioctl(s_fd, RADIO_GET_INFO, (unsigned long) info);
}

/*!
 * ��ȡ�ź�ǿ��
 * �������������intensity--�ź�ǿ��ֵָ��
 * ����ֵ��0-��ȡ�ɹ���-1-��ȡʧ��
 */
static inline int fm_engine_get_intensity(unsigned long *intensity)
{
    return ioctl(s_fd, RADIO_GET_INTENSITY, (unsigned long) intensity);
}

/*!
 * ���ò���
 * ������band = �����������ֵ 0--US/China, 1--Europe�� 2--Japan
 * ����ֵ��0-���óɹ���-1-����ʧ��
 */
static inline int fm_engine_set_band(enum BAND_MODE band)
{
    return ioctl(s_fd, RADIO_SET_BAND_MODE, band);
}

/*!
 * ��ȡ��ǰ����
 * �������������band--�����������ֵָ��
 * ����ֵ��0-��ȡ�ɹ���-1-��ȡʧ��
 */
static inline int fm_engine_get_band(enum BAND_MODE *band)
{
    return ioctl(s_fd, RADIO_GET_BAND_MODE, (unsigned long) band);
}

/*!
 * ���õ�̨Ƶ�ʣ���λ KHZ��
 * ������value--Ƶ��ֵ
 * ����ֵ��0-���óɹ���-1-����ʧ��
 */
static inline int fm_engine_set_freq(unsigned long value)
{
    
    if (s_engine_seek.seek_over_flg == false)
    {
        return -1;
    }
   
    return ioctl(s_fd, RADIO_SET_FREQ, value);
}

/*!
 * ��ȡ��̨Ƶ�ʣ���λ KHZ��
 * �������������freq
 * ����ֵ���ɹ��򷵻ص�ǰ��Ļ��Ƶ�ʣ�ʧ�ܷ���-1
 */
static inline int fm_engine_get_freq(unsigned long *freq)
{
    
    return ioctl(s_fd, RADIO_GET_FREQ, (unsigned long) freq);
    
}

/*!
 * ����������̨���ź�ǿ������ֵ
 * ������value--ǿ������ֵ
 * ����ֵ��0-���óɹ���-1-����ʧ��
 */
static inline int fm_engine_set_threshold(unsigned long value)
{
    return ioctl(s_fd, RADIO_SET_THRESHOLD, value);
}

/*!
 * ʹFM������˳�standby
 * ������standby = 0--�˳�standby��1--����standby
 * ����ֵ��0-���óɹ���-1-����ʧ��
 */
static inline int fm_engine_standby(unsigned long standby)
{
    return ioctl(s_fd, RADIO_SET_ENTER_STANDBY, standby);
}

/*!
 * FM�Զ�����
 * ������en_seek_info--�����ṹ��ָ��
 * ����ֵ��0-���óɹ���-1-����ʧ��
 */
static inline int fm_engine_auto_seek(const en_seek_info_t *en_seek_info)
{
    sys_forbid_auto_standby(true);
    
    if (s_engine_seek.seek_over_flg == false)
    {
        return -1;
    }
    
    s_engine_seek.seek_over_flg = false;
    s_engine_seek.need_seek = true;
    /*s_engine_seekǰ��Ľṹ��en_seek_info_t��ͬ��*/
    memcpy((en_seek_info_t*) &s_engine_seek, en_seek_info, sizeof(en_seek_info_t));
    
    if (s_engine_seek.mode == AUTO_SEARCH)
    {
        memset(&s_auto_seek_info, 0, sizeof(eg_seek_info_t));
        s_engine_seek.cur_freq = s_engine_seek.min_freq;
        /*����������СƵ�ʣ����Ƿ�Ϊһ��̨��ԭ����RADIO_SET_AUTO_SEEKֻ�������ǰ��*/
        if (0 == ioctl(s_fd, RADIO_SET_FREQ, s_engine_seek.cur_freq))
        {
            s_auto_seek_info.channel_freq[s_auto_seek_info.channel_num] = s_engine_seek.cur_freq;
            s_auto_seek_info.channel_num++;
            stop_timer( s_engine_seek.timer_id_seek );
        }
        s_seek_info.search_direction = AUTO_ADD;/*Ϊ��ʱ���ص�ʹ��.*/
    }
    else if (s_engine_seek.mode == HALF_AUTO_SEARCH_ADD)
    {
        s_seek_info.search_direction = AUTO_ADD;/*Ϊ��ʱ���ص�ʹ��.*/
        ioctl(s_fd, RADIO_GET_FREQ, (unsigned long) &(s_engine_seek.cur_freq));
    }
    else if (s_engine_seek.mode == HALF_AUTO_SEARCH_MINUS)
    {
        s_seek_info.search_direction = AUTO_MINUS;/*Ϊ��ʱ���ص�ʹ��.*/
        ioctl(s_fd, RADIO_GET_FREQ, (unsigned long) &(s_engine_seek.cur_freq));
    }
    else
    {
        print_dbg("s_engine_seek.mode err\n");
        s_engine_seek.seek_over_flg = true;
        return -1;
    }
    
    ioctl(s_fd, RADIO_SET_STOP, 0);
    if (-1 == s_engine_seek.timer_id_seek)
    {
        s_engine_seek.timer_id_seek = set_timer(60, (timer_proc) _once_seek_timer, NULL);
        if (-1 == s_engine_seek.timer_id_seek)
        {
            s_engine_seek.seek_over_flg = true;
            ioctl(s_fd, RADIO_SET_PLAY, 0);
            return -1;
        }
    }
    else
    {
        restart_timer(s_engine_seek.timer_id_seek);
    }
    return 0;
}
/*!
 * ���Ŷ�ʱ������������2s������Զ���̨
 * ������para--��̨��ʱ��ָ��
 * ����ֵ����
 */
static void _scene_tmp_timer(void * param)
{
    restart_timer(*(int*)param);
    ioctl(s_fd, RADIO_SET_STOP, 0);
    print_dbg("_scene_tmp_timer\n");
    return;
}

/*���ģ��֧���Զ���̨*/
#ifdef REAL_AUTO_SEEK


/*!
 * �Զ���̨��������
 * ��������
 * ����ֵ����
 */
static void _auto_seek_over(void)
{
    bool bresult = stop_timer(s_engine_seek.timer_id_seek);
    if (false == bresult)
    {
        print_dbg("stop_timer err %d\n",s_engine_seek.timer_id_seek);
        return;
    }
    s_engine_seek.seek_over_flg = true;
    
    if (s_auto_seek_info.channel_num> 0)
    {
        ioctl(s_fd, RADIO_SET_FREQ, s_auto_seek_info.channel_freq[0]);
    }
    else
    {
        ioctl(s_fd, RADIO_SET_FREQ, s_engine_seek.min_freq);
    }
    ioctl(s_fd, RADIO_SET_PLAY, 0);
    return;
}
/*!
 * �Զ���̨��ʱ������
 * ��������
 * ����ֵ����
 */
static void _once_seek_timer(void )
{
    struct radio_info_t info;
    int result = 0;
    bool bresult = false;
    if(s_engine_seek.seek_over_flg == true)
    {
        stop_timer(s_engine_seek.timer_id_seek);
        ioctl(s_fd, RADIO_SET_PLAY, 0);
        return;
    }
    if ((s_engine_seek.cur_freq > s_engine_seek.max_freq) 
        || (s_engine_seek.cur_freq < s_engine_seek.min_freq)
            || (s_auto_seek_info.channel_num >= MAX_CHANNEL_NUM))
    {
        _auto_seek_over();
        return;
    }
    if (true == s_engine_seek.need_seek)
    {
        s_seek_info.search_freq = s_engine_seek.cur_freq;
        s_seek_info.search_step = 1;
        result = ioctl(s_fd, RADIO_SET_AUTO_SEEK, (unsigned long) &s_seek_info);
        if(result != 0)
        {
            return;
        }
    }
    result = ioctl(s_fd, RADIO_GET_INFO, (unsigned long) &info);
    if (0 != result)/*δ������Ҫ�ض�*/
    {
        s_engine_seek.need_seek = false;
        return;
    }
    
    switch (info.seek_status)
    {
        case FOUND_STATION:
            
        ioctl(s_fd, RADIO_SET_PLAY, 0);
        bresult = stop_timer(s_engine_seek.timer_id_seek);
        if(bresult == false)
        {
            return;
        }
        if ((s_engine_seek.mode == HALF_AUTO_SEARCH_ADD)
                || (s_engine_seek.mode == HALF_AUTO_SEARCH_MINUS))
        {
            
            s_engine_seek.seek_over_flg = true;
            
            return;
        }
        

        
        //��ͣ��̨����������        
        if(-1 == s_engine_seek.timer_id_play)
        {
            s_engine_seek.timer_id_play = set_single_shot_timer (INTER_SEEK_PLAY, (timer_proc)_scene_tmp_timer, 
                                                                   &(s_engine_seek.timer_id_seek));
        }
        else
        {
            restart_timer(s_engine_seek.timer_id_play);
        }

        
        s_engine_seek.cur_freq = info.current_freq;
        s_auto_seek_info.channel_freq[s_auto_seek_info.channel_num] = info.current_freq;
        s_auto_seek_info.channel_num++;
        s_engine_seek.need_seek = true;
        return;
        case REACH_BANDLIMIT:/*��������Ƶ����Ե*/
        if ((s_engine_seek.mode == HALF_AUTO_SEARCH_ADD)
                || (s_engine_seek.mode == HALF_AUTO_SEARCH_MINUS))
        {
            bresult = stop_timer(s_engine_seek.timer_id_seek);
            if(bresult == false)
            {
                return;
            }
            s_engine_seek.seek_over_flg = true;
            ioctl(s_fd, RADIO_SET_PLAY, 0);
            return;
        }
        _auto_seek_over();
        return;
        case FOUND_NOSTAION:
        s_engine_seek.need_seek = false;
        return;
        case INVALID_STATION:/*������һ����̨�����˵�̨������Ч��̨*/
#ifdef  SAVE_INVALID_STATION
        if ((s_engine_seek.mode != HALF_AUTO_SEARCH_ADD)
                && (s_engine_seek.mode != HALF_AUTO_SEARCH_MINUS))
        {
            s_auto_seek_info.channel_freq[s_auto_seek_info.channel_num] = info.current_freq;
            s_auto_seek_info.channel_num++;
        }
#endif
        s_engine_seek.cur_freq = info.current_freq;
        s_engine_seek.need_seek = true;
        return;
        default: /* 150Operation now in progress */
        return;
    }
}
#else/*���ģ�鲻֧���Զ���̨*/
/*!
 * �Զ���̨��������
 * ��������
 * ����ֵ����
 */
static void _auto_seek_over(void)
{
    bool bresult = false;
    bresult =stop_timer(s_engine_seek.timer_id_seek);
    if(false == bresult)
    {
        return;
    }
    
    s_engine_seek.seek_over_flg = true;
    
    if ((s_auto_seek_info.channel_num > 0)&&(s_engine_seek.mode == AUTO_SEARCH))
    {
        ioctl(s_fd, RADIO_SET_FREQ, s_auto_seek_info.channel_freq[0]);
    }
    else
    {
        ioctl(s_fd, RADIO_SET_FREQ, s_engine_seek.min_freq);
    }
    ioctl(s_fd, RADIO_SET_PLAY, 0);
    return;
}
/*!
 * �Զ���̨��ʱ������
 * ��������
 * ����ֵ����
 */
static void _once_seek_timer(void )
{
    bool bresult = false;
    if ((s_engine_seek.mode == HALF_AUTO_SEARCH_ADD)
                || (s_engine_seek.mode == AUTO_SEARCH))
    {
        s_engine_seek.cur_freq = s_engine_seek.cur_freq + s_engine_seek.step;
    }
    else
    {
        s_engine_seek.cur_freq = s_engine_seek.cur_freq - s_engine_seek.step;
    }
    if ((s_engine_seek.cur_freq > s_engine_seek.max_freq) 
        ||(s_engine_seek.cur_freq < s_engine_seek.min_freq) 
        || (s_auto_seek_info.channel_num >= MAX_CHANNEL_NUM))
    {
        _auto_seek_over();
        return;
    }
    if (0 == ioctl(s_fd, RADIO_SET_FREQ, s_engine_seek.cur_freq))
    {
        if(s_engine_seek.mode == AUTO_SEARCH)
        {
            s_auto_seek_info.channel_freq[s_auto_seek_info.channel_num] = s_engine_seek.cur_freq;
            s_auto_seek_info.channel_num++;
            ioctl(s_fd, RADIO_SET_PLAY, 0);
            bresult = stop_timer(s_engine_seek.timer_id_seek);/*��ͣ��̨����������*/
            if(bresult == false)
            {
                return;
            }
            if(-1 == s_engine_seek.timer_id_play)
            {
                s_engine_seek.timer_id_play = set_single_shot_timer (INTER_SEEK_PLAY, (timer_proc)_scene_tmp_timer, 
                                                                       &(s_engine_seek.timer_id_seek));
            }
            else
            {
                restart_timer(s_engine_seek.timer_id_play);
            }
            return;
        }
        else
        {
            bresult =stop_timer(s_engine_seek.timer_id_seek);
            if(false == bresult)
            {
                return;
            }
            
            s_engine_seek.seek_over_flg = true;
            ioctl(s_fd, RADIO_SET_PLAY, 0);
        }
    }
    return;
     
}
#endif

/*!
 * �˳��Զ�����
 * ��������
 * ����ֵ��0-���óɹ���-1-����ʧ��
 */
static inline int fm_engine_auto_seek_break(void)
{
    int ret;
    unsigned int l_num;
    
    
    ret = ioctl(s_fd, RADIO_SET_AUTO_SEEK_BREAK, 0);
    
    if (-1 != s_engine_seek.timer_id_seek)
    {
        stop_timer(s_engine_seek.timer_id_seek);
    }
    
    if (-1 != s_engine_seek.timer_id_play)
    {
        stop_timer(s_engine_seek.timer_id_play);
    }
    
    s_engine_seek.seek_over_flg = true;
    s_engine_seek.need_seek = false;
    
    l_num = s_auto_seek_info.channel_num;
    if((0 < l_num) && (l_num <= 20))
    {
        ioctl(s_fd, RADIO_SET_FREQ, s_auto_seek_info.channel_freq[l_num-1]);
    }
    ioctl(s_fd, RADIO_SET_PLAY, 0);
    return ret;
}

/*!
 * fm����ص������������������̷���fm�������Ϣ
 * ������msg--Ӧ����Ϣָ��
 * ����ֵ��
 */
static int fmengine_cb(const msg_apps_t *msg)
{
    int ret = -2;
    msg_reply_t msg_reply;

    if (NULL == msg)
    {
        return -1;
    }
    

    memset(msg_reply.content, 0, sizeof(msg_reply.content));

    switch (msg->type)
    {
    case MSG_FMENGINE_GET_STEREO_MODE:
        ret = fm_engine_get_stereo((unsigned long *) msg_reply.content);
        break;

    case MSG_FMENGINE_GET_INFO:
        ret = fm_engine_get_info((struct radio_info_t *) msg_reply.content);
        break;

    case MSG_FMENGINE_GET_INTENSITY:
        ret = fm_engine_get_intensity((unsigned long *) msg_reply.content);
        break;

    case MSG_FMENGINE_GET_BAND_MODE:
        ret = fm_engine_get_band((enum BAND_MODE *) msg_reply.content);
        break;

    case MSG_FMENGINE_GET_FREQ:
        ret = fm_engine_get_freq((unsigned long *) msg_reply.content);
        break;

    case MSG_FMENGINE_GET_ANTENNA:
        ret = fm_engine_get_antenna((unsigned long *) msg_reply.content);
        break;

    case MSG_FMENGINE_SET_FREQ:
        
        ret = fm_engine_set_freq(*(unsigned long *) msg->content);
        break;

    case MSG_FMENGINE_SET_THRESHOLD:
        ret = fm_engine_set_threshold(*(unsigned long *) msg->content);
        break;

    case MSG_FMENGINE_SET_BAND_MODE:
        ret = fm_engine_set_band(*(enum BAND_MODE *) msg->content);
        break;

    case MSG_FMENGINE_SET_PLAY:
        ret = fm_engine_play();
        s_app_expect_state = FM_PLAYING;
        break;

    case MSG_FMENGINE_SET_STOP:
        ret = fm_engine_stop();
        s_app_expect_state = FM_STOPPED;
        break;

    case MSG_FMENGINE_SET_ENTER_STANDBY:
        ret = fm_engine_standby(*(unsigned long *) msg->content);
        break;

    case MSG_FMENGINE_SET_AUTO_SEEK:
        ret = fm_engine_auto_seek((en_seek_info_t*) msg->content);
        break;

    case MSG_FMENGINE_SET_AUTO_SEEK_BREAK:
        ret = fm_engine_auto_seek_break();
        break;
        
    case MSG_FMENGINE_AUTO_SEEK_OVER:
        *(bool*)(msg_reply.content) = s_engine_seek.seek_over_flg;
        ret = 0;
        break;
        
    case MSG_FMENGINE_GET_AUTO_SEEK:
        memcpy(msg_reply.content, &s_auto_seek_info, sizeof(eg_seek_info_t));
        if( s_engine_seek.seek_over_flg == false )
        {
            restart_timer(s_engine_seek.timer_id_seek );
        }

        ret = 0;
        break;

    case MSG_ACTIVE_SYNC:
        ret = 0;
        break;
        
    case MSG_APP_QUIT:
        
        exit_msg_loop();
        break;

    default:
        break;
    }

    if(ret != -2)
    {
        msg_reply.type = (ret == 0) ? MSG_REPLY_SUCCESS : MSG_REPLY_FAILED;
        reply_sync_msg(&msg_reply);
    }

    return ret;
}

#if 0
/*!
 * ��������line in
 * ��������
 * ����ֵ����
 */
static void _check_earinsert(void)
{
    int value, result,valueli;
    msg_apps_t msg;
    static bool inserted = true;
    static bool lo_linein = false;
    static bool lo_stopped = false;

    result = ioctl(s_fd, RADIO_GET_ANTENNA, (unsigned long)&value);
    ioctl(s_fd, RADIO_CHECK_LINEIN,(unsigned long)&valueli);
    
    if(value == 0)
    {
        msg.type = MSG_FMENGINE_EARPHONE_NOT_INSERT;
        broadcast_msg(&msg);

        if(inserted == true)
        {
            if(lo_stopped == false)
            {
                fm_engine_stop();
                lo_stopped = true;
            }
            inserted =  false;
        }
        return;
    }
    else
    {
        if(inserted == false)
        {
            if((lo_stopped == true)&& (valueli != LINEIN_IN))
            {
                fm_engine_play();
                lo_stopped = false;
            }
            inserted =  true;
        }
    }


    
   
    if(valueli == LINEIN_IN)
    {
        
        msg.type = MSG_FMENGINE_LINEIN_INSERT;
        broadcast_msg(&msg);
       
        if(false == lo_linein)
        {
            lo_linein = true;
            if(lo_stopped == false)
            {
                fm_engine_stop();
                lo_stopped = true;
            }
        }
    }
    
    else/*û��line in*/
    {
        if(true == lo_linein)
        {
            if((lo_stopped == true)&& (inserted == true))
            {
                fm_engine_play();
                lo_stopped = false;
            }
            lo_linein = false;
            
        }
    }
}

/*!
 * ������
 * ��������
 * ����ֵ����
 */
static void _check_earinsert_nolinein(void)
{
    int value, result;
    msg_apps_t msg;
    static bool inserted = true;

    result = ioctl(s_fd, RADIO_GET_ANTENNA, (unsigned long) &value);
   
    if (value == 0)
    {
        msg.type = MSG_FMENGINE_EARPHONE_NOT_INSERT;
        broadcast_msg(&msg);

        if (inserted == true)
        {
            fm_engine_stop();
            inserted = false;
        }
    }
    else
    {
        if (inserted == false)
        {
            fm_engine_play();
            inserted = true;
        }
    }
}
#endif

/*
�������Ƿ����
build by dengjihai
*/
static void _check_earinsert( void )
{
    int result;
    int ear_value,line_in_value;
    msg_apps_t msg;
    
    result = ioctl(s_fd, RADIO_GET_ANTENNA, (unsigned long)&ear_value);
    if( result < 0 )
    {
        print_err("RADIO_GET_ANTENNA error!\n");
        return;
    }
    result = ioctl(s_fd, RADIO_CHECK_LINEIN,(unsigned long)&line_in_value);
    if( result < 0 )
    {
        print_err("RADIO_CHECK_LINEIN error!\n");
        return;
    }
    
    /*δ����������line in��ͣ��*/
    if((0 == ear_value) || (LINEIN_IN == line_in_value))
    {      
        /*��ǰ�Ǵ򿪵ģ���ر�*/
        if( FM_PLAYING == s_engine_state )
        {
            fm_engine_stop();
        }
        
        if( 0 == ear_value )
        {
            msg.type = MSG_FMENGINE_EARPHONE_NOT_INSERT;
            broadcast_msg(&msg);
        }
        else
        {
            msg.type = MSG_FMENGINE_LINEIN_INSERT;
            broadcast_msg(&msg);
        }
        return;
    }
    else
    {
        /*������ǰ�ǹرյģ�����Ӧ��ϣ���򿪣��������*/
        /*��������Ŀ�����ھ���ʱ�γ��������ٲ��벢��������������ʾѡ��yes����֤��ʱ���Ǵ��ھ���״̬*/
        if( (FM_STOPPED == s_engine_state ) && (FM_PLAYING == s_app_expect_state) )
        {
            fm_engine_play();
        }
    }
}


/*!
 * ������
 * ��������
 * ����ֵ����
 */
static void _check_earinsert_nolinein(void)
{
    int value, result;
    msg_apps_t msg;

    result = ioctl(s_fd, RADIO_GET_ANTENNA, (unsigned long) &value);
    if( result < 0 )
    {
        print_err("RADIO_GET_ANTENNA error!\n");
        return;
    }
   
    if (value == 0)
    {
        if ( FM_PLAYING == s_engine_state )
        {
            fm_engine_stop();
        }
        
        msg.type = MSG_FMENGINE_EARPHONE_NOT_INSERT;
        broadcast_msg(&msg);        
    }
    else
    {
        /*������ǰ�ǹرյģ�����Ӧ��ϣ���򿪣��������*/
        /*��������Ŀ�����ھ���ʱ�γ��������ٲ��벢��������������ʾѡ��yes����֤��ʱ���Ǵ��ھ���״̬*/
        if( (FM_STOPPED == s_engine_state ) && (FM_PLAYING == s_app_expect_state) )
        {
            fm_engine_play();
        }
    }
}

/*
 * FM����main����
 */
int main(int argc, const char *argv[])
{
    int ret = 0;
    int timer_id;
    msg_apps_t msg;
    void *l_config_lib = NULL;
    int l_check_earinsert = 0;

    _lib_init();

    /* Ӧ�ÿ��ʼ�� */
    applib_init(argc, argv, APP_TYPE_CONSOLE);
    
    l_config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (l_config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        return -1;
    }
    if (get_config(CFG_FM_LINEIN_CHECK, (char *) &l_check_earinsert, sizeof(l_check_earinsert)) == -1)
    {
        return -1;
    }
    

    if (!fm_engine_open())
    {
        print_err("open fm device file error\n");
        fm_engine_close();
        if(dlclose(l_config_lib) < 0)
        {
            print_err("close config lib err!");
            return -1;
        }
        applib_quit();
        return -1;
    }
    
    if(1 == l_check_earinsert)
    {
        timer_id = set_timer(1000, (timer_proc)_check_earinsert, NULL);
    }
    else
    {
        timer_id = set_timer(1000, (timer_proc)_check_earinsert_nolinein, NULL); 
    }
    s_engine_seek.timer_id_seek = -1;
    s_engine_seek.timer_id_play = -1;
    
    sys_set_key_tone_enable(0);/*�ذ�����*/

    register_sys_dispatcher((sys_msg_proc) fmengine_cb);

    while (get_msg(&msg) == TRUE)
    {
        dispatch_msg(&msg);
    }
    unregister_sys_dispatcher();

    kill_timer(timer_id);
    if (s_engine_seek.timer_id_seek != -1)
    {
        kill_timer(s_engine_seek.timer_id_seek);
        s_engine_seek.timer_id_seek = -1;
    }
    if (s_engine_seek.timer_id_play != -1)
    {
        kill_timer(s_engine_seek.timer_id_play);
        s_engine_seek.timer_id_play = -1;
    }
    
    sys_set_key_tone_enable(1);/*��������*/

    fm_engine_close();
    if(dlclose(l_config_lib) < 0)
    {
        print_err("close config lib err!");
        return -1;
    }

    /* Ӧ�ÿ��˳� */
    applib_quit();

    _lib_deinit();

    return ret;
}

