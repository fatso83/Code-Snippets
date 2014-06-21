#!/bin/sh

sudo mp3fs /gigantdisk/musikkarkiv,128 /mnt/mp3/ -f -o allow_other,ro
sudo umount /mnt/mp3fs_flac
