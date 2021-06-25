/********************************************************************************
 *                                        USDK1100
 *                                       Module: Kernel
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fanqh     2008-02-01 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file   page.h
 * \brief  分页内存管理头文件
 * \author fanqh
 * \par GENERAL DESCRIPTION:
 *       page.c头文件，内核专用
 * \par EXTERNALIZED FUNCTIONS:
 *       这里描述调用到外面的模块
 *  \version 1.0
 *  \date  2008/2/1
 *******************************************************************************/

#ifndef __PAGE_H__
#define __PAGE_H__

#include "ucos/memory.h"
#include "ucos/os.h"
#include "ucos/os_cfg.h"
#include "asm-mips/addrspace.h"

/*!
 *   ASID相关宏定义
 */
#define ASID_SHIFT  8
#define ASID_SIZE   (1UL << ASID_SHIFT)
#define ASID_MASK   (~(ASID_SIZE-1))

/*!
 *   MODULE_ID相关宏定义
 */
#define MODULE_ID_SHIFT  8
#define MODULE_ID_SIZE   (1UL << MODULE_ID_SHIFT)
#define MODULE_ID_MASK   (~(MODULE_ID_SIZE-1))

/*!
 *   PAGE相关宏定义
 */
#define PAGE_SHIFT  12
#define PAGE_SIZE   (1UL << PAGE_SHIFT)
#define PAGE_MASK   (~(PAGE_SIZE-1))
/* to align the pointer to the (next) page boundary */
#define PAGE_ALIGN(addr)    ((((addr)+PAGE_SIZE)-1)&PAGE_MASK)

/*!
 *   MODULE相关宏定义
 */
#define MODULE_SHIFT  22
#define MODULE_SIZE  (1UL << MODULE_SHIFT)
#define MODULE_MASK (~(MODULE_SIZE-1))
#define MODULE_ALIGN(addr)  ((((addr)+MODULE_SIZE)-1)&MODULE_MASK)
#define MODULE_NR(addr)    (((unsigned int)(addr)& ~MODULE_MASK) >> PAGE_SHIFT)

#define PAGES_PER_MODULE (MODULE_SIZE/PAGE_SIZE)
//#define TEXT_OFFSET                    0x1000
/* This handles the memory map.. */
#define PAGE_OFFSET     0x80000000
#define OS_START        0xc0000000
#define NAND_START      0xc1800000

#define OS_PAGESIZE     0x40000         //256k
#define NAND_PAGESIZE   0x10000         //64k
#define MAX_TASK_STACK_PAGES  (MAX_TASK_STACK_SIZE/PAGE_SIZE)      //max 16 page
#define KERNEL_SP_NUM(ptr)   ((MODULE_NR(ptr)&(~(MAX_TASK_STACK_PAGES-1))) + (MAX_TASK_STACK_PAGES-1))

#define MAP_NR(addr)      (TO_PHYS((unsigned int)addr)>>PAGE_SHIFT)
#define PHY_ADDR(i)         (PAGE_OFFSET+((i)<<PAGE_SHIFT))
#define MAP_PAGE_RESERVED   (1<<7)
#define PAGE_FLAG  (1<<31)

/*!
 *   MODULE_ID分配宏定义
 */
#define KO_ZONE_NUMBER      128
#define KO_RESERVE_ZONE_NUMBER  128
#define SO_ZONE_NUMBER      128
#define APP_ZONE_NUMBER     128

#define KERNEL_MODULE_ID_START   0
#define KERNEL_MODULE_ID_END     ((KERNEL_MODULE_ID_START+KO_ZONE_NUMBER)-1)
#define USER_MODULE_ID_START     (KERNEL_MODULE_ID_START+KO_ZONE_NUMBER+KO_RESERVE_ZONE_NUMBER)
#define SO_MODULE_ID_START       USER_MODULE_ID_START
#define APP_MODULE_ID_START      (USER_MODULE_ID_START+SO_ZONE_NUMBER)
#define USER_MODULE_ID_END       ((USER_MODULE_ID_START+SO_ZONE_NUMBER+APP_ZONE_NUMBER)-1)
#define ALL_MODULE_NUMBER   (KERNEL_MODULE_ID_START+KO_ZONE_NUMBER+KO_RESERVE_ZONE_NUMBER+SO_ZONE_NUMBER+APP_ZONE_NUMBER)

/*!
 *   四块特殊buf起始地址宏定义
 */
#define W_BACK_START_ADDR      0x80000000
#define W_THR_START_ADDR        0x10000000
#define UNCACHE_START_ADDR     0xa0000000

#define W_BACK_ALLOC_START  0xe0000000
//#define W_THR_ALLOC_START         0xe0400000
//#define W_THR_NO_ALLOC_START  0xe0800000
//#define NO_CACHE_START                0xe0c00000

