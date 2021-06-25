#ifndef __CARD_DEV_H__    //
#define __CARD_DEV_H__

/*! \cond CARD_API*/
#define HDIO_GETC_CAP    		0xcad1	//:获取卡容量
#define HDIO_GETC_STA    		0xcad2	//：获取卡当前状态 返回0 无卡；1 有卡；2 未知。
#define HDIO_CHECK_LOCK 		0xcad3	//：卡检测屏蔽
#define HDIO_CHECK_ULOCK		0xcad4	//：卡检测使能
/*! \endcond*/

#endif
