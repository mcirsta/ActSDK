#ifndef _SSF_H_
#define _SSF_H_

typedef enum SSF_CMD 
{
    SSF_PLAY=0,  // ��������״̬, ��״̬��ssf_ioctl������valueֵ������
    SSF_NEW_FRQ  // �����µ�Ƶ�㣬valueֵΪ18��Ƶ�ʵ��Ӧ�ķ�ֵ
    
}se_cmd_t;

typedef struct ssf_env_s {
    void *buf;                          // ��ʾ��ʼ��ַ��������frameBuffer+offset
    unsigned int buf_width_in_pixel;    // frameBuffer�Ŀ������ؼ���
    unsigned int output_w;              // �������Ŀ�
    unsigned int output_h;              // �������ĸ�
    unsigned int bpp;             // ������ȣ�rgb565Ϊ2��Ŀǰֻ֧��rgb565
} se_env_t;


/******************
����Ч
���룺��ʼ����ȷ�Ĳ���
���������Ϊ0������-1;
**********************/
void* se_ssf_open(se_env_t *ssf_env);

/******************
�ر���Ч,���ܳ���ʲô״����������������ssf_open()�ɶ�ʹ�á�
**********************/
void se_ssf_close(void *handle);

/**********************
���ƺ���,�����ȷ��ָ�������Ҫ�Ĳ���ͨ��value����
*******************/
int se_ssf_ioctl(void *handle, se_cmd_t cmd, int param);



void* se_wav_open(se_env_t *se_wav);

void se_wav_close(void *handle);

int se_wav_ioctl(void *handle, se_cmd_t cmd, int param);

/*****************ʹ�þ���**************************/
/*
    
    void* handle;
    int frq[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    ssf_env_t ssf_env;
    ssf_env.buf= FrameBuffer;
    ssf_env.buf_width_in_pixel = 320;
    ssf_env.output_w = 320;
    ssf_env.output_h = 240;
    ssf_env.bpp = 2;

    se_ssf_open(&handle, &ssf_env);
    
        
    while(condition is TRUE)
    {
        se_ssf_ioctl(handle, SSF_NEW_FRQ, (U32)frq);    //����Ƶ��
        S_Set_Framebuffer_Changed();
        S_LCD_SCREEN_UPDATE();
    
        for(j=0;j<20;j++)
        {
            se_ssf_ioctl(handle, SSF_PLAY, 0);
            S_Set_Framebuffer_Changed();
            S_LCD_SCREEN_UPDATE();
        }
    }
    
    se_ssf_close(handle)
*/

#endif
