################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/grblHALComm/parser.c \
../Core/grblHALComm/sender.c 

OBJS += \
./Core/grblHALComm/parser.o \
./Core/grblHALComm/sender.o 

C_DEPS += \
./Core/grblHALComm/parser.d \
./Core/grblHALComm/sender.d 


# Each subdirectory must supply rules for building sources it contributes
Core/grblHALComm/%.o Core/grblHALComm/%.su Core/grblHALComm/%.cyclo: ../Core/grblHALComm/%.c Core/grblHALComm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/Comm -I../Core/R61529 -I../Core/FT5436 -I../Core/SoundFX -I../Core/grblHALParser -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-grblHALComm

clean-Core-2f-grblHALComm:
	-$(RM) ./Core/grblHALComm/parser.cyclo ./Core/grblHALComm/parser.d ./Core/grblHALComm/parser.o ./Core/grblHALComm/parser.su ./Core/grblHALComm/sender.cyclo ./Core/grblHALComm/sender.d ./Core/grblHALComm/sender.o ./Core/grblHALComm/sender.su

.PHONY: clean-Core-2f-grblHALComm

