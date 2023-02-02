#!/bin/sh
pacman --noconfirm --needed -Sy \
    make git \
    mingw-w64-x86_64-arm-none-eabi-gcc \
    mingw-w64-x86_64-arm-none-eabi-gdb \
    mingw-w64-x86_64-arm-none-eabi-newlib \
    mingw-w64-x86_64-arm-none-eabi-binutils \
    mingw-w64-x86_64-stlink \
    mingw-w64-x86_64-gdb-multiarch \
    python python-pip

pip3 install compiledb

printf "\n\n\ninstalling packages done!"
