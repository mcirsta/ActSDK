/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_lyric.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_lyric.c
* \brief    music_playing_lyric operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
 
#include "music_playing_lyric.h"


#define TIME_DELAY       (-1)	//负数表示将提前的秒数，正数表示将延迟的秒数
#if TIME_DELAY > 0
#define DELAY_SECONDS    TIME_DELAY
static int cur_delay;
#endif

#if TIME_DELAY < 0
#define DELAY_SECONDS    (TIME_DELAY*-1)
static int cur_delay;
#endif

#define LYRIC_BEGIN_ADDR 0
#define LYRIC_INDEX_ADDR LYRIC_BEGIN_ADDR
#define TIMESNUM_ADDR    LYRIC_INDEX_ADDR
#define TIMESTICK_ADDR   4+TIMESNUM_ADDR
#define LRCOFFSET_ADDR   8+TIMESNUM_ADDR
#define MUSICPATH_ADDR   12+TIMESNUM_ADDR
#define LYRIC_LRC_ADDR   LYRIC_INDEX_ADDR+512

#define MAX_TIME_STICK 29

typedef struct
{
    int lyricbuffer_len; //Current lyric page No, current lyric page is the lyric page got by SeekFor /GetNext/GetPrev
    int lyric_point;
} lyric_vars_t;

typedef struct
{
    unsigned char ms;
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
} lyric_time_t;

typedef struct
{
    lyric_vars_t lyric_vars;

    char *lyric_buffer;
    lyric_time_t time_stick[MAX_TIME_STICK];
    int time_stick_num;
} lyric_t;

typedef enum
{
    CharType_NUM, //数字（0-9）
    CharType_UPER, //大写字母（A-Z）
    CharType_DNER, //小写字母（a-z）
    CharType_SPACE, //空格 (" ")
    CharType_OTHER
//其他值
} char_type_e;


static void _strcon(char *string);

static void _convert_time_offset(lyric_offset_t *lyric_offset, char *tmpbuff,
        unsigned char i);
static bool _read_one_sector(lyric_t *Lyric);
static bool _adjust_banlaned_merge(void);
static char_type_e _check_type_char(char ch);
static bool _get_offset_data(lyric_t *Lyric);
static signed char _get_time_stick(lyric_t *Lyric);
static void _inside_merge(int num);
static bool _load_lyric_to_sd_buf(void);
static void _lyric_close_file(void);
static bool _lyric_load(void);
static bool _lyric_open_file(char *dirent);
static bool _read_offset(void);
static bool _resort_time_stick(void);
static bool _search_lrc_file(lyric_t *Lyric, char *str);
static int _write_lyric_to_sd_buf(lyric_t *Lyric, int lyric_addr);
static void _write_to_vbuf(void *buf, int addr, int size);
static void _read_from_sd(void *buf, int addr, int size);
static void _time_add(int *time, lyric_offset_t *lyric_offset);
static void _write_to_sd(int addr, int size);
static void _write_dword_to_vsector(int src, int offset);
static void _write_byte_to_vsector(char src, int offset);
static void _write_vsector_to_sd(int offset);

static char s_offsetkeyword[] = "[offset:";//找 "[OFFSET:"字串的位置

static int s_fs_handle;
static void *s_file_buf; //512
static void *s_lyric_tmp; //128
static void *s_sort_buf; //2k--512个时间点
static void *s_lrc_buffer; //1k

static lyric_info_t s_lyric_info;
static char s_vram[16*1024];    //for test


static void _strcon(char *string)
{
    for (;;)
    {
        if ( ( *string >= 'a' ) &&  (*string <= 'z' ) )
        {
            *string -= 0x20;
        }
        else if (*string == 0)
        {
            break;
        }
        else
        {
        }
        
        string++;
    }
}

static bool _lyric_load(void)
{
    if (!_read_offset()) //读OFFSET值
    {
        return false;
    }
    if ( true == _load_lyric_to_sd_buf() )
    {
        return _resort_time_stick();
    }

    return false;
}

