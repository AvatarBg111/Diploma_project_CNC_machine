################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../spindle/modbus_rtu.c \
../spindle/onoff.c \
../spindle/pwm.c \
../spindle/pwm_clone.c \
../spindle/select.c \
../spindle/stepper.c 

OBJS += \
./spindle/modbus_rtu.o \
./spindle/onoff.o \
./spindle/pwm.o \
./spindle/pwm_clone.o \
./spindle/select.o \
./spindle/stepper.o 

C_DEPS += \
./spindle/modbus_rtu.d \
./spindle/onoff.d \
./spindle/pwm.d \
./spindle/pwm_clone.d \
./spindle/select.d \
./spindle/stepper.d 


# Each subdirectory must supply rules for building sources it contributes
spindle/%.o spindle/%.su spindle/%.cyclo: ../spindle/%.c spindle/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-spindle

clean-spindle:
	-$(RM) ./spindle/modbus_rtu.cyclo ./spindle/modbus_rtu.d ./spindle/modbus_rtu.o ./spindle/modbus_rtu.su ./spindle/onoff.cyclo ./spindle/onoff.d ./spindle/onoff.o ./spindle/onoff.su ./spindle/pwm.cyclo ./spindle/pwm.d ./spindle/pwm.o ./spindle/pwm.su ./spindle/pwm_clone.cyclo ./spindle/pwm_clone.d ./spindle/pwm_clone.o ./spindle/pwm_clone.su ./spindle/select.cyclo ./spindle/select.d ./spindle/select.o ./spindle/select.su ./spindle/stepper.cyclo ./spindle/stepper.d ./spindle/stepper.o ./spindle/stepper.su

.PHONY: clean-spindle

