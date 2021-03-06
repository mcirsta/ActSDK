#**************************************************************************
#                                                                         *
#   PROJECT     : MIPS port for UCOS-II                                   *
#                                                                         *
#   MODULE      : MakeFile                                                *
#                                                                         *
#   AUTHOR      : Wurui                                                   *
#                                                                         *
#   PROCESSOR   : MIPS 4KEc (32 bit RISC) - Actions board                 *
#                                                                         *
#   Tool-chain  :  Cygnus                                                 *
#                                                                         *
#   DESCRIPTION :                                                         *
#   Makefile used for building application.                               *
#                                                                         *
#   targets are :                                                         *
#                                                                         *
#*************************************************************************/
PRJROOT = ../../..
FWSPROOT  = $(PRJROOT)/fwsp
FWSP_SROOT	= $(PRJROOT)/fwsp_s
CASEROOT  = ../../case
PSP_RELROOT = ../

USERMODULEROOT = $(FWSPROOT)/usermodule
BOOT_SROOT  = $(FWSP_SROOT)/boot
BOOT_S_ADFU_ROOT = $(BOOT_SROOT)/adfu

DRVROOT   = $(FWSPROOT)/drivers
USBROOT   = $(DRVROOT)/usb

LIBSRCROOT	= $(FWSPROOT)/libsrc
LIBCROOT  = $(LIBSRCROOT)/libc

SDK_API_DIR=  $(PSP_RELROOT)/lib

FSROOT    = $(FWSPROOT)/fs/cfg
KERNELROOT = $(FWSPROOT)/kernel
MMMROOT	= $(USERMODULEROOT)/midware

BOOT_MFDIR = $(BOOT_S_ADFU_ROOT)/libsrc $(BOOT_S_ADFU_ROOT)/adfudec $(BOOT_S_ADFU_ROOT)/adfuserver \
            $(BOOT_SROOT)/fwsc \
            $(BOOT_SROOT)/hwsc \
            $(FWSPROOT)/boot/brec \
            $(FWSPROOT)/boot/mbrc $(FWSPROOT)/boot/mbrc_lba $(FWSPROOT)/boot/mbrc_spi
                        
KO_MFDIR =  $(FWSP_SROOT)/drivers/flash $(FWSP_SROOT)/drivers/flash_lba $(FWSP_SROOT)/drivers/spi\
            $(USBROOT)/usblib $(USBROOT)/monitor $(USBROOT)/msc $(USBROOT)/udc $(USBROOT)/uvc $(USBROOT)/udc4uvc $(USBROOT)/mtp $(USBROOT)/upgrade \
            $(USBROOT)/stor $(USBROOT)/ucore $(USBROOT)/uhc\
			$(FWSP_SROOT)/drivers/stub/cfg \
            $(DRVROOT)/card \
            $(DRVROOT)/tvout \
            $(DRVROOT)/framebuffer \
            $(DRVROOT)/pm \
            $(DRVROOT)/vram \
            $(DRVROOT)/ada \
            $(DRVROOT)/charge \
            $(DRVROOT)/fm \
            $(DRVROOT)/key \
            $(DRVROOT)/lcd \
            $(DRVROOT)/shutoff \
            $(DRVROOT)/startup \
            $(DRVROOT)/touchscreen \
            $(DRVROOT)/video_in \
            $(DRVROOT)/watchdog \
            $(DRVROOT)/gsensor \
            $(DRVROOT)/rtc \
            $(DRVROOT)/alarm \
            $(DRVROOT)/sysinfo \
            $(DRVROOT)/uview \

PMM_MFDIR = $(USERMODULEROOT)/gui/cfg \
			$(USERMODULEROOT)/imengine \
            $(USERMODULEROOT)/sqlite \
            $(USERMODULEROOT)/id3parse \
            $(USERMODULEROOT)/medianavi \
            $(USERMODULEROOT)/PlayList \
            $(USERMODULEROOT)/Multilanguage \
            $(USERMODULEROOT)/init_process \
            $(USERMODULEROOT)/apconfig \
            
MMM_MFDIR = $(MMMROOT)/act_mem \
            $(MMMROOT)/mmm_audio \
            $(MMMROOT)/ad_dsp \
            $(MMMROOT)/mmm_video \
            $(MMMROOT)/mmm_camera \
            $(MMMROOT)/mmm_record \
		                    
