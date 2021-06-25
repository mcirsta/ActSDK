#!/bin/bash
PRJROOT=""
NAME=""

while [ x"$PRJROOT" = x"" ]
do
if ls -d */ | grep "usdk*" >/dev/null;then
NAME=$(ls -d usdk*)
PRJROOT=$(pwd)/$NAME
echo $PRJROOT
else
cd ../
fi
done

# PRJROOT=$(while [ x"$IPRJROOT" = x"" ]; \
		# do if ls -d */ | grep "usdk*" >/dev/null;then \
		# NAME=$(ls -d usdk*) && IPRJROOT=$(pwd)/$NAME && echo $IPRJROOT;  \
		# else \
		# cd ..; \
		# fi \
		# done)
#echo $PRJROOT
# export PRJROOT

# IS_LCD=$(while [ -d "test" ];do printf "no test" >/dev/console;done)
# echo $IS_LCD

