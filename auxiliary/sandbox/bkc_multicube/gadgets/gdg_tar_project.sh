#! /bin/bash
#############################
#create the tarball of the 
#bkc project
#

#############################
#paths for this scripts
#
gdg_cm_cur_path=`pwd`
gdg_cm_topdir=`cd ../../../../; pwd`
gdg_cm_outdir=`cd ../../../../../; pwd`

echo $gdg_cm_outdir
cd $gdg_cm_outdir; cp ./trunk ./bkc -R;find ./bkc -name "*.svn" | xargs rm -rf; \
find ./bkc -name "*~" | xargs rm -rf;\
tar czvf  bkc_$1_$2.tar.gz ./bkc; rm -rf ./bkc; cp ./bkc_$1_$2.tar.gz $gdg_cm_cur_path/
cd $gdg_cm_cur_path

echo "bkc_$1_$2.tar.gz created!"
