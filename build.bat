arm-none-eabi-gcc -I kernel -I firmware -c kernel/*.c -o build/kernel.o
arm-none-eabi-g++ -I kernel -I firmware -c firmware/*.cpp -o build/firmware.o
arm-none-eabi-g++ build/*.o -o build/firmware.elf