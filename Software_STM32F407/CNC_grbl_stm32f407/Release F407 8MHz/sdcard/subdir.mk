################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sdcard/fs_fatfs.c \
../sdcard/fs_littlefs.c \
../sdcard/macros.c \
../sdcard/sdcard.c \
../sdcard/ymodem.c 

OBJS += \
./sdcard/fs_fatfs.o \
./sdcard/fs_littlefs.o \
./sdcard/macros.o \
./sdcard/sdcard.o \
./sdcard/ymodem.o 

C_DEPS += \
./sdcard/fs_fatfs.d \
./sdcard/fs_littlefs.d \
./sdcard/macros.d \
./sdcard/sdcard.d \
./sdcard/ymodem.d 


# Each subdirectory must supply rules for building sources it contributes
sdcard/%.o sdcard/%.su sdcard/%.cyclo: ../sdcard/%.c sdcard/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-sdcard

clean-sdcard:
	-$(RM) ./sdcard/fs_fatfs.cyclo ./sdcard/fs_fatfs.d ./sdcard/fs_fatfs.o ./sdcard/fs_fatfs.su ./sdcard/fs_littlefs.cyclo ./sdcard/fs_littlefs.d ./sdcard/fs_littlefs.o ./sdcard/fs_littlefs.su ./sdcard/macros.cyclo ./sdcard/macros.d ./sdcard/macros.o ./sdcard/macros.su ./sdcard/sdcard.cyclo ./sdcard/sdcard.d ./sdcard/sdcard.o ./sdcard/sdcard.su ./sdcard/ymodem.cyclo ./sdcard/ymodem.d ./sdcard/ymodem.o ./sdcard/ymodem.su

.PHONY: clean-sdcard

