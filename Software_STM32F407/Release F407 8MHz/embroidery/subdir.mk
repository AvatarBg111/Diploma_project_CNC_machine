################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../embroidery/brother.c \
../embroidery/embroidery.c \
../embroidery/tajima.c 

OBJS += \
./embroidery/brother.o \
./embroidery/embroidery.o \
./embroidery/tajima.o 

C_DEPS += \
./embroidery/brother.d \
./embroidery/embroidery.d \
./embroidery/tajima.d 


# Each subdirectory must supply rules for building sources it contributes
embroidery/%.o embroidery/%.su embroidery/%.cyclo: ../embroidery/%.c embroidery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-embroidery

clean-embroidery:
	-$(RM) ./embroidery/brother.cyclo ./embroidery/brother.d ./embroidery/brother.o ./embroidery/brother.su ./embroidery/embroidery.cyclo ./embroidery/embroidery.d ./embroidery/embroidery.o ./embroidery/embroidery.su ./embroidery/tajima.cyclo ./embroidery/tajima.d ./embroidery/tajima.o ./embroidery/tajima.su

.PHONY: clean-embroidery

