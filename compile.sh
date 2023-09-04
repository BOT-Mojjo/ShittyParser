#!/bin/bash
force=$1
cc main.cpp -o2 -o prototype.exe
if ! [[ $force ]]
then
    ./prototype.exe
fi
