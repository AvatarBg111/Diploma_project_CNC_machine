################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/MPGmovement/mpg_movement.c 

OBJS += \
./Core/MPGmovement/mpg_movement.o 

C_DEPS += \
./Core/MPGmovement/mpg_movement.d 


# Each subdirectory must supply rules for building sources it contributes
Core/MPGmovement/%.o Core/MPGmovement/%.su Core/MPGmovement/%.cyclo: ../Core/MPGmovement/%.c Core/MPGmovement/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/Comm -I../Core/R61529 -I../Core/FT5436 -I../Core/SoundFX -I../Core/grblHALParser -I../Core/MPGpendant -I../Core/MPGmovement -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-MPGmovement

clean-Core-2f-MPGmovement:
	-$(RM) ./Core/MPGmovement/mpg_movement.cyclo ./Core/MPGmovement/mpg_movement.d ./Core/MPGmovement/mpg_movement.o ./Core/MPGmovement/mpg_movement.su

.PHONY: clean-Core-2f-MPGmovement

