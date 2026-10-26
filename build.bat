@echo off
if not exist build mkdir build

set CPUFLAGS=-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

set PLATFORM=platform\stm32f411\
set INCLUDES=-I kernel -I app ^
-I libraries/cmsis-device-f4/Include ^
-I libraries/CMSIS_6/CMSIS/Core/Include ^
-I %PLATFORM%



for %%f in (kernel\*.c) do (
    arm-none-eabi-gcc %CPUFLAGS% -ffreestanding %INCLUDES% -c "%%f" -o "build\kernel_%%~nf.o"
    if errorlevel 1 exit /b 1
)


for %%f in (app\*.cpp) do (
    arm-none-eabi-g++ %CPUFLAGS% -ffreestanding -fno-exceptions -fno-rtti %INCLUDES% -c "%%f" -o "build\app_%%~nf.o"
    if errorlevel 1 exit /b 1
)

for %%f in (platform\stm32f411\*.c) do (
    arm-none-eabi-gcc %CPUFLAGS% -ffreestanding %INCLUDES% -c "%%f" -o "build\platform_%%~nf.o"
    if errorlevel 1 exit /b 1
)

arm-none-eabi-gcc %CPUFLAGS% -c %PLATFORM%startup.s -o build/startup.o
if errorlevel 1 exit /b 1

arm-none-eabi-g++ %CPUFLAGS% -T %PLATFORM%linker.ld -nostartfiles -Wl,--gc-sections build\*.o -o build\firmware.elf
if errorlevel 1 exit /b 1


arm-none-eabi-objcopy -O binary build/firmware.elf build/firmware.bin
if errorlevel 1 exit /b 1
