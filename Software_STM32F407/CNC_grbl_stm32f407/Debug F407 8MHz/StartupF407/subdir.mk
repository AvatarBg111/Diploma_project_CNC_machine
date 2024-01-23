################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../StartupF407/startup_stm32f407vgtx.s 

OBJS += \
./StartupF407/startup_stm32f407vgtx.o 

S_DEPS += \
./StartupF407/startup_stm32f407vgtx.d 


# Each subdirectory must supply rules for building sources it contributes
StartupF407/%.o: ../StartupF407/%.s StartupF407/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g -c -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-StartupF407

clean-StartupF407:
	-$(RM) ./StartupF407/startup_stm32f407vgtx.d ./StartupF407/startup_stm32f407vgtx.o

.PHONY: clean-StartupF407

