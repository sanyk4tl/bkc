#! /bin/bash

#############################
#paths for this scripts
#
gdg_cm_cur_path=`pwd`
gdg_cm_topdir=`cd ../../../../; pwd`

cp -p $gdg_cm_cur_path/gdg_create_mod_res/$1 ./$2 -r
echo "\$1 is $1, \$2 is $2"
files_name=`find $gdg_cm_cur_path/$2  -name '*.[c|h]' -type f -print`
files_name+=' '
files_name+=$(find $gdg_cm_cur_path/$2 -name '*.dia' -type f -print)
files_name+=' '
files_name+=$(find $gdg_cm_cur_path/$2 -name '*.jpeg' -type f -print)
files_name+=' '
files_name+=$(find $gdg_cm_cur_path/$2 -name '*.jpg' -type f -print)
files_name+=' '
files_name+=$(find $gdg_cm_cur_path/$2 -name '*.txt' -type f -print)

echo $files_name
#echo ""

for file in $files_name ;do
    mv $file `echo $file|sed s/$1/$2/g`
done

files_name=$(find $gdg_cm_cur_path/$2 -name '*.[c|h]' -type f -print)
files_name+=' '
files_name+=$(find $gdg_cm_cur_path/$2 -name 'configure' -type f -print)

echo $files_name

for file in $files_name ;do
    cp $file "$file.$$"
    sed -e  s/`echo $1 | tr "[:lower:]" "[:upper:]"`/`echo $2 | tr "[:lower:]" "[:upper:]"`/g \
        -e s/`echo $1 | tr "[:upper:]" "[:lower:]"`/`echo $2 | tr "[:upper:]" "[:lower:]"`/g $file >"$file.$$"
    rm $file -rf
    mv "$file.$$" $file
done

rm -rf $gdg_cm_topdir/src/$2
cp -r $gdg_cm_cur_path/$2  $gdg_cm_topdir/src
rm -rf $gdg_cm_cur_path/$2

echo "$2 is in bkc/src, congratulatins!!!"
