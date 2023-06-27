################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_LPC54628.c 

C_DEPS += \
./device/system_LPC54628.d 

OBJS += \
./device/system_LPC54628.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\nxp\Pojects\nxp-proj\board" -I"C:\nxp\Pojects\nxp-proj\source" -I"C:\nxp\Pojects\nxp-proj" -I"C:\nxp\Pojects\nxp-proj\drivers" -I"C:\nxp\Pojects\nxp-proj\device" -I"C:\nxp\Pojects\nxp-proj\CMSIS" -I"C:\nxp\Pojects\nxp-proj\freertos\freertos_kernel\include" -I"C:\nxp\Pojects\nxp-proj\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\nxp\Pojects\nxp-proj\utilities" -I"C:\nxp\Pojects\nxp-proj\component\serial_manager" -I"C:\nxp\Pojects\nxp-proj\component\lists" -I"C:\nxp\Pojects\nxp-proj\component\uart" -I"C:\nxp\Pojects\nxp-proj\source\display" -I"C:\nxp\Pojects\nxp-proj\source\display\driver\config" -I"C:\nxp\Pojects\nxp-proj\source\display\driver\resources" -I"C:\nxp\Pojects\nxp-proj\source\display\menu" -I"C:\nxp\Pojects\nxp-proj\source\hmi" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_LPC54628.d ./device/system_LPC54628.o

.PHONY: clean-device