#define DISCRETE_START_ADDR    W_BACK_ALLOC_START

#define W_BACK_ALLOC_MODULE_ID           (KERNEL_MODULE_ID_END+1)
//#define W_THR_ALLOC_MODULE_ID          (KERNEL_MODULE_ID_END+2)
//#define W_THR_NO_ALLOC_MODULE_ID       (KERNEL_MODULE_ID_END+3)
//#define NO_CACHE_MODULE_ID             (KERNEL_MODULE_ID_END+4)

//#define  STACK_MODULE_ID_START         (KERNEL_MODULE_ID_END+5)
//#define  STACK_MODULE_ID_END           (KERNEL_MODULE_ID_END+64)

/*!
 *   页表结构相关宏定义
 */
#define PAGE_AGE_SHIFT     26
#define PAGE_AGE_BITS       6
#define PAGE_AGE_SIZE       (1<<PAGE_AGE_BITS)
#define PAGE_AGE_MASK       (~((PAGE_AGE_SIZE-1)<<PAGE_AGE_SHIFT))

#define PAGE_PFN_SHIFT     6
#define PAGE_PFN_BITS      20

#define PAGE_CACHE_SHIFT   3
#define PAGE_CACHE_BITS     3

#define PAGE_DIRTY_SHIFT   2
#define PAGE_VALID_SHIFT   1
#define PAGE_GLOBAL_SHIFT  0

/*cache prioperty,occupy 3 bit*/
#define W_BACK_ALLOC        (0x3<<PAGE_CACHE_SHIFT)
#define W_THROUGH_ALLOC     (0x1<<PAGE_CACHE_SHIFT)
#define W_THROUGH_NO_ALLOC  (0x0<<PAGE_CACHE_SHIFT)
#define W_NO_CACHE          (0x2<<PAGE_CACHE_SHIFT)

/*write enable bit*/
#define W_ENABLE            (0x1<<PAGE_DIRTY_SHIFT)

/*page valid bit*/
#define PAGE_VALID          (0x1<<PAGE_VALID_SHIFT)

/*global bit*/
#define PAGE_GLOBAL         (0x1<<PAGE_GLOBAL_SHIFT)

#define PAGE_CACHE_MASK     (~(0x7<<PAGE_CACHE_SHIFT))
#define PAGE_DVG_MASK       (~(W_ENABLE | PAGE_VALID | PAGE_GLOBAL))

/*分配页面给页表使用时先清0，用0来填充无效项*/
#define PAGE_LINE_WEN_VALID(addr)  ((0x3f<<26) | (((addr)&PAGE_MASK&0x0fffffff)>>6) | W_ENABLE | PAGE_VALID | PAGE_GLOBAL )
#define PAGE_LINE_WDIS_VALID(addr)  ((0x3f<<26) | (((addr)&PAGE_MASK&0x0fffffff)>>6) | PAGE_VALID | PAGE_GLOBAL )
#define PAGE_LINE_TO_ADDR(page_line)  (((page_line<<6)&PAGE_MASK) | 0x80000000)
#define MY_BYTE_ALIGN(x) (((x + 15)/16)*16)             /* alloc based on 16 byte */
#define M_LOAD      0xff
#define M_ULOAD     0x00

/*!
 *   DMA页面区域宏定义
 */
#define DMA_USED    0x55
#define DMA_FREE    0xaa
#define BADPAGE     0xff

#ifndef _ASSEMBLER_

/*!
 *  \brief
 *  内存区域枚举值
 *  \li ZONE_MAP_UNSWAP : 经过映射，可以搬移，不可换出
 *  \li ZONE_MAP_SWAP : 经过映射，可以搬移，可换出（运行延迟加载的模块的代码段部分）
 *  \li ZONE_DMA : 不经过映射，不可搬移，不可换出
 */
enum
{
    ZONE_MAP_UNSWAP,
    ZONE_MAP_SWAP,
    ZONE_DMA,
};

/*!
 * \brief
 *      监控资源（内存/信号量/消息队列）申请/释放的链表节点数据结构
 */
typedef struct mem_info
{
    /*! 下一个节点*/
    struct mem_info * next;
    /*! 申请该资源的调用地址*/
    unsigned int call_address;
    /*! 获取到的资源*/
    unsigned int get_pointer;
} mem_info_t;

/*!
 * \brief
 *      模块内虚拟内存页使用标志数据结构
 */
typedef struct
{
    /* 模块内的内存也使用标志用char型表示 */
    unsigned char flags;
} module_mem_map_t;

/*!
 * \brief
 *      bucket节点数据结构
 */
