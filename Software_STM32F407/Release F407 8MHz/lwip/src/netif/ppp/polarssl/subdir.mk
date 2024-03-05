################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/netif/ppp/polarssl/arc4.c \
../lwip/src/netif/ppp/polarssl/des.c \
../lwip/src/netif/ppp/polarssl/md4.c \
../lwip/src/netif/ppp/polarssl/md5.c \
../lwip/src/netif/ppp/polarssl/sha1.c 

OBJS += \
./lwip/src/netif/ppp/polarssl/arc4.o \
./lwip/src/netif/ppp/polarssl/des.o \
./lwip/src/netif/ppp/polarssl/md4.o \
./lwip/src/netif/ppp/polarssl/md5.o \
./lwip/src/netif/ppp/polarssl/sha1.o 

C_DEPS += \
./lwip/src/netif/ppp/polarssl/arc4.d \
./lwip/src/netif/ppp/polarssl/des.d \
./lwip/src/netif/ppp/polarssl/md4.d \
./lwip/src/netif/ppp/polarssl/md5.d \
./lwip/src/netif/ppp/polarssl/sha1.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/netif/ppp/polarssl/%.o lwip/src/netif/ppp/polarssl/%.su lwip/src/netif/ppp/polarssl/%.cyclo: ../lwip/src/netif/ppp/polarssl/%.c lwip/src/netif/ppp/polarssl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl

clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl:
	-$(RM) ./lwip/src/netif/ppp/polarssl/arc4.cyclo ./lwip/src/netif/ppp/polarssl/arc4.d ./lwip/src/netif/ppp/polarssl/arc4.o ./lwip/src/netif/ppp/polarssl/arc4.su ./lwip/src/netif/ppp/polarssl/des.cyclo ./lwip/src/netif/ppp/polarssl/des.d ./lwip/src/netif/ppp/polarssl/des.o ./lwip/src/netif/ppp/polarssl/des.su ./lwip/src/netif/ppp/polarssl/md4.cyclo ./lwip/src/netif/ppp/polarssl/md4.d ./lwip/src/netif/ppp/polarssl/md4.o ./lwip/src/netif/ppp/polarssl/md4.su ./lwip/src/netif/ppp/polarssl/md5.cyclo ./lwip/src/netif/ppp/polarssl/md5.d ./lwip/src/netif/ppp/polarssl/md5.o ./lwip/src/netif/ppp/polarssl/md5.su ./lwip/src/netif/ppp/polarssl/sha1.cyclo ./lwip/src/netif/ppp/polarssl/sha1.d ./lwip/src/netif/ppp/polarssl/sha1.o ./lwip/src/netif/ppp/polarssl/sha1.su

.PHONY: clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl

