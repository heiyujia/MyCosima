################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../config.cfg 


# Each subdirectory must supply rules for building sources it contributes
configPkg/linker.cmd: ../config.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"/xs" --xdcpath= xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.concertoM3:F28M35H52C1 -r release -c --compileOptions "-g --optimize_with_debug" "$<"
	@echo 'Finished building: $<'
	@echo ' '