static bool _lyric_open_file(char *dirent)
{
    char *last_dot;
    char fullpath[PATH_MAX];
    
    /* Find last '.'  */
    strncpy(fullpath, dirent, sizeof(fullpath));
    last_dot = strrchr(fullpath, '.'); 
    if( last_dot == NULL )
    {
        print_err("file name error!");
        return false;
    }    
    else
    {
        last_dot ++;
        strncpy(last_dot, "lrc", sizeof("lrc"));
    }
        
    // open file
    s_fs_handle = open( fullpath, O_RDONLY );
    if ( s_fs_handle < 0 )
    {
        print_dbg( "open file %s failed!", fullpath );
        return false;
    }
    
    return true;
}

static void _lyric_close_file(void)
{
    close(s_fs_handle);
}

static bool _resort_time_stick(void)
{
    return _adjust_banlaned_merge();
}
static bool _read_offset(void)
{
    bool result;
    lyric_t *lyric = (lyric_t *) s_lyric_tmp;
    lyric->lyric_buffer = (unsigned char *) s_file_buf;
    lyric->lyric_vars.lyricbuffer_len = 0;
    lyric->lyric_vars.lyric_point = 0;

    lyric->lyric_vars.lyricbuffer_len = read(s_fs_handle, lyric->lyric_buffer, 512);
    
    if ( lyric->lyric_vars.lyricbuffer_len <= 0)
    {
        s_lyric_info.lyric_offset.signflag = 0;
        return false;
    }

    result = _search_lrc_file(lyric, (char*)s_offsetkeyword);    
    if ( result == true)
    {
        result = _get_offset_data(lyric);
    }

    if (!result)
    {
        s_lyric_info.lyric_offset.signflag = 0;
    }
    return true;
}

static bool _search_lrc_file(lyric_t *Lyric, char *str)
{
    int offset = 0;
    unsigned char i = 0;

    offset = Lyric->lyric_vars.lyric_point & 0x000001ff;

    while (1)
    {
        if (offset >= Lyric->lyric_vars.lyricbuffer_len)
        {
            if (!_read_one_sector(Lyric))
            {
                return false;
            }
            offset = 0;
        }
        if (str[i] == 0)
        {
            break;
        }
        if (Lyric->lyric_buffer[offset] == str[i])
        {
            i++;
        }
        else
        {
            i = 0;
        }
        offset++;
    }
    Lyric->lyric_vars.lyric_point = offset + (Lyric->lyric_vars.lyric_point
            & (int)0xfffffe00);
    return true;
}
static bool _get_offset_data(lyric_t *Lyric)
{
    unsigned char tmpbuff[5] =
    { 0, 0, 0, 0, 0 };
    unsigned char i = 0;
    int offset;

    offset = Lyric->lyric_vars.lyric_point & 0x000001ff;

    if (Lyric->lyric_buffer[offset] == '-')
    {
        s_lyric_info.lyric_offset.signflag = -1;
        offset++;
    }
    else if (Lyric->lyric_buffer[offset] == '+')
    {
        s_lyric_info.lyric_offset.signflag = 1;
        offset++;
    }
    else
    {
        s_lyric_info.lyric_offset.signflag = 1;
    }

    while (1)
    {
        if (offset >= Lyric->lyric_vars.lyricbuffer_len)
        {
            if (!_read_one_sector(Lyric))
            {
                return false;
            }
            offset = 0;
        }
        if (Lyric->lyric_buffer[offset] == ']')
        {
            //已读到数据，作转换，并返回真
            _convert_time_offset(&s_lyric_info.lyric_offset, tmpbuff, i);
            break;
        }
        else if (_check_type_char(Lyric->lyric_buffer[offset]) != CharType_NUM)//碰到不是数字的就出
        {
            return false;
        }
        else
        {
        }

        if (i < 5)//BUFF 只有五个byte
        {
            tmpbuff[i] = (unsigned char)(Lyric->lyric_buffer[offset] - (char)'0');
            i++;
        }

        offset++;
    }
    
    Lyric->lyric_vars.lyric_point = offset + (Lyric->lyric_vars.lyric_point & (int)0xfffffe00);
    
    return true;
}

