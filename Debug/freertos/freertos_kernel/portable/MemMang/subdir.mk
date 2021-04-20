################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/portable/MemMang/heap_4.c 

OBJS += \
./freertos/freertos_kernel/portable/MemMang/heap_4.o 

C_DEPS += \
./freertos/freertos_kernel/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/portable/MemMang/%.o: ../freertos/freertos_kernel/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\board" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\source" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\drivers" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\device" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\CMSIS" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\freertos\freertos_kernel\include" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\utilities" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\component\serial_manager" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\component\lists" -I"C:\Users\chris\git\NXP_Gustl_SumEck\Gustl_SumEck\component\uart" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


