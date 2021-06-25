
/*
*******************************************************************************
*
*                       NOYA1100 ---style共享库
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* FileName: style.h     Author:  zhaowenjie        Date:2008/12/16
* Description:    style 共享库提供给外部使用的接口。
* Others:
* History:
*      <author>    <time>       <version >    <desc>
*         zhaowenjie      2008/12/16     1.0         建立该文件。
*         zhaowenjie      2008/12/16     1.1         将命令改为符合1100 的命令规则。
*******************************************************************************
*
*/

/********************************************************************************
*                              NOYA1100 ---style共享库
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:
*      <author>    <time>       <version >    <desc>
*         zhaowenjie      2008/12/16     1.0         建立该文件。
*         zhaowenjie      2008/12/16     1.1         将命令改为符合1100 的命令规则。
********************************************************************************/
/*!
* \file     style.h
* \brief    是style 共享库提供给应用程序使用的接口。
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/1/14
*******************************************************************************/

#ifndef __STYLE_H__
#define __STYLE_H__

#ifdef _MSC_VER
#define PACK  // 1表示1字节对齐
#else
#define PACK __attribute__ ((packed))
#endif

/*资源的类型*/
/*! 图片*/
#define	RESOURCE_PICREGION 1
/*! 字符串*/
#define	RESOURCE_STRING 2
/*!  资源组*/
#define RESOURCE_RESGROUP 3
/*!  资源组*/
#define RESOURCE_PICTURE 4

#ifdef _MSC_VER
#pragma pack(1)
#endif

typedef struct style_s
{
    int file;
    /*! scene 的总数 */
    unsigned int sum; 
    /*! scene 的索引 */
    int* scenes;
}PACK
style_t;


/*
  * 在程序中使用的resource 格式，是所有资源结构的公有部分
  * 为了能把不同的资源结构连成一条链表
*/

/*!
*\brief
	资源的基本结构
*/
typedef struct resource_s
{
    /*!资源的类型*/
    unsigned char type;		
    /*! 该资源的ID */
    unsigned int  	 id;						
    /*! 指向下一个资源*/
    unsigned int* offset;		
}PACK
resource_t;

/*!
*\brief
	资源组的结构
*/
typedef struct
{
    /*!资源的基本信息*/
    resource_t   	 resource;

    /*! 在屏幕的绝对横坐标  */
    short  absolute_x;
    /*! 在屏幕的绝对纵坐标  */
    short  absolute_y;
    /*! 在父亲中的横坐标  */
    short  x;
    /*! 在父亲中的纵坐标  */
    short  y;
    
    /*! 宽度  */
    short  width;
    /*! 高度  */
    short  height;
    /*! 背景色  */
    unsigned int	 background;
        /*! 是否可见  */
    unsigned char  visible;
    /*! 是否透明 */
    unsigned char   opaque;
    /*! 透明度  */
    unsigned short  transparence;

    /*! 该resgroup 中资源的数目*/
    unsigned int  	 resource_sum;
    /*! 该resgroup 中资源的偏移*/
    unsigned int child_offset;	
}PACK
resgroup_resource_t;


/*!
*\brief
	图片区域资源的 结构
*/
typedef struct
{
    /*! 资源的基本结构*/
    resource_t   	 resource;
    /*! 在父亲中的横坐标*/
    short  x;
    /*! 在父亲中的纵坐标*/
    short  y;
    /*! 宽度*/
    short  width;
    /*! 高度*/
    short  height;
    /*! 是否可见*/
    unsigned char   visible;
    /*! 该图片区域资源拥有的图层数*/
    unsigned short 	 frames;		

    /*! 记录该图片区域资源中所有图层图片的ID */
    unsigned int pic_offset;
}PACK
picregion_resource_t;

/*!
*\brief
	图片区域中某一层的完整 结构
*/
typedef struct
{
    /*! 该图层的图片区域资源的ID 号*/
    unsigned int  	 id;   
    /*! 该图层的层数*/
    unsigned short  frame;
    /*! 图层所属图片区域的横坐标*/
    short  x;
    /*!  图层所属图片区域的纵坐标*/
    short  y;
    /*! 图层所属图片区域的宽度*/
    short  width;
     /*! 图层所属图片区域的高度*/
    short  height;
    /*! 该图层所使用的图片ID */
    unsigned short  pic_id;
}picregion_frame_t;



/*!
*\brief
	字符串资源的 结构
*/
typedef struct
{
    /*! 资源的基本结构*/
    resource_t   	 resource;
    /*! 矩形框的x 坐标 */
    short  x;     
    /*! 矩形框的y 坐标 */
    short  y;            		
    /*! 矩形框的宽度 */
    short  width;	
    /*! 矩形框的高度 */
    short  height;	
    /*! 字体的颜色 */
    unsigned int  	 foreground;	
    /*! 矩形框的背景色*/
    unsigned int	 background;
    /*! 是否可见 */
    unsigned char   visible;
    /*! 字体在矩形框中的对齐方式 */
    unsigned char   text_align;
    /*! 字体的模式 */
    unsigned char   mode;		
    /*!字体的大小*/
    unsigned char 	 font_height;	
    	
    		
    /*! 字符串的资源ID  */
    unsigned short str_id;			
    /*! 是否滚动: 1 表示滚动；0 表示不滚动 */
    unsigned char scroll;		
    /*! 滚动的方向: -1 表示从右到左；0 表示从左到右 */
    char direction;	
    /*! 滚动的区域，单位是象素 */
    unsigned short space;			
    /*! 滚动的步长,单位是象素 */
    unsigned short pixel;			
}PACK
string_resource_t;


