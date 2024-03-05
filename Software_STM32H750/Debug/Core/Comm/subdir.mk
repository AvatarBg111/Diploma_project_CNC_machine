################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Comm/grbl_cpu_comm.c \
../Core/Comm/parser.c 

OBJS += \
./Core/Comm/grbl_cpu_comm.o \
./Core/Comm/parser.o 

C_DEPS += \
./Core/Comm/grbl_cpu_comm.d \
./Core/Comm/parser.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Comm/%.o Core/Comm/%.su Core/Comm/%.cyclo: ../Core/Comm/%.c Core/Comm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Core/ILI9341 -I../Core/R61529 -I../Core/FT5436 -I../Core/Comm -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Comm

clean-Core-2f-Comm:
	-$(RM) ./Core/Comm/grbl_cpu_comm.cyclo ./Core/Comm/grbl_cpu_comm.d ./Core/Comm/grbl_cpu_comm.o ./Core/Comm/grbl_cpu_comm.su ./Core/Comm/parser.cyclo ./Core/Comm/parser.d ./Core/Comm/parser.o ./Core/Comm/parser.su

.PHONY: clean-Core-2f-Comm

