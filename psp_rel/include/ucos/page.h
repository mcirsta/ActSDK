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
 * \brief  ��ҳ�ڴ����ͷ�ļ�
 * \author fanqh
 * \par GENERAL DESCRIPTION:
 *       page.cͷ�ļ����ں�ר��
 * \par EXTERNALIZED FUNCTIONS:
 *       �����������õ������ģ��
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
 *   ASID��غ궨��
 */
#define ASID_SHIFT  8
#define ASID_SIZE   (1UL << ASID_SHIFT)
#define ASID_MASK   (~(ASID_SIZE-1))

/*!
 *   MODULE_ID��غ궨��
 */
#define MODULE_ID_SHIFT  8
#define MODULE_ID_SIZE   (1UL << MODULE_ID_SHIFT)
#define MODULE_ID_MASK   (~(MODULE_ID_SIZE-1))

/*!
 *   PAGE��غ궨��
 */
#define PAGE_SHIFT  12
#define PAGE_SIZE   (1UL << PAGE_SHIFT)
#define PAGE_MASK   (~(PAGE_SIZE-1))
/* to align the pointer to the (next) page boundary */
#define PAGE_ALIGN(addr)    ((((addr)+PAGE_SIZE)-1)&PAGE_MASK)

/*!
 *   MODULE��غ궨��
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
 *   MODULE_ID����궨��
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
 *   �Ŀ�����buf��ʼ��ַ�궨��
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
 *   ҳ��ṹ��غ궨��
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

/*����ҳ���ҳ��ʹ��ʱ����0����0�������Ч��*/
#define PAGE_LINE_WEN_VALID(addr)  ((0x3f<<26) | (((addr)&PAGE_MASK&0x0fffffff)>>6) | W_ENABLE | PAGE_VALID | PAGE_GLOBAL )
#define PAGE_LINE_WDIS_VALID(addr)  ((0x3f<<26) | (((addr)&PAGE_MASK&0x0fffffff)>>6) | PAGE_VALID | PAGE_GLOBAL )
#define PAGE_LINE_TO_ADDR(page_line)  (((page_line<<6)&PAGE_MASK) | 0x80000000)
#define MY_BYTE_ALIGN(x) (((x + 15)/16)*16)             /* alloc based on 16 byte */
#define M_LOAD      0xff
#define M_ULOAD     0x00

/*!
 *   DMAҳ������궨��
 */
#define DMA_USED    0x55
#define DMA_FREE    0xaa
#define BADPAGE     0xff

#ifndef _ASSEMBLER_

/*!
 *  \brief
 *  �ڴ�����ö��ֵ
 *  \li ZONE_MAP_UNSWAP : ����ӳ�䣬���԰��ƣ����ɻ���
 *  \li ZONE_MAP_SWAP : ����ӳ�䣬���԰��ƣ��ɻ����������ӳټ��ص�ģ��Ĵ���β��֣�
 *  \li ZONE_DMA : ������ӳ�䣬���ɰ��ƣ����ɻ���
 */
enum
{
    ZONE_MAP_UNSWAP,
    ZONE_MAP_SWAP,
    ZONE_DMA,
};

/*!
 * \brief
 *      �����Դ���ڴ�/�ź���/��Ϣ���У�����/�ͷŵ�����ڵ����ݽṹ
 */
typedef struct mem_info
{
    /*! ��һ���ڵ�*/
    struct mem_info * next;
    /*! �������Դ�ĵ��õ�ַ*/
    unsigned int call_address;
    /*! ��ȡ������Դ*/
    unsigned int get_pointer;
} mem_info_t;

/*!
 * \brief
 *      ģ���������ڴ�ҳʹ�ñ�־���ݽṹ
 */
typedef struct
{
    /* ģ���ڵ��ڴ�Ҳʹ�ñ�־��char�ͱ�ʾ */
    unsigned char flags;
} module_mem_map_t;

/*!
 * \brief
 *      bucket�ڵ����ݽṹ
 */
struct bucket_desc
{
    /* �ڵ�ָ���ڴ�ҳ����>=4KB�����׵�ַ */
    void *page;
    /* ����next */
    struct bucket_desc *next;
    /* �׸����������׵�ַ */
    void *freeptr;
    /* �ڴ�ҳ��Ӧ�ô���(���bucket_size<4KB) */
    unsigned short refcnt;
    /* ��bueket�ĵ�λ��֧�ֳ���4KB�� */
    unsigned int bucket_size;
};

/*!
 * \brief
 *      bucket���ݽṹ
 */
struct _bucket_dir
{
    /* bucket��С�����4KB��������4KB��ʵ�ʳ�����struct bucket_desc���ݽṹ��bucket_size�����֣� */
    int size;
    /* bucket��ͷ�ڵ� */
    struct bucket_desc *chain;
};

#if (MALLOC_REALIZE_TYPE == 0)
/*!
 * \brief
 *      �ѿռ�����Ľڵ����ݽṹ
 */
typedef struct alloc_struct
{
    /*! �ýڵ�����buf���׵�ַ*/
    unsigned long address;
    /*! �ýڵ�����buf�Ĵ�С*/
    unsigned long size;
    /*! ��һ���ڵ�*/
    struct alloc_struct *next;
    /*! ����*/
    unsigned long reserve;
} alloc_struct_t;

#else

/*!
 * \brief
 *      ģ��ѿռ�������ݽṹ
 */
