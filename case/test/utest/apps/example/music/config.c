/********************************************************************************
 *                        USDK130
 *                     platform configure
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: config.c
 * By  :
 * Version: 1> v1.00     first version     2006-9-25 8:28
 *******************************************************************************
 */

//#include "pthread.h"
//#include "Atj213x.h"

#include <syscalls.h>

#define Multi_CON1      0xB01C001c

void enable_jtag(void)
{
	unsigned int value;
	value = act_readl(Multi_CON1);
    value &= ~0x1000;                  //switch to jtag,fanqh added
	act_writel(value, Multi_CON1);
}

void disable_jtag(void)
{
	unsigned int value;
	value = act_readl(Multi_CON1);
    value |= 0x1000;                  //switch to jtag,fanqh added
	act_writel(value, Multi_CON1);
}

int main(int argc, char *argv[])
{

#if 1
    int i;
    char *path = getenv("PATH");
    char *file = getenv("FILE");

    printf("%s:%s:%d path=%p\n", __FILE__, __FUNCTION__, __LINE__,path);
    if( path )
    {
        printf("%s:%s:%d path=%s\n", __FILE__, __FUNCTION__, __LINE__, path);
    }

    printf("%s:%s:%d file=%p\n", __FILE__, __FUNCTION__, __LINE__,file);
    if( file )
    {
        printf("%s:%s:%d file=%s\n", __FILE__, __FUNCTION__, __LINE__, file);
    }
	printf("config create success\n");

    printf("argc = %d\n", argc);
	for( i = 0; i < argc; i++ )
	{
	    printf("argv[i] = %s\n", argv[i]);
	}

#endif

    enable_jtag();
    /*add you want to do here*/
    utest_main();

	while(1)
	{
		printf("config is running\n");
		sleep(1);
	}
	return 0;
}
