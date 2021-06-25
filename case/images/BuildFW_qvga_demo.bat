@echo off
:::::::::::::::::::::::::::::::::::::::::::::::::::::
:: ����1��ʾ�ýű��ĵ��÷��� ��%1=DBT��ʾ��DBT���øýű�
:: ����2��ʾ���ɵĹ̼� Ŀǰֻ�е�%1ΪDBTʱ%2��������
:: ����3��ʾMake���ɵı�����Ϣ
:: ����4��ʾ�ýű��ķ���ֵ ���гɹ�����1 ����ʧ�ܷ���0
:::::::::::::::::::::::::::::::::::::::::::::::::::::

rem ----------------------------------------
set callfrom=%1
set firmware=%2
set msgfile=%3
set retval=0

set cur_disk=%~d0
set current_dir=%cur_disk%\%~p0

cd %current_dir%

cp -f basegbk.hzt  basic.hzt
cp -f pygbk.pyt    pinyin.pyt
cp -f config_qvga_demo.txt config.txt
cp -f .\misc\lang_qvga_demo.desktop .\misc\lang.desktop
cp -f ..\..\psp_rel\bin\sysinfo_normal.bin ..\..\psp_rel\bin\sysinfo.bin

::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: make the image
::::::::::::::::::::::::::::::::::::::::::::::::::::::::
if "%callfrom%"=="" (
    rem ˫������
    ..\..\..\tools\fw_maker\FIMaker.exe fwimage_qvga_demo.cfg
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
    ..\..\..\tools\fw_maker\Maker.exe -c fwimage_qvga_demo.cfg -o %firmware%
    set retval=1
    goto make_finish
)
..\..\..\tools\fw_maker\Maker.exe -c fwimage_qvga_demo.cfg -o %firmware% >%msgfile% 2>&1
set retval=1
:make_finish

if not "%4"=="" (
    set %4=%retval%
)
goto :eof
