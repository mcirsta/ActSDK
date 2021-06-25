tab=usb;
DISP_EN="usb property";
DISP_CH="usb����";

tab=theme;
DISP_EN="theme";
DISP_CH="������";

tab=app;
DISP_EN="app config";
DISP_CH="Ӧ������";

tab=file;
DISP_EN="file support";
DISP_CH="�ļ�֧��";

tab=launcher_app;
DISP_EN="Launcher app config";
DISP_CH="Launcher Ӧ������";

tab=music;
DISP_EN="music property";
DISP_CH="��������";

tab=tvin;
DISP_EN="tvin property";
DISP_CH="��Ƶ¼������";

tab=camera;
DISP_EN="camera property";
DISP_CH="���������";

tab=radio;
DISP_EN="radio property";
DISP_CH="����������";

tab=photo;
DISP_EN="photo property";
DISP_CH="ͼƬ����";

key=VER;
type=STR;
operate=EDIT;
len=32;
tabname=app;
DISP_EN="firmware verison";
DISP_CH="�̼��汾";

key=UHOST_SUPPORT;
type=STR;
operate=SINGLE;
len=256;
RANGE="on","off";
tabname=app;
DISP_EN="uhost support: on,off";
DISP_CH="�Ƿ�֧��uhost����: on,off";

key=DATE;
type=STR;
operate=EDIT;
len=32;
tabname=app;
DISP_EN="initial product date: for example,2008-01-01";
DISP_CH="��ʼ����: ��ʽ����������-����-���գ���2008-01-01";

key=TIME;
type=STR;
operate=EDIT;
len=32;
tabname=app;
DISP_EN="initial product time: for example,08:56";
DISP_CH="��ʼʱ��: ��ʽ��ʱʱ:�ַ�"����08:56


key=TIME_FORMAT;
type=DATA;
operate=SINGLE;
len=2;
RANGE="24","12";
tabname=theme;
DISP_EN="time format";
DISP_CH="ʱ����ʽ";

key=BACKLIGHT_BRIGHTNESS;
type=DATA;
operate=SINGLE;
len=2;
RANGE=1-5;
tabname=theme;
DISP_EN="backlight brightness level";
DISP_CH="�������ȵȼ�";

key=BACKLIGHT_DURATION;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,5,10,20,30,60,300;
tabname=theme;
DISP_EN="backlight duration time";
DISP_CH="�Զ�����ʱ��";

key=IDLE_TIME;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,10,30,60,180,300,600;
tabname=theme;
DISP_EN="time before poweroff";
DISP_CH="ʡ��ػ�ʱ��";

key=CUR_THEME;
type=STR;
operate=SINGLE;
len=256;
RANGE="qvga_style_1_sidelong";
tabname=theme;
DISP_EN="current theme type";
DISP_CH="��ǰ�������";

key=LANG_CODES;
type=STR;
operate=SINGLE;
len=16;
RANGE="en_GB","zh_CN";
tabname=theme;
DISP_EN="language codes";
DISP_CH="����codes";

key=VOICE_VOLUM;
type=DATA;
operate=EDIT;
len=2;
RANGE=0-31;
tabname=theme;
DISP_EN="voice volume: 0~31";
DISP_CH="����: 0~31";

key=SCREEN_MODE;
type=STR;
operate=SINGLE;
len=16;
RANGE="Horizontal";
tabname=theme;
DISP_EN="lcd mode";
DISP_CH="��Ļģʽ";

key=CONNECT_TYPE;
type=STR;
operate=SINGLE;
len=16;
RANGE="UDISK";
tabname=theme;
DISP_EN="USB connect mode";
DISP_CH="USB��������";

key=DEFAULT_FONT;
type=STR;
operate=SINGLE;
len=32;
RANGE="FWQYS";
tabname=theme;
DISP_EN="system default font name";
DISP_CH="ϵͳĬ�������ļ�";

key=CUR_THEME;
type=STR;
operate=SINGLE;
len=256;
RANGE="qvga_style_1_sidelong";
tabname=theme;
DISP_EN="current theme type";
DISP_CH="��ǰ�������";

