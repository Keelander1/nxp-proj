################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c \
../utilities/fsl_str.c 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d \
./utilities/fsl_str.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o \
./utilities/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_LPC54628J512ET180=1 -DCPU_LPC54628J512ET180_cm4 -DCPU_LPC54628 -D__USE_CMSIS -DSERIAL_PORT_TYPE_UART=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -D__REDLIB__ -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\board" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\source" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\drivers" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\device" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\CMSIS" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\freertos\freertos_kernel\include" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\utilities" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\component\serial_manager" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\component\lists" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\component\uart" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\source\display" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\source\display\driver\config" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\source\display\driver\resources" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\source\display\menu" -I"C:\Users\Martina\Desktop\FHLandshut_ET_Ma\FH_Landshut_ET_1.Semester\ESE Projekt\Projekt\nxp-proj\source\hmi" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o ./utilities/fsl_str.d ./utilities/fsl_str.o

.PHONY: clean-utilities

