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

//初始化emulator
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
    simcomm_mq_handle_t msgqueue; //ui发送给emulator core的消息队列
    simcomm_mq_handle_t synSemaphore; //emulator发送会送消息的队列
    simcomm_mq_handle_t keymsg; //emulator收按键消息队列


} emulator_core_queue_t;

#define MAX_FILENAME_LEN 1024
#define SHORT_FILE_NAME  13

typedef struct
{
    char dirName[MAX_FILENAME_LEN]; // 目录名，形如 "c:\\nes\\" ,存在反斜线'\'
    char fileName[SHORT_FILE_NAME]; // 文件名

} EmuFileNaviParam;
typedef struct
{
    int btn_map[8]; //按键映射，分别对应于A，B，Select, START, Up, Down, Left, Right
    int video_mode; //屏幕大小，1,全屏， 0， 正常大小
    int audio_enable; //声音是否打开，1已打开，0,关闭
} emu_config_info_t;

typedef struct
{
    int charge_status_before_reset; //reset之前是否充电
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
    unsigned int type; // 消息的类型。
    unsigned int sub_type; // 消息的类型。
    union // 消息的内容或参数。
    {
        unsigned char pointer[MAX_FILENAME_LEN];
        unsigned short data[2];
    } msg;
} emu_ctrl_t;

typedef struct
{
    unsigned int type; // 消息的类型。
    unsigned int sub_type; // 消息的类型。
    union // 消息的内容或参数。
    {
        unsigned int pointer;
        unsigned short data[2]; /*data[0]键值，data[1](1按下，0抬起)*/
    } msg;
} emu_input_msg_t;

/************************* 命令字 ***********************************
 *
 * typedef struct {
 INT8U        type;           // 消息的类型。
 INT8U        sub_type;       // 消息的类型。
 union                        // 消息的内容或参数。
 {
 INT32U    pointer;
 INT16U   data[2];
 }msg;
 }msg_t;

 *   EMU_INIT: msg_t:  type=EMU_INIT
 *   EMU_PLAY: msg_t: type=EMU_PLAY, pointer: 要播放rom的路径名,如果msg.pointer
 *                   值为0, 那么表示play当前的rom
 *                   sub_type=0, 正常播放, sub_type=1, 从进度文件保存的地方开始
 *   EMU_CONINUE: 继续当前rom的播放(可能之前被暂停过)
 *   EMU_RESET:   将当前rom reset,重新开始
 *   EMU_EXIT:     退出模拟器
 *   EMU_SET:      改变音视频输出,包括是否允许声音输出,视频全屏还是正常大小
 *
 *  以上五种消息都需要回应
 *  一般序列: 发送者发送命令,然后等待在syn信号量上,等待返回
 *
 *  EMU_KEY_ACTION:  msg.type=EMU_KEY_ACTION,
 *  msg.msg.data[0]=按下的键, msg.msg.data[1]状态(按下/抬起)
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

//SUPPORT_BUFFERED_IO: 对于模拟器,如果打开此宏,那么输出将采用队列方式,效率提高20%左右
//但是由于队列方式增加了buffer大小,故而rom能使用的buffer会减少.
//每次,如果启动/关闭SUPPORT_BUFFERED_IO,那么需要重新编译sdk/sim目录下所有文件
#define SUPPORT_BUFFERED_IO

#endif

