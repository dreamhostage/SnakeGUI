#!/bin/bash

mkdir build
cd build
cmake ../Snake
make -j$(($(nproc)-2))
cd ../Snake/libs
echo -e "\e[1;31m \nNeed permission for copying libopenal.so.1 to /usr/lib/x86_64-linux-gnu\n\e[0m"
sudo cp libopenal.so.1 /usr/lib/x86_64-linux-gnu
cd ../
cd ../build
mv Snake ../
cd ../
./Snake
