#!/bin/bash
img_file="usb.img"

QEMU/qemu-img.exe create -f raw $img_file 1G

sudo parted $img_file mklabel gpt mkpart primary fat32 1M 100% set 1 esp on

sudo mkfs.vfat -v --offset 2048 $img_file

fdisk -l $img_file