static void _convert_time_offset(lyric_offset_t *lyric_offset, char *tmpbuff,
        unsigned char i)
{
    unsigned char j;
    for (j = 0; j < i; j++)
    {
        tmpbuff[4 - j] = tmpbuff[i - j - 1];
    }
    for (j = 0; j < (5 - i); j++)
    {
        tmpbuff[j] = 0;
    }
    lyric_offset->offset_sec = ( tmpbuff[0] * 10 ) + tmpbuff[1];//求出秒数
    lyric_offset->offset_p1s = tmpbuff[2];
}
static bool _read_one_sector(lyric_t *Lyric)
{
    Lyric->lyric_vars.lyricbuffer_len = read(s_fs_handle, Lyric->lyric_buffer, 512);
    if (Lyric->lyric_vars.lyricbuffer_len > 0)
    {
        Lyric->lyric_vars.lyric_point += Lyric->lyric_vars.lyricbuffer_len;
        return true;
    }

    return false;
}
static bool _load_lyric_to_sd_buf(void)
{
    signed char num;
    signed char i;
    int lyric_addr = LYRIC_LRC_ADDR;
    int _time_addr = 0;

    lyric_t *Lyric = (lyric_t *) s_lyric_tmp;
    Lyric->lyric_buffer = (unsigned char *) s_file_buf;

    Lyric->lyric_vars.lyricbuffer_len = 0;
    Lyric->lyric_vars.lyric_point = 0;
    Lyric->time_stick_num = 0;

    lseek(s_fs_handle, 0, SEEK_SET);
    Lyric->lyric_vars.lyricbuffer_len = read(s_fs_handle, Lyric->lyric_buffer,512);
    if (Lyric->lyric_vars.lyricbuffer_len <= 0)
    {
        s_lyric_info.lyric_offset.signflag = 0;
        return false;
    }

    num = _get_time_stick(Lyric);
    
    while (num > 0)
    {
        Lyric->time_stick_num += num;
        for (i = 0; i < num; i++)
        {
            _write_to_vbuf(&Lyric->time_stick[i], _time_addr, sizeof(lyric_time_t));
            _time_addr += (int)sizeof(lyric_time_t);
            _write_to_vbuf(&lyric_addr, _time_addr, sizeof(int));
            _time_addr += (int)sizeof(int);
        }
        lyric_addr += _write_lyric_to_sd_buf(Lyric, lyric_addr);
        
        num = _get_time_stick(Lyric);    
    }
    
    if ( 0 != (lyric_addr & 0x000001ff) )
    {
        _write_vsector_to_sd(lyric_addr & (int)0xfffffe00);
    }

    s_lyric_info.lyric_index.lyric_total = (int) Lyric->time_stick_num;
    if ( 0 != (((int) lyric_addr) & 0x01ff) )
    {
        s_lyric_info.lyric_index.lyric_time_addr = (lyric_addr
                & (int)0xfffffe00) + 512;
    }
    else
    {
        s_lyric_info.lyric_index.lyric_time_addr = (lyric_addr
                & (int)0xfffffe00);
    }

    _write_dword_to_vsector(Lyric->time_stick_num, 0);
    _write_dword_to_vsector(s_lyric_info.lyric_index.lyric_time_addr,
            TIMESTICK_ADDR);
    _write_dword_to_vsector(*((int*) &s_lyric_info.lyric_offset),
            LRCOFFSET_ADDR);
    _write_vsector_to_sd(LYRIC_INDEX_ADDR);
    return true;
}
static int _write_lyric_to_sd_buf(lyric_t *Lyric, int lyric_addr)
{
    int offset;
    int sector_off, sector_num = 0;
    int status = 0;
    char letter;
    int writenum = 0;
    offset = Lyric->lyric_vars.lyric_point & 0x000001ff;
    sector_off = lyric_addr & 0x000001ff;

    while (1)
    {
        if (status == 1) //上一次文件已到尾，加个'\0'就结束
        {
            letter = 0;
            status = 2; //标记本次结束
        }
        else
        {
            letter = Lyric->lyric_buffer[offset];
        }
        
        if (letter == 0x0d)
        {
            letter = 0x0a;
            // continue;
        }
        else if( (letter == '[') && (Lyric->lyric_buffer[offset+3] == ':') && \
               (Lyric->lyric_buffer[offset+6] == '.') && (Lyric->lyric_buffer[offset+9] == ']'))
        {
            letter = 0x0a; 
        }  
        else
        {
        }  

        if (letter == 0x0a)
        {
            letter = 0;
            _write_byte_to_vsector(letter, sector_off);
            sector_off++;
            writenum++;
            if (sector_off >= 512)
            {
                sector_off = 0;
                _write_vsector_to_sd((lyric_addr & (int)0xfffffe00) + (sector_num << 9));
                sector_num++;
            }
            break;
        }
        _write_byte_to_vsector(letter, sector_off);
        sector_off++;
        writenum++;
        if (sector_off >= 512)
        {
            sector_off = 0;
            _write_vsector_to_sd((lyric_addr & (int)0xfffffe00) + (sector_num << 9));
            sector_num++;
        }
        if (status == 2) //因为文件到尾，产生的结束
        {
            break;
        }
        offset++;
        if(offset >= Lyric->lyric_vars.lyricbuffer_len)
        {
            if(!_read_one_sector( Lyric))
            {
                status = 1;	//文件已到尾
            }
            if(offset>=512)
            {
                offset = 0;
            }
        }
    }
    Lyric->lyric_vars.lyric_point = offset + (Lyric->lyric_vars.lyric_point
            & (int)0xfffffe00);
    return writenum;
}

