.POSIX:

CFLAGS ::= \
	-std=c17 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-mno-red-zone \
	-ffreestanding \
	-nostdlib

LDFLAGS ::= \
	-Wl,--subsystem,10 \
	-Wl,-entry,efi_main \
	-nostdlib

EFISRC ::= \
	efi.c

#EFICC ::= gcc
EFICC ::= x86_64-w64-mingw32-gcc

OUT.EFI: $(EFISRC)
	$(EFICC) $(CFLAGS) -c -o efi.o efi.c
	$(EFICC) $(LDFLAGS) -o OUT.EFI efi.o

clean:
	rm -rf *.efi *.EFI *.o *.d
