################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ECUAL/KEYPAD/KEYPAD.c \
../ECUAL/KEYPAD/KEYPAD_cfg.c 

OBJS += \
./ECUAL/KEYPAD/KEYPAD.o \
./ECUAL/KEYPAD/KEYPAD_cfg.o 

C_DEPS += \
./ECUAL/KEYPAD/KEYPAD.d \
./ECUAL/KEYPAD/KEYPAD_cfg.d 


# Each subdirectory must supply rules for building sources it contributes
ECUAL/KEYPAD/%.o ECUAL/KEYPAD/%.su ECUAL/KEYPAD/%.cyclo: ../ECUAL/KEYPAD/%.c ECUAL/KEYPAD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"/home/sunbeam/CDAC/Sunbeam_internship/stm32/keypad1.2/ECUAL/LCD16x2" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ECUAL-2f-KEYPAD

clean-ECUAL-2f-KEYPAD:
	-$(RM) ./ECUAL/KEYPAD/KEYPAD.cyclo ./ECUAL/KEYPAD/KEYPAD.d ./ECUAL/KEYPAD/KEYPAD.o ./ECUAL/KEYPAD/KEYPAD.su ./ECUAL/KEYPAD/KEYPAD_cfg.cyclo ./ECUAL/KEYPAD/KEYPAD_cfg.d ./ECUAL/KEYPAD/KEYPAD_cfg.o ./ECUAL/KEYPAD/KEYPAD_cfg.su

.PHONY: clean-ECUAL-2f-KEYPAD

