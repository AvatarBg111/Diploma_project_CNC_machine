################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../keypad/keypad.c \
../keypad/macros.c 

OBJS += \
./keypad/keypad.o \
./keypad/macros.o 

C_DEPS += \
./keypad/keypad.d \
./keypad/macros.d 


# Each subdirectory must supply rules for building sources it contributes
keypad/%.o keypad/%.su keypad/%.cyclo: ../keypad/%.c keypad/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-keypad

clean-keypad:
	-$(RM) ./keypad/keypad.cyclo ./keypad/keypad.d ./keypad/keypad.o ./keypad/keypad.su ./keypad/macros.cyclo ./keypad/macros.d ./keypad/macros.o ./keypad/macros.su

.PHONY: clean-keypad

