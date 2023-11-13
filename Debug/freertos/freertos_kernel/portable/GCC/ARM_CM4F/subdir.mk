################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.c 

C_DEPS += \
./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.d 

OBJS += \
./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.o 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/portable/GCC/ARM_CM4F/%.o: ../freertos/freertos_kernel/portable/GCC/ARM_CM4F/%.c freertos/freertos_kernel/portable/GCC/ARM_CM4F/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\Users\tobia\Documents\git\nxp-proj\board" -I"C:\Users\tobia\Documents\git\nxp-proj\source" -I"C:\Users\tobia\Documents\git\nxp-proj" -I"C:\Users\tobia\Documents\git\nxp-proj\drivers" -I"C:\Users\tobia\Documents\git\nxp-proj\device" -I"C:\Users\tobia\Documents\git\nxp-proj\CMSIS" -I"C:\Users\tobia\Documents\git\nxp-proj\freertos\freertos_kernel\include" -I"C:\Users\tobia\Documents\git\nxp-proj\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\tobia\Documents\git\nxp-proj\utilities" -I"C:\Users\tobia\Documents\git\nxp-proj\component\serial_manager" -I"C:\Users\tobia\Documents\git\nxp-proj\component\lists" -I"C:\Users\tobia\Documents\git\nxp-proj\component\uart" -I"C:\Users\tobia\Documents\git\nxp-proj\source\display" -I"C:\Users\tobia\Documents\git\nxp-proj\source\display\driver\config" -I"C:\Users\tobia\Documents\git\nxp-proj\source\display\driver\resources" -I"C:\Users\tobia\Documents\git\nxp-proj\source\display\menu" -I"C:\Users\tobia\Documents\git\nxp-proj\source\hmi" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-freertos-2f-freertos_kernel-2f-portable-2f-GCC-2f-ARM_CM4F

clean-freertos-2f-freertos_kernel-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.d ./freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.o

.PHONY: clean-freertos-2f-freertos_kernel-2f-portable-2f-GCC-2f-ARM_CM4F

