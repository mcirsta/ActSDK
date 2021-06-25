/********************************************************************************
 *                              USDK130
 *                            Module: ELF
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     elf.h
 * \brief    elf格式相关数据结构和elf解析函数声明
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __ELF_H__
#define __ELF_H__

#include <ucos/types.h>

/* 32-bit ELF base types. */
typedef __u32 Elf32_Addr;
typedef __u16 Elf32_Half;
typedef __u32 Elf32_Off;
typedef __s32 Elf32_Sword;
typedef __u32 Elf32_Word;

/* These constants define the different elf file types */
#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

#define	EI_MAG0		0		/* e_ident[] indexes */
#define	EI_MAG1		1
#define	EI_MAG2		2
#define	EI_MAG3		3
#define	EI_CLASS	4
#define	EI_DATA		5
#define	EI_VERSION	6
#define	EI_OSABI	7
#define	EI_PAD		8

#define	ELFMAG0		0x7f		/* EI_MAG */
#define	ELFMAG1		'E'
#define	ELFMAG2		'L'
#define	ELFMAG3		'F'
#define	ELFMAG			"\177ELF"

/* sh_type */
#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_NUM		12
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff

/* sh_flags */
#define SHF_WRITE	0x1
#define SHF_ALLOC	0x2
#define SHF_EXECINSTR	0x4
#define SHF_MASKPROC	0xf0000000

/* special section indexes */
#define SHN_UNDEF	0
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff

/* This info is needed when parsing the symbol table */
#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4

#define ELF_ST_BIND(x)		((x) >> 4)
#define ELF_ST_TYPE(x)		(((unsigned int) x) & 0xf)
#define ELF32_ST_BIND(x)	ELF_ST_BIND(x)
#define ELF32_ST_TYPE(x)	ELF_ST_TYPE(x)
#define ELF64_ST_BIND(x)	ELF_ST_BIND(x)
#define ELF64_ST_TYPE(x)	ELF_ST_TYPE(x)

/* The following are used with relocations */
#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)

#define Elf_Shdr	Elf32_Shdr
#define Elf_Sym		Elf32_Sym
#define Elf_Ehdr	Elf32_Ehdr
#define Elf_Addr	Elf32_Addr

#define Elf_Mips_Rel	Elf32_Rel
#define Elf_Mips_Rela	Elf32_Rela

#define ELF_MIPS_R_SYM(rel) ELF32_R_SYM(rel.r_info)
#define ELF_MIPS_R_TYPE(rel) ELF32_R_TYPE(rel.r_info)

#define R_MIPS_NONE		0
#define R_MIPS_16		1
#define R_MIPS_32		2
#define R_MIPS_REL32		3
#define R_MIPS_26		4
#define R_MIPS_HI16		5
#define R_MIPS_LO16		6
#define R_MIPS_GPREL16		7
#define R_MIPS_LITERAL		8
#define R_MIPS_GOT16		9
#define R_MIPS_PC16		10
#define R_MIPS_CALL16		11
#define R_MIPS_GPREL32		12
/* The remaining relocs are defined on Irix, although they are not
 in the MIPS ELF ABI.  */
#define R_MIPS_UNUSED1		13
#define R_MIPS_UNUSED2		14
#define R_MIPS_UNUSED3		15
#define R_MIPS_SHIFT5		16
#define R_MIPS_SHIFT6		17
#define R_MIPS_64		18
#define R_MIPS_GOT_DISP		19
#define R_MIPS_GOT_PAGE		20
#define R_MIPS_GOT_OFST		21
/*
 * The following two relocation types are specified in the MIPS ABI
 * conformance guide version 1.2 but not yet in the psABI.
 */
#define R_MIPS_GOTHI16		22
#define R_MIPS_GOTLO16		23
#define R_MIPS_SUB		24
#define R_MIPS_INSERT_A		25
#define R_MIPS_INSERT_B		26
#define R_MIPS_DELETE		27
#define R_MIPS_HIGHER		28
#define R_MIPS_HIGHEST		29
/*
 * The following two relocation types are specified in the MIPS ABI
 * conformance guide version 1.2 but not yet in the psABI.
 */
#define R_MIPS_CALLHI16		30
#define R_MIPS_CALLLO16		31
/*
 * This range is reserved for vendor specific relocations.
 */
#define R_MIPS_LOVENDOR		100
#define R_MIPS_HIVENDOR		127

#ifndef _ASSEMBLER_

enum
{
    LOAD_EXE,
    LOAD_MODULE,
    LOAD_DL,
};

#define EI_NIDENT	16

typedef struct elf32_hdr
{
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry; /* Entry point */
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Ehdr;

typedef struct elf32_phdr
{
    Elf32_Word p_type;
    Elf32_Off p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
} Elf32_Phdr;

typedef struct
{
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_Shdr;

typedef struct elf32_rel
{
    Elf32_Addr r_offset;
    Elf32_Word r_info;
} Elf32_Rel;

typedef struct elf32_rela
{
    Elf32_Addr r_offset;
    Elf32_Word r_info;
    Elf32_Sword r_addend;
} Elf32_Rela;

typedef struct elf32_sym
{
    Elf32_Word st_name;
    Elf32_Addr st_value;
    Elf32_Word st_size;
    unsigned char st_info;
    unsigned char st_other;
    Elf32_Half st_shndx;
} Elf32_Sym;
extern int load_elf_binary(char *elf_name, unsigned char type, void *arg);
extern char fileExist(char *filename);

#endif /*_ASSEMBLER_*/
#endif/*__ELF_H__*/
