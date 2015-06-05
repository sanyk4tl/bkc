#! /bin/sh

########################################
#the path of the current script
#
cmpfa_curpath=`pwd`
cmpfa_bkc_topdir=`cd ../../../../; pwd`
echo $cmpfa_bkc_topdir
echo $cmpfa_curpath

########################################
#the path of the current script
#
cmpfa_almodlist="bkc_dlist bkc_pasync bkc_queue bkc_socket bkc_store bkc_time \
    bkc_crc bkc_libc bkc_pthread bkc_regex bkc_stack bkc_string bkc_timer bkc_sandbox"

cmpfa_almodlist=`echo $cmpfa_almodlist | sort`
echo $cmpfa_almodlist

########################################
#replacing
#

#for sub modules

for sin_mod in $cmpfa_almodlist; do

    case $sin_mod in
    bkc_pthread*)
        ./gdg_cp_mf.sh ENV_BKC_MK_MODNAME=$sin_mod \
        ENV_BKC_MK_SUBMOD_LIST=\# ENV_BKC_MK_NBMOD_LIST=bkc_dlist\# \
        ENV_BKC_MK_MOD_LOCATION=bkc_mk_location_embeded ENV_BKC_MK_EP=BKC_MK_MOD_TOPDIR/../../include \
        ENV_BKC_MK_SP=BKC_MK_MOD_TOPDIR/../../build ENV_CPMF_MK_DESTPATH=BKC_TOPDIR/src/$sin_mod/build
        ;;
    *)
        ./gdg_cp_mf.sh ENV_BKC_MK_MODNAME=$sin_mod \
        ENV_BKC_MK_SUBMOD_LIST=\# ENV_BKC_MK_NBMOD_LIST=\# \
        ENV_BKC_MK_MOD_LOCATION=bkc_mk_location_embeded ENV_BKC_MK_EP=BKC_MK_MOD_TOPDIR/../../include \
        ENV_BKC_MK_SP=BKC_MK_MOD_TOPDIR/../../build ENV_CPMF_MK_DESTPATH=BKC_TOPDIR/src/$sin_mod/build
        ;;
    esac

done

#for top module
./gdg_cp_mf.sh ENV_BKC_MK_MODNAME=bkc ENV_BKC_MK_SUBMOD_LIST="$cmpfa_almodlist" ENV_BKC_MK_NBMOD_LIST=\# \
ENV_BKC_MK_MOD_LOCATION=bkc_mk_location_toplevel ENV_BKC_MK_EP=BKC_MK_MOD_TOPDIR/include \
ENV_BKC_MK_SP=BKC_MK_MOD_TOPDIR/build ENV_CPMF_MK_DESTPATH=BKC_TOPDIR/build

