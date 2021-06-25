#!/bin/bash
#param1:alsp sub-dir name
#param2:test or build or qam

param1=$1
param2=$2

if [ $param1 = 1 ]; then
	ALSP_IC="213x"
elif [ $param1 = 2 ]; then
	ALSP_IC="215x"
fi

if [ $param2 = build ]; then
	COMMAND_CHECKOUT="cleartool checkout -nc %CLEARCASE_PN%"
	COMMAND_CHECKIN="cleartool checkin -c "int_$date" -ide   %CLEARCASE_PN%"
else
	COMMAND_CHECKOUT="chmod 744"
	COMMAND_CHECKIN="chmod 444"
fi

echo $ALSP_IC
echo $COMMAND_CHECKOUT
echo $COMMAND_CHECKIN

cd ../../
SDKROOT=$(pwd)

#################################################
#						 						#
#   checkout header files in fwsp/include		#
#						 						#
#################################################
find ./psp_rel/include -name '*.h' -or -name '*.xn' | xargs $COMMAND_CHECKOUT
find ./psp_rel/lib -name '*.a' | xargs $COMMAND_CHECKOUT

cd ../
PRJROOT=$(pwd)


echo $SDKROOT
echo $PRJROOT

#################################################
#						 						#
#      		 dos2unix the reservefiles		 		#
#						 						#
#################################################
dos2unix $SDKROOT/psp_rel/bin/reservefiles
dos2unix $SDKROOT/psp_rel/lib/reservefiles
dos2unix $SDKROOT/case/images/reservefiles
dos2unix $SDKROOT/case/lib/reservefiles
dos2unix $SDKROOT/case/makefile/create_api.sh

#################################################
#						 						#
#   checkout binary files						#
#						 						#
#################################################
cd $SDKROOT
find . -name '*.a' -or -name '*.o' -or -name '*.ko' -or -name '*.so' -or -name '*.ao' | xargs $COMMAND_CHECKOUT
find . -name '*.app' -or -name '*.lib' -or -name '*.drv' -or -name '*.elf' -or -name '*.exe' | xargs $COMMAND_CHECKOUT
find . -name '*.bin' -or -name '*.sys' -or -name '*.spec' -or -name '*.lst' -or -name '*.map' | xargs $COMMAND_CHECKOUT

cd $PRJROOT/fwsp/include
find . -name '*.h' | xargs $COMMAND_CHECKOUT

#################################################
#						 						#
#   checkout opensource driver					#
#						 						#
#################################################
cd $SDKROOT/psp_rel/drivers/sample

find . -name '*.c' -or -name '*.h' -or -name '*.S' -or -name '*.xn' -or -name 'Makefile*' | xargs $COMMAND_CHECKOUT

cd $SDKROOT/case/makefile/
make clean

#################################################
#						 						#
#      		 clean case/lib		 	 			#
#						 						#
#################################################
echo "clean case/lib"
cd ../lib/

mkdir bin_tmp
for file in $(cat reservefiles)
do
    mv $file bin_tmp
