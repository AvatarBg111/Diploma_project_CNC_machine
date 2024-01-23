################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../networking/wiznet/enet.c \
../networking/wiznet/socket.c \
../networking/wiznet/wizchip_conf.c 

OBJS += \
./networking/wiznet/enet.o \
./networking/wiznet/socket.o \
./networking/wiznet/wizchip_conf.o 

C_DEPS += \
./networking/wiznet/enet.d \
./networking/wiznet/socket.d \
./networking/wiznet/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
networking/wiznet/%.o networking/wiznet/%.su networking/wiznet/%.cyclo: ../networking/wiznet/%.c networking/wiznet/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-networking-2f-wiznet

clean-networking-2f-wiznet:
	-$(RM) ./networking/wiznet/enet.cyclo ./networking/wiznet/enet.d ./networking/wiznet/enet.o ./networking/wiznet/enet.su ./networking/wiznet/socket.cyclo ./networking/wiznet/socket.d ./networking/wiznet/socket.o ./networking/wiznet/socket.su ./networking/wiznet/wizchip_conf.cyclo ./networking/wiznet/wizchip_conf.d ./networking/wiznet/wizchip_conf.o ./networking/wiznet/wizchip_conf.su

.PHONY: clean-networking-2f-wiznet