/*!
*\brief
	图片资源的 结构
*/
typedef struct
{
     /*! 资源的基本结构*/
    resource_t   	 resource;
    short  x;
    short  y;
    short  width;
    short  height;
    unsigned char  visible;
    unsigned short    pic_id;
}PACK
picture_resource_t;

/*!
*\brief
	场景的 结构
*/
typedef struct
{	      
    /*! 横坐标  */
    short  x;
    /*! 纵坐标  */
    short  y;
    /*! 宽度  */
    short  width;
    /*! 高度  */
    short  height;
    /*! 背景色  */
    unsigned int background;     
    /*! scene 是否可见  */
    unsigned char  visible;
    /*! scene 是否透明 */
    unsigned char   opaque;
    /*! scene 的透明度  */
    unsigned short  transparence;
    
    /*! 该scene 中孩子资源的数目*/
    unsigned int resource_sum;
    /*! 孩子的偏移量 */
    unsigned int child_offset;
    /*! 场景方向*/
    unsigned char direction;
    
    unsigned char keys[ 16 ];
}PACK
scene_t;

#ifdef _MSC_VER
#pragma pack()
#endif

/******************************************************************************/
/*!                    
* \par  Description:
*      打开 sty 文件。
* \param[in]    filepath -- 该sty 文件的路径。
* \retval       该style 的指针或0( 没找到 )。
*\par example code
*\code
*	style_t* style = open_style_file( "music.sty" );
* \endcode
*******************************************************************************/
style_t* open_style_file( const unsigned char *filepath );

/******************************************************************************/
/*!                    
* \par  Description:
*      关闭 sty 文件。
* \param[in]    style -- 保存文件句柄的结构。
*\par example code
*\code
*	close_style_file( style );
* \endcode
*******************************************************************************/
void close_style_file( style_t* style );

/******************************************************************************/
/*!                    
* \par  Description:
*      从sty 文件中取得指定ID 的scene  。
* \param[in]    filepath -- 该sty 文件的路径。
* \param[in]    id -- 该scene 的ID。
* \retval       该scene 的指针或0( 没找到 )。
*\par example code
*\code
*	scene_t* scene = load_scene( "music.sty" , 1 );
* \endcode
*******************************************************************************/
scene_t* load_scene( style_t* style, const unsigned id );

/******************************************************************************/
/*!                    
* \par  Description:
*      释放 scene 资源  。
* \param[in]    scene -- 要释放的 scene 。
*\par example code
*\code
*	free_scene( scene );
* \endcode
*******************************************************************************/
void free_scene( scene_t* scene );

/******************************************************************************/
/*!                    
* \par  Description:
*      从sty 文件中取得指定ID 的scene  。
* \param[in]    filepath -- 该sty 文件的路径。
* \param[in]    id -- 该scene 的ID。
* \retval       该scene 的指针或0( 没找到 )。
*\par example code
*\code
*	scene_t* scene = load_scene( "music.sty" , 1 );
* \endcode
*******************************************************************************/
scene_t* load_scene( style_t* style, const unsigned id );

/******************************************************************************/
/*!                    
* \par  Description:
*      在指定的scene 中查找指定 局部ID 的资源。
* \param[in]    scene -- 该scene 的指针。
* \param[in]    id -- 要查找的资源的ID。
* \retval      该资源的指针或0( 没找到)。
*\par example code
*\code
*	void* resource = get_scene_child( resgroup, 2 );
* \endcode
*******************************************************************************/
void* get_scene_child( const scene_t* scene, const unsigned int id );

/******************************************************************************/
/*!                    
* \par  Description:
*      在指定的resgroup 中查找指定 局部ID 的资源。
* \param[in]    resgroup -- 该resgroup 的指针。
* \param[in]    id -- 要查找的资源的ID。
* \retval      该资源的指针或0( 没找到)。
*\par example code
*\code
*	void* resource = get_resgroup_child( resgroup, 2 );
* \endcode
*******************************************************************************/
void* get_resgroup_child( const resgroup_resource_t* resgroup, const unsigned int id );

/******************************************************************************/
/*!                    g
* \par  Description:
*      从picRegion 中取得帧数为frame 的图片帧，放置到输入参数pictureframe 中。
* \param[in]    picRegion -- 该picRegion 的指针。
* \param[in]    frame -- 该帧的序号，从 0 开始。
* \param[out]    pictureframe -- 返回的帧结构。
* \retval      0 成功，-1 失败。
*\par example code
*\code
*	picregion_resource_t* picture = (picregion_resource_t*)get_resource( scene, 2 );
*	picregion_frame_t* pictureframe = (picregion_frame_t* )malloc( sizeof( picregion_frame_t ) );
*	get_picregion_frame( picture, 3, pictureframe );
* \endcode
*******************************************************************************/
int get_picregion_frame( const picregion_resource_t* picRegion , const unsigned short frame , picregion_frame_t* pictureframe );

typedef struct
{
    style_t* (*const open_style_file)( const unsigned char* filepath );
    void (*const close_style_file)( style_t* style );
    scene_t* (*const load_scene ) ( style_t* style, const unsigned id );
    void (*const free_scene ) ( scene_t* scene );
    void* (*const get_scene_child ) ( const scene_t* scene, const unsigned int id );
    void* (*const get_resgroup_child ) ( const resgroup_resource_t* resgroup, const unsigned int id );
    int (*const get_picregion_frame ) ( const picregion_resource_t* picRegion , const unsigned short frame , picregion_frame_t* pictureframe );
}style_api_t;

/******************************************************************************/
/*!                    
* \par  Description:
*     取得style 的所有api 的结构体。
* \retval   存储着style 所有api 的结构体。
*\par example code
*\code
*	style_api_t* style = get_style_api( );
*      style->debug_style_file();
* \endcode
*******************************************************************************/
style_api_t* get_style_api();

#endif /*__STYLE_H__*/