static signed char _get_time_stick(lyric_t *Lyric)
{
    int offset;
    char letter;
    unsigned char timeval;
    signed char num = 0;
    signed char n = 0;
    bool is_loop = true;
    
    if (!_search_lrc_file(Lyric, "["))
    {
        return -1;
    }
    offset = Lyric->lyric_vars.lyric_point & 0x000001ff;

    while (1)
    {
        letter = Lyric->lyric_buffer[offset];
        offset++;
        if ((letter < '0') || (letter > '9'))//不是数字的
        {
            if (!_search_lrc_file(Lyric, "["))
            {
                return -1;
            }
            offset = Lyric->lyric_vars.lyric_point & 0x000001ff;
            continue;
        }
        timeval = (unsigned char)(letter - '0');
        n = 0;        
        is_loop = true;
        
        while (is_loop == true)
        {
            if (offset >= Lyric->lyric_vars.lyricbuffer_len)
            {
                if (!_read_one_sector(Lyric))
                {
                    return -1;
                }
                offset = 0;
            }
            letter = Lyric->lyric_buffer[offset];
            offset++;            

            switch(letter)
            {
                case ':':
                {
                    Lyric->time_stick[num].minute = timeval;
                    timeval = 0;
                    n++;
                    break;
                }
                
                case '.':
                {
                    Lyric->time_stick[num].second = timeval;
                    timeval = 0;
                    n++;
                    break;
                }
                
                case ']':
                {
                    Lyric->time_stick[num].hour = 0;
                    if (n == 1)
                    {
                        Lyric->time_stick[num].second = timeval;
                    }
                    else
                    {
                        Lyric->time_stick[num].ms = timeval;
                    }
                    num++;
                    is_loop = false;
                    break;
                }
                
                case '-':
                {
                    if (!_search_lrc_file(Lyric, "]"))
                    {
                        return -1;
                    }
                    offset = Lyric->lyric_vars.lyric_point & 0x000001ff;
                    is_loop = false;
                    break;
                }

                default:
                {
                    if ((letter < '0') || (letter > '9'))//不是数字的
                    {
                        if (!_search_lrc_file(Lyric, "["))
                        {
                            return -1;
                        }
                        offset = Lyric->lyric_vars.lyric_point & 0x000001ff;
                        is_loop = false;
                        letter = '-';   //continue
                    }
                    else
                    {
                        timeval = ((timeval * 10) + letter) - '0';
                    }
                    break;
                }
            }  
        }

        if (letter == '-')
        {
            continue;
        }
        if (offset >= Lyric->lyric_vars.lyricbuffer_len)
        {
            if (!_read_one_sector(Lyric))
            {
                return -1;
            }
            offset = 0;
        }
        if (num >= MAX_TIME_STICK)
        {
            num = MAX_TIME_STICK-1;
        }
        if (Lyric->lyric_buffer[offset] == '[')
        {
            offset++;
            if (offset >= Lyric->lyric_vars.lyricbuffer_len)
            {
                if (!_read_one_sector(Lyric))
                {
                    return -1;
                }
                offset = 0;
            }

        }
        else
        {
            break;
        }
    }
    Lyric->lyric_vars.lyric_point = offset + (Lyric->lyric_vars.lyric_point
            & (int)0xfffffe00);
    return num;
}
static char_type_e _check_type_char(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
    {
        return CharType_NUM;
    }
    else if ((ch >= 'A') && (ch <= 'Z'))
    {
        return CharType_UPER;
    }
    else if ((ch >= 'a') && (ch <= 'z'))
    {
        return CharType_DNER;
    }
    else if (ch == ' ')
    {
        return CharType_SPACE;
    }
    else
    {
    }

    return CharType_OTHER;
}

