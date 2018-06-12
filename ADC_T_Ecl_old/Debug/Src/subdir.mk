################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/dac.c \
../Src/dma.c \
../Src/gpio.c \
../Src/main.c \
../Src/opamp.c \
../Src/stm32f3xx_hal_msp.c \
../Src/stm32f3xx_it.c \
../Src/system_stm32f3xx.c \
../Src/tim.c 

OBJS += \
./Src/adc.o \
./Src/dac.o \
./Src/dma.o \
./Src/gpio.o \
./Src/main.o \
./Src/opamp.o \
./Src/stm32f3xx_hal_msp.o \
./Src/stm32f3xx_it.o \
./Src/system_stm32f3xx.o \
./Src/tim.o 

C_DEPS += \
./Src/adc.d \
./Src/dac.d \
./Src/dma.d \
./Src/gpio.d \
./Src/main.d \
./Src/opamp.d \
./Src/stm32f3xx_hal_msp.d \
./Src/stm32f3xx_it.d \
./Src/system_stm32f3xx.d \
./Src/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303xC -I"/Users/amin/Documents/project_lab_imp_measuring/ADC_T_Ecl/Inc" -I"/Users/amin/Documents/project_lab_imp_measuring/ADC_T_Ecl/Drivers/STM32F3xx_HAL_Driver/Inc" -I"/Users/amin/Documents/project_lab_imp_measuring/ADC_T_Ecl/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"/Users/amin/Documents/project_lab_imp_measuring/ADC_T_Ecl/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"/Users/amin/Documents/project_lab_imp_measuring/ADC_T_Ecl/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


