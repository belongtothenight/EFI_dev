@echo off
REM Image is mounted to windows at "G:\"
REM Compiled output is at dir "out"

REM Removing previous artifacts
del /s /q G:\
del /s /q out

REM Clean and compile
cd src
make clean
make
cd ..

REM Move output to image
xcopy /y src\startup.nsh G:\
xcopy /y src\OUT.EFI out\
xcopy /y src\OUT.EFI G:\

REM Launch image with loaded binaries
QEMU\qemu-system-x86_64.exe ^
    -bios OVMF\OVMF-pure-efi.fd ^
    -net none ^
    -M q35 ^
    -drive file=usb.img,format=raw,if=none,id=usbstick ^
    -device qemu-xhci,id=xhci ^
    -device usb-storage,drive=usbstick
