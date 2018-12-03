#!/bin/bash


HEADERS="/home/zawadeusz/Documents/Programowanie/Programy/ftp/src/headers/"
SOURCES="/home/zawadeusz/Documents/Programowanie/Programy/ftp/src/sources/"
MAIN="/home/zawadeusz/Documents/Programowanie/Programy/ftp/src/main.cpp"

PP=""

for file in $SOURCES*; do
	PP="$PP $file"
done


g++ -std=c++11 -I $HEADERS -Wall -Wextra main.cpp $PP -lcrypt && ./a.out
