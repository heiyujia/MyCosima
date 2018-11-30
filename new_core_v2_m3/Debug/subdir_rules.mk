################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
TMDXDOCKH52C1.obj: ../TMDXDOCKH52C1.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M3 --code_state=16 -me --include_path="C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/tirtos_c2000_2_12_01_33/products/MWare_v202a/MWare" --include_path="C:/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages/ti/ndk/inc/bsd" -g --gcc --define=ccs --define=MWARE --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="TMDXDOCKH52C1.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

build-94579345: ../config.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_31_00_24_core/xs" --xdcpath="C:/ti/tirtos_c2000_2_12_01_33/packages;C:/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ipc_3_30_01_12/packages;C:/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages;C:/ti/tirtos_c2000_2_12_01_33/products/uia_2_00_02_39/packages;C:/ti/tirtos_1_21_00_09/packages;C:/ti/ccs620/ccsv6/ccs_base;C:/ti/ccs620/ccsv6/ccs_base;C:/ti/tirtos_1_21_00_09/products/bios_6_37_00_20/packages;C:/ti/tirtos_1_21_00_09/products/ipc_3_10_01_11/packages;C:/ti/tirtos_1_21_00_09/products/ndk_2_23_00_00/packages;C:/ti/tirtos_1_21_00_09/products/uia_1_04_00_06/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.concertoM3:F28M35H52C1 -r release -c "C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS" --compileOptions "-g --optimize_with_debug" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-94579345
configPkg/compiler.opt: build-94579345
configPkg/: build-94579345

main_m3.obj: ../main_m3.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M3 --code_state=16 -me --include_path="C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/tirtos_c2000_2_12_01_33/products/MWare_v202a/MWare" --include_path="C:/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages/ti/ndk/inc/bsd" -g --gcc --define=ccs --define=MWARE --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="main_m3.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tcpEchoHooks.obj: ../tcpEchoHooks.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M3 --code_state=16 -me --include_path="C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --include_path="C:/ti/tirtos_c2000_2_12_01_33/products/MWare_v202a/MWare" --include_path="C:/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages/ti/ndk/inc/bsd" -g --gcc --define=ccs --define=MWARE --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="tcpEchoHooks.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


