#!/bin/sh
#
# USAGE: usb-autounmounter.sh DEVICE 
#   DEVICE   is the actual device node at /dev/DEVICE

/usr/local/sbin/udev-auto-unmount.sh ${1} &
