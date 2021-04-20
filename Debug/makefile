################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/servo/subdir.mk
-include source/rpmMeas/subdir.mk
-include source/drive/subdir.mk
-include source/display/interface/subdir.mk
-include source/display/hmi/subdir.mk
-include source/display/driver/subdir.mk
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
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: nxp-aes-proj.axf

# Tool invocations
nxp-aes-proj.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\libs" -Xlinker -Map="nxp-aes-proj.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T lpcxpresso54628_freertos_hello_Debug.ld -o "nxp-aes-proj.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) nxp-aes-proj.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "nxp-aes-proj.axf"; # arm-none-eabi-objcopy -v -O binary "nxp-aes-proj.axf" "nxp-aes-proj.bin" ; # checksum -p LPC54628J512 -d "nxp-aes-proj.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
