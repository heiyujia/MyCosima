#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/tirtos_1_10_00_23/packages;C:/ti/tirtos_1_10_00_23/products/bios_6_35_01_29/packages;C:/ti/tirtos_1_10_00_23/products/ipc_1_25_03_15/packages;C:/ti/tirtos_1_10_00_23/products/ndk_2_22_03_20/packages;C:/ti/tirtos_1_10_00_23/products/uia_1_03_00_02/packages;C:/ti/ccsv5/ccs_base;C:/ti/workspace/WS-Git/Prototyp/Prototype_M3/.config
override XDCROOT = C:/ti/xdctools_3_25_02_70
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/tirtos_1_10_00_23/packages;C:/ti/tirtos_1_10_00_23/products/bios_6_35_01_29/packages;C:/ti/tirtos_1_10_00_23/products/ipc_1_25_03_15/packages;C:/ti/tirtos_1_10_00_23/products/ndk_2_22_03_20/packages;C:/ti/tirtos_1_10_00_23/products/uia_1_03_00_02/packages;C:/ti/ccsv5/ccs_base;C:/ti/workspace/WS-Git/Prototyp/Prototype_M3/.config;C:/ti/xdctools_3_25_02_70/packages;..
HOSTOS = Windows
endif
