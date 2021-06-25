#ifndef _SSF_H_
#define _SSF_H_

typedef enum SSF_CMD 
{
    SSF_PLAY=0,  // 正常播放状态, 该状态下ssf_ioctl（）的value值被忽略
    SSF_NEW_FRQ  // 导入新的频点，value值为18个频率点对应的幅值
    
}se_cmd_t;

typedef struct ssf_env_s {
    void *buf;                          // 显示开始地址，可能是frameBuffer+offset
    unsigned int buf_width_in_pixel;    // frameBuffer的宽，按像素计算
    unsigned int output_w;              // 输出区域的宽
    unsigned int output_h;              // 输出区域的高
    unsigned int bpp;             // 象素深度，rgb565为2，目前只支持rgb565
} se_env_t;


/******************
打开特效
输入：初始化正确的参数
输出：正常为0，否则-1;
**********************/
void* se_ssf_open(se_env_t *ssf_env);

/******************
关闭特效,不管出现什么状况，这个函数必须和ssf_open()成对使用。
**********************/
void se_ssf_close(void *handle);

/**********************
控制函数,输出正确的指令，并将必要的参数通过value传递
*******************/
int se_ssf_ioctl(void *handle, se_cmd_t cmd, int param);



void* se_wav_open(se_env_t *se_wav);

void se_wav_close(void *handle);

int se_wav_ioctl(void *handle, se_cmd_t cmd, int param);

/*****************使用举例**************************/
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
        se_ssf_ioctl(handle, SSF_NEW_FRQ, (U32)frq);    //生成频点
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
