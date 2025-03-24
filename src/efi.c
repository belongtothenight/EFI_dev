#include "efi.h"
#include "efi_lib.h"

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    (void)ImageHandle;
    EFI_STATUS Status = EFI_SUCCESS;
    BOOLEAN Verbose = TRUE;
    //SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_YELLOW,EFI_GREEN));
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Hello, World!\r\n\r\n");

    //Read from USB
    if (Status == EFI_SUCCESS) {
        //Status = read_handle_count(SystemTable, &HandleCount, HandleBuffer);
        Status = read_file(SystemTable, Verbose);
    }

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Press any key to shutdown...");
    EFI_INPUT_KEY key;
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) != EFI_SUCCESS);

    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, Status, 0, NULL);
    return EFI_SUCCESS;
}
