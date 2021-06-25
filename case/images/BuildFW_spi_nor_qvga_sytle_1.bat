@echo off
:::::::::::::::::::::::::::::::::::::::::::::::::::::
:: 参数1表示该脚本的调用方法 如%1=DBT表示是DBT调用该脚本
:: 参数2表示生成的固件 目前只有当%1为DBT时%2才有意义
:: 参数3表示Make生成的报告信息
:: 参数4表示该脚本的返回值 运行成功返回1 运行失败返回0
:::::::::::::::::::::::::::::::::::::::::::::::::::::

rem ----------------------------------------
set callfrom=%1
set firmware=%2
set msgfile=%3
set retval=0

set cur_disk=%~d0
set current_dir=%cur_disk%\%~p0

cd %current_dir%

cp -f base2312.hzt  basic.hzt
cp -f py2312.pyt    pinyin.pyt
cp -f config_spi_nor_qvga_style_1.txt config.txt
cp -f config_spi_nor.spc config.spc
cp -f .\misc\lang_spi_nor.desktop .\misc\lang.desktop

mv -f .\apps\welcome.bin .\apps\welcome_normal.bin
mv -f .\drv\lcd.ko .\drv\lcd_normal.ko
mv -f .\drv\fm.ko .\drv\fm_normal.ko
mv -f .\drv\gsensor.ko .\drv\gsensor_normal.ko
mv -f .\drv\video_in.ko .\drv\video_in_normal.ko
mv -f .\mullang.xml .\mullang_normal.xml

cp -f .\apps\welcome_spi_nor.bin .\apps\welcome.bin
cp -f .\drv\lcd_spi_nor.ko .\drv\lcd.ko
cp -f .\drv\fm_spi_nor.ko .\drv\fm.ko
cp -f .\drv\gsensor_spi_nor.ko .\drv\gsensor.ko
cp -f .\drv\video_in_spi_nor.ko .\drv\video_in.ko
cp -f .\mullang_spi_nor.xml  .\mullang.xml

cp -f ..\..\psp_rel\bin\sysinfo_spi_nor.bin ..\..\psp_rel\bin\sysinfo.bin


::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: make the image
::::::::::::::::::::::::::::::::::::::::::::::::::::::::
if "%callfrom%"=="" (
    rem 双击运行
    ..\..\..\tools\fw_maker\FIMaker.exe fwimage_spi_nor_qvga_style_1.cfg
    ..\..\..\tools\adfu\BatchProduct.exe
    set retval=1
    goto make_finish
)

if not %callfrom%==DBT (
    set retval=0
    goto make_finish
)

rem now callfrom must be DBT
if "%firmware%"=="" (
    rem firmware path must exist
    set retval=0
    goto make_finish
)
if "%msgfile%"=="" (
    ..\..\..\tools\fw_maker\Maker.exe -c fwimage_spi_nor_qvga_style_1.cfg -o %firmware%
    set retval=1
    goto make_finish
)
..\..\..\tools\fw_maker\Maker.exe -c fwimage_spi_nor_qvga_style_1.cfg -o %firmware% >%msgfile% 2>&1
set retval=1
:make_finish

mv -f .\apps\welcome_normal.bin .\apps\welcome.bin
mv -f .\drv\lcd_normal.ko .\drv\lcd.ko
mv -f .\drv\fm_normal.ko .\drv\fm.ko
mv -f .\drv\gsensor_normal.ko .\drv\gsensor.ko
mv -f .\drv\video_in_normal.ko .\drv\video_in.ko
mv -f .\mullang_normal.xml .\mullang.xml

if not %callfrom%==DBT (
    ..\..\..\tools\adfu\BatchProduct.exe
)

if not "%4"=="" (
    set %4=%retval%
)
goto :eof
