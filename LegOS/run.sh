#!/bin/sh
echo "running: "
qemu-system-i386 -kernel myos.bin -display sdl,gl=off -monitor stdio -M isapc

