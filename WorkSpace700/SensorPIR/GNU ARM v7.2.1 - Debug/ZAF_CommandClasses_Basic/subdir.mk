################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1/ZAF/CommandClasses/Basic/CC_BasicController.c 

OBJS += \
./ZAF_CommandClasses_Basic/CC_BasicController.o 

C_DEPS += \
./ZAF_CommandClasses_Basic/CC_BasicController.d 


# Each subdirectory must supply rules for building sources it contributes
ZAF_CommandClasses_Basic/CC_BasicController.o: D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1/ZAF/CommandClasses/Basic/CC_BasicController.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DZAF_BUILD_NO=0xAABB' '-DZAF_VERSION_MAJOR=10' '-DZAF_VERSION_MINOR=11' '-DZAF_VERSION_PATCH=0' '-DSDK_VERSION_MAJOR=7' '-DSDK_VERSION_MINOR=11' '-DSDK_VERSION_PATCH=1' '-DZGM130S037HGN1=1' '-DEFR32FG13P231F512GM48=1' '-DEFR32FG=1' '-DZW_SLAVE_ENHANCED_232=1' '-DZW_SLAVE=1' '-DRADIO_BOARD_ZGM130S=1' '-DEXT_BOARD_8029A=1' '-DDISABLE_USART1=0' '-DNO_MULTICHAN_VER=1' -I"D:\GitRep\ZWave6_81\PresenZW\WorkSpace700\SensorPIR\ZAF_CommandClasses_MultiChan_inc" -I"D:\GitRep\ZWave6_81\PresenZW\WorkSpace700\SensorPIR\src" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZWave/API" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1/" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/bootloader" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/bootloader/api" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/bootloader/core" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/CMSIS/Include" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/Device/SiliconLabs/EFR32FG13P/Include" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/emlib/inc" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/emdrv/gpiointerrupt/inc" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/emdrv/sleep/inc" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/halconfig/inc/hal-config" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//util/third_party/freertos/Source/portable/GCC/ARM_CM4F" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/emdrv/nvm3/inc" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//platform/emdrv/common/inc" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//hardware/kit/common/bsp" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//hardware/kit/common/halconfig" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Components/QueueNotifying" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Components/SwTimer" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Components/CRC" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Components/EventDistributor" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Components/DebugPrint" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Components/Assert" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Components/Utils" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Components/SyncEvent" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Drivers/zw700" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Processor/gecko" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//Processor/gecko/hal-config" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ThirdParty/Freertos/include" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ThirdParty/Freertos/include/gecko" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/MultiChan" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/PowerLevel" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/ApplicationUtilities/PowerManagement" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/DeviceResetLocally" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/ManufacturerSpecific" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/Basic" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/Supervision" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/Common" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/FirmwareUpdate" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/ApplicationUtilities" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/Battery" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/ApplicationUtilities/_commonIF" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/WakeUp" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/Association" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/ApplicationUtilities/TrueStatusEngine" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/ApplicationUtilities/EventHandling" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/Version" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/Indicator" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/ZWavePlusInfo" -I"D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/zwave/v7.11.1//ZAF/CommandClasses/Notification" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"ZAF_CommandClasses_Basic/CC_BasicController.d" -MT"ZAF_CommandClasses_Basic/CC_BasicController.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


