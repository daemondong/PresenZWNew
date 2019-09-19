################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include emdrv/subdir.mk
-include ZAF_CommandClasses_ZWavePlusInfo/subdir.mk
-include ZAF_CommandClasses_Supervision/subdir.mk
-include ZAF_CommandClasses_PowerLevel/subdir.mk
-include ZAF_CommandClasses_MultiChan/subdir.mk
-include ZAF_CommandClasses_ManufacturerSpecific/subdir.mk
-include ZAF_CommandClasses_Indicator/subdir.mk
-include ZAF_CommandClasses_FirmwareUpdate/subdir.mk
-include ZAF_CommandClasses_DeviceResetLocally/subdir.mk
-include ZAF_CommandClasses_BinarySwitch/subdir.mk
-include ZAF_CommandClasses_Basic/subdir.mk
-include ZAF_CommandClasses_Association/subdir.mk
-include ZAF_ApplicationUtilities_TrueStatusEngine/subdir.mk
-include ZAF_ApplicationUtilities/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SwitchOnOff.axf

# Tool invocations
SwitchOnOff.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/Device/SiliconLabs/EFR32FG13P/Source/GCC/efr32fg13p-zw700.ld" -L"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZWave/lib/" -L"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//SubTree/rail-import/platform/radio/rail_lib/autogen/librail_release/" -Xlinker --gc-sections -Xlinker -Map="SwitchOnOff.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o SwitchOnOff.axf -Wl,--start-group "./ZAF_ApplicationUtilities/AppTimer.o" "./ZAF_ApplicationUtilities/ZAF_CmdPublisher.o" "./ZAF_ApplicationUtilities/ZAF_app_version.o" "./ZAF_ApplicationUtilities/ZAF_command_class_utils.o" "./ZAF_ApplicationUtilities/ZAF_network_learn.o" "./ZAF_ApplicationUtilities/ZAF_nvm3_app.o" "./ZAF_ApplicationUtilities/ZAF_tx_mutex.o" "./ZAF_ApplicationUtilities/ZAF_uart_utils.o" "./ZAF_ApplicationUtilities/ZW_TransportEndpoint.o" "./ZAF_ApplicationUtilities/ZW_TransportMulticast.o" "./ZAF_ApplicationUtilities/ZW_TransportSecProtocol.o" "./ZAF_ApplicationUtilities/application_properties.o" "./ZAF_ApplicationUtilities/board.o" "./ZAF_ApplicationUtilities/board_BRD420x.o" "./ZAF_ApplicationUtilities/board_indicator.o" "./ZAF_ApplicationUtilities_TrueStatusEngine/ZAF_TSE.o" "./ZAF_CommandClasses_Association/CC_Association.o" "./ZAF_CommandClasses_Association/CC_AssociationGroupInfo.o" "./ZAF_CommandClasses_Association/CC_MultiChanAssociation.o" "./ZAF_CommandClasses_Association/agi.o" "./ZAF_CommandClasses_Association/association_plus.o" "./ZAF_CommandClasses_Basic/CC_Basic.o" "./ZAF_CommandClasses_Basic/CC_BasicController.o" "./ZAF_CommandClasses_BinarySwitch/CC_BinarySwitch.o" "./ZAF_CommandClasses_DeviceResetLocally/CC_DeviceResetLocally.o" "./ZAF_CommandClasses_FirmwareUpdate/CC_FirmwareUpdate.o" "./ZAF_CommandClasses_FirmwareUpdate/ota_util.o" "./ZAF_CommandClasses_Indicator/CC_Indicator.o" "./ZAF_CommandClasses_ManufacturerSpecific/CC_ManufacturerSpecific.o" "./ZAF_CommandClasses_MultiChan/multichannel.o" "./ZAF_CommandClasses_PowerLevel/CC_PowerLevel.o" "./ZAF_CommandClasses_Supervision/CC_Supervision.o" "./ZAF_CommandClasses_ZWavePlusInfo/CC_ZWavePlusInfo.o" "./emdrv/gpiointerrupt.o" "./emlib/em_letimer.o" "./emlib/em_timer.o" "./src/SwitchOnOff.o" -lZWaveSlave -lrail_efr32xg13_gcc_release -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: SwitchOnOff.hex'
	arm-none-eabi-objcopy -O ihex "SwitchOnOff.axf" "SwitchOnOff.hex"
	@echo ' '

	@echo 'Building bin file: SwitchOnOff.bin'
	arm-none-eabi-objcopy -O binary "SwitchOnOff.axf" "SwitchOnOff.bin"
	@echo ' '

	@echo 'Building s37 file: SwitchOnOff.s37'
	arm-none-eabi-objcopy -O srec "SwitchOnOff.axf" "SwitchOnOff.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "SwitchOnOff.axf"
	@echo ' '

	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SwitchOnOff.axf
	-@echo ' '

post-build:
	-"D:\SiliconLabs\SimplicityStudio\v4\developer\adapter_packs\commander\commander.exe" gbl create "SwitchOnOff.gbl" --app "SwitchOnOff.hex" --sign "D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//BootLoader/sample-keys/sample_sign.key" --encrypt  "D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//BootLoader/sample-keys/sample_encrypt.key"  --compress lz4
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
