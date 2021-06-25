/********************************************************************************
 *                              USDK(1100)
 *                            Module: DEVICE
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     device_driver.h
 * \brief    设备驱动管理头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __DEVICE_DRIVER_H__
#define __DEVICE_DRIVER_H__

#ifndef _ASSEMBLER_

struct device
{
    void *driver_data;
};

typedef struct pm_message
{
    int event;
} pm_message_t;

/* should be realized for driver_register and driver_unregister. */
struct device_driver
{
    const char * name;
    struct module * owner;
    int (*probe)(struct device * dev);
    //	int	(*remove)	(struct device * dev);
    void (*shutdown)(struct device * dev);
    int (*suspend)(struct device * dev, pm_message_t state, unsigned int level);
    int (*resume)(struct device * dev, unsigned int level);
};

extern int driver_register(struct device_driver * drv);
extern void driver_unregister(struct device_driver * drv);

#endif /*_ASSEMBLER_*/
#endif /* __DEVICE_DRIVER_H__ */
