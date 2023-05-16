################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_ctimer.c \
../drivers/fsl_dma.c \
../drivers/fsl_eeprom.c \
../drivers/fsl_emc.c \
../drivers/fsl_flexcomm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_i2c_dma.c \
../drivers/fsl_power.c \
../drivers/fsl_reset.c \
../drivers/fsl_rit.c \
../drivers/fsl_sctimer.c \
../drivers/fsl_usart.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_ctimer.d \
./drivers/fsl_dma.d \
./drivers/fsl_eeprom.d \
./drivers/fsl_emc.d \
./drivers/fsl_flexcomm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_i2c_dma.d \
./drivers/fsl_power.d \
./drivers/fsl_reset.d \
./drivers/fsl_rit.d \
./drivers/fsl_sctimer.d \
./drivers/fsl_usart.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_ctimer.o \
./drivers/fsl_dma.o \
./drivers/fsl_eeprom.o \
./drivers/fsl_emc.o \
./drivers/fsl_flexcomm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_i2c_dma.o \
./drivers/fsl_power.o \
./drivers/fsl_reset.o \
./drivers/fsl_rit.o \
./drivers/fsl_sctimer.o \
./drivers/fsl_usart.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\nxp\Pojects\nxp-proj\board" -I"C:\nxp\Pojects\nxp-proj\source" -I"C:\nxp\Pojects\nxp-proj" -I"C:\nxp\Pojects\nxp-proj\drivers" -I"C:\nxp\Pojects\nxp-proj\device" -I"C:\nxp\Pojects\nxp-proj\CMSIS" -I"C:\nxp\Pojects\nxp-proj\freertos\freertos_kernel\include" -I"C:\nxp\Pojects\nxp-proj\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\nxp\Pojects\nxp-proj\utilities" -I"C:\nxp\Pojects\nxp-proj\component\serial_manager" -I"C:\nxp\Pojects\nxp-proj\component\lists" -I"C:\nxp\Pojects\nxp-proj\component\uart" -I"C:\nxp\Pojects\nxp-proj\source\display" -I"C:\nxp\Pojects\nxp-proj\source\display\driver\config" -I"C:\nxp\Pojects\nxp-proj\source\display\driver\resources" -I"C:\nxp\Pojects\nxp-proj\source\display\menu" -I"C:\nxp\Pojects\nxp-proj\source\hmi" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_ctimer.d ./drivers/fsl_ctimer.o ./drivers/fsl_dma.d ./drivers/fsl_dma.o ./drivers/fsl_eeprom.d ./drivers/fsl_eeprom.o ./drivers/fsl_emc.d ./drivers/fsl_emc.o ./drivers/fsl_flexcomm.d ./drivers/fsl_flexcomm.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_i2c.d ./drivers/fsl_i2c.o ./drivers/fsl_i2c_dma.d ./drivers/fsl_i2c_dma.o ./drivers/fsl_power.d ./drivers/fsl_power.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_rit.d ./drivers/fsl_rit.o ./drivers/fsl_sctimer.d ./drivers/fsl_sctimer.o ./drivers/fsl_usart.d ./drivers/fsl_usart.o

.PHONY: clean-drivers

