#! /bin/bash

cp -p ./$1 ./$2 -r
echo "\$1 is $1, \$2 is $2"
files_name=`find ./$2  -name '*.[c|h]' -type f -print`
files_name+=' '
files_name+=$(find ./$2 -name '*.dia' -type f -print)
files_name+=' '
files_name+=$(find ./$2 -name '*.jpeg' -type f -print)
files_name+=' '
files_name+=$(find ./$2 -name '*.txt' -type f -print)

echo $files_name
#echo ""

for file in $files_name ;do
    mv $file `echo $file|sed s/$1/$2/g`
done

files_name=$(find ./$2 -name '*.[c|h]' -type f -print)
files_name+=' '
files_name+=$(find ./$2 -name 'Makefile' -type f -print)

echo $files_name

for file in $files_name ;do
    cp $file "$file.$$"
    sed -e  s/BKC_MOD/`echo $2 | tr "[:lower:]" "[:upper:]"`/g -e s/bkc_mod/`echo $2 | tr "[:upper:]" "[:lower:]"`/g $file >"$file.$$"
    rm $file -rf
    mv "$file.$$" $file
done

rm -rf ../../../src/$2
cp -r ./$2  ../../../src/
rm -rf ./$2

echo "$2 is in bkc/src, congratulatins!!!"
