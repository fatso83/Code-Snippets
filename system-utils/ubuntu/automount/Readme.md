# Auto-mount and unmount usb disks based on disk labels

These scripts are basically ripped from several answers
on StackOverflow. The information was spread over several
answers, and some were a bit outdated, so I tried to 
compile all the information and put it here.

The scripts have been found to be working on Ubuntu 10.10
If they do not work for you, try to see if there are tips
in the original SO discussion (see below).
You can then pass me that info, and maybe I will integrate
it into a specific patch.

All scripts are to be put in /usr/local/sbin
The udev rules are to be put in /etc/udev/rules.d/

For reference, all info was ripped from here:
http://superuser.com/questions/53978/ubuntu-automatically-mount-external-drives-to-media-label-on-boot-without-a-u?lq=1
