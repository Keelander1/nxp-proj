################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main_gustl.c \
../source/param.c \
../source/semihost_hardfault.c \
../source/settings.c 

C_DEPS += \
./source/main_gustl.d \
./source/param.d \
./source/semihost_hardfault.d \
./source/settings.d 

OBJS += \
./source/main_gustl.o \
./source/param.o \
./source/semihost_hardfault.o \
./source/settings.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\board" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\source" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\drivers" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\device" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\CMSIS" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\freertos\freertos_kernel\include" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\utilities" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\component\serial_manager" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\component\lists" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\component\uart" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\source\display" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\source\display\driver\config" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\source\display\driver\resources" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\source\display\menu" -I"C:\Users\keela\OneDrive\Desktop\Studium\Master ET\Semester2NXP\nxp-proj\source\hmi" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/main_gustl.d ./source/main_gustl.o ./source/param.d ./source/param.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/settings.d ./source/settings.o

.PHONY: clean-source

