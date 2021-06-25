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
