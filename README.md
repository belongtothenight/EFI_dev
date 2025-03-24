# EFI_dev

1. Get GCC from MSYS or Mingw
2. Install QEMU
3. Download OVMF
4. Create Virtual USB Stick Image
    1. Create image with QEMU-IMG `qemu-img create -f raw usb.img 1G`.
    2. Check first available loopback devices `losetup`. If the last line `/dev/loop1`, then it's `2`.
    3. Mount blank image `sudo losetup loop2 usb.img`. The `2` is from the previous step.
    4. Partitioning/formatting `sudo -H gparted /dev/loop2`
        1. GPT
        2. FAT32
    5. Unmount the image `sudo losetup -d /dev/loop2` (optional)
5. Get EFI spec ABI header files

## Scripts

1. Generate image file: `create_img.sh` (execute in Linux environment, may require additional dependencies)
2. Compile and move file for development in VM: `launch_vm.bat` (required to be executed within MinGW or MSYS environment)
 
## Sources

1. <https://askubuntu.com/questions/667291/create-blank-disk-image-for-file-storage>
2. <https://github.com/queso-fuego/uefi-dev>
