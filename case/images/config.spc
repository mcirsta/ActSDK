tab=usb;
DISP_EN="usb property";
DISP_CH="usb属性";

tab=theme;
DISP_EN="theme";
DISP_CH="界面风格";

tab=app;
DISP_EN="app config";
DISP_CH="应用配置";

tab=file;
DISP_EN="file support";
DISP_CH="文件支持";

tab=launcher_app;
DISP_EN="Launcher app config";
DISP_CH="Launcher 应用配置";

tab=music;
DISP_EN="music property";
DISP_CH="音乐属性";

tab=tvin;
DISP_EN="tvin property";
DISP_CH="视频录制属性";

tab=camera;
DISP_EN="camera property";
DISP_CH="照相机属性";

tab=radio;
DISP_EN="radio property";
DISP_CH="收音机属性";

tab=photo;
DISP_EN="photo property";
DISP_CH="图片属性";

key=VER;
type=STR;
operate=EDIT;
len=32;
tabname=app;
DISP_EN="firmware verison";
DISP_CH="固件版本";

key=UHOST_SUPPORT;
type=STR;
operate=SINGLE;
len=256;
RANGE="on","off";
tabname=app;
DISP_EN="uhost support: on,off";
DISP_CH="是否支持uhost功能: on,off";

key=DATE;
type=STR;
operate=EDIT;
len=32;
tabname=app;
DISP_EN="initial product date: for example,2008-01-01";
DISP_CH="初始日期: 格式：年年年年-月月-日日，如2008-01-01";

key=TIME;
type=STR;
operate=EDIT;
len=32;
tabname=app;
DISP_EN="initial product time: for example,08:56";
DISP_CH="初始时间: 格式：时时:分分"，如08:56


key=TIME_FORMAT;
type=DATA;
operate=SINGLE;
len=2;
RANGE="24","12";
tabname=theme;
DISP_EN="time format";
DISP_CH="时间制式";

key=BACKLIGHT_BRIGHTNESS;
type=DATA;
operate=SINGLE;
len=2;
RANGE=1-5;
tabname=theme;
DISP_EN="backlight brightness level";
DISP_CH="背光亮度等级";

key=BACKLIGHT_DURATION;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,5,10,20,30,60,300;
tabname=theme;
DISP_EN="backlight duration time";
DISP_CH="自动背光时间";

key=IDLE_TIME;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,10,30,60,180,300,600;
tabname=theme;
DISP_EN="time before poweroff";
DISP_CH="省电关机时间";

key=CUR_THEME;
type=STR;
operate=SINGLE;
len=256;
RANGE="qvga_style_1_sidelong";
tabname=theme;
DISP_EN="current theme type";
DISP_CH="当前风格类型";

key=LANG_CODES;
type=STR;
operate=SINGLE;
len=16;
RANGE="en_GB","zh_CN";
tabname=theme;
DISP_EN="language codes";
DISP_CH="语言codes";

key=VOICE_VOLUM;
type=DATA;
operate=EDIT;
len=2;
RANGE=0-31;
tabname=theme;
DISP_EN="voice volume: 0~31";
DISP_CH="音量: 0~31";

key=SCREEN_MODE;
type=STR;
operate=SINGLE;
len=16;
RANGE="Horizontal";
tabname=theme;
DISP_EN="lcd mode";
DISP_CH="屏幕模式";

key=CONNECT_TYPE;
type=STR;
operate=SINGLE;
len=16;
RANGE="UDISK";
tabname=theme;
DISP_EN="USB connect mode";
DISP_CH="USB连接类型";

key=DEFAULT_FONT;
type=STR;
operate=SINGLE;
len=32;
RANGE="FWQYS";
tabname=theme;
DISP_EN="system default font name";
DISP_CH="系统默认字体文件";

key=CUR_THEME;
type=STR;
operate=SINGLE;
len=256;
RANGE="qvga_style_1_sidelong";
tabname=theme;
DISP_EN="current theme type";
DISP_CH="当前风格类型";

key=THEMES;
type=STR;
operate=COMPLEX;
separator=","
len=256;
tabname=theme;
RANGE="qvga_style_1_sidelong";
DISP_EN="system total themes"
DISP_CH="系统所有的风格"

key=KEY_TONE_ON;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0-1;
tabname=theme;
DISP_EN="use the key tone or not";
DISP_CH="是否打开按键音";

key=KEY_TONE_CUR;
type=STR;
operate=COMPLEX;
separator=","
len=16;
tabname=theme;
RANGE="KEY_TONE_1";
DISP_EN="keytone support"
DISP_CH="按键音支持"

