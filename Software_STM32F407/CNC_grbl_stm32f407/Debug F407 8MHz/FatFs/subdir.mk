################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FatFs/ff.c \
../FatFs/ffsystem.c \
../FatFs/ffunicode.c 

OBJS += \
./FatFs/ff.o \
./FatFs/ffsystem.o \
./FatFs/ffunicode.o 

C_DEPS += \
./FatFs/ff.d \
./FatFs/ffsystem.d \
./FatFs/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
FatFs/%.o FatFs/%.su FatFs/%.cyclo: ../FatFs/%.c FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FatFs

clean-FatFs:
	-$(RM) ./FatFs/ff.cyclo ./FatFs/ff.d ./FatFs/ff.o ./FatFs/ff.su ./FatFs/ffsystem.cyclo ./FatFs/ffsystem.d ./FatFs/ffsystem.o ./FatFs/ffsystem.su ./FatFs/ffunicode.cyclo ./FatFs/ffunicode.d ./FatFs/ffunicode.o ./FatFs/ffunicode.su

.PHONY: clean-FatFs

