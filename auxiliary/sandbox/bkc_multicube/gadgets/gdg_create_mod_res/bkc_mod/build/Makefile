##########################################################################
#module general settings
##########################################################################
#module top dir
bkc_mk_mtopdir = $(shell cd ../; pwd)

#module configure checking
$(shell ./configure)
-include $(bkc_mk_mtopdir)/build/bkc_env_settings.cfg
ifneq ($(ENV_BKC_MK_IS_CONFIGURE),yes)
    $(error ./configure need to be run first, please have a check)
endif

#module name
bkc_mk_mod_name = $(ENV_BKC_MK_MODNAME)
bkc_mk_modlib_sname = lib$(bkc_mk_mod_name).a
bkc_mk_modlib_dname = lib$(bkc_mk_mod_name).so
bkc_mk_output_slib = $(bkc_mk_modlib_sname)
bkc_mk_output_slib_fpath = $(bkc_mk_mtopdir)/out/$(bkc_mk_output_slib)

#sub module list
bkc_mk_submod_list = $(ENV_BKC_MK_SUBMOD_LIST)#
#neighbor module list
bkc_mk_nbmod_list = $(ENV_BKC_MK_NBMOD_LIST)#

#module location can be:
#bkc_mk_location_toplevel(for toplevel module, like the total bkc module)
#bkc_mk_location_embeded(for any inner module, like bkc_dlist)
bkc_mk_mod_location = $(ENV_BKC_MK_MOD_LOCATION)

#sub module full pathname list
bkc_mk_submod_fpname_list = $(foreach sin_mod, $(bkc_mk_submod_list), $(bkc_mk_mtopdir)/src/$(sin_mod)/out/lib$(sin_mod).a)
bkc_mk_submod_topdir_list = $(foreach sin_mod, $(bkc_mk_submod_list), $(bkc_mk_mtopdir)/src/$(sin_mod))
#virtual module full pathname list
bkc_mk_vsubmod_fpname_list = $(foreach sin_mod, $(bkc_mk_submod_list), libbkc_mk_vsubmod_$(sin_mod).a)
#neighbor module full pathname lists
bkc_mk_nbmod_fpname_list = $(foreach sin_mod, $(bkc_mk_nbmod_list), $(shell cd $(bkc_mk_mtopdir)/..; pwd)/$(sin_mod)/out/lib$(sin_mod).a)
bkc_mk_nbmod_topdir_list = $(foreach sin_mod, $(bkc_mk_nbmod_list), $(shell cd $(bkc_mk_mtopdir)/..; pwd)/$(sin_mod))
#virtual neighbor module full pathname list
bkc_mk_vnbmod_fpname_list = $(foreach sin_mod, $(bkc_mk_nbmod_list), libbkc_mk_vnbmod_$(sin_mod).a)

#module objects src list
bkc_mk_mod_src_list := $(shell cd $(bkc_mk_mtopdir)/src; ls *.c)
#module object list
bkc_mk_mod_object_list := $(patsubst %.c,%.o,$(bkc_mk_mod_src_list))

#module test src list
bkc_mk_mod_tsrc_list := $(shell cd $(bkc_mk_mtopdir)/test; ls *.c)
bkc_mk_mod_tsrc_main := $(bkc_mk_mod_name)_test_main.c
bkc_mk_mod_tout_main := $(bkc_mk_mod_name)_test_main
#module test load flag
bkc_mk_mod_test_ldflag += -L$(bkc_mk_mtopdir)/out -l$(bkc_mk_mod_name)

#module exposure path
bkc_mk_ep = $(patsubst BKC_MK_MOD_TOPDIR%,$(bkc_mk_mtopdir)%,$(ENV_BKC_MK_EP))
#module include dirs
#public include dir(the topper include dir)
ifeq ($(bkc_mk_mod_location), bkc_mk_location_toplevel)
	#for toplevel location
	bkc_mk_mod_include += .
	bkc_mk_mod_include += $(bkc_mk_mtopdir)/include
	bkc_mk_mod_include += $(bkc_mk_mtopdir)/src/inn_inc
else
    #for embeded location, and this is the default value
	bkc_mk_mod_include += .
	bkc_mk_mod_include += $(bkc_mk_mtopdir)/include
	bkc_mk_mod_include += $(bkc_mk_mtopdir)/src/inn_inc
	bkc_mk_mod_include += $(bkc_mk_mtopdir)/../../include
	bkc_mk_mod_include += $(bkc_mk_mtopdir)/../inn_inc
	bkc_mk_mod_include += $(bkc_mk_ep)