key=USERNAME;
type=STR;
operate=EDIT;
len=64;
tabname=theme;
DISP_EN="user name";
DISP_CH="用户名";


key=SEARCH_FOLDER;
type=STR;
operate=SINGLE;
len=256;
RANGE="on","off";
tabname=theme;
DISP_EN="wether or not search the sub directories";
DISP_CH="是否搜索子目录";

key=GSENSOR_SHAKE_SUPPORT;
type=STR;
operate=SINGLE;
len=256;
RANGE="on","off";
tabname=theme;
DISP_EN="wether or not support g_sensor drag";
DISP_CH="是否支持g_sensor甩动功能";

key=WATCHDOG_TIME;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,15000,30000,60000;
tabname=theme;
DISP_EN="watchdog timerout time: (s)";
DISP_CH="看门狗超时时间: (单位:秒)";

key=SCREEN_X;
type=STR;
operate=SINGLE;
len=256;
tabname=theme;
RANGE="-g_sensor_x","+g_sensor_x","-g_sensor_y","+g_sensor_y","-g_sensor_z","+g_sensor_z";
DISP_EN="X axis"
DISP_CH="X轴坐标关系"

key=SCREEN_Y;
type=STR;
operate=SINGLE;
len=256;
tabname=theme;
RANGE="-g_sensor_x","+g_sensor_x","-g_sensor_y","+g_sensor_y","-g_sensor_z","+g_sensor_z";
DISP_EN="Y axis"
DISP_CH="Y轴坐标关系"

key=SCREEN_Z;
type=STR;
operate=SINGLE;
len=256;
tabname=theme;
RANGE="-g_sensor_x","+g_sensor_x","-g_sensor_y","+g_sensor_y","-g_sensor_z","+g_sensor_z";
DISP_EN="Z axis"
DISP_CH="Z轴坐标关系"

key=poweron_enable;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,1;
tabname=theme;
DISP_EN="where or not show thepoweron amination";
DISP_CH="开始是否显示开机动画";

key=poweroff_enable;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,1;
tabname=theme;
DISP_EN="where or not show thepoweroff amination";
DISP_CH="开始是否显示关机动画";

key=app_support_ext_music;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="mp3","wma","asf","ogg","ape","flac","wav","mpc","aif","aiff","amr","aac","m4a","ra","ram";
tabname=file;
DISP_EN="music types which system support"
DISP_CH="系统支持的音乐类型"

key=app_support_ext_video;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="avi","flv","wmv","rmvb","rm","3gp","mp4","mov","mkv";
tabname=file;
DISP_EN="video types which system support"
DISP_CH="系统支持的视频类型"

key=app_support_ext_photo;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="jpg","jpeg","jpe","bmp","gif","png","tiff","tif";
tabname=file;
DISP_EN="picture types which system support"
DISP_CH="系统支持的图片类型"

key=app_support_ext_ebook;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="txt","jar";
tabname=file;
DISP_EN="ebook types which system support"
DISP_CH="系统支持的文本类型"

key=app_support_ext_emulator;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="nes","gb","gbc","smc","smd","bin","mdx","gba";
tabname=file;
DISP_EN="emulator roms types which system support"
DISP_CH="系统支持的游戏模拟器ROM文件类型"

key=app_support_ext_all;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="mp3","wma","asf","ogg","ape","flac","wav","mpc","aif","aiff","amr","aac","m4a","ra","ram","avi","flv","wmv","rmvb","rm","3gp","mp4","mov","mkv","jpg","jpeg","jpe","bmp","gif","png","tiff","tif","txt","jar","nes","gb","gbc","smc","smd","bin","mdx","gba";
tabname=file;
DISP_EN="file types which system support"
DISP_CH="系统支持的多媒体文件类型"

key=photo_previewmode;
type=DATA;
operate=SINGLE;
len=4;
RANGE=0,1;
tabname=photo;
DISP_EN="photo preview mode: 0-list,1-thumbnail";
DISP_CH="默认图片预览模式: 0-列表,1-缩略图";

key=photo_slide_effect_mode;
type=DATA;
operate=SINGLE;
len=4;
RANGE=0-10;
tabname=photo;
DISP_EN="photo slide effect: 0-Random,1-Fly In,2-Chessboard,3-Cut,4-Erase,5-Louver,6-Random Line,7-Water Wave,8-Cube,9-3D Rotation,10-OFF";
DISP_CH="图片播放特效: 0-随机,1-飞入,2-棋盘,3-切割,4-擦除,5-百叶窗,6-随机线,7-水波纹,8-立方体,9-3D旋转,10-无特效";


