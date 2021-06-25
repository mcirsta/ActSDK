#ifndef __EMULATOR_H_
#define __EMULATOR_H_

#define EMULATOR_SIMCOMMON_FIFO  "/tmp/emulator/.emulator_fifo"
#define EMULATOR_SIMCOMMON_SYN  "/tmp/emulator/.emualtor_syn"
#define EMULATOR_SIMCOMMON_KEYMSG "/tmp/emulator/.emulator_keymsg"

typedef enum
{
    HW_QVGA,
    HW_QCF,
    SUPPORT_NUM
} lcd_types_support_t;

typedef unsigned int simcomm_mq_handle_t;

//��ʼ��emulator
#define  EMULATOR_INIT   0x1
//reset  emulator
#define  EMULATOR_RESET  0x2
//load rom
#define  EMULATOR_LOAD_ROM 0x3
//malloc cache RAM space
#define  EMULATOR_MALLOC_CACHE  0x4

#define EMULATOR_LOW_POWER  0x5
#define EMULATOR_USB_CON    0x6
#define EMULATOR_INTERNAL_ERR 0x7
#define EMULATOR_START_FROM_FILENAVI 0x8
#define EMULATOR_EXIT_LOOP  0x9
#define EMULATOR_USB_RECHARGE  0xa

typedef int (*emulator_machine_init)(void);
typedef void(*emulator_machine_reset)(void);
typedef int (*emulator_rom_cache_malloc)();
typedef int (*emulator_rom_cache_free)();
typedef void(*emulator_rom_cache_init)();
typedef int (*emulator_rom_load)(char * fileName);
typedef int (*emulator_rom_free)();
typedef int (*emulator_rom_get_status)();
typedef void (*emulator_rom_set_status)();
typedef int (*emulator_rom_save_state)(int misc, char *version);
typedef int (*emulator_rom_load_state)(int misc);
typedef int (*emulator_audio_get_status)();
typedef void (*emulator_audio_set_status)(int status);
typedef char* (*emulator_get_mapper_name)(void);
typedef int (*emulator_get_keyboard_map)(int *mapArray);
typedef int (*emulator_set_keyboard_map)(int *mapArray);
typedef int (*emulator_get_video_mode)(void);
typedef void (*emulator_set_video_mode)(int mode);
typedef void (*emulator_startaudio)();
typedef void (*emulator_stopaudio)();
typedef void (*emulator_enqueuekey)(int key, int state);
typedef int (*emulator_setreturnkey)(int key);
typedef int (*emulator_setphysconfig)(lcd_types_support_t config);

typedef struct
{
    simcomm_mq_handle_t msgqueue; //ui���͸�emulator core����Ϣ����
    simcomm_mq_handle_t synSemaphore; //emulator���ͻ�����Ϣ�Ķ���
    simcomm_mq_handle_t keymsg; //emulator�հ�����Ϣ����


} emulator_core_queue_t;

#define MAX_FILENAME_LEN 1024
#define SHORT_FILE_NAME  13

typedef struct
{
    char dirName[MAX_FILENAME_LEN]; // Ŀ¼�������� "c:\\nes\\" ,���ڷ�б��'\'
    char fileName[SHORT_FILE_NAME]; // �ļ���

} EmuFileNaviParam;
typedef struct
{
    int btn_map[8]; //����ӳ�䣬�ֱ��Ӧ��A��B��Select, START, Up, Down, Left, Right
    int video_mode; //��Ļ��С��1,ȫ���� 0�� ������С
    int audio_enable; //�����Ƿ�򿪣�1�Ѵ򿪣�0,�ر�
} emu_config_info_t;

typedef struct
{
    int charge_status_before_reset; //reset֮ǰ�Ƿ���
} ResetParam;

#define EMUAPI  ((emulator_api_t *)&taskParam->emuApi)

#define ENTER_EMU_NO_RESET

