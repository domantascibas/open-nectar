#!/bin/bash
# compile mbed program

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
echo "DIR is '$DIR'"
cd $DIR/../..

# clean bin
rm -rf ./source/NS-project/bin/NS-main
mkdir -p ./source/NS-project/bin/NS-main

# source/TESTS/
# source/cpputest/
# source/NS-project/

# prepare .mbedignore
echo "
mbed-os/components/*
mbed-os/features/*

submodules/nectar_serial/mbed/*
submodules/nectar_serial/.vscode/*
submodules/nectar_serial/test/*
submodules/nectar_serial/.gitignore
submodules/nectar_serial/library.properties
submodules/nectar_serial/README.md

source/src/target-specific/NS-main/device_menu/src/Screens/00/*
source/src/target-specific/NS-main/device_menu/src/Screens/01/*
source/src/target-specific/NS-main/device_menu/src/Screens/02/*
source/src/target-specific/NS-main/device_menu/src/Screens/engineering_menu/*
source/src/target-specific/NS-main/device_menu/src/Screens/error/*

source/src/modules/flash-storage/*
source/src/modules/generator-controller/*
source/src/modules/temperature-controller/*
source/src/target-specific/NS-power/*
" >> ./.mbedignore

# compile project
mbed compile

# create bin
cp ./BUILD/NUCLEO_F091RC/GCC_ARM/*.csv ./source/NS-project/bin/NS-main/main_board.csv
cp ./BUILD/NUCLEO_F091RC/GCC_ARM/*.html ./source/NS-project/bin/NS-main/main_board.html
cp ./BUILD/NUCLEO_F091RC/GCC_ARM/*.json ./source/NS-project/bin/NS-main/main_board.json
cp ./BUILD/NUCLEO_F091RC/GCC_ARM/*.elf ./source/NS-project/bin/NS-main/main_board.elf

/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-objcopy -O ihex ./source/NS-project/bin/NS-main/main_board.elf ./source/NS-project/bin/NS-main/main_board.hex
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-objcopy -O binary ./source/NS-project/bin/NS-main/main_board.elf ./source/NS-project/bin/NS-main/main_board.bin
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-size ./source/NS-project/bin/NS-main/main_board.elf
/opt/xPacks/arm-none-eabi-gcc/8.3.1-1.3/bin/arm-none-eabi-size ./source/NS-project/bin/NS-main/main_board.hex