struct bucket_desc
{
    /* 节点指向内存页区域（>=4KB）的首地址 */
    void *page;
    /* 链表next */
    struct bucket_desc *next;
    /* 首个空闲区域首地址 */
    void *freeptr;
    /* 内存页内应用次数(针对bucket_size<4KB) */
    unsigned short refcnt;
    /* 该bueket的单位（支持超过4KB） */
    unsigned int bucket_size;
};

/*!
 * \brief
 *      bucket数据结构
 */
struct _bucket_dir
{
    /* bucket大小（最大4KB，超过按4KB的实际长度在struct bucket_desc数据结构的bucket_size中体现） */
    int size;
    /* bucket链头节点 */
    struct bucket_desc *chain;
};

#if (MALLOC_REALIZE_TYPE == 0)
/*!
 * \brief
 *      堆空间链表的节点数据结构
 */
typedef struct alloc_struct
{
    /*! 该节点管理的buf的首地址*/
    unsigned long address;
    /*! 该节点管理的buf的大小*/
    unsigned long size;
    /*! 下一个节点*/
    struct alloc_struct *next;
    /*! 保留*/
    unsigned long reserve;
} alloc_struct_t;

#else

/*!
 * \brief
 *      模块堆空间管理数据结构
 */
typedef struct
{
    /*! 是否初始化标志*/
    int flags;
    /*! 页面位图指针*/
    module_mem_map_t mem_map[PAGES_PER_MODULE];
    /*! bucket_dir指针*/
    struct _bucket_dir bucket_dir[9];
    /*! 空闲bucket_desc指针*/
    void *free_bucket_desc;
}module_heap_t;

#endif

/*!
 * \brief
 *      segment的block压缩信息数据结构
 */
typedef struct
{
    /*! block的内存虚拟地址*/
    unsigned int virt_addr;
    /*! block在文件内的偏移*/
    unsigned int file_offset;
    /*! block大小*/
    unsigned long orgin_len;
    /*! block压缩后的大小*/
    unsigned long compr_len;
} block_compr_tbl_t;

/*!
 * \brief
 *      elf文件压缩信息数据结构
 */
typedef struct
{
    /*! 压缩信息buf的首地址*/
    block_compr_tbl_t *first;
    /*! block数*/
    unsigned int num;
} compr_info_t;

/*!
 * \brief
 *      模块信息数据结构
 */
typedef struct module_s
{
    /*! elf文件的entry*/
    unsigned int entry;
    /*! elf文件的text段在文件内的偏移*/
    unsigned int text_offset;
    /*! elf文件的data/bss段在文件内的偏移*/
    unsigned int data_offset;
    /*! elf文件的text段链接末地址对齐或data/bss段链接起始地址*/
    unsigned int text_end;
    /*! elf文件的data段链接末地址*/
    unsigned int data_end;
    /*! elf文件的bss段链接末地址*/
    unsigned int bss_end;
    /*! elf文件的bss段链接末地址cacheline长度对齐即堆开始地址*/
    unsigned int brk;
    /*! 模块内线程栈顶地址*/
    unsigned int stack_end;
    /*! 模块内线程栈位图（单个线程栈支持64KB内自由浮动）*/
    unsigned long long stack_bit;
    /*! 模块名（不包含路径）*/
    unsigned char name[19];
    /*! 模块（应用）所属进程号*/
    unsigned short pid;
    /*! 模块内堆空间最大值*/
    unsigned int brk_max;
    /*! 模块内kmalloc的buf大小*/
    unsigned int kmalloc_size;
    /*! 模块内kmalloc的buf的最大值*/
    unsigned int kmalloc_size_max;
    /*! 文件描述符*/
    int fd;
    /*! 加载方式*/
    unsigned char load_flag;
    /*! cache属性*/
    unsigned char cache_flag;
    /*! 保留*/
    unsigned char reserve;
    /*! 模块内堆空间起始地址*/
    unsigned int brk_min;
    /*! 模块内堆*/
#if (MALLOC_REALIZE_TYPE == 0)
    alloc_struct_t head;
#else
    module_heap_t module_heap;
#endif
    /*! 模块信息链表，指向下一个模块信息数据结构指针*/
    struct module_s * next;
    /*! 监控模块内申请的内存的链表*/
    mem_info_t * memlist;
    /*! 监控模块内申请的信号量的链表*/
    mem_info_t * semlist;
    /*! 监控模块内申请的消息队列的链表*/
    mem_info_t * qlist;
    /*! 模块延迟加载所需的lseek函数指针*/
    off_t (*elf_lseek)(unsigned int, off_t, unsigned int);
    /*! 模块延迟加载所需的read函数指针*/
    ssize_t (*elf_read)(unsigned int, char *, size_t);
    /*! 模块延迟加载所需的close函数指针*/
    long (*elf_close)(unsigned int);
    /*! block_compr_tbl指针*/
    compr_info_t compr_info;
}__attribute__ ((packed)) module_t;