key=THEMES;
type=STR;
operate=COMPLEX;
separator=","
len=256;
tabname=theme;
RANGE="qvga_style_1_sidelong";
DISP_EN="system total themes"
DISP_CH="ϵͳ���еķ��"

key=KEY_TONE_ON;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0-1;
tabname=theme;
DISP_EN="use the key tone or not";
DISP_CH="�Ƿ�򿪰�����";

key=KEY_TONE_CUR;
type=STR;
operate=COMPLEX;
separator=","
len=16;
tabname=theme;
RANGE="KEY_TONE_1";
DISP_EN="keytone support"
DISP_CH="������֧��"

key=USERNAME;
type=STR;
operate=EDIT;
len=64;
tabname=theme;
DISP_EN="user name";
DISP_CH="�û���";


key=SEARCH_FOLDER;
type=STR;
operate=SINGLE;
len=256;
RANGE="on","off";
tabname=theme;
DISP_EN="wether or not search the sub directories";
DISP_CH="�Ƿ�������Ŀ¼";

key=GSENSOR_SHAKE_SUPPORT;
type=STR;
operate=SINGLE;
len=256;
RANGE="on","off";
tabname=theme;
DISP_EN="wether or not support g_sensor drag";
DISP_CH="�Ƿ�֧��g_sensor˦������";

key=WATCHDOG_TIME;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,15000,30000,60000;
tabname=theme;
DISP_EN="watchdog timerout time: (s)";
DISP_CH="���Ź���ʱʱ��: (��λ:��)";

key=SCREEN_X;
type=STR;
operate=SINGLE;
len=256;
tabname=theme;
RANGE="-g_sensor_x","+g_sensor_x","-g_sensor_y","+g_sensor_y","-g_sensor_z","+g_sensor_z";
DISP_EN="X axis"
DISP_CH="X�������ϵ"

key=SCREEN_Y;
type=STR;
operate=SINGLE;
len=256;
tabname=theme;
RANGE="-g_sensor_x","+g_sensor_x","-g_sensor_y","+g_sensor_y","-g_sensor_z","+g_sensor_z";
DISP_EN="Y axis"
DISP_CH="Y�������ϵ"

key=SCREEN_Z;
type=STR;
operate=SINGLE;
len=256;
tabname=theme;
RANGE="-g_sensor_x","+g_sensor_x","-g_sensor_y","+g_sensor_y","-g_sensor_z","+g_sensor_z";
DISP_EN="Z axis"
DISP_CH="Z�������ϵ"

key=poweron_enable;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,1;
tabname=theme;
DISP_EN="where or not show thepoweron amination";
DISP_CH="��ʼ�Ƿ���ʾ��������";

key=poweroff_enable;
type=DATA;
operate=SINGLE;
len=2;
RANGE=0,1;
tabname=theme;
DISP_EN="where or not show thepoweroff amination";
DISP_CH="��ʼ�Ƿ���ʾ�ػ�����";

key=app_support_ext_music;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="mp3","wma","asf","ogg","ape","flac","wav","mpc","aif","aiff","amr","aac","m4a","ra","ram";
tabname=file;
DISP_EN="music types which system support"
DISP_CH="ϵͳ֧�ֵ���������"

key=app_support_ext_video;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="avi","flv","wmv","rmvb","rm","3gp","mp4","mov","mkv";
tabname=file;
DISP_EN="video types which system support"
DISP_CH="ϵͳ֧�ֵ���Ƶ����"

key=app_support_ext_photo;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="jpg","jpeg","jpe","bmp","gif","png","tiff","tif";
tabname=file;
DISP_EN="picture types which system support"
DISP_CH="ϵͳ֧�ֵ�ͼƬ����"

key=app_support_ext_ebook;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="txt","jar";
tabname=file;
DISP_EN="ebook types which system support"
DISP_CH="ϵͳ֧�ֵ��ı�����"

key=app_support_ext_emulator;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="nes","gb","gbc","smc","smd","bin","mdx","gba";
tabname=file;
DISP_EN="emulator roms types which system support"
DISP_CH="ϵͳ֧�ֵ���Ϸģ����ROM�ļ�����"

