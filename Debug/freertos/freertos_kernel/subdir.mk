################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos_kernel/croutine.c \
../freertos/freertos_kernel/event_groups.c \
../freertos/freertos_kernel/list.c \
../freertos/freertos_kernel/queue.c \
../freertos/freertos_kernel/stream_buffer.c \
../freertos/freertos_kernel/tasks.c \
../freertos/freertos_kernel/timers.c 

OBJS += \
./freertos/freertos_kernel/croutine.o \
./freertos/freertos_kernel/event_groups.o \
./freertos/freertos_kernel/list.o \
./freertos/freertos_kernel/queue.o \
./freertos/freertos_kernel/stream_buffer.o \
./freertos/freertos_kernel/tasks.o \
./freertos/freertos_kernel/timers.o 

C_DEPS += \
./freertos/freertos_kernel/croutine.d \
./freertos/freertos_kernel/event_groups.d \
./freertos/freertos_kernel/list.d \
./freertos/freertos_kernel/queue.d \
./freertos/freertos_kernel/stream_buffer.d \
./freertos/freertos_kernel/tasks.d \
./freertos/freertos_kernel/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos_kernel/%.o: ../freertos/freertos_kernel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\board" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\source" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\drivers" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\device" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\CMSIS" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\freertos\freertos_kernel\include" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\utilities" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\component\serial_manager" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\component\lists" -I"C:\Users\chris\OneDrive\Desktop\NXP-Git\nxp-aes-proj\component\uart" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


