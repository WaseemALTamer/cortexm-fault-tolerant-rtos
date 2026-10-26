@echo off
if not exist build mkdir build

set CPUFLAGS=-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

arm-none-eabi-gcc %CPUFLAGS% -ffreestanding -I kernel -I app -I platform/stm32f411 -c kernel/main.c -o build/kernel_main.o
if errorlevel 1 exit /b 1

arm-none-eabi-g++ %CPUFLAGS% -ffreestanding -fno-exceptions -fno-rtti -I kernel -I app -I platform/stm32f411 -c app/main.cpp -o build/app_main.o
if errorlevel 1 exit /b 1

arm-none-eabi-gcc %CPUFLAGS% -c platform/stm32f411/startup.s -o build/startup.o
if errorlevel 1 exit /b 1

arm-none-eabi-g++ %CPUFLAGS% -T platform/stm32f411/linker.ld -nostartfiles -Wl,--gc-sections build/startup.o build/kernel_main.o build/app_main.o -o build/firmware.elf
if errorlevel 1 exit /b 1


arm-none-eabi-objcopy -O binary build/firmware.elf build/firmware.bin
if errorlevel 1 exit /b 1
