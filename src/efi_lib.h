#include "efi.h"

#define USB_RECORD_BUFFER_SIZE 1024
#define UINT_2_CHAR16_BUFFER_SIZE 20

void UInt2Char16(CHAR16 *Buffer, UINTN Value) {
    CHAR16 Temp[UINT_2_CHAR16_BUFFER_SIZE];  // Temporary buffer for reversed number
    UINTN Index = 0;
    
    if (Value == 0) {
        Buffer[0] = u'0';
        Buffer[1] = u'\0';
        return;
    }

    // Convert number to string (in reverse order)
    while (Value > 0) {
        Temp[Index++] = (Value % 10) + L'0';
        Value /= 10;
    }

    // Reverse the string
    for (UINTN i = 0; i < Index; i++) {
        Buffer[i] = Temp[Index - i - 1];
    }
    Buffer[Index] = u'\0'; // Null-terminate
}

/* @return EFI_SUCCESS: Opened & Read content successfully
 * @return != EFI_SUCCESS: Failed, resume to next step
 */
EFI_STATUS read_file(EFI_SYSTEM_TABLE *SystemTable, BOOLEAN Verbose) {
    EFI_STATUS Status = EFI_SUCCESS; // TODO Handle what different status means
    EFI_HANDLE *HandleBuffer;
    EFI_GUID sfs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem = NULL;
    EFI_FILE_PROTOCOL *Root;
    EFI_FILE_PROTOCOL *File;
    UINTN HandleCount = 9999;
    UINTN BufferSize = USB_RECORD_BUFFER_SIZE;
    CHAR16 Buffer[USB_RECORD_BUFFER_SIZE];

    // ==================================================
    // Locate all handles that support Simple File System
    if (Status != EFI_SUCCESS) {
        Status = SystemTable->BootServices->LocateHandleBuffer(ByProtocol, &sfs_guid, NULL, &HandleCount, &HandleBuffer);
    }
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Locating handle buffer failed\r\n");
        goto close_function;
    } else if (Verbose == TRUE) {
        CHAR16 HandleCountStr[UINT_2_CHAR16_BUFFER_SIZE];
        UInt2Char16(HandleCountStr, HandleCount);
        //UInt2Char16(HandleCountStr, 10000000000000);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Found handle count: ");
        SystemTable->ConOut->OutputString(SystemTable->ConOut, HandleCountStr);
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
    }

    // ======================
    // Select USB file system
    // Currently assume the last handle corresponds to file system of USB
    // TODO need to select by type
    if (Status != EFI_SUCCESS) {
        Status = SystemTable->BootServices->HandleProtocol(HandleBuffer[0], &sfs_guid, (VOID**)&SimpleFileSystem);
    }
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Selecting USB file system failed\r\n");
        goto close_function;
    } else if (Verbose == TRUE) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Selected USB file system\r\n");
    }

    // =======================
    // Open the root directory
    if (Status != EFI_SUCCESS) {
        Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);
    }
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Opening root directory failed\r\n");
        goto close_function;
    } else if (Verbose == TRUE) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Opened root directory\r\n");
    }

    // ====================
    // Open the record file
    if (Status != EFI_SUCCESS) {
        Status = Root->Open(Root, &File, u".record", EFI_FILE_MODE_READ, 0);
    }
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Opening record file failed\r\n");
        goto close_root;
    } else if (Verbose == TRUE) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Opened record file\r\n");
    }

    // Read the file contents
    if (Status != EFI_SUCCESS) {
        Status = File->Read(File, &BufferSize, Buffer);
    }
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Reading file contents failed\r\n");
        goto close_file;
    } else if (Verbose == TRUE) {
        Buffer[BufferSize] = '\0';  // Ensure NULL termination
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"File contents:\r\n");
        SystemTable->ConOut->OutputString(SystemTable->ConOut, Buffer);
    }

    // Close the file and root
close_file:
    File->Close(File);
close_root:
    Root->Close(Root);
close_function:
    return Status;
}