typedef struct
{
    /*! �Ƿ��ʼ����־*/
    int flags;
    /*! ҳ��λͼָ��*/
    module_mem_map_t mem_map[PAGES_PER_MODULE];
    /*! bucket_dirָ��*/
    struct _bucket_dir bucket_dir[9];
    /*! ����bucket_descָ��*/
    void *free_bucket_desc;
}module_heap_t;

#endif

/*!
 * \brief
 *      segment��blockѹ����Ϣ���ݽṹ
 */
typedef struct
{
    /*! block���ڴ������ַ*/
    unsigned int virt_addr;
    /*! block���ļ��ڵ�ƫ��*/
    unsigned int file_offset;
    /*! block��С*/
    unsigned long orgin_len;
    /*! blockѹ����Ĵ�С*/
    unsigned long compr_len;
} block_compr_tbl_t;

/*!
 * \brief
 *      elf�ļ�ѹ����Ϣ���ݽṹ
 */
typedef struct
{
    /*! ѹ����Ϣbuf���׵�ַ*/
    block_compr_tbl_t *first;
    /*! block��*/
    unsigned int num;
} compr_info_t;

/*!
 * \brief
 *      ģ����Ϣ���ݽṹ
 */
typedef struct module_s
{
    /*! elf�ļ���entry*/
    unsigned int entry;
    /*! elf�ļ���text�����ļ��ڵ�ƫ��*/
    unsigned int text_offset;
    /*! elf�ļ���data/bss�����ļ��ڵ�ƫ��*/
    unsigned int data_offset;
    /*! elf�ļ���text������ĩ��ַ�����data/bss��������ʼ��ַ*/
    unsigned int text_end;
    /*! elf�ļ���data������ĩ��ַ*/
    unsigned int data_end;
    /*! elf�ļ���bss������ĩ��ַ*/
    unsigned int bss_end;
    /*! elf�ļ���bss������ĩ��ַcacheline���ȶ��뼴�ѿ�ʼ��ַ*/
    unsigned int brk;
    /*! ģ�����߳�ջ����ַ*/
    unsigned int stack_end;
    /*! ģ�����߳�ջλͼ�������߳�ջ֧��64KB�����ɸ�����*/
    unsigned long long stack_bit;
    /*! ģ������������·����*/
    unsigned char name[19];
    /*! ģ�飨Ӧ�ã��������̺�*/
    unsigned short pid;
    /*! ģ���ڶѿռ����ֵ*/
    unsigned int brk_max;
    /*! ģ����kmalloc��buf��С*/
    unsigned int kmalloc_size;
    /*! ģ����kmalloc��buf�����ֵ*/
    unsigned int kmalloc_size_max;
    /*! �ļ�������*/
    int fd;
    /*! ���ط�ʽ*/
    unsigned char load_flag;
    /*! cache����*/
    unsigned char cache_flag;
    /*! ����*/
    unsigned char reserve;
    /*! ģ���ڶѿռ���ʼ��ַ*/
    unsigned int brk_min;
    /*! ģ���ڶ�*/
#if (MALLOC_REALIZE_TYPE == 0)
    alloc_struct_t head;
#else
    module_heap_t module_heap;
#endif
    /*! ģ����Ϣ����ָ����һ��ģ����Ϣ���ݽṹָ��*/
    struct module_s * next;
    /*! ���ģ����������ڴ������*/
    mem_info_t * memlist;
    /*! ���ģ����������ź���������*/
    mem_info_t * semlist;
    /*! ���ģ�����������Ϣ���е�����*/
    mem_info_t * qlist;
    /*! ģ���ӳټ��������lseek����ָ��*/
    off_t (*elf_lseek)(unsigned int, off_t, unsigned int);
    /*! ģ���ӳټ��������read����ָ��*/
    ssize_t (*elf_read)(unsigned int, char *, size_t);
    /*! ģ���ӳټ��������close����ָ��*/
    long (*elf_close)(unsigned int);
    /*! block_compr_tblָ��*/
    compr_info_t compr_info;
}__attribute__ ((packed)) module_t;

/*!
 * \brief
 *      ģ��ҳ�����Ϣ���ݽṹ
 */
typedef struct module_entry
{
    /*! ҳ��ָ��*/
    unsigned int * page_table;
    /*! ģ����Ϣָ��*/
    module_t * module_info;
} module_entry_t;

/*!
 * \brief
 *      �õ��ڴ�������Ϣ���ݽṹ
 */
typedef struct
{
    /*! �õ��ڴ�����ʼҳ��*/
    int start_page;
    /*! ���ڴ�������ҳ��*/
    unsigned int page_num;
}__attribute__ ((packed)) good_buffer_t;

/*!
 * \brief
 *      �ڴ�������ʼ�����Ϣ
 */
typedef struct
{
    /*! �ڴ�����*/
    unsigned int good_buffer_id;
    /*! �ڴ����ڵ�page��*/
    unsigned int page_id;
//    /*! �ڴ�����page����*/
//    unsigned int  page_attr;
} mem_zone_info_t;

/*!
 * \brief
 *      �����ڴ�ҳʹ�ñ�־���ݽṹ
 */
typedef struct
{
    /* �����ڴ�ҳʹ�ñ�־��unsigned int�ͱ�ʾ */
    unsigned int flags;
} mem_map_t;

/*!
 * \brief
 *      �ڴ�ҳʹ�ñ�־���ݽṹ
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