endif

#module share path
bkc_mk_sp = $(patsubst BKC_MK_MOD_TOPDIR%,$(bkc_mk_mtopdir)%,$(ENV_BKC_MK_SP))

##########################################################################
#compiler settings
##########################################################################
#CFLAGS and LDFLAGS
include $(bkc_mk_sp)/gen_gcc_flags.mak
CFLAGS += $(foreach sin_mod, $(bkc_mk_mod_include), -I$(sin_mod))

#LDFLAGS
#null

#compiler tool-chain
include $(bkc_mk_sp)/gen_toolchain.mak

##########################################################################
#special target for make
##########################################################################
.NOTPARALLEL:
.PHONY:clean test all slight_comp heavy_comp slight_clean heavy clean

##########################################################################
#main targets
##########################################################################
all: heavy_comp

heavy_comp: bkc_mk_pre_check_tag slight_comp

slight_comp: $(bkc_mk_output_slib_fpath)

$(bkc_mk_output_slib_fpath):$(bkc_mk_output_slib)
	@$(CP)  $< $@

$(bkc_mk_output_slib): $(bkc_mk_vsubmod_fpname_list) $(bkc_mk_vnbmod_fpname_list) \
    $(bkc_mk_mod_object_list)
	@echo ""
	@echo "[#$(bkc_mk_mod_name):re-archiving all modules in $(bkc_mk_mod_name)]"
	@echo ""
	@$(AR) rcs $(bkc_mk_modlib_sname)  *.o
	@$(RANLIB) $(bkc_mk_modlib_sname)

##########################################################################
#pre-checking subsidiary modules
##########################################################################
bkc_mk_pre_check_tag:
	@echo "[#$(bkc_mk_mod_name):checking...]"
	@echo "[#subsidiary modules]:$(bkc_mk_submod_list)"
	@echo "[#neighbor modules]:$(bkc_mk_nbmod_list)"
	@echo ""
	@for sin_mod in $(bkc_mk_submod_topdir_list); do \
        echo "make[`echo $(MAKELEVEL) | awk '{print $1+1}'`]:entering $$sin_mod/build";\
	    ${MAKE}  --no-print-directory -C $$sin_mod/build; \
        echo "make[`echo $(MAKELEVEL) | awk '{print $1+1}'`]:leaving $$sin_mod/build";\
        echo "";\
    done
	@for sin_mod in $(bkc_mk_nbmod_topdir_list); do \
        echo "make[`echo $(MAKELEVEL) | awk '{print $1+1}'`]:entering $$sin_mod/build";\
	    ${MAKE}  --no-print-directory -C $$sin_mod/build; \
        echo "make[`echo $(MAKELEVEL) | awk '{print $1+1}'`]:leaving $$sin_mod/build";\
        echo "";\
    done
	@echo "[#$(bkc_mk_mod_name):checking complete]"
	@echo ""
	@touch bkc_mk_pre_check_tag

##########################################################################
#test target
##########################################################################
vpath %.c $(bkc_mk_mtopdir)/test
test : $(bkc_mk_mod_tsrc_list)
	@echo "";
	@echo "[#making test files]";
	@echo "";
	$(CC) $(CFLAGS) $^ -o $(bkc_mk_mod_tout_main) $(bkc_mk_mod_test_ldflag)
	$(MV) $(bkc_mk_mod_tout_main) $(bkc_mk_mtopdir)/test
	@echo ""
	@echo "[#msg]"
	@echo "$(bkc_mk_mtopdir)/test/$(bkc_mk_mod_tout_main)"
	@echo "has been created, please try it."
	@echo ""

