################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/SoundFX/sound_fx.c 

OBJS += \
./Core/SoundFX/sound_fx.o 

C_DEPS += \
./Core/SoundFX/sound_fx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/SoundFX/%.o Core/SoundFX/%.su Core/SoundFX/%.cyclo: ../Core/SoundFX/%.c Core/SoundFX/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/R61529 -I../Core/FT5436 -I../Core/SoundFX -I../Core/grblHALComm -I../Core/MPGpendant -I../Core/MPGmovement -I../Core/ILI9341 -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-SoundFX

clean-Core-2f-SoundFX:
	-$(RM) ./Core/SoundFX/sound_fx.cyclo ./Core/SoundFX/sound_fx.d ./Core/SoundFX/sound_fx.o ./Core/SoundFX/sound_fx.su

.PHONY: clean-Core-2f-SoundFX
