#!/bin/bash

# run_bochs.sh
# mounts the correct loopback device, runs bochs, then unmounts.

sudo /sbin/losetup /dev/loop1 floppy.img
# -f : specify config file
# -q : skip initial menu
sudo bochs -f bochsrc.txt -q
sudo /sbin/losetup -d /dev/loop1

