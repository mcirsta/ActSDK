#ifndef _EFFECT_H
#define _EFFECT_H

#ifdef WIN32
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#endif


typedef int PFreal;
#define PFREAL_SHIFT 10

#define IANGLE_MAX 1024
#define IANGLE_MASK 1023

#ifndef NULL
#define NULL 0
#endif
//---------- api-----------------
  
typedef enum
{
	TRANS_PAGE,		//翻页
	TRANS_SMOOTHMOVE,	//
	TRANS_RANDOMLINE, //随机线条
	TRANS_LOUVER,		//天窗
	TRANS_ERASE,      //擦去
	TRANS_CUT,		//分割
	TRANS_CHECK,		//棋盘
	TRANS_CUBE,	    //立方体
	TRANS_WAVE,		//波浪
	TRANS_MOVE,		//推送
	TRANS_REVERSE,	//翻转
	TRANS_PPT,		//换灯片
	TRANS_FADE,		//淡入淡出
	TRANS_ZOOM,		//普通缩放
	TRANS_FADEZOOM,	//淡出,放大
	TRANS_ROTATIONZOOM,
	TRANS_ROTATION_2D, //other
	TRANS_POPUP,		//弹出操作
	TRANS_POPUPALPHA,
	TRANS_MOVEALPHA		//输入输出均带alpha
}trans_mode_t;

typedef enum
{
	TRANS_UP,			//下往上
	TRANS_DOWN,       //上往下
	TRANS_LEFT,		//从右向左
	TRANS_RIGHT,		//从左向右
}trans_direct_t;

#ifdef I32
#undef I32
#endif
#ifdef I16
#undef I16
#endif
#ifdef I8
#undef I8
#endif
#ifdef U8
#undef U8
#endif
#ifdef U16
#undef U16
#endif
#ifdef U32
#undef U32
#endif
typedef signed long I32;
typedef short I16;
typedef char I8;
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int  U32;

typedef struct 
{
	I32  xview;       //输出buf宽,起始位置为屏幕左上角
	I32  yview;		  //输出buf高	
	I32  xpos;        //输出的x起始位置
	I32  ypos;        //输出的y起始位置
}coordinate_t;

typedef struct 
{
	trans_mode_t mode;			//模型trans_mode_t
	U8 *srcbuf;			//源图像
	U8 *desbuf;			//目标图像
	I32  width;			//宽，4对齐
	I32  height;		//高
	I32  totalframes;	//总帧数 1 - 10			
	I32  currentframe;  //当前帧号
	trans_direct_t  directflag;	//运动方向， trans_direct_t	
	I32 alpha;          //旋转角度，在popup时为透明参数 1 - 9
	U8 *bufout;			//输出buf
	coordinate_t viewparam;    //输出图像位置coordinate_t
}transval_t;

I32 transimage(transval_t *transval);

//------------------------------------*/
#endif

