#!/bin/bash
# compile mbed program

cd ./../../

# clean bin
rm -rf ./source/NS-project/bin/NS-power
mkdir -p ./source/NS-project/bin/NS-power

# check if required software exists
if command -v mbed-cli; then echo "mbed-cli found"; fi
if command -v mbed; then echo "mbed found"; fi

# compile project
mbed compile

# create bin
mv ./BUILD/NUCLEO_F091RC/GCC_ARM/*.csv ./source/NS-project/bin/NS-power/
mv ./BUILD/NUCLEO_F091RC/GCC_ARM/*.html ./source/NS-project/bin/NS-power/
mv ./BUILD/NUCLEO_F091RC/GCC_ARM/*.json ./source/NS-project/bin/NS-power/
mv ./BUILD/NUCLEO_F091RC/GCC_ARM/*.elf ./source/NS-project/bin/NS-power/
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-objcopy -O ihex ./source/NS-project/bin/NS-power/power_board.elf ./source/NS-project/bin/NS-power/power_board.hex
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-objcopy -O binary ./source/NS-project/bin/NS-power/power_board.elf ./source/NS-project/bin/NS-power/power_board.bin
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-size ./source/NS-project/bin/NS-power/power_board.elf
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-size ./source/NS-project/bin/NS-power/power_board.hex

# remove BUILD
# rm -r BUILD
