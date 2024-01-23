################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../networking/base64.c \
../networking/cJSON.c \
../networking/fs_ram.c \
../networking/fs_stream.c \
../networking/ftpd.c \
../networking/http_upload.c \
../networking/httpd.c \
../networking/mqtt.c \
../networking/multipartparser.c \
../networking/networking.c \
../networking/sfifo.c \
../networking/sha1.c \
../networking/ssdp.c \
../networking/strutils.c \
../networking/telnetd.c \
../networking/urldecode.c \
../networking/urlencode.c \
../networking/utils.c \
../networking/webdav.c \
../networking/websocketd.c 

OBJS += \
./networking/base64.o \
./networking/cJSON.o \
./networking/fs_ram.o \
./networking/fs_stream.o \
./networking/ftpd.o \
./networking/http_upload.o \
./networking/httpd.o \
./networking/mqtt.o \
./networking/multipartparser.o \
./networking/networking.o \
./networking/sfifo.o \
./networking/sha1.o \
./networking/ssdp.o \
./networking/strutils.o \
./networking/telnetd.o \
./networking/urldecode.o \
./networking/urlencode.o \
./networking/utils.o \
./networking/webdav.o \
./networking/websocketd.o 

C_DEPS += \
./networking/base64.d \
./networking/cJSON.d \
./networking/fs_ram.d \
./networking/fs_stream.d \
./networking/ftpd.d \
./networking/http_upload.d \
./networking/httpd.d \
./networking/mqtt.d \
./networking/multipartparser.d \
./networking/networking.d \
./networking/sfifo.d \
./networking/sha1.d \
./networking/ssdp.d \
./networking/strutils.d \
./networking/telnetd.d \
./networking/urldecode.d \
./networking/urlencode.d \
./networking/utils.d \
./networking/webdav.d \
./networking/websocketd.d 


# Each subdirectory must supply rules for building sources it contributes
networking/%.o networking/%.su networking/%.cyclo: ../networking/%.c networking/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DHSE_VALUE=8000000 -D_VOLUMES=1 -D_USE_IOCTL=1 -D_USE_WRITE=1 -c -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/lwip/src/include" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/networking/wiznet" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/FatFs/STM" -I"E:/Projects/STM32CubeIDE/CNC_grbl_stm32f407/Drivers/FATFS/Target" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-networking

clean-networking:
	-$(RM) ./networking/base64.cyclo ./networking/base64.d ./networking/base64.o ./networking/base64.su ./networking/cJSON.cyclo ./networking/cJSON.d ./networking/cJSON.o ./networking/cJSON.su ./networking/fs_ram.cyclo ./networking/fs_ram.d ./networking/fs_ram.o ./networking/fs_ram.su ./networking/fs_stream.cyclo ./networking/fs_stream.d ./networking/fs_stream.o ./networking/fs_stream.su ./networking/ftpd.cyclo ./networking/ftpd.d ./networking/ftpd.o ./networking/ftpd.su ./networking/http_upload.cyclo ./networking/http_upload.d ./networking/http_upload.o ./networking/http_upload.su ./networking/httpd.cyclo ./networking/httpd.d ./networking/httpd.o ./networking/httpd.su ./networking/mqtt.cyclo ./networking/mqtt.d ./networking/mqtt.o ./networking/mqtt.su ./networking/multipartparser.cyclo ./networking/multipartparser.d ./networking/multipartparser.o ./networking/multipartparser.su ./networking/networking.cyclo ./networking/networking.d ./networking/networking.o ./networking/networking.su ./networking/sfifo.cyclo ./networking/sfifo.d ./networking/sfifo.o ./networking/sfifo.su ./networking/sha1.cyclo ./networking/sha1.d ./networking/sha1.o ./networking/sha1.su ./networking/ssdp.cyclo ./networking/ssdp.d ./networking/ssdp.o ./networking/ssdp.su ./networking/strutils.cyclo ./networking/strutils.d ./networking/strutils.o ./networking/strutils.su ./networking/telnetd.cyclo ./networking/telnetd.d ./networking/telnetd.o ./networking/telnetd.su ./networking/urldecode.cyclo ./networking/urldecode.d ./networking/urldecode.o ./networking/urldecode.su ./networking/urlencode.cyclo ./networking/urlencode.d ./networking/urlencode.o ./networking/urlencode.su ./networking/utils.cyclo ./networking/utils.d ./networking/utils.o ./networking/utils.su ./networking/webdav.cyclo ./networking/webdav.d ./networking/webdav.o ./networking/webdav.su ./networking/websocketd.cyclo ./networking/websocketd.d ./networking/websocketd.o ./networking/websocketd.su

.PHONY: clean-networking