static bool _adjust_banlaned_merge(void)
{
    int num;
    int lyric_addr;

    num = s_lyric_info.lyric_index.lyric_total;
    lyric_addr = s_lyric_info.lyric_index.lyric_time_addr;
    _inside_merge(num);
    //DEBUG_PRINT("we have get sizeof lyric_sorttime=%d\n",num*sizeof(timeindex_t));
    _write_to_sd(lyric_addr, num * (int)sizeof(timeindex_t));
    return true;
}

static void _inside_merge(int num)
{
    int i, j, n, k;
    timeindex_t *timeindex = (timeindex_t*) s_sort_buf;
    timeindex_t tmp;

    for (n = 0, k = 0, j = num; n < num; n++, k = n, j--)
    {
        for (i = k + 1; i < num; i++)
        {
            if (timeindex[k].timestick > timeindex[i].timestick)
            {
                k = i;
            }
        }
        if (k != n)
        {
            memcpy(&tmp, &timeindex[n], sizeof(timeindex_t));
            memcpy(&timeindex[n], &timeindex[k], sizeof(timeindex_t));
            memcpy(&timeindex[k], &tmp, sizeof(timeindex_t));
        }
    }
}

static void _time_add(int *time, lyric_offset_t *lyric_offset)
{
    signed char *distime = (signed char*) time;
    signed char sit = 0;
    if (lyric_offset->signflag > 0)
    {
        if ( (distime[0] + lyric_offset->offset_p1s) > 10)
        {
            distime[0] = ( distime[0] + lyric_offset->offset_p1s ) - 10;
            sit = 1;
        }
        else
        {
            distime[0] = distime[0] + lyric_offset->offset_p1s;
            sit = 0;
        }
        if ( (distime[1] + lyric_offset->offset_sec + sit ) > 60)
        {
            distime[1] = ( ( distime[1] + lyric_offset->offset_sec ) + sit ) - 60;
            sit = 1;
        }
        else
        {
            distime[1] = ( distime[1] + lyric_offset->offset_sec ) + sit;
            sit = 0;
        }
        distime[2] += sit;
    }
    else if (lyric_offset->signflag < 0)
    {
        if ( (distime[0] - lyric_offset->offset_p1s) < 0)
        {
            distime[0] = ( distime[0] + 10 ) - lyric_offset->offset_p1s;
            sit = 1;
        }
        else
        {
            distime[0] = distime[0] - lyric_offset->offset_p1s;
            sit = 0;
        }
        if ((distime[1] - lyric_offset->offset_sec - sit) < 0)
        {
            distime[1] = ( distime[1] + 60 ) - lyric_offset->offset_sec - sit;
            sit = 1;
        }
        else
        {
            distime[1] = distime[1] - lyric_offset->offset_sec - sit;
            sit = 0;
        }
        distime[2] -= sit;
        if (distime[2] < 0)
        {
            *time = 0;
        }
    }
    else
    {
    }
}
static void _write_to_vbuf(void *buf, int addr, int size)
{
    memcpy((char*) s_sort_buf + addr, buf, (unsigned int)size);
}