done
rm -f *.a
cp ./bin_tmp/* .
rm -rf bin_tmp
#################################################
#						 						#
#      		 clean psp_rel/bin		 	 		#
#						 						#
#################################################
echo "clean sdk/bin"
cd $SDKROOT/psp_rel/bin

mkdir bin_tmp
for file in $(cat reservefiles)
do
    mv $file bin_tmp
done
rm -f *.drv
rm -f *.so
rm -f *.ko
rm -f *.bin
rm -f *.sys
rm -f *.exe
cp ./bin_tmp/* .
rm -rf bin_tmp

#################################################
#						 						#
#       	clean psp_rel/lib			 		#
#								 				#
#################################################
echo "clean sdk/lib"
cd ../lib
mkdir lib_tmp
for file in $(cat reservefiles)
do
    mv $file lib_tmp
done
rm -f *.a
rm -f *.o
cp ./lib_tmp/* .
rm -rf lib_tmp

#################################################
#						 						#
#       	clean psp_rel/lib/libmmm			#
#						 						#
#################################################
cd ./libmmm
mkdir libmmm_tmp
for file in $(cat reservefiles)
do
    mv $file libmmm_tmp
done
rm -f *.a
rm -f *.o   
rm -f *.so
cp ./libmmm_tmp/* .
rm -rf libmmm_tmp

cd ../

#################################################
#						 						#
#      	    clean all the *.o files		 		#
#						 						#
#################################################
#echo "clean all the *.o files"
#find ../../../ -name '*.o' -exec rm -f {} \;

#################################################
#						 						#
#      		 start make			 				#
#						 						#
#################################################
echo "Clean finished, type Enter to start make!"
#read

cd $SDKROOT/psp_rel/config/

make clean

if [[ $1 = "debug" ]]
then 
read -p "sdk_api"
fi
make sdk_api

if [[ $1 = "debug" ]]
then
read -p "boot"
fi
make boot

if [[ $1 = "debug" ]]
then
read -p "os"
fi
make os

#################################################
#						 						#
#      		 ensure alsp make before mmm		#
#						 						#
#################################################
cd $PRJROOT/alsp/$ALSP_IC
make clean
make

cd $SDKROOT/psp_rel/config/

if [[ $1 = "debug" ]]
then
read -p "ko"
fi
make ko

if [[ $1 = "debug" ]]
then
read -p "pmm"
fi
make pmm

if [[ $1 = "debug" ]]
then
read -p "mmm"
fi
make mmm

#################################################
#						 						#
#      		 start link	alsp		 			#
#						 						#
#################################################
#read
echo "Python gen_makefile.py Makefile_alsp $PRJROOT/alsp/$ALSP_IC/depend.cfg >Makefile"
Python gen_makefile.py Makefile_alsp $PRJROOT/alsp/$ALSP_IC/depend.cfg >Makefile_a
make -f Makefile_a


#################################################
#						 						#
# 		start copy header files					#
# 	form fwsp/include to psp_rel/include		#
#						 						#
#################################################
cp -rf $PRJROOT/fwsp/include $SDKROOT/psp_rel

#################################################
#						 						#
# 		start copy opensource driver			#
# 	form fwsp/drivers to psp_rel/drivers/sample #
#						 						#
#################################################
for file in $(cat $PRJROOT/fwsp/drivers/opensource.txt)
do
    cp -rf $PRJROOT/fwsp/drivers/$file $SDKROOT/psp_rel/drivers/sample
done

#################################################
#						 						#
#      		 start make	case		 			#
#						 						#
#################################################
cd $SDKROOT/case/makefile/
if [[ $1 = "debug" ]]
then 
read -p "case_api"
fi
make case_api

if [[ $1 = "debug" ]]
then
read -p "drv"
fi
make drv

if [[ $1 = "debug" ]]
then
read -p "libs"
fi
make libs

if [[ $1 = "debug" ]]
then
read -p "apps"
fi
make apps

cd $PRJROOT/fwsp_s/encrypt/
./BuildSpec.bat
./BuildSpec_spi_nor.bat
#################################################
#						 						#
#       	start strip			 				#
#						 						#
#################################################
echo "Finished make,start strip"
cd $SDKROOT
find . -name '*.so' | xargs -exec sde-strip -S
find . -name '*.exe' | xargs -exec sde-strip -S
find . -name '*.app' | xargs -exec sde-strip -S
find . -name '*.ko' | xargs -exec sde-strip -S
find . -name '*.a' | xargs -exec sde-strip -S

find . -name '*.so' | xargs -exec sde-strip -R .pdr -R .comment -R .symtab -R .strtab -R .reginfo
find . -name '*.exe' | xargs -exec sde-strip -R .pdr -R .comment -R .symtab -R .strtab -R .reginfo
find . -name '*.app' | xargs -exec sde-strip -R .pdr -R .comment -R .symtab -R .strtab -R .reginfo
find . -name '*.ko' | xargs -exec sde-strip -R .pdr -R .comment -R .symtab -R .strtab -R .reginfo

#################################################
#						 						#
#   checkin header files in fwsp/include		#
#						 						#
#################################################

cd $PRJROOT/fwsp/include
find . -name '*.h' | xargs $COMMAND_CHECKIN

cd $SDKROOT
#################################################
#						 						#
#   checkin header files in psp_rel/include		#
#						 						#
#################################################

find ./psp_rel/include -name '*.h' -or -name '*.xn' | xargs $COMMAND_CHECKIN

#################################################
#						 						#
#   checkin binary files						#
#						 						#
#################################################

find . -name '*.a' -or -name '*.o' -or -name '*.ko' -or -name '*.so' -or -name '*.ao' -or -name '*.app' -or -name '*.lib' -or -name '*.drv' -or -name '*.bin' -or -name '*.elf' -or -name '*.exe' -or -name '*.sys' -or -name '*.spec' -or -name '*.lst' -or -name '*.map' | xargs $COMMAND_CHECKIN

#################################################
#						 						#
#   checkin opensource driver					#
#						 						#
#################################################
cd $SDKROOT/psp_rel/drivers/sample

find . -name '*.c' -or -name '*.h' -or -name '*.S' -or -name '*.xn' -or -name 'Makefile*' | xargs $COMMAND_CHECKIN

if [ $param2 == build ]; then

#################################################
#						 						#
#   auto add private file to control			#
#						 						#
#################################################
for private_file in $(cleartool ls -view_only -r -short)
do
	echo $private_file
	private_file=$(echo $private_file|sed -e 's,\\,/,g')
	echo $private_file
    #cleartool mkelem -c "add" -mkpath -ci $private_file
	if [ ${private_file##*.} = c -o ${private_file##*.} = S -o ${private_file##*.} = h -o ${private_file##*.} = xn -o ${private_file##*/} = Makefile_cov -o ${private_file##*/} = Makefile ] ; then
    	cleartool mkelem -c "add" -mkpath -ci $private_file
    fi    
done

cd ../../include
for private_file in $(cleartool ls -view_only -r -short)
do
	echo $private_file
	private_file=$(echo $private_file|sed -e 's,\\,/,g')
	echo $private_file
    cleartool mkelem -c "add" -mkpath -ci $private_file
done

cd $PRJROOT/fwsp/include
for private_file in $(cleartool ls -view_only -r -short)
do
	echo $private_file
	private_file=$(echo $private_file|sed -e 's,\\,/,g')
	echo $private_file
    cleartool mkelem -c "add" -mkpath -ci $private_file
done
fi

if [ $param2 != qam ]; then
#################################################
#						 						#
#       	start buildimages		 			#
#						 						#
#################################################
echo "starting buildimage"

cd $SDKROOT/case/images/
#./BuildFW.bat
../../../tools/fw_maker/Maker.exe  -c fwimage_qvga.cfg -o 1.fwu
../../../tools/adfu/BatchProduct.exe -f 1.fwu
echo "Finished all, type Enter to finish!"
#read
fi