key=app_support_ext_all;
type=STR;
operate=COMPLEX;
separator="|"
len=256;
RANGE="mp3","wma","asf","ogg","ape","flac","wav","mpc","aif","aiff","amr","aac","m4a","ra","ram","avi","flv","wmv","rmvb","rm","3gp","mp4","mov","mkv","jpg","jpeg","jpe","bmp","gif","png","tiff","tif","txt","jar","nes","gb","gbc","smc","smd","bin","mdx","gba";
tabname=file;
DISP_EN="file types which system support"
DISP_CH="ϵͳ֧�ֵĶ�ý���ļ�����"

key=photo_previewmode;
type=DATA;
operate=SINGLE;
len=4;
RANGE=0,1;
tabname=photo;
DISP_EN="photo preview mode: 0-list,1-thumbnail";
DISP_CH="Ĭ��ͼƬԤ��ģʽ: 0-�б�,1-����ͼ";

key=photo_slide_effect_mode;
type=DATA;
operate=SINGLE;
len=4;
RANGE=0-10;
tabname=photo;
DISP_EN="photo slide effect: 0-Random,1-Fly In,2-Chessboard,3-Cut,4-Erase,5-Louver,6-Random Line,7-Water Wave,8-Cube,9-3D Rotation,10-OFF";
DISP_CH="ͼƬ������Ч: 0-���,1-����,2-����,3-�и�,4-����,5-��Ҷ��,6-�����,7-ˮ����,8-������,9-3D��ת,10-����Ч";


key=photo_slide_interval;
type=DATA;
operate=SINGLE;
len=4;
RANGE=1-10;
tabname=photo;
DISP_EN="photo slide interval: 1~10s";
DISP_CH="ͼƬȫ���Զ�����ʱ����: 1~10��";

key=Applications;
type=STR;
operate=COMPLEX;
separator=";"
len=256;
RANGE="music","video","tvin","photo","camera","recorder","radio","ebook","games","tools","browser","setting";
tabname=launcher_app;
DISP_EN="system total applications"
DISP_CH="ϵͳ���е�Ӧ��"

key=music_play_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-5;
tabname=music;
DISP_EN="Loop mode: 0-Sequence, 1-Repeat One, 2-Repeat All, 3-Shuffle, 4-Shuffle + Repeat, 5-Intro";
DISP_CH="ѭ��ģʽ: 0-Sequence, 1-Repeat One, 2-Repeat All, 3-Shuffle, 4-Shuffle + Repeat, 5-Intro";

key=music_display_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-4;
tabname=music;
DISP_EN="display mode: 0-Spectrum, 1-Lyric, 2-Effect1, 3-Effect2, 4-Effect3";
DISP_CH="��ʾģʽ: 0-Spectrum, 1-Lyric, 2-Effect1, 3-Effect2, 4-Effect3";

key=music_display_effect;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="display effect: 0-Disable, 1-Enable";
DISP_CH="��ʾ��Ч: 0-Disable, 1-Enable";

key=music_eq_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-10;
tabname=music;
DISP_EN="Eq mode: 0-Norma, 1-Pop, 2-Classic, 3-Soft, 4-Jazz, 5-Rock, 6-DBB, 7-Custom EQ, 8-SRS WOW, 9-SRS WOW HD, 10-SRS USER Mode";
DISP_CH="Eq����: 0-Norma, 1-Pop, 2-Classic, 3-Soft, 4-Jazz, 5-Rock, 6-DBB, 7-Custom EQ, 8-SRS WOW, 9-SRS WOW HD, 10-SRS USER Mode";

key=music_fade;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="Fade in Fade out: 0-disable, 1-enable";
DISP_CH="���뵭��: 0-disable, 1-enable";

key=music_ffb_speed;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-4;
tabname=music;
DISP_EN="ffb speed: 0-2X, 1-4X, 2-8X, 3-16X, 4-32X";
DISP_CH="����ٶ�: 0-2X, 1-4X, 2-8X, 3-16X, 4-32X";

