################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include trinamic/subdir.mk
-include spindle/vfd/subdir.mk
-include spindle/subdir.mk
-include sdcard/subdir.mk
-include odometer/subdir.mk
-include networking/wiznet/port/subdir.mk
-include networking/wiznet/arch/subdir.mk
-include networking/wiznet/W5500/subdir.mk
-include networking/wiznet/W5100S/subdir.mk
-include networking/wiznet/subdir.mk
-include networking/modbus/subdir.mk
-include networking/subdir.mk
-include motors/subdir.mk
-include lwip/src/netif/ppp/polarssl/subdir.mk
-include lwip/src/netif/ppp/subdir.mk
-include lwip/src/netif/subdir.mk
-include lwip/src/core/ipv6/subdir.mk
-include lwip/src/core/ipv4/subdir.mk
-include lwip/src/core/subdir.mk
-include lwip/src/apps/mqtt/subdir.mk
-include lwip/src/apps/mdns/subdir.mk
-include lwip/src/apps/altcp_tls/subdir.mk
-include lwip/src/api/subdir.mk
-include laser/subdir.mk
-include keypad/display/subdir.mk
-include keypad/subdir.mk
-include grbl/kinematics/subdir.mk
-include grbl/subdir.mk
-include eeprom/subdir.mk
-include bluetooth/subdir.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include StartupF407/subdir.mk
-include Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include FatFs/STM/subdir.mk
-include FatFs/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/FATFS/Target/subdir.mk
-include Drivers/FATFS/App/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := GRBL Driver STM32F401
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
GRBL\ Driver\ STM32F401.elf \

MAP_FILES += \
GRBL\ Driver\ STM32F401.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
GRBL\ Driver\ STM32F401.list \


# All Target
all: main-build

# Main-build Target
main-build: GRBL\ Driver\ STM32F401.elf secondary-outputs

# Tool invocations
GRBL\ Driver\ STM32F401.elf GRBL\ Driver\ STM32F401.map: $(OBJS) $(USER_OBJS) E:\Projects\STM32CubeIDE\CNC_grbl_stm32f407\STM32F407VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "GRBL Driver STM32F401.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"E:\Projects\STM32CubeIDE\CNC_grbl_stm32f407\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="GRBL Driver STM32F401.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

GRBL\ Driver\ STM32F401.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "GRBL Driver STM32F401.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) GRBL\ Driver\ STM32F401.elf GRBL\ Driver\ STM32F401.list GRBL\ Driver\ STM32F401.map GRBL\\ Driver\\ STM32F401.elf default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
