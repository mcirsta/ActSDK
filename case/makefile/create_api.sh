#!/bin/bash
#go to dir: sdk/drivers/vram 
#./create_lib.sh input_file  out_put_dir  sdk_root
#set -o xtrace
ROOT=$3
input_file=$1
out_dir=$2






cur_dir=$(pwd)

if [ -z $input_file ]; then
    echo 'usage: ./create_lib.sh input_file  [Output Directory] [Project Root Directory]'
    echo 'example: ../../../case/makefile/create_api.sh card_api.S ../../lib ../../../../usdk130_flash'
    echo 'param 2 3 is optional, default value: current directory'
    exit
elif [ $input_file = ${input_file#/} ]; then
    input_file=$cur_dir/$input_file
fi

if [ -z $ROOT ]; then
    ROOT=$cur_dir
elif [ $ROOT = ${ROOT#/} ]; then
    ROOT=$cur_dir/$ROOT
fi

if [ -z $out_dir ]; then
    out_dir=$cur_dir
elif [ $out_dir = ${out_dir#/} ]; then
    out_dir=$cur_dir/$out_dir
fi

 echo 
 echo 
 echo 
 echo ===========================================================================
 echo $input_file 
 echo $out_dir 
 echo $ROOT


OFORMAT=-EL
CASEROOT=$ROOT/case
TASKROOT=$CASEROOT/apps
SDKROOT=$ROOT
INCLUDE="-I$SDKROOT/include -I$CASEROOT/include -I$TASKROOT/common -I$SDKROOT/inc"
CC_OPTS="-O0 -G0 $INCLUDE -mips32r2 -fno-builtin $OFORMAT -DDEBUG"
CC_OPTS_A="$CC_OPTS -D_ASSEMBLER_"

stem_file=${input_file%%.S}
stem_file=${stem_file##*/}

create_simple_region()
{

    local input_file=$1
    local head_file=$2
    local tail_file=$3


    local line1 line2 leafs ends out_file
    leafs=$(grep -n -e '^LEAF.*(.*).*$' $input_file|cut -d: -f1)
    ends=$(grep -n -e '^END.*(.*).*$' $input_file|cut -d: -f1)
    for i in $leafs; do
        line1=${leafs%%
*}
        leafs=${leafs#*
}
        line2=${ends%%
*}
        ends=${ends#*
}
        out_file=$stem_file$((region1+line1)).S
        cat $head_file > $out_file
        ed -s $input_file  >> $out_file <<EOF
 $line1,${line2}p
EOF

        cat $tail_file >> $out_file
        asm_file_num=asm_file_num+1

    done
}

create_asm()
{
    local in_file=$1
    local head_file=$4
    local start_line
    local end_line
    declare -i start_line
    declare -i end_line
    local elseif
    local tmp_file=tmp_file
    local tmp_file1=tmp_file1
    local tmp_head_file=tmp_head_file
    local tmp_tail_file=tmp_tail_file
    local ifdef
    local endif
    local tmp_file2
    local tmp_head_file1
    
    start_line=$2
    region1=$start_line
    end_line=$3
    region2=$end_line
    ed -s $in_file > $tmp_file <<EOF
 $start_line,${end_line}p
EOF

    ifdef=$(grep -n -e '^[\t ]*#ifdef.*$' $tmp_file|cut -d: -f1)
    if [ -z $ifdef ]; then 
        echo > $tmp_tail_file
        create_simple_region $tmp_file $head_file $tmp_tail_file
    else
        endif=$(grep -n -e '^[\t ]*#endif.*$' $tmp_file|cut -d: -f1)
        cat $head_file > $tmp_head_file
        ed -s $tmp_file >> $tmp_head_file <<EOF
 $ifdef,${ifdef}p
EOF
        ed -s $tmp_file > $tmp_tail_file<<EOF
 $endif,${endif}p
EOF

#remove ifdef endif
    ed -s $in_file > $tmp_file <<EOF
 $start_line+1,${end_line}-1p
EOF
        elseif=$(grep -n -e '^[\t ]*#el.*$' $tmp_file|cut -d: -f1)
        if [ -z $elseif ];then
            create_simple_region $tmp_file $tmp_head_file $tmp_tail_file
        else
            ed -s $tmp_file > $tmp_file1 <<EOF
 $ifdef,${elseif}-1p
EOF
            create_simple_region $tmp_file1 $tmp_head_file $tmp_tail_file
            ed -s $tmp_file > $tmp_file2 <<EOF
 ${elseif}+1,${endif}p
EOF
            ed -s $tmp_head_file > $tmp_head_file1 <<EOF
 $elseif,${elseif}p
EOF
            create_simple_region $tmp_file2 $tmp_head_file1 $tmp_tail_file
        fi
    fi
}





tmp_line=$(grep -n -e '^LEAF.*(.*).*$' $input_file|cut -d: -f1)
first_leaf=${tmp_line%%
*}

ifdef=$(grep -n -e '^[\t ]*#ifdef.*$' $input_file|cut -d: -f1)
endif=$(grep -n -e '^[\t ]*#endif.*$' $input_file|cut -d: -f1)


declare -i index asm_file_num
declare -i i1 i2
let asm_file_num=0



tmpdir=tmp_$stem_file
mkdir -p ${tmpdir}
cd ${tmpdir}
#rm *

head_file=${stem_file}.head


let i1=first_leaf
tmp=${ifdef%%
*}

if [ -z $tmp ]; then
    let i2=0
else
    let i2=${ifdef%%
*}
fi

#create header & firset sector before ifdef

if [ $i2 -eq 0 ]; then
    ed -s $input_file > $head_file <<EOF
 1,$first_leaf-1p
EOF
elif [ $i1 -gt $i2 ];then
    ed -s $input_file > $head_file <<EOF
 1,$i2-1p
EOF
else
    ed -s $input_file > $head_file <<EOF
 1,$first_leaf-1p
EOF
    create_asm $input_file $first_leaf $((i2-1)) $head_file
fi


line2=$first_leaf
for i in $ifdef; do
    line1=${ifdef%%
*}
    ifdef=${ifdef#*
}
    line2=${endif%%
*}
    endif=${endif#*
}
    create_asm $input_file $line1 $line2 $head_file
done

last_line=$(wc -l $input_file)
last_line=${last_line%%/*}
create_asm $input_file $line2 $last_line $head_file

echo "create asm files finished tota api number  ****  $asm_file_num  ****"
echo 
echo 

asm_files=$(ls *.S)
echo sde-gcc  $CC_OPTS_A -c $asm_files
sde-gcc  $CC_OPTS_A -c $asm_files

obj_files=$(ls *.o)
echo rm $out_dir/${stem_file}.a
rm $out_dir/${stem_file}.a
echo sde-ar rcs $out_dir/${stem_file}.a $obj_files
sde-ar rcs $out_dir/${stem_file}.a $obj_files

cd $cur_dir
rm -fr $tmpdir




