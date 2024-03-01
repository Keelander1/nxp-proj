################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/ultrasonic/subdir.mk
-include source/servo/subdir.mk
-include source/rpmMeas/subdir.mk
-include source/hmi/input_devices/subdir.mk
-include source/hmi/subdir.mk
-include source/drive/subdir.mk
-include source/display/menu/design/subdir.mk
-include source/display/menu/subdir.mk
-include source/display/interface/subdir.mk
-include source/display/driver/subdir.mk
-include source/display/adapter/subdir.mk
-include source/display/subdir.mk
-include source/camera/subdir.mk
-include source/board/subdir.mk
-include source/subdir.mk
-include freertos/freertos_kernel/portable/MemMang/subdir.mk
-include freertos/freertos_kernel/portable/GCC/ARM_CM4F/subdir.mk
-include freertos/freertos_kernel/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include component/lists/subdir.mk
-include board/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Gustl_SumEck
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: Gustl_SumEck.axf

# Tool invocations
Gustl_SumEck.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\libs" -Xlinker -Map="Gustl_SumEck.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T Gustl_SumEck_Debug.ld -o "Gustl_SumEck.axf" $(OBJS) $(USER_OBJS) $(LIBS) -lpower_hardabi
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Gustl_SumEck.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Gustl_SumEck.axf"; # arm-none-eabi-objcopy -v -O binary "Gustl_SumEck.axf" "Gustl_SumEck.bin" ; # checksum -p LPC54628J512 -d "Gustl_SumEck.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