key=photo_slide_interval;
type=DATA;
operate=SINGLE;
len=4;
RANGE=1-10;
tabname=photo;
DISP_EN="photo slide interval: 1~10s";
DISP_CH="图片全屏自动播放时间间隔: 1~10秒";

key=Applications;
type=STR;
operate=COMPLEX;
separator=";"
len=256;
RANGE="music","video","tvin","photo","camera","recorder","radio","ebook","games","tools","browser","setting";
tabname=launcher_app;
DISP_EN="system total applications"
DISP_CH="系统所有的应用"

key=music_play_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-5;
tabname=music;
DISP_EN="Loop mode: 0-Sequence, 1-Repeat One, 2-Repeat All, 3-Shuffle, 4-Shuffle + Repeat, 5-Intro";
DISP_CH="循环模式: 0-Sequence, 1-Repeat One, 2-Repeat All, 3-Shuffle, 4-Shuffle + Repeat, 5-Intro";

key=music_display_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-4;
tabname=music;
DISP_EN="display mode: 0-Spectrum, 1-Lyric, 2-Effect1, 3-Effect2, 4-Effect3";
DISP_CH="显示模式: 0-Spectrum, 1-Lyric, 2-Effect1, 3-Effect2, 4-Effect3";

key=music_display_effect;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="display effect: 0-Disable, 1-Enable";
DISP_CH="显示特效: 0-Disable, 1-Enable";

key=music_eq_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-10;
tabname=music;
DISP_EN="Eq mode: 0-Norma, 1-Pop, 2-Classic, 3-Soft, 4-Jazz, 5-Rock, 6-DBB, 7-Custom EQ, 8-SRS WOW, 9-SRS WOW HD, 10-SRS USER Mode";
DISP_CH="Eq类型: 0-Norma, 1-Pop, 2-Classic, 3-Soft, 4-Jazz, 5-Rock, 6-DBB, 7-Custom EQ, 8-SRS WOW, 9-SRS WOW HD, 10-SRS USER Mode";

key=music_fade;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="Fade in Fade out: 0-disable, 1-enable";
DISP_CH="淡入淡出: 0-disable, 1-enable";

key=music_ffb_speed;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-4;
tabname=music;
DISP_EN="ffb speed: 0-2X, 1-4X, 2-8X, 3-16X, 4-32X";
DISP_CH="快进速度: 0-2X, 1-4X, 2-8X, 3-16X, 4-32X";

key=music_ffb_time;
type=DATA;
operate=EDIT;
len=2;
RANGE=0-500;
tabname=music;
DISP_EN="ffb play time: 0~500ms";
DISP_CH="快进过程播放音乐时间: 0~500ms";

key=music_ab_count;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1-10;
tabname=music;
DISP_EN="AB count: 1~10";
DISP_CH="AB循环的次数: 1~10";

key=music_ab_time;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1-7;
tabname=music;
DISP_EN="AB gap: 1~7 s";
DISP_CH="AB循环的间隔: 1～7秒";

key=music_nowplaying_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="nowplaying item visible: 0-no, 1-yes";
DISP_CH="正在播放项是否可见: 0-否, 1-是";

key=music_allmusic_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="all music item visible: 0-no, 1-yes";
DISP_CH="所有曲目项是否可见: 0-否, 1-是";

key=music_artist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="artist item visible: 0-no, 1-yes";
DISP_CH="歌手列表项是否可见: 0-否, 1-是";

key=music_album_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="album item visible: 0-no, 1-yes";
DISP_CH="专辑列表项是否可见: 0-否, 1-是";

key=music_genre_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="genre item visible: 0-no, 1-yes";
DISP_CH="流派列表项是否可见: 0-否, 1-是";

key=music_myplaylist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="myplaylist item visible: 0-no, 1-yes";
DISP_CH="我的播放列表项是否可见: 0-否, 1-是";

key=music_dirlist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="dirlist item visible: 0-no, 1-yes";
DISP_CH="目录列表项是否可见: 0-否, 1-是";

key=music_reclist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="record list item visible: 0-no, 1-yes";
DISP_CH="录音列表项是否可见: 0-否, 1-是";

key=music_myrating_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="myrating item visible: 0-no, 1-yes";
DISP_CH="星级列表项是否可见: 0-否, 1-是";

key=music_taglist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="tag list item visible: 0-no, 1-yes";
DISP_CH="标签列表项是否可见: 0-否, 1-是";

