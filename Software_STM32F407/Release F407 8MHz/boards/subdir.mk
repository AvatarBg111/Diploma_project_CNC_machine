################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../boards/Devtronic_CNC_Controller.c \
../boards/btt_skr_2.0.c \
../boards/flexi_hal.c \
../boards/st_morpho.c \
../boards/st_morpo_dac.c 

OBJS += \
./boards/Devtronic_CNC_Controller.o \
./boards/btt_skr_2.0.o \
./boards/flexi_hal.o \
./boards/st_morpho.o \
./boards/st_morpo_dac.o 

C_DEPS += \
./boards/Devtronic_CNC_Controller.d \
./boards/btt_skr_2.0.d \
./boards/flexi_hal.d \
./boards/st_morpho.d \
./boards/st_morpo_dac.d 


# Each subdirectory must supply rules for building sources it contributes
boards/%.o boards/%.su boards/%.cyclo: ../boards/%.c boards/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-boards

clean-boards:
	-$(RM) ./boards/Devtronic_CNC_Controller.cyclo ./boards/Devtronic_CNC_Controller.d ./boards/Devtronic_CNC_Controller.o ./boards/Devtronic_CNC_Controller.su ./boards/btt_skr_2.0.cyclo ./boards/btt_skr_2.0.d ./boards/btt_skr_2.0.o ./boards/btt_skr_2.0.su ./boards/flexi_hal.cyclo ./boards/flexi_hal.d ./boards/flexi_hal.o ./boards/flexi_hal.su ./boards/st_morpho.cyclo ./boards/st_morpho.d ./boards/st_morpho.o ./boards/st_morpho.su ./boards/st_morpo_dac.cyclo ./boards/st_morpo_dac.d ./boards/st_morpo_dac.o ./boards/st_morpo_dac.su

.PHONY: clean-boards