key=music_ffb_time;
type=DATA;
operate=EDIT;
len=2;
RANGE=0-500;
tabname=music;
DISP_EN="ffb play time: 0~500ms";
DISP_CH="������̲�������ʱ��: 0~500ms";

key=music_ab_count;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1-10;
tabname=music;
DISP_EN="AB count: 1~10";
DISP_CH="ABѭ���Ĵ���: 1~10";

key=music_ab_time;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1-7;
tabname=music;
DISP_EN="AB gap: 1~7 s";
DISP_CH="ABѭ���ļ��: 1��7��";

key=music_nowplaying_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="nowplaying item visible: 0-no, 1-yes";
DISP_CH="���ڲ������Ƿ�ɼ�: 0-��, 1-��";

key=music_allmusic_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="all music item visible: 0-no, 1-yes";
DISP_CH="������Ŀ���Ƿ�ɼ�: 0-��, 1-��";

key=music_artist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="artist item visible: 0-no, 1-yes";
DISP_CH="�����б����Ƿ�ɼ�: 0-��, 1-��";

key=music_album_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="album item visible: 0-no, 1-yes";
DISP_CH="ר���б����Ƿ�ɼ�: 0-��, 1-��";

key=music_genre_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="genre item visible: 0-no, 1-yes";
DISP_CH="�����б����Ƿ�ɼ�: 0-��, 1-��";

key=music_myplaylist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="myplaylist item visible: 0-no, 1-yes";
DISP_CH="�ҵĲ����б����Ƿ�ɼ�: 0-��, 1-��";

key=music_dirlist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="dirlist item visible: 0-no, 1-yes";
DISP_CH="Ŀ¼�б����Ƿ�ɼ�: 0-��, 1-��";

key=music_reclist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="record list item visible: 0-no, 1-yes";
DISP_CH="¼���б����Ƿ�ɼ�: 0-��, 1-��";

key=music_myrating_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="myrating item visible: 0-no, 1-yes";
DISP_CH="�Ǽ��б����Ƿ�ɼ�: 0-��, 1-��";

key=music_taglist_visible;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=music;
DISP_EN="tag list item visible: 0-no, 1-yes";
DISP_CH="��ǩ�б����Ƿ�ɼ�: 0-��, 1-��";

key=udisk_PID;
type=DATA;
operate=EDIT;
len=2;
RANGE=0x0000-0xffff;
tabname=usb;
DISP_EN="Actions MSC PID: 0x0000-0xffff";
DISP_CH="usb�豸�Ĳ�Ʒ���PID: 0x0000-0xffff";

key=UdiskVolume;
type=STR;
operate=EDIT;
len=11;
tabname=usb;
DISP_EN="Udisk Volume name";
DISP_CH="U�̾����";

key=udisk_VID;
type=DATA;
operate=EDIT;
len=2;
RANGE=0x0000-0xffff;
tabname=usb;
DISP_EN="Actions USB VID: 0x0000-0xffff";
DISP_CH="usb�豸���������̱��VID: 0x0000-0xffff";

key=udisk_release;
type=DATA;
operate=EDIT;
len=2;
RANGE=0x0000-0xffff;
tabname=usb;
DISP_EN="Actions MSC device release number: 0x0000-0xffff";
DISP_CH="usb�豸�Ĳ�Ʒ���κ���: 0x0000-0xffff";

key=udisk_vendor;
type=STR;
operate=EDIT;
len=8;
tabname=usb;
DISP_EN="The name of the verdor";
DISP_CH="usb�豸��������������";

key=udisk_product;
type=STR;
operate=EDIT;
len=16;
tabname=usb;
DISP_EN="The product name of the usb device";
DISP_CH="usb�豸�Ĳ�Ʒ����";

key=udisk_serial;
type=STR;
operate=EDIT;
len=33;
tabname=usb;
DISP_EN="The serial number";
DISP_CH="���к�";

