################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs620/ccsv6/tools/compiler/c2000_15.12.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main_c28.obj" \
"./ImportedFiles/F28M35x_Adc.obj" \
"./ImportedFiles/F28M35x_CodeStartBranch.obj" \
"./ImportedFiles/F28M35x_CpuTimers.obj" \
"./ImportedFiles/F28M35x_DefaultIsr.obj" \
"./ImportedFiles/F28M35x_EPwm.obj" \
"./ImportedFiles/F28M35x_GlobalVariableDefs.obj" \
"./ImportedFiles/F28M35x_Gpio.obj" \
"./ImportedFiles/F28M35x_PieCtrl.obj" \
"./ImportedFiles/F28M35x_PieVect.obj" \
"./ImportedFiles/F28M35x_SysCtrl.obj" \
"./ImportedFiles/F28M35x_usDelay.obj" \
"./hw_init/adc.obj" \
"./hw_init/eCap.obj" \
"./hw_init/epwm.obj" \
"./hw_init/my_posspeed.obj" \
"./hw_init/platform.obj" \
"./sw/application.obj" \
"./sw/errordetection.obj" \
"./sw/ipc_c28.obj" \
"./sw/isr.obj" \
"./sw/javascope.obj" \
"../F28M35x_Headers_nonBIOS.cmd" \
"../c28.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-lrts2800_fpu32.lib \
-l"C:/ti/controlSUITE/libs/math/IQmath/v160/lib/IQmath_fpu32.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include ImportedFiles/subdir_vars.mk
-include hw_init/subdir_vars.mk
-include sw/subdir_vars.mk
-include subdir_rules.mk
-include ImportedFiles/subdir_rules.mk
-include hw_init/subdir_rules.mk
-include sw/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
new_core_v2_c28.out \

EXE_OUTPUTS__QUOTED += \
"new_core_v2_c28.out" \

BIN_OUTPUTS += \
new_core_v2_c28.hex \

BIN_OUTPUTS__QUOTED += \
"new_core_v2_c28.hex" \


# All Target
all: new_core_v2_c28.out

# Tool invocations
new_core_v2_c28.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs620/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --float_support=fpu32 -z -m"new_core_v2_c28.map" --stack_size=0x1000 --warn_sections -i"C:/ti/ccs620/ccsv6/tools/compiler/c2000_15.12.3.LTS/lib" -i"C:/ti/ccs620/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --reread_libs --xml_link_info="new_core_v2_c28_linkInfo.xml" --rom_model -o "new_core_v2_c28.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

new_core_v2_c28.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccs620/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/hex2000"  -o "new_core_v2_c28.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "main_c28.d" "ImportedFiles\F28M35x_Adc.d" "ImportedFiles\F28M35x_CpuTimers.d" "ImportedFiles\F28M35x_DefaultIsr.d" "ImportedFiles\F28M35x_EPwm.d" "ImportedFiles\F28M35x_GlobalVariableDefs.d" "ImportedFiles\F28M35x_Gpio.d" "ImportedFiles\F28M35x_PieCtrl.d" "ImportedFiles\F28M35x_PieVect.d" "ImportedFiles\F28M35x_SysCtrl.d" "hw_init\adc.d" "hw_init\eCap.d" "hw_init\epwm.d" "hw_init\my_posspeed.d" "hw_init\platform.d" "sw\application.d" "sw\errordetection.d" "sw\ipc_c28.d" "sw\isr.d" "sw\javascope.d" 
	-$(RM) "main_c28.obj" "ImportedFiles\F28M35x_Adc.obj" "ImportedFiles\F28M35x_CodeStartBranch.obj" "ImportedFiles\F28M35x_CpuTimers.obj" "ImportedFiles\F28M35x_DefaultIsr.obj" "ImportedFiles\F28M35x_EPwm.obj" "ImportedFiles\F28M35x_GlobalVariableDefs.obj" "ImportedFiles\F28M35x_Gpio.obj" "ImportedFiles\F28M35x_PieCtrl.obj" "ImportedFiles\F28M35x_PieVect.obj" "ImportedFiles\F28M35x_SysCtrl.obj" "ImportedFiles\F28M35x_usDelay.obj" "hw_init\adc.obj" "hw_init\eCap.obj" "hw_init\epwm.obj" "hw_init\my_posspeed.obj" "hw_init\platform.obj" "sw\application.obj" "sw\errordetection.obj" "sw\ipc_c28.obj" "sw\isr.obj" "sw\javascope.obj" 
	-$(RM) "ImportedFiles\F28M35x_CodeStartBranch.d" "ImportedFiles\F28M35x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

