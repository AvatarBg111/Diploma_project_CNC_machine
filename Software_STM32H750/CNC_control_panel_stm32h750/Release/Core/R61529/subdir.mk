################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/R61529/fonts.c \
../Core/R61529/r61529.c 

OBJS += \
./Core/R61529/fonts.o \
./Core/R61529/r61529.o 

C_DEPS += \
./Core/R61529/fonts.d \
./Core/R61529/r61529.d 


# Each subdirectory must supply rules for building sources it contributes
Core/R61529/%.o Core/R61529/%.su Core/R61529/%.cyclo: ../Core/R61529/%.c Core/R61529/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/Comm -I../Core/R61529 -I../Core/FT5436 -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-R61529

clean-Core-2f-R61529:
	-$(RM) ./Core/R61529/fonts.cyclo ./Core/R61529/fonts.d ./Core/R61529/fonts.o ./Core/R61529/fonts.su ./Core/R61529/r61529.cyclo ./Core/R61529/r61529.d ./Core/R61529/r61529.o ./Core/R61529/r61529.su

.PHONY: clean-Core-2f-R61529