/*!
 *  state of emulator core \n
 *  \li EMU_RUNNING : the emulator core has exit
 *  \li EMU_SLEEP   : the emuator is wait for sth
 *  \li EMU_EXIT    :  emulator has exited
 *
 */

typedef enum
{
    EMU_STAT_STOP = 0,
    EMU_STAT_RUNNING,
    EMU_STAT_PAUSE

} emu_state_t;

typedef struct
{
    unsigned int type; // ��Ϣ�����͡�
    unsigned int sub_type; // ��Ϣ�����͡�
    union // ��Ϣ�����ݻ������
    {
        unsigned char pointer[MAX_FILENAME_LEN];
        unsigned short data[2];
    } msg;
} emu_ctrl_t;

typedef struct
{
    unsigned int type; // ��Ϣ�����͡�
    unsigned int sub_type; // ��Ϣ�����͡�
    union // ��Ϣ�����ݻ������
    {
        unsigned int pointer;
        unsigned short data[2]; /*data[0]��ֵ��data[1](1���£�0̧��)*/
    } msg;
} emu_input_msg_t;

/************************* ������ ***********************************
 *
 * typedef struct {
 INT8U        type;           // ��Ϣ�����͡�
 INT8U        sub_type;       // ��Ϣ�����͡�
 union                        // ��Ϣ�����ݻ������
 {
 INT32U    pointer;
 INT16U   data[2];
 }msg;
 }msg_t;

 *   EMU_INIT: msg_t:  type=EMU_INIT
 *   EMU_PLAY: msg_t: type=EMU_PLAY, pointer: Ҫ����rom��·����,���msg.pointer
 *                   ֵΪ0, ��ô��ʾplay��ǰ��rom
 *                   sub_type=0, ��������, sub_type=1, �ӽ����ļ�����ĵط���ʼ
 *   EMU_CONINUE: ������ǰrom�Ĳ���(����֮ǰ����ͣ��)
 *   EMU_RESET:   ����ǰrom reset,���¿�ʼ
 *   EMU_EXIT:     �˳�ģ����
 *   EMU_SET:      �ı�����Ƶ���,�����Ƿ������������,��Ƶȫ������������С
 *
 *  ����������Ϣ����Ҫ��Ӧ
 *  һ������: �����߷�������,Ȼ��ȴ���syn�ź�����,�ȴ�����
 *
 *  EMU_KEY_ACTION:  msg.type=EMU_KEY_ACTION,
 *  msg.msg.data[0]=���µļ�, msg.msg.data[1]״̬(����/̧��)
 *
 *
 */

typedef enum
{
    EMU_INIT = 0,
    EMU_PLAY,
    EMU_CONTINUE,
    EMU_RESET,
    EMU_EXIT,
    EMU_KEY_ACTION,
    EMU_GET_ERR,
    EMU_SAVE_STATE,
    EMU_SET_AUDIO,
    EMU_SET_VIDEO,
    EMU_SET_KEYMAP,
    EMU_SET_RETURNKEY
} emu_cmd_t;

typedef enum
{
    EMU_ERR_OK = 0,
    EMU_ERR_NOT_SUPPORT,
    EMU_ERR_FILE_MISSSING,
    EMU_ERR_RETURN_REQUEST

} emulator_error_t;

typedef enum
{
    EMU_MODE_NORMAL = 0,
    EMU_MODE_FULLSCR

} emulator_video_mode_t;

typedef enum
{
    EMU_PLAY_MODE_RESTART = 0,
    EMU_PLAY_MODE_RESET,
    EMU_PLAY_MODE_LOADSTAT
} emulator_play_mode_t;

//SUPPORT_BUFFERED_IO: ����ģ����,����򿪴˺�,��ô��������ö��з�ʽ,Ч�����20%����
//�������ڶ��з�ʽ������buffer��С,�ʶ�rom��ʹ�õ�buffer�����.
//ÿ��,�������/�ر�SUPPORT_BUFFERED_IO,��ô��Ҫ���±���sdk/simĿ¼�������ļ�
#define SUPPORT_BUFFERED_IO

#endif