static void _read_from_sd(void *buf, int addr, int size)
{
    if ( addr > (int)(sizeof(s_vram) - 1) )
    {
        return;
    }
    else
    {
        memcpy( buf, &s_vram[addr], (unsigned int)size );
    }
}

static void _write_to_sd(int addr, int size)
{
    if ( addr > (int)(sizeof(s_vram) - 1) )
    {
        return;
    }
    else
    {
        memcpy( &s_vram[addr], s_sort_buf, (unsigned int)size );
    }      
}

static void _write_vsector_to_sd(int offset)
{
    if ( offset > (int)(sizeof(s_vram) - 1) )
    {
        return;
    }
    else
    {
        memcpy( &s_vram[offset], s_lrc_buffer, 512 );
    }  
}

static void _write_dword_to_vsector(int src, int offset)
{
    *((int*) ((char*) s_lrc_buffer + offset)) = src;
}

static void _write_byte_to_vsector(char src, int offset)
{
    *((char*) s_lrc_buffer + offset) = src;
}

bool lyric_init(char *dirent)
{
    bool result;
    void *heap_buf;
    char ext[4];

    if (!_lyric_open_file(dirent))
    {
        return false;
    }

#if TIME_DELAY != 0

    strcpy(ext, ( dirent + strlen(dirent) ) - 3);
    _strcon(ext);
    if (strcmp(ext, "MP3") == 0)
    {
        cur_delay = DELAY_SECONDS;// + 1;
    }
    else
    {
        cur_delay = DELAY_SECONDS;
    }
#endif

    heap_buf = (void*) malloc(sizeof(lyric_t) + 512 + 2048 + 1024);
    if (heap_buf == NULL)
    {
        printf("lyric heap buf error\n");
        return false;
    }
    s_lyric_tmp = heap_buf;
    s_file_buf = (char*) heap_buf + sizeof(lyric_t); //simulator_pc   08-7-17
    s_sort_buf = (char*) heap_buf + sizeof(lyric_t) + 512;
    s_lrc_buffer = (char*) heap_buf + sizeof(lyric_t) + 512 + 2048;

    result = _lyric_load();

    free(heap_buf);

    memset(&s_lyric_info.lyric_index.timeindex, 0, sizeof(timeindex_t)
            * 3);
    s_lyric_info.lyric_index.lyric_cur = -1;
    _lyric_close_file();

    return result;
}

