#include "Uefi.h"

int cmdcheck(const CHAR16 *s1, const CHAR16 *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
        return *s1 - *s2;
    }
void run_shell(EFI_SYSTEM_TABLE *SystemTable) {
    EFI_INPUT_KEY Key;
    UINTN index;
    CHAR16 buffer[100];
    UINTN buf_idx = 0;



    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x0f);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Welcome to MyOS Shell!\r\nMyOS> ");
    
    while (1) {
    SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &index);
        if (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) == 0) {
            if (Key.UnicodeChar == L'\r') {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
                buffer[buf_idx] = 0;
                if (cmdcheck(buffer,L"clear") == 0){
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\nMyOS> ");
                } else if (cmdcheck(buffer,L"help") == 0) {
                    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Available commands: 'help' | 'clear'\r\nMyos> ");
                } else if (buf_idx > 0){
                    SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Command Unknown try 'help'\r\nMyos> ");
                } else {
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"MyOS> ");
                }
                buf_idx = 0;
            } else if (Key.UnicodeChar != 0) {
                CHAR16 buf[2] = { Key.UnicodeChar, 0 };
                SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
                buffer[buf_idx] = Key.UnicodeChar;
                if(buf_idx < 99){
                buf_idx++;
                }
            }
        }
            }
        }


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    run_shell(SystemTable);

    return 0;
}