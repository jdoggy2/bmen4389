################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
build-488569942: ../empty_driverlib_library.syscfg
	@echo 'SysConfig - building file: "$<"'
	"/Applications/ti/ccs2050/ccs/utils/sysconfig_1.27.0/sysconfig_cli.sh" -s "/Users/jordyncoffin/ti/mspm0_sdk_2_10_00_04/.metadata/product.json" --script "/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library/empty_driverlib_library.syscfg" -o "syscfg" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/device_linker.cmd: build-488569942 ../empty_driverlib_library.syscfg
syscfg/device.opt: build-488569942
syscfg/device.cmd.genlibs: build-488569942
syscfg/ti_msp_dl_config.c: build-488569942
syscfg/ti_msp_dl_config.h: build-488569942
syscfg/Event.dot: build-488569942
syscfg: build-488569942

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/Applications/ti/ccs2050/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library" -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library/Debug" -I"/Users/jordyncoffin/ti/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"/Users/jordyncoffin/ti/mspm0_sdk_2_10_00_04/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: /Users/jordyncoffin/ti/mspm0_sdk_2_10_00_04/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/Applications/ti/ccs2050/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library" -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library/Debug" -I"/Users/jordyncoffin/ti/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"/Users/jordyncoffin/ti/mspm0_sdk_2_10_00_04/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"/Applications/ti/ccs2050/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library" -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library/Debug" -I"/Users/jordyncoffin/ti/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"/Users/jordyncoffin/ti/mspm0_sdk_2_10_00_04/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/jordyncoffin/workspace_ccstheia/empty_driverlib_library/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