bool lyric_get_prev(char *lyric_buf, int len)
{
    if (s_lyric_info.lyric_index.lyric_cur > 1)
    {
        _read_from_sd(lyric_buf,
                s_lyric_info.lyric_index.timeindex[0].addr, len);
        lyric_buf[len - 1] = 0;
        return true;
    }
    lyric_buf[0] = 0;
    return false;
}
bool lyric_get_next(char *lyric_buf, int len)
{
    if ( ( s_lyric_info.lyric_index.lyric_cur >= 0 )
            && ( s_lyric_info.lyric_index.lyric_cur
                    <= (s_lyric_info.lyric_index.lyric_total - 1) ) )
    {
        _read_from_sd(lyric_buf,
                s_lyric_info.lyric_index.timeindex[2].addr, len);
        lyric_buf[len - 1] = 0;
        return true;
    }
    lyric_buf[0] = 0;
    return false;
}
bool lyric_get_current(char *lyric_buf, int len)
{
    if ( ( s_lyric_info.lyric_index.lyric_cur > 0 )
            && ( s_lyric_info.lyric_index.lyric_cur
                    <= s_lyric_info.lyric_index.lyric_total ) )
    {
        _read_from_sd(lyric_buf,
                s_lyric_info.lyric_index.timeindex[1].addr, len);
        lyric_buf[len - 1] = 0;
        return true;
    }
    lyric_buf[0] = 0;
    return false;
}
bool lyric_set_current_time(int time)
{
    unsigned char dectime[4];
    int ms;

    ms = time % 1000;
    time = time / 1000;

#if TIME_DELAY > 0
    if( time >= cur_delay )
    {
        time -= cur_delay;
    }
#endif
#if TIME_DELAY < 0
    time += cur_delay;
#endif

    dectime[3] = (unsigned char)(time / 3600);
    time = time % 3600;
    dectime[2] = (unsigned char)(time / 60);
    dectime[1] = (unsigned char)(time % 60);
    dectime[0] = (unsigned char)(ms / 100);

    if ( 0 != s_lyric_info.lyric_offset.signflag )
    {
        _time_add((int*) dectime, &s_lyric_info.lyric_offset);
    }
    while (1)
    {
        if (*((int*) dectime)
                > s_lyric_info.lyric_index.timeindex[2].timestick)
        {
            if (s_lyric_info.lyric_index.lyric_cur
                    < s_lyric_info.lyric_index.lyric_total)
            {
                s_lyric_info.lyric_index.lyric_cur++;
                _read_from_sd(&s_lyric_info.lyric_index.timeindex,
                        ( s_lyric_info.lyric_index.lyric_time_addr
                                + (s_lyric_info.lyric_index.lyric_cur
                                        * 8) )- 16, 24);
                if (s_lyric_info.lyric_index.lyric_cur == 1)
                {
                    s_lyric_info.lyric_index.timeindex[1].timestick
                            = -1;
                }
            }
            if (s_lyric_info.lyric_index.lyric_cur
                    >= s_lyric_info.lyric_index.lyric_total)
            {
                s_lyric_info.lyric_index.timeindex[2].timestick
                        = 0x7fffffff;
                return true;
            }
            if (*((int*) dectime)
                    > s_lyric_info.lyric_index.timeindex[2].timestick)
            {
                continue;
            }
            return true;
        }
        else if (*((int*) dectime)
                < s_lyric_info.lyric_index.timeindex[1].timestick)
        {
            if (s_lyric_info.lyric_index.lyric_cur > 0)
            {
                s_lyric_info.lyric_index.lyric_cur--;
                _read_from_sd(&s_lyric_info.lyric_index.timeindex,
                        (s_lyric_info.lyric_index.lyric_time_addr
                                + (s_lyric_info.lyric_index.lyric_cur
                                        * 8)) - 16, 24);
            }
            if (s_lyric_info.lyric_index.lyric_cur <= 0)
            {
                s_lyric_info.lyric_index.timeindex[1].timestick = -1;
                return true;
            }
            if (*((int*) dectime)
                    < s_lyric_info.lyric_index.timeindex[1].timestick)
            {
                continue;
            }
            return true;
        }
        else if (s_lyric_info.lyric_index.lyric_cur == -1)
        {
            s_lyric_info.lyric_index.lyric_cur = 0;
            _read_from_sd(&s_lyric_info.lyric_index.timeindex[2],
                    s_lyric_info.lyric_index.lyric_time_addr,
                    sizeof(timeindex_t));
            return true;
        }        
        else
        {
        }
        break;
    }
    return false;
}

