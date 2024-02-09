################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/MCP3221.c \
../Src/diskio.c \
../Src/driver.c \
../Src/flash.c \
../Src/i2c.c \
../Src/ioports.c \
../Src/ioports_analog.c \
../Src/main.c \
../Src/neopixel_spi.c \
../Src/serial.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c \
../Src/tmc_spi.c \
../Src/tmc_uart.c \
../Src/usb_serial.c \
../Src/w5x00_ll_driver.c 

OBJS += \
./Src/MCP3221.o \
./Src/diskio.o \
./Src/driver.o \
./Src/flash.o \
./Src/i2c.o \
./Src/ioports.o \
./Src/ioports_analog.o \
./Src/main.o \
./Src/neopixel_spi.o \
./Src/serial.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o \
./Src/tmc_spi.o \
./Src/tmc_uart.o \
./Src/usb_serial.o \
./Src/w5x00_ll_driver.o 

C_DEPS += \
./Src/MCP3221.d \
./Src/diskio.d \
./Src/driver.d \
./Src/flash.d \
./Src/i2c.d \
./Src/ioports.d \
./Src/ioports_analog.d \
./Src/main.d \
./Src/neopixel_spi.d \
./Src/serial.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d \
./Src/tmc_spi.d \
./Src/tmc_uart.d \
./Src/usb_serial.d \
./Src/w5x00_ll_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/MCP3221.cyclo ./Src/MCP3221.d ./Src/MCP3221.o ./Src/MCP3221.su ./Src/diskio.cyclo ./Src/diskio.d ./Src/diskio.o ./Src/diskio.su ./Src/driver.cyclo ./Src/driver.d ./Src/driver.o ./Src/driver.su ./Src/flash.cyclo ./Src/flash.d ./Src/flash.o ./Src/flash.su ./Src/i2c.cyclo ./Src/i2c.d ./Src/i2c.o ./Src/i2c.su ./Src/ioports.cyclo ./Src/ioports.d ./Src/ioports.o ./Src/ioports.su ./Src/ioports_analog.cyclo ./Src/ioports_analog.d ./Src/ioports_analog.o ./Src/ioports_analog.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/neopixel_spi.cyclo ./Src/neopixel_spi.d ./Src/neopixel_spi.o ./Src/neopixel_spi.su ./Src/serial.cyclo ./Src/serial.d ./Src/serial.o ./Src/serial.su ./Src/spi.cyclo ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/stm32f4xx_hal_msp.cyclo ./Src/stm32f4xx_hal_msp.d ./Src/stm32f4xx_hal_msp.o ./Src/stm32f4xx_hal_msp.su ./Src/stm32f4xx_it.cyclo ./Src/stm32f4xx_it.d ./Src/stm32f4xx_it.o ./Src/stm32f4xx_it.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32f4xx.cyclo ./Src/system_stm32f4xx.d ./Src/system_stm32f4xx.o ./Src/system_stm32f4xx.su ./Src/tmc_spi.cyclo ./Src/tmc_spi.d ./Src/tmc_spi.o ./Src/tmc_spi.su ./Src/tmc_uart.cyclo ./Src/tmc_uart.d ./Src/tmc_uart.o ./Src/tmc_uart.su ./Src/usb_serial.cyclo ./Src/usb_serial.d ./Src/usb_serial.o ./Src/usb_serial.su ./Src/w5x00_ll_driver.cyclo ./Src/w5x00_ll_driver.d ./Src/w5x00_ll_driver.o ./Src/w5x00_ll_driver.su

.PHONY: clean-Src

