################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/MPGpendant/mpg_pendant.c 

OBJS += \
./Core/MPGpendant/mpg_pendant.o 

C_DEPS += \
./Core/MPGpendant/mpg_pendant.d 


# Each subdirectory must supply rules for building sources it contributes
Core/MPGpendant/%.o Core/MPGpendant/%.su Core/MPGpendant/%.cyclo: ../Core/MPGpendant/%.c Core/MPGpendant/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/Comm -I../Core/R61529 -I../Core/FT5436 -I../Core/SoundFX -I../Core/grblHALParser -I../Core/MPGpendant -I../Core/MPGmovement -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-MPGpendant

clean-Core-2f-MPGpendant:
	-$(RM) ./Core/MPGpendant/mpg_pendant.cyclo ./Core/MPGpendant/mpg_pendant.d ./Core/MPGpendant/mpg_pendant.o ./Core/MPGpendant/mpg_pendant.su

.PHONY: clean-Core-2f-MPGpendant

