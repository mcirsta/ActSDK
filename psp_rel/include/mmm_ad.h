#include "mmm_decoder.h"

#define MMM_SET_FILE_FORMAT 0x40

/* 定义seek的参数结构 */
typedef struct
{
    /* 获取信息函数地址 */
    void *func;

    char *format;
    int file_len;
    int total_time;
    mmm_audio_tag_t *audio_tag;

} seek_param_t;

void * mmm_audio_open(void *param);
int mmm_audio_dec(void *handle, unsigned int cmd, unsigned int param);
int mmm_audio_close(void *handle, void *param);
