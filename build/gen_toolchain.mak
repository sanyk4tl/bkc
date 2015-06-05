#######################################################
#tool-chain settings
#######################################################

###############
#for mipsel

#CROSS_COMPILE=mipsel-linux-
#toolChainDir = /opt/toolchains/crosstools_sf-linux-2.6.12.0_gcc-
#3.4.6-20_uclibc-0.9.28-20050817-20070131
#toolChainBinDir = $(toolChainDir)/bin
#toolChainLibDir = $(toolChainDir)/mipsel-linux-uclibc/lib

#CC      = $(toolChainBinDir)/$(CROSS_COMPILE)gcc
#CPP     = $(toolChainBinDir)/$(CROSS_COMPILE)cpp
#CXX     = $(toolChainBinDir)/$(CROSS_COMPILE)c++
#LD      = $(toolChainBinDir)/$(CROSS_COMPILE)ld
#AS      = $(toolChainBinDir)/$(CROSS_COMPILE)as
#AR      = $(toolChainBinDir)/$(CROSS_COMPILE)ar
#NM      = $(toolChainBinDir)/$(CROSS_COMPILE)nm
#STRIP   = $(toolChainBinDir)/$(CROSS_COMPILE)strip
#OBJCOPY = $(toolChainBinDir)/$(CROSS_COMPILE)objcopy
#OBJDUMP = $(toolChainBinDir)/$(CROSS_COMPILE)objdump
#RANLIB  = $(toolChainBinDir)/$(CROSS_COMPILE)ranlib

################
#for x86_32_64

CC      = gcc
CPP     = cpp
CXX     = c++
LD      = ld
AS      = as
AR      = ar
NM      = nm
STRIP   = strip
OBJCOPY = objcopy
OBJDUMP = objdump
RANLIB  = ranlib

CP      = cp -fv
RM      = rm -rf
SORT    = sort
SED     = sed
TOUCH   = touch
MKDIR   = mkdir -p
MV      = mv
SET     = set
SED     = sed
SH      = sh
CD      = cdECHO    = echo -e
