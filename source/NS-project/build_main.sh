#!/bin/bash
# compile mbed program

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
echo "DIR is '$DIR'"
cd $DIR/../..

# clean bin
rm -rf ./source/NS-project/bin/NS-main
mkdir -p ./source/NS-project/bin/NS-main

# prepare .mbedignore
echo "submodules/nectar_serial/mbed/*
submodules/nectar_serial/.vscode/*
submodules/nectar_serial/test/*
submodules/nectar_serial/.gitignore
submodules/nectar_serial/library.properties
submodules/nectar_serial/README.md

submodules/nucleo_helpers/TESTS/*
submodules/nucleo_helpers/mbed-os/*
submodules/nucleo_helpers/mbed-os.lib
submodules/nucleo_helpers/mbed_settings.py
submodules/nucleo_helpers/.mbed

source/TESTS/
source/cpputest/
source/NS-project/

submodules/device_menu/*

source/src/modules/*
source/src/target-specific/NS-power/*" >> ./.mbedignore

# compile project
mbed compile

# create bin
mv ./BUILD/NUCLEO_F091RC/GCC_ARM/*.csv ./source/NS-project/bin/NS-main/main_board.csv
mv ./BUILD/NUCLEO_F091RC/GCC_ARM/*.html ./source/NS-project/bin/NS-main/main_board.html
mv ./BUILD/NUCLEO_F091RC/GCC_ARM/*.json ./source/NS-project/bin/NS-main/main_board.json
mv ./BUILD/NUCLEO_F091RC/GCC_ARM/*.elf ./source/NS-project/bin/NS-main/main_board.elf

/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-objcopy -O ihex ./source/NS-project/bin/NS-main/main_board.elf ./source/NS-project/bin/NS-main/main_board.hex
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-objcopy -O binary ./source/NS-project/bin/NS-main/main_board.elf ./source/NS-project/bin/NS-main/main_board.bin
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-size ./source/NS-project/bin/NS-main/main_board.elf
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-size ./source/NS-project/bin/NS-main/main_board.hex

# remove BUILD
rm -r BUILD
rm -f ./.mbedignore
