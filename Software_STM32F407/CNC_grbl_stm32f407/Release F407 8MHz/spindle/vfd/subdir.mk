################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../spindle/vfd/gs20.c \
../spindle/vfd/h100.c \
../spindle/vfd/huanyang.c \
../spindle/vfd/huanyang2.c \
../spindle/vfd/modvfd.c \
../spindle/vfd/spindle.c \
../spindle/vfd/yl620.c 

OBJS += \
./spindle/vfd/gs20.o \
./spindle/vfd/h100.o \
./spindle/vfd/huanyang.o \
./spindle/vfd/huanyang2.o \
./spindle/vfd/modvfd.o \
./spindle/vfd/spindle.o \
./spindle/vfd/yl620.o 

C_DEPS += \
./spindle/vfd/gs20.d \
./spindle/vfd/h100.d \
./spindle/vfd/huanyang.d \
./spindle/vfd/huanyang2.d \
./spindle/vfd/modvfd.d \
./spindle/vfd/spindle.d \
./spindle/vfd/yl620.d 


# Each subdirectory must supply rules for building sources it contributes
spindle/vfd/%.o spindle/vfd/%.su spindle/vfd/%.cyclo: ../spindle/vfd/%.c spindle/vfd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-spindle-2f-vfd

clean-spindle-2f-vfd:
	-$(RM) ./spindle/vfd/gs20.cyclo ./spindle/vfd/gs20.d ./spindle/vfd/gs20.o ./spindle/vfd/gs20.su ./spindle/vfd/h100.cyclo ./spindle/vfd/h100.d ./spindle/vfd/h100.o ./spindle/vfd/h100.su ./spindle/vfd/huanyang.cyclo ./spindle/vfd/huanyang.d ./spindle/vfd/huanyang.o ./spindle/vfd/huanyang.su ./spindle/vfd/huanyang2.cyclo ./spindle/vfd/huanyang2.d ./spindle/vfd/huanyang2.o ./spindle/vfd/huanyang2.su ./spindle/vfd/modvfd.cyclo ./spindle/vfd/modvfd.d ./spindle/vfd/modvfd.o ./spindle/vfd/modvfd.su ./spindle/vfd/spindle.cyclo ./spindle/vfd/spindle.d ./spindle/vfd/spindle.o ./spindle/vfd/spindle.su ./spindle/vfd/yl620.cyclo ./spindle/vfd/yl620.d ./spindle/vfd/yl620.o ./spindle/vfd/yl620.su

.PHONY: clean-spindle-2f-vfd

