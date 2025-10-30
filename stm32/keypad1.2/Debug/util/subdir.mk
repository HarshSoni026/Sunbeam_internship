################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../util/DWT_Delay.c \
../util/Timer_Delay.c 

OBJS += \
./util/DWT_Delay.o \
./util/Timer_Delay.o 

C_DEPS += \
./util/DWT_Delay.d \
./util/Timer_Delay.d 


# Each subdirectory must supply rules for building sources it contributes
util/%.o util/%.su util/%.cyclo: ../util/%.c util/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"/home/sunbeam/CDAC/Sunbeam_internship/stm32/keypad1.2/ECUAL/LCD16x2" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-util

clean-util:
	-$(RM) ./util/DWT_Delay.cyclo ./util/DWT_Delay.d ./util/DWT_Delay.o ./util/DWT_Delay.su ./util/Timer_Delay.cyclo ./util/Timer_Delay.d ./util/Timer_Delay.o ./util/Timer_Delay.su

.PHONY: clean-util