key=mtp_PID;
type=DATA;
operate=EDIT;
len=4;
RANGE=0x00000000-0xffffffff;
tabname=usb;
DISP_EN="mtp device product id: 0x00000000-0xffffffff";
DISP_CH="mtp�豸�Ĳ�Ʒ���: 0x00000000-0xffffffff";

key=mtp_VID;
type=DATA;
operate=EDIT;
len=4;
RANGE=0x00000000-0xffffffff;
tabname=usb;
DISP_EN="mtp device vendor id: 0x00000000-0xffffffff";
DISP_CH="mtp�豸�ĳ��̱��: 0x00000000-0xffffffff";

key=mtp_EXT;
type=DATA;
operate=EDIT;
len=4;
RANGE=0x00000000-0xffffffff;
tabname=usb;
DISP_EN="mtp device extend id: 0x00000000-0xffffffff";
DISP_CH="mtp�豸����չ���: 0x00000000-0xffffffff";

key=mtp_dev_manufacturer;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device manufacturer description";
DISP_CH="��������";

key=mtp_dev_model;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device mode";
DISP_CH="MTP�豸ģʽ";

key=mtp_dev_version;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device version";
DISP_CH="MTP�豸�汾";

key=mtp_dev_serial;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device serial number";
DISP_CH="MTP�豸���к�";

key=mtp_store_inner_desc;
type=STR;
operate=EDIT;
len=40;
tabname=usb;
DISP_EN="mtp device's internal storage disk description";
DISP_CH="MTP�豸�ڲ��洢������";

key=mtp_store_portable_desc;
type=STR;
operate=EDIT;
len=40;
tabname=usb;
DISP_EN="mtp device's portable storage disk description: (usually card)";
DISP_CH="MTP�豸�ƶ��洢������: ��ͨ���ǿ���";

key=mtp_dev_ext_desc0;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device's extend description";
DISP_CH="MTP�豸����չ����";

key=mtp_dev_ext_desc1;
type=STR;
operate=EDIT;
len=60;
tabname=usb;
DISP_EN="mtp device's extend description";
DISP_CH="MTP�豸����չ����";

key=usb_show_u_disk;
type=DATA;
operate=single;
len=1;
RANGE=0-1;
tabname=usb;
DISP_EN="show the u disk or not";
DISP_CH="�Ƿ񵯳�u�̷���";

key=usb_show_card_disk;
type=DATA;
operate=single;
len=1;
RANGE=0-1;
tabname=usb;
DISP_EN="show the card disk or not";
DISP_CH="�Ƿ񵯳�card�̷���";

key=usb_upgrade_fw_file;
type=STR;
operate=EDIT;
len=1024;
tabname=usb;
DISP_EN="The path of the upgrade file";
DISP_CH="�����̼��ļ��ķ���·��";

//tvin_signal = "PAL";  
key=tvin_signal;
type=STR;
operate=SINGLE;
len=5;
RANGE="PAL","NTSC";
tabname=tvin;
DISP_EN="default tvin signal input";
DISP_CH="TVIN��Ƶ�ź�����Ĭ��ֵ";
       
//tvin_savepath = 0;    
key=tvin_savepath;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1;
tabname=tvin;
DISP_EN="tvin save path: 1-card memory";
DISP_CH="��Ƶ����·��: 1-card memory";    
    
//tvin_record_quality = 0;  
key=tvin_record_quality;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=tvin;
DISP_EN="tvin record quality: 0-low, 1-medium, 2-high, 3-auto";
DISP_CH="��Ƶ¼������: 0-low, 1-medium, 2-high, 3-auto";

//tvin_lcd_onoff = "LCD_ON";     
key=tvin_lcd_onoff;
type=STR;
operate=SINGLE;
len=8;
RANGE="LCD_ON","LCD_OFF";
tabname=tvin;
DISP_EN="lcd on or off when recording";
DISP_CH="¼��ʱ�Ƿ�ص���Ļ";
  
key=dsc_whitebalance;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-4;
tabname=camera;
DISP_EN="whitebalance: 0-Auto,1-Fine,2-loudy,3-Filament Lamp,4-Fluorescent Lamp";
DISP_CH="��ƽ��: 0-�Զ� 1-���� 2-���� 3-�׳�� 4-ӫ���";

