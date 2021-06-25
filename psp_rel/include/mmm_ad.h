#include "mmm_decoder.h"

#define MMM_SET_FILE_FORMAT 0x40

/* ����seek�Ĳ����ṹ */
typedef struct
{
    /* ��ȡ��Ϣ������ַ */
    void *func;

    char *format;
    int file_len;
    int total_time;
    mmm_audio_tag_t *audio_tag;

} seek_param_t;

void * mmm_audio_open(void *param);
int mmm_audio_dec(void *handle, unsigned int cmd, unsigned int param);
int mmm_audio_close(void *handle, void *param);