key=udisk_PID;
type=DATA;
operate=EDIT;
len=2;
RANGE=0x0000-0xffff;
tabname=usb;
DISP_EN="Actions MSC PID: 0x0000-0xffff";
DISP_CH="usb设备的产品编号PID: 0x0000-0xffff";

key=UdiskVolume;
type=STR;
operate=EDIT;
len=11;
tabname=usb;
DISP_EN="Udisk Volume name";
DISP_CH="U盘卷标名";

key=udisk_VID;
type=DATA;
operate=EDIT;
len=2;
RANGE=0x0000-0xffff;
tabname=usb;
DISP_EN="Actions USB VID: 0x0000-0xffff";
DISP_CH="usb设备的生产厂商编号VID: 0x0000-0xffff";

key=udisk_release;
type=DATA;
operate=EDIT;
len=2;
RANGE=0x0000-0xffff;
tabname=usb;
DISP_EN="Actions MSC device release number: 0x0000-0xffff";
DISP_CH="usb设备的产品批次号码: 0x0000-0xffff";

key=udisk_vendor;
type=STR;
operate=EDIT;
len=8;
tabname=usb;
DISP_EN="The name of the verdor";
DISP_CH="usb设备的生产厂商名称";

key=udisk_product;
type=STR;
operate=EDIT;
len=16;
tabname=usb;
DISP_EN="The product name of the usb device";
DISP_CH="usb设备的产品名称";

key=udisk_serial;
type=STR;
operate=EDIT;
len=33;
tabname=usb;
DISP_EN="The serial number";
DISP_CH="序列号";

key=mtp_PID;
type=DATA;
operate=EDIT;
len=4;
RANGE=0x00000000-0xffffffff;
tabname=usb;
DISP_EN="mtp device product id: 0x00000000-0xffffffff";
DISP_CH="mtp设备的产品编号: 0x00000000-0xffffffff";

key=mtp_VID;
type=DATA;
operate=EDIT;
len=4;
RANGE=0x00000000-0xffffffff;
tabname=usb;
DISP_EN="mtp device vendor id: 0x00000000-0xffffffff";
DISP_CH="mtp设备的厂商编号: 0x00000000-0xffffffff";

key=mtp_EXT;
type=DATA;
operate=EDIT;
len=4;
RANGE=0x00000000-0xffffffff;
tabname=usb;
DISP_EN="mtp device extend id: 0x00000000-0xffffffff";
DISP_CH="mtp设备的扩展编号: 0x00000000-0xffffffff";

key=mtp_dev_manufacturer;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device manufacturer description";
DISP_CH="厂商描述";

key=mtp_dev_model;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device mode";
DISP_CH="MTP设备模式";

key=mtp_dev_version;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device version";
DISP_CH="MTP设备版本";

key=mtp_dev_serial;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device serial number";
DISP_CH="MTP设备序列号";

key=mtp_store_inner_desc;
type=STR;
operate=EDIT;
len=40;
tabname=usb;
DISP_EN="mtp device's internal storage disk description";
DISP_CH="MTP设备内部存储盘描述";

key=mtp_store_portable_desc;
type=STR;
operate=EDIT;
len=40;
tabname=usb;
DISP_EN="mtp device's portable storage disk description: (usually card)";
DISP_CH="MTP设备移动存储盘描述: （通常是卡）";

key=mtp_dev_ext_desc0;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device's extend description";
DISP_CH="MTP设备的扩展描述";

key=mtp_dev_ext_desc1;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device's extend description";
DISP_CH="MTP设备的扩展描述";

key=usb_show_u_disk;
type=DATA;
operate=single;
len=1;
RANGE=0-1;
tabname=usb;
DISP_EN="show the u disk or not";
DISP_CH="是否弹出u盘分区";

key=usb_show_card_disk;
type=DATA;
operate=single;
len=1;
RANGE=0-1;
tabname=usb;
DISP_EN="show the card disk or not";
DISP_CH="是否弹出card盘分区";

key=usb_upgrade_fw_file;
type=STR;
operate=EDIT;
len=1024;
tabname=usb;
DISP_EN="The path of the upgrade file";
DISP_CH="升级固件文件的放置路径";

//tvin_signal = "PAL";  
key=tvin_signal;
type=STR;
operate=SINGLE;
len=5;
RANGE="PAL","NTSC";
tabname=tvin;
DISP_EN="default tvin signal input";
DISP_CH="TVIN视频信号输入默认值";
       
//tvin_savepath = 0;    
key=tvin_savepath;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1;
tabname=tvin;
DISP_EN="tvin save path: 1-card memory";
DISP_CH="视频保存路径: 1-card memory";    
    