OS_MFDIR = $(LIBSRCROOT)/libprof \
            $(LIBSRCROOT)/libsemihosting \
            $(LIBSRCROOT)/libutest \
            $(LIBSRCROOT)/libcom \
            $(KERNELROOT)\
            $(FSROOT) \
            $(LIBSRCROOT)/libcrt \
            $(LIBCROOT)/sysindep \
            $(LIBCROOT)/general/cfg \
            $(LIBCROOT)/fs/cfg \

MFDIR = $(BOOT_MFDIR) $(KO_MFDIR) $(OS_MFDIR) $(MMM_MFDIR)  $(PMM_MFDIR)
	
SDKDRVDIR := $(FWSPROOT) $(FWSP_SROOT)
SDK_API_FILES := $(shell find $(SDKDRVDIR) -name '*api.S' )
# **********************************************
# Rules
# **********************************************

.PHONY : all
all:
	@echo make nothing,please choose object to make for example :
	
	@echo make sdk_api    //create xxx.a in skd/lib/
	@echo make boot    //����boot
	@echo make os    	//����os
	@echo make ko    	//����sdk\driversĿ¼�������ں�ģ��	
	@echo make pmm  //����pmm	
	@echo make mmm  //����mmm 
	@echo make clean      //clean
	
.PHONY : sdk_api boot os ko pmm mmm clean
boot:
	@$(foreach dir, $(BOOT_MFDIR), \
		echo Make -C $(dir);\
		if [ -d $(dir) ];then \
			if [ -f $(dir)/Makefile ];then \
				$(MAKE) -C $(dir);\
			else \
				echo error :Can not find Makefile in $(dir);\
			fi;\
		else \
			echo error :Can not find directory $(dir);\
		fi;\
		echo end;\
		echo;\
	)

ko:
	@$(foreach dir, $(KO_MFDIR), \
		echo Make -C $(dir);\
		if [ -d $(dir) ];then \
			if [ -f $(dir)/Makefile ];then \
				$(MAKE) -C $(dir);\
			else \
				echo error :Can not find Makefile in $(dir);\
			fi;\
		else \
			echo error :Can not find directory $(dir);\
		fi;\
		echo end;\
		echo;\
	)

os:
	@$(foreach dir, $(OS_MFDIR), \
		echo Make -C $(dir);\
		if [ -d $(dir) ];then \
			if [ -f $(dir)/Makefile ];then \
				$(MAKE) -C $(dir);\
			else \
				echo error :Can not find Makefile in $(dir);\
			fi;\
		else \
			echo error :Can not find directory $(dir);\
		fi;\
		echo end;\
		echo;\
	)

pmm:
	@$(foreach dir, $(PMM_MFDIR), \
		echo Make -C $(dir);\
		if [ -d $(dir) ];then \
			if [ -f $(dir)/Makefile ];then \
				$(MAKE) -C $(dir);\
			else \
				echo error :Can not find Makefile in $(dir);\
			fi;\
		else \
			echo error :Can not find directory $(dir);\
		fi;\
		echo end;\
		echo;\
	)
	
mmm:
	make -C $(MMMROOT)/act_mem
	make -C $(MMMROOT)/mmm_audio
	make -C $(MMMROOT)/mmm_record
	make -C $(MMMROOT)/ad_dsp
	make -C $(MMMROOT)/mmm_video
	make dc -C $(MMMROOT)/mmm_camera
	make dv -C $(MMMROOT)/mmm_camera
	@echo ��ϲ�����ɹ�

# Usage: ./create_lib.sh  idir  [odir]  [sdkdir]
# idir: input api file name with path, such as usdk130/dev/sdk/kernel/mips/os_api.S
# odir: output dir for api.a, such as usdk130/sdk/lib
# sdkdir: dir for sdk locate in, such as usdk130/

sdk_api:
	for api_file in $(SDK_API_FILES); do $(CASEROOT)/makefile/create_api.sh  $$api_file  $(SDK_API_DIR) $(ROOT)../ ; done

clean :
	@$(foreach dir, $(MFDIR) , \
		echo Make clean -C $(dir);\
		if [ -d $(dir) ];then \
			if [ -f $(dir)/Makefile ];then \
				$(MAKE) clean -C $(dir);\
			else \
				echo error :Can not find Makefile in $(dir);\
			fi;\
		else \
			echo error :Can not find directory $(dir);\
		fi;\
		echo end;\
		echo;\
	)

