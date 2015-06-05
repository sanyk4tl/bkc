#! /bin/sh

########################################
#the path of the current script
#
cmpf_curpath=`pwd`
cmpf_bkc_topdir=`cd ../../../../; pwd`
echo $cmpf_bkc_topdir
echo $cmpf_curpath

. $cmpf_curpath/gdg_cp_mf.shcfg

########################################
#setting the vars properly
#according to the command-line arguments
#

ac_opt=
ac_arg=
while [ $# -gt 0 ]; do
    ac_opt=$1
    ac_arg=`expr "$ac_opt" : '[^=]*=\s*\(.*\)'`

    case $ac_opt in
    ENV_BKC_MK_MODNAME*)
        env_bkc_mk_modname=$ac_arg
        ;;
    ENV_BKC_MK_SUBMOD_LIST*)
        env_bkc_mk_submod_list=$ac_arg
        ;;
    ENV_BKC_MK_NBMOD_LIST*)
        env_bkc_mk_nbmod_list=$ac_arg
        ;;
    ENV_BKC_MK_MOD_LOCATION*)
        env_bkc_mk_mod_location=$ac_arg
        ;;
    ENV_BKC_MK_EP*)
        env_bkc_mk_ep=$ac_arg
        ;;
    ENV_BKC_MK_SP*)
        env_bkc_mk_sp=$ac_arg
        ;;
    ENV_CPMF_MK_DESTPATH*)
        env_cpmf_mk_destpath=$ac_arg
        ;;
    -help)
    echo "
        usage: just like\n
        ./gdg_cp_mf.sh ENV_BKC_MK_MODNAME=bkc_dlist \
        ENV_BKC_MK_SUBMOD_LIST=# ENV_BKC_MK_NBMOD_LIST=# \
        ENV_BKC_MK_MOD_LOCATION=bkc_mk_location_embeded \
        ENV_BKC_MK_EP=BKC_MK_MOD_TOPDIR/../../include \
        ENV_BKC_MK_SP=BKC_MK_MOD_TOPDIR/../../build \
        ENV_CPMF_MK_DESTPATH=BKC_TOPDIR/src/bkc_dlist/build"

        exit 1
        ;;
    --)
        #By convention, --ends options
        shift
        break
        ;;
    *)
        echo $0: $ac_opt: unrecognized option >&2
        ;;
    esac

    shift
done

#replace the bkc_topdir to the absolute path
env_cpmf_mk_destpath=`echo "$env_cpmf_mk_destpath"|sed "s BKC_TOPDIR $cmpf_bkc_topdir g"`

########################################
#print the user input
#
echo "env_bkc_mk_modname=$env_bkc_mk_modname"
echo "env_bkc_mk_submod_list=$env_bkc_mk_submod_list"
echo "env_bkc_mk_nbmod_list=$env_bkc_mk_nbmod_list"
echo "env_bkc_mk_mod_location=$env_bkc_mk_mod_location"
echo "env_bkc_mk_ep=$env_bkc_mk_ep"
echo "env_bkc_mk_sp=$env_bkc_mk_sp"
echo "env_cpmf_mk_destpath=$env_cpmf_mk_destpath"

########################################
#modify the template makefile
#according to the command-line arguments
#

#back up the original Makefile and configure
cp -fv $env_cpmf_mk_destpath/Makefile $env_cpmf_mk_destpath/Makefile_ori
cp -fv $env_cpmf_mk_destpath/configure $env_cpmf_mk_destpath/configure_ori
cp -fv $env_cpmf_mk_destpath/pre_compile_process \
    $env_cpmf_mk_destpath/pre_compile_process_ori
#get the new working copy
cp -fv $cmpf_curpath/gdg_cp_mf_res/configure_template $cmpf_curpath/configure_template_tmp
cp -fv $cmpf_curpath/gdg_cp_mf_res/Makefile_template $cmpf_curpath/Makefile_template_tmp
cp -fv $cmpf_curpath/gdg_cp_mf_res/pre_compile_process_template \
    $cmpf_curpath/pre_compile_process_template_tmp

#replacing
#cp $cmpf_curpath/configure_template_tmp $cmpf_curpath/configure_template_tbmodified
sed -e "s/ENV_BKC_MK_MODNAME=.*/ENV_BKC_MK_MODNAME=$env_bkc_mk_modname/g" \
    -e "s/ENV_BKC_MK_SUBMOD_LIST=.*\#/ENV_BKC_MK_SUBMOD_LIST=$env_bkc_mk_submod_list/g" \
    -e "s/ENV_BKC_MK_NBMOD_LIST=.*\#/ENV_BKC_MK_NBMOD_LIST=$env_bkc_mk_nbmod_list/g" \
    -e "s/ENV_BKC_MK_MOD_LOCATION=.*/ENV_BKC_MK_MOD_LOCATION=$env_bkc_mk_mod_location/g" \
    -e "s ENV_BKC_MK_EP=.* ENV_BKC_MK_EP=$env_bkc_mk_ep g" \
    -e "s ENV_BKC_MK_SP=.* ENV_BKC_MK_SP=$env_bkc_mk_sp g" \
    $cmpf_curpath/configure_template_tmp >$cmpf_curpath/configure_template_tmp_tbmodified

rm $cmpf_curpath/configure_template_tmp -rf
#redundant mv, just be convenient for the last mv
mv $cmpf_curpath/configure_template_tmp_tbmodified $cmpf_curpath/configure_template_tmp

#moving to the destination
chmod +x $cmpf_curpath/configure_template_tmp
mv $cmpf_curpath/configure_template_tmp  $env_cpmf_mk_destpath/configure
mv $cmpf_curpath/Makefile_template_tmp  $env_cpmf_mk_destpath/Makefile
mv $cmpf_curpath/pre_compile_process_template_tmp  $env_cpmf_mk_destpath/pre_compile_process
