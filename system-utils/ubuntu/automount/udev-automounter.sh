#!/bin/sh
#
# USAGE: usb-automounter.sh DEVICE 
#   DEVICE   is the actual device node at /dev/DEVICE

/usr/local/sbin/udev-auto-mount.sh ${1} &
