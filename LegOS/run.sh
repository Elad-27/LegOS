#!/bin/sh
echo "running: "
qemu-system-i386 -kernel myos.bin -display sdl,gl=off -monitor stdio -M isapc \
-device isa-debug-exit,iobase=0xf4,iosize=0x04

