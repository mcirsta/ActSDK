
Processor = mips

ROOT      = ./../../../..
CASEROOT  = $(ROOT)/case
SDKROOT   = $(ROOT)/sdk
LIBROOT   = $(SDKROOT)/lib
CASELIBDIR = $(CASEROOT)/lib
IMGDIR    = $(CASEROOT)/images/$(DIRNAME)
ARCHROOT  = $(SDKROOT)/kernel/$(Processor)
DRVROOT   = $(SDKROOT)/drivers
			
SRCDIR    = .
SDKINCDIR    = $(SDKROOT)/include
CASEINCDIR   = $(CASEROOT)/include
BINDIR    = $(SDKROOT)/bin
LINKDIR   = .
EXENAME = fm_standby


INCLUDE   = -I$(SDKINCDIR) \
			-I$(CASEINCDIR) \
			-I$(SRCDIR)
LINCLUDE = -L$(CASEROOT)/include

include $(SDKROOT)/include/SDKConfig		

all: i2c.c fm_standby.c
	sde-gcc $(INCLUDE) -O0  -G0 -fsigned-char -mtune=4kec -mips32r2 -EL  -c i2c.c  -o i2c.o
	sde-gcc $(INCLUDE) -O0  -G0  -fsigned-char -mtune=4kec -mips32r2 -EL  -c fm_standby.c  -o fm_standby.o
	
	sde-ld -G0 $(LINCLUDE) -T fm_standby.xn  -EL i2c.o fm_standby.o $(LIBROOT)/$(DIRNAME)/app_lib.a $(LIBROOT)/os_api.a -o  $(EXENAME).exe

	sde-objdump -D $(EXENAME).exe > $(EXENAME).lst
	sde-objcopy -O binary $(EXENAME).exe $(EXENAME).bin


	@echo ----------------------------------------
	@echo ¹§Ï²Äú,±àÒë³É¹¦£¡£¨Ð¡¶Ë£©

	cp $(EXENAME).bin $(IMGDIR)
	rm *.o *.exe *.bin
clean:
	rm    *.o  *.exe *.lst *.bin

