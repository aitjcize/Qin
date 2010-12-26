#!/bin/bash

make -C qin/src 'DEFINES += -DDEBUG' #'CFLAGS += -g' 'CXXFLAGS += -g'
make -C orion/src

qvfb -width 640 -height 480 &
sleep 2

if [ "$1" == "-d" ]; then
  gnome-terminal -e "gdb qin/src/qin"
else
  qin/src/qin -qws &
fi

orion/src/orion
#./application