key=dsc_brightness;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-6;
tabname=camera;
DISP_EN="dsc brightness: 0(-3),1(-2),2(-1),3(0),4(1),5(2),6(3)";
DISP_CH="������: 0(-3),1(-2),2(-1),3(0),4(1),5(2),6(3)";

key=dsc_exposure_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-2;
tabname=camera;
DISP_EN="dsc exposure mode: 0-Auto,1-Indoor,2-Outdoor";
DISP_CH="�ع�ģʽ:0-�Զ� 1-���� 2-����";

key=dsc_face_identify;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-1;
tabname=camera;
DISP_EN="dsc face identify: 0-OFF,1-ON";
DISP_CH="�������: 0-�ر� 1-��";

key=dsc_specialeffect;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=camera;
DISP_EN="dsc_specialeffect: 0-OFF,1-Black White,2-Sepia,3-Negative";
DISP_CH="��Ч: 0-�� 1-�ڰ� 2-���� 3-��Ƭ";

key=dv_savepath;
type=DATA;
operate=SINGLE;
len=1;
RANGE=2;
tabname=camera;
DISP_EN="Save Path: 2-Card Memory";
DISP_CH="����·��: 2-��";

key=dsc_shutter_sound;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=camera;
DISP_EN="Shutter Sound: 0-OFF,1-Sound1,2-Sound2,3-Sound3";
DISP_CH="��������: 0-�ر� 1-��Ч1 2-��Ч2 3-��Ч3";

key=dsc_dater;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-2;
tabname=camera;
DISP_EN="dsc_dater: 0-OFF,1-Date,2-Date Time";
DISP_CH="��Ƭ���ڱ��: 0-�ر�,1-����,2-����ʱ��";


key=dv_whitebalance;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-4;
tabname=camera;
DISP_EN="dv whitebalance flag: 0-Auto,1-Fine,2-loudy,3-Filament Lamp,4-Fluorescent Lamp";
DISP_CH="��Ƶ��ƽ��: 0-�Զ�,1-����,2-����,3-�׳��,4-ӫ���";

key=dv_brightness;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-6;
tabname=camera;
DISP_EN="dv brightness: 0(-3),1(-2),2(-1),3(0),4(1),5(2),6(3)";
DISP_CH="��Ƶ������: 0(-3),1(-2),2(-1),3(0),4(1),5(2),6(3)";

key=dv_exposure_mode;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-2;
tabname=camera;
DISP_EN="dv exposure mode: 0-Auto,1-Indoor,2-Outdoor";
DISP_CH="��Ƶ�ع�ģʽ: 0-�Զ�,1-����,2-����";

key=dv_specialeffect;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=camera;
DISP_EN="dv_specialeffect: 0-OFF,1-Black & White,2-Sepia,3-Negative";
DISP_CH="��Ƶ��Ч: 0-��,1-�ڰ�,2-����,3-��Ƭ";

key=fm_band;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-3;
tabname=radio;
DISP_EN="Tuner Region: 0-China,1-USA,2-Japan,3-Europe"
DISP_CH="��̨Ƶ��: 0-�й�,1-����,2-�ձ�,3-ŷ��"

key=fm_intensity;
type=DATA;
operate=SINGLE;
len=1;
RANGE=5-10;
tabname=radio;
DISP_EN="fm intensity: 5~10"
DISP_CH="FM������: 5~10"

key=fm_record_quality;
type=DATA;
operate=SINGLE;
len=1;
RANGE=0-2;
tabname=radio;
DISP_EN="record quality: 0-low,1-middle,2-high"
DISP_CH="¼������: 0-��,1-��,2-��"

key=fm_record_gain;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1-5;
tabname=radio;
DISP_EN="record gain: 1~5"
DISP_CH="¼������: 1~5"

key=fm_record_path;
type=DATA;
operate=SINGLE;
len=1;
RANGE=1;
tabname=radio;
DISP_EN="save path: 1-card"
DISP_CH="¼���ļ�����·��: 1-��"
