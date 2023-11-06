################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/serial_manager/serial_manager.c \
../component/serial_manager/serial_port_uart.c 

C_DEPS += \
./component/serial_manager/serial_manager.d \
./component/serial_manager/serial_port_uart.d 

OBJS += \
./component/serial_manager/serial_manager.o \
./component/serial_manager/serial_port_uart.o 


# Each subdirectory must supply rules for building sources it contributes
component/serial_manager/%.o: ../component/serial_manager/%.c component/serial_manager/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
<<<<<<< HEAD
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\Users\tobia\Documents\git\nxp-proj\board" -I"C:\Users\tobia\Documents\git\nxp-proj\source" -I"C:\Users\tobia\Documents\git\nxp-proj" -I"C:\Users\tobia\Documents\git\nxp-proj\drivers" -I"C:\Users\tobia\Documents\git\nxp-proj\device" -I"C:\Users\tobia\Documents\git\nxp-proj\CMSIS" -I"C:\Users\tobia\Documents\git\nxp-proj\freertos\freertos_kernel\include" -I"C:\Users\tobia\Documents\git\nxp-proj\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\tobia\Documents\git\nxp-proj\utilities" -I"C:\Users\tobia\Documents\git\nxp-proj\component\serial_manager" -I"C:\Users\tobia\Documents\git\nxp-proj\component\lists" -I"C:\Users\tobia\Documents\git\nxp-proj\component\uart" -I"C:\Users\tobia\Documents\git\nxp-proj\source\display" -I"C:\Users\tobia\Documents\git\nxp-proj\source\display\driver\config" -I"C:\Users\tobia\Documents\git\nxp-proj\source\display\driver\resources" -I"C:\Users\tobia\Documents\git\nxp-proj\source\display\menu" -I"C:\Users\tobia\Documents\git\nxp-proj\source\hmi" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
=======
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\board" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\source" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\drivers" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\device" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\CMSIS" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\freertos\freertos_kernel\include" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\utilities" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\component\serial_manager" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\component\lists" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\component\uart" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\source\display" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\source\display\driver\config" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\source\display\driver\resources" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\source\display\menu" -I"D:\Studium\NXP-Cup\nxp-proj\nxp-proj\source\hmi" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
>>>>>>> 2340b50b78e4ba6629c977e8f135bd97dd62bfb2
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-serial_manager

clean-component-2f-serial_manager:
	-$(RM) ./component/serial_manager/serial_manager.d ./component/serial_manager/serial_manager.o ./component/serial_manager/serial_port_uart.d ./component/serial_manager/serial_port_uart.o

.PHONY: clean-component-2f-serial_manager

