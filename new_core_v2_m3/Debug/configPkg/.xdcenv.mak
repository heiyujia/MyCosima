#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/tirtos_c2000_2_12_01_33/packages;C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ipc_3_30_01_12/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages;C:/ti/tirtos_c2000_2_12_01_33/products/uia_2_00_02_39/packages;C:/ti/tirtos_1_21_00_09/packages;C:/ti/ccs620/ccsv6/ccs_base;C:/ti/tirtos_1_21_00_09/products/bios_6_37_00_20/packages;C:/ti/tirtos_1_21_00_09/products/ipc_3_10_01_11/packages;C:/ti/tirtos_1_21_00_09/products/ndk_2_23_00_00/packages;C:/ti/tirtos_1_21_00_09/products/uia_1_04_00_06/packages
override XDCROOT = C:/ti/xdctools_3_31_00_24_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/tirtos_c2000_2_12_01_33/packages;C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ipc_3_30_01_12/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages;C:/ti/tirtos_c2000_2_12_01_33/products/uia_2_00_02_39/packages;C:/ti/tirtos_1_21_00_09/packages;C:/ti/ccs620/ccsv6/ccs_base;C:/ti/tirtos_1_21_00_09/products/bios_6_37_00_20/packages;C:/ti/tirtos_1_21_00_09/products/ipc_3_10_01_11/packages;C:/ti/tirtos_1_21_00_09/products/ndk_2_23_00_00/packages;C:/ti/tirtos_1_21_00_09/products/uia_1_04_00_06/packages;C:/ti/xdctools_3_31_00_24_core/packages;..
HOSTOS = Windows
endif