//tvin_record_quality = 0;  
key=tvin_record_quality;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=tvin;
DISP_EN="tvin record quality: 0-low, 1-medium, 2-high, 3-auto";
DISP_CH="视频录制质量: 0-low, 1-medium, 2-high, 3-auto";

//tvin_lcd_onoff = "LCD_ON";     
key=tvin_lcd_onoff;
type=STR;
operate=SINGLE;
len=8;
RANGE="LCD_ON","LCD_OFF";
tabname=tvin;
DISP_EN="lcd on or off when recording";
DISP_CH="录像时是否关掉屏幕";
  
key=dsc_whitebalance;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-4;
tabname=camera;
DISP_EN="whitebalance: 0-Auto,1-Fine,2-loudy,3-Filament Lamp,4-Fluorescent Lamp";
DISP_CH="白平衡: 0-自动 1-晴天 2-阴天 3-白炽灯 4-荧光灯";

key=dsc_brightness;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-6;
tabname=camera;
DISP_EN="dsc brightness: 0(-3),1(-2),2(-1),3(0),4(1),5(2),6(3)";
DISP_CH="明亮度: 0(-3),1(-2),2(-1),3(0),4(1),5(2),6(3)";

key=dsc_exposure_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-2;
tabname=camera;
DISP_EN="dsc exposure mode: 0-Auto,1-Indoor,2-Outdoor";
DISP_CH="曝光模式:0-自动 1-室内 2-室外";

key=dsc_face_identify;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=camera;
DISP_EN="dsc face identify: 0-OFF,1-ON";
DISP_CH="人脸检测: 0-关闭 1-打开";

key=dsc_specialeffect;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=camera;
DISP_EN="dsc_specialeffect: 0-OFF,1-Black White,2-Sepia,3-Negative";
DISP_CH="特效: 0-无 1-黑白 2-复古 3-负片";

key=dv_savepath;
type=DATA;
operate=SINGLE;
len=1;
RANGE=2;
tabname=camera;
DISP_EN="Save Path: 2-Card Memory";
DISP_CH="保存路径: 2-卡";

key=dsc_shutter_sound;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=camera;
DISP_EN="Shutter Sound: 0-OFF,1-Sound1,2-Sound2,3-Sound3";
DISP_CH="快门声音: 0-关闭 1-音效1 2-音效2 3-音效3";

key=dsc_dater;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-2;
tabname=camera;
DISP_EN="dsc_dater: 0-OFF,1-Date,2-Date Time";
DISP_CH="照片日期标记: 0-关闭,1-日期,2-日期时间";


key=dv_whitebalance;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-4;
tabname=camera;
DISP_EN="dv whitebalance flag: 0-Auto,1-Fine,2-loudy,3-Filament Lamp,4-Fluorescent Lamp";
DISP_CH="视频白平衡: 0-自动,1-晴天,2-阴天,3-白炽灯,4-荧光灯";

key=dv_brightness;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-6;
tabname=camera;
DISP_EN="dv brightness: 0(-3),1(-2),2(-1),3(0),4(1),5(2),6(3)";
DISP_CH="视频明亮度: 0(-3),1(-2),2(-1),3(0),4(1),5(2),6(3)";

key=dv_exposure_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-2;
tabname=camera;
DISP_EN="dv exposure mode: 0-Auto,1-Indoor,2-Outdoor";
DISP_CH="视频曝光模式: 0-自动,1-室内,2-室外";

key=dv_specialeffect;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=camera;
DISP_EN="dv_specialeffect: 0-OFF,1-Black & White,2-Sepia,3-Negative";
DISP_CH="视频特效: 0-无,1-黑白,2-复古,3-负片";

key=fm_band;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=radio;
DISP_EN="Tuner Region: 0-China,1-USA,2-Japan,3-Europe"
DISP_CH="电台频段: 0-中国,1-美国,2-日本,3-欧洲"

key=fm_intensity;
type=DATA;
operate=SINGLE;
len=1;
RANGE=5-10;
tabname=radio;
DISP_EN="fm intensity: 5~10"
DISP_CH="FM灵敏度: 5~10"

key=fm_record_quality;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-2;
tabname=radio;
DISP_EN="record quality: 0-low,1-middle,2-high"
DISP_CH="录音质量: 0-低,1-中,2-高"

key=fm_record_gain;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1-5;
tabname=radio;
DISP_EN="record gain: 1~5"
DISP_CH="录音增益: 1~5"

key=fm_record_path;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1;
tabname=radio;
DISP_EN="save path: 1-card"
DISP_CH="录音文件保存路径: 1-卡"