/*!
 * \brief
 *      模块页表和信息数据结构
 */
typedef struct module_entry
{
    /*! 页表指针*/
    unsigned int * page_table;
    /*! 模块信息指针*/
    module_t * module_info;
} module_entry_t;

/*!
 * \brief
 *      好的内存区域信息数据结构
 */
typedef struct
{
    /*! 好的内存区起始页号*/
    int start_page;
    /*! 该内存区的中页数*/
    unsigned int page_num;
}__attribute__ ((packed)) good_buffer_t;

/*!
 * \brief
 *      内存区域起始点的信息
 */
typedef struct
{
    /*! 内存区号*/
    unsigned int good_buffer_id;
    /*! 内存区内的page号*/
    unsigned int page_id;
//    /*! 内存区的page属性*/
//    unsigned int  page_attr;
} mem_zone_info_t;

/*!
 * \brief
 *      物理内存页使用标志数据结构
 */
typedef struct
{
    /* 物理内存页使用标志用unsigned int型表示 */
    unsigned int flags;
} mem_map_t;

/*!
 * \brief
 *      内存页使用标志数据结构
 */
typedef struct
{
    unsigned int flag :1;
    unsigned int reserve :3;
    unsigned int pid :8;
    unsigned int paddr :20;
} mem_map_flags_t;

#define PAGE_PID_SHIFT     4
#define PAGE_PID_BITS       8
#define PAGE_PID_SIZE       (1UL <<PAGE_PID_BITS)
#define PAGE_PID_MASK       (~((PAGE_PID_SIZE-1)<<PAGE_PID_SHIFT))
//#define PID_IN_PAGE(x)    ((x & ~(PAGE_PID_SIZE-1))<<PAGE_PID_SHIFT)
//#define PAGE_TO_PID(x)    ((x & PAGE_PID_MASK)>>PAGE_PID_SHIFT)

extern int cur_pid;

extern unsigned int module_id_w_back_alloc;
extern unsigned int module_id_w_thr_alloc;
extern unsigned int module_id_w_thr_no_alloc;
extern unsigned int module_id_no_cache;
extern unsigned int module_space_cnts;

extern unsigned int sdram_cap;
extern unsigned int ebase;
extern unsigned int ebase_flash;
extern unsigned int page_num;
extern unsigned int total_free_pages;
extern unsigned int *phy_pages;
extern module_entry_t module[];
extern module_entry_t *module_so; //the pt index of so
extern module_entry_t *p_module_so_cur;

extern void mem_init(void);
extern void nand_mem_init(unsigned int nand_len);
extern unsigned int get_free_pages(unsigned int size, unsigned int flags);
extern unsigned int get_free_page(unsigned int flags);
extern unsigned int get_zero_page(unsigned int flags);
extern unsigned int get_max_free_map_page_num(void);
extern int update_max_free_map_page_num(void);
extern module_t *register_module(int module_id);
extern int free_page(unsigned int addr);
extern int free_pages(void *ptr, unsigned int size);
extern void invalid_tlb(void);
extern int addr_to_module_id(unsigned int addr);
extern void relate_paddr_vaddr(unsigned int paddr, unsigned int vaddr, int pid);

#if (MALLOC_REALIZE_TYPE == 0)
extern void *sbrk(int module_id, unsigned int increment);
#else
extern void *sbrk(module_t *p_module_info, unsigned int increment);
#endif

extern void copy_page_table(int module_id, unsigned int startpage,
        unsigned int endpage);
extern void unload_module(int module_id);
extern void unload_module_so(int module_id, int pid);
extern module_entry_t *inner_get_module_pointer(int module_id, int pid);
extern module_entry_t *get_module_pointer(int module_id);
extern module_t *alloc_module_info(void);
extern void free_module_info(module_t *mdesc);
extern void clear_module_entry(module_entry_t *p_module);
extern void syn_tlb(unsigned int entryhi);
extern void sync_tlb(unsigned int entryhi, int pid);
extern unsigned int is_page_in_tlb(unsigned int entryhi);
extern unsigned int
        module_load(unsigned char * name, unsigned int module_entry);//retval: 1 -> module load, 0 -> module no load

extern int dump_module_mem_info(unsigned int addr, int pid);
extern int dump_all_module_mem_info(void);

extern void * sys_mem_allocate(unsigned int size, unsigned int mem_flag);
extern int sys_mem_free(char * pbuf);
extern void * sys_get_phyaddr(char * pbuf);
extern unsigned int sys_get_max_free(void);

#endif
#endif/*__PAGE_H__*/