##########################################################################
#slight clean target
##########################################################################
clean: heavy_clean
slight_clean:
	@$(RM) $(bkc_mk_mtopdir)/build/*.o  $(bkc_mk_mtopdir)/build/*.a
	@$(RM) $(bkc_mk_mtopdir)/build/*.sdep $(bkc_mk_mtopdir)/build/*.d
	@$(RM) $(bkc_mk_mtopdir)/build/*ori
	@$(RM) $(bkc_mk_mtopdir)/build/bkc_env_settings.cfg

##########################################################################
#heavy clean target
##########################################################################
heavy_clean:
	@$(RM) $(bkc_mk_mtopdir)/build/*.o  $(bkc_mk_mtopdir)/build/*.a
	@$(RM) $(bkc_mk_mtopdir)/build/*.sdep $(bkc_mk_mtopdir)/build/*.d
	@$(RM) $(bkc_mk_mtopdir)/build/*ori
	@$(RM) $(bkc_mk_mtopdir)/build/bkc_env_settings.cfg
	@$(RM) $(bkc_mk_mtopdir)/out/$(bkc_mk_output_slib)
	@$(RM) $(bkc_mk_mtopdir)/build/bkc_mk_pre_check_tag
	@for sin_mod in $(bkc_mk_submod_topdir_list); do \
        echo "make[`echo $(MAKELEVEL) | awk '{print $1+1}'`]:entering $$sin_mod/build";\
        ${MAKE} --no-print-directory -C  $$sin_mod/build heavy_clean; \
        echo "make[`echo $(MAKELEVEL) | awk '{print $1+1}'`]:leaving $$sin_mod/build";\
        echo "";\
    done
	@for sin_mod in $(bkc_mk_nbmod_topdir_list); do \
	    echo "make[`echo $(MAKELEVEL) | awk '{print $1+1}'`]:entering $$sin_mod/build";\
	    ${MAKE} --no-print-directory -C  $$sin_mod/build heavy_clean; \
		echo "make[`echo $(MAKELEVEL) | awk '{print $1+1}'`]:leaving $$sin_mod/build";\
        echo "";\
    done


##########################################################################
#module includes
##########################################################################
-include $(bkc_mk_mod_src_list:.c=.d)
-include $(foreach sin_mod, $(bkc_mk_vsubmod_fpname_list), $(sin_mod).vsubm.sdep)
-include $(foreach sin_mod, $(bkc_mk_vnbmod_fpname_list), $(sin_mod).vnbm.sdep)

##########################################################################
#module pattern rules
##########################################################################
vpath %.c $(bkc_mk_mtopdir)/src
%.o: %.c
	@echo "[#compiling $(notdir $<)]"
	$(CC) $(CFLAGS) -g -w -c $< -o $@
	@echo ""

%.d:%.c
	@set -e;\
	rm -f $@;\
	$(CPP) $(CFLAGS) -MM  $< > $@.$$$$;\
	sed -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	$(RM)  $@.$$$$

%.vsubm.sdep: Makefile
	@echo "`echo $@ | sed -e 's/.vsubm.sdep//g'`: $(bkc_mk_mtopdir)/src/`echo $@ | sed -e 's/.a.vsubm.sdep//g' | sed -e 's/libbkc_mk_vsubmod_//g'`/out/lib`echo $@ | sed -e 's/.a.vsubm.sdep//g' | sed -e 's/libbkc_mk_vsubmod_//g'`.a" > $@
#	@echo -e \\t"@echo [#mv module output]" >> $@
	@echo -e \\t"@\$$(CP) $$< \$$@" >> $@
	@echo -e \\t"@\$$(AR) -x \$$@ " >> $@

$(bkc_mk_submod_fpname_list):
	@echo ""
#	@echo "[#making module: $@]"
	@echo "make[$(shell echo $(MAKELEVEL) | awk '{print $1+1}')]:entering $(shell cd $(dir $@)/../build; pwd)"
	@${MAKE} --no-print-directory -C  $(shell cd $(dir $@)/../build; pwd)
	@echo "make[$(shell echo $(MAKELEVEL) | awk '{print $1+1}')]:leaving $(shell cd $(dir $@)/../build; pwd)"
	@echo ""

%.vnbm.sdep: Makefile
	@echo "`echo $@ | sed -e 's/.vnbm.sdep//g'`: $(shell cd $(bkc_mk_mtopdir)/../; pwd)/`echo $@ | sed -e 's/.a.vnbm.sdep//g' | sed -e 's/libbkc_mk_vnbmod_//g'`/out/lib`echo $@ | sed -e 's/.a.vnbm.sdep//g' | sed -e 's/libbkc_mk_vnbmod_//g'`.a" > $@
#	@echo -e \\t"@echo [#mv module output]" >> $@
	@echo -e \\t"@\$$(CP) $$< \$$@" >> $@
	@echo -e \\t"@\$$(AR) -x \$$@ " >> $@

$(bkc_mk_nbmod_fpname_list):
	@echo ""
#	@echo "[#making module: $@]"
	@echo "make[$(shell echo $(MAKELEVEL) | awk '{print $1+1}')]:entering $(shell cd $(dir $@)/../build"
	@${MAKE} --no-print-directory -C  $(shell cd $(dir $@)/../build; pwd)
	@echo "make[$(shell echo $(MAKELEVEL) | awk '{print $1+1}')]:leaving $(shell cd $(dir $@)/../build"
	@echo ""
