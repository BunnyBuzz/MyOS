[org 0x7c00]

    ; 1. حفظ رقم القرص وقراءة النواة (15 قطاعاً) إلى الذاكرة عند 0x1000
    mov [BOOT_DRIVE], dl

    mov ah, 0x02            ; خدمة قراءة القرص (int 0x13)
    mov al, 15              ; عدد القطاعات المراد قراءتها
    mov ch, 0               ; الأسطوانة 0
    mov cl, 2               ; البدء من القطاع 2 (الذي بعد البوت لودر مباشرة)
    mov dh, 0               ; الرأس 0
    mov dl, [BOOT_DRIVE]    ; رقم القرص
    mov bx, 0x1000          ; تحميل النواة عند العنوان 0x1000 في الرام
    int 0x13                ; تنفيذ القراءة

    ; 2. التحويل إلى الوضع المحمي (32-bit Protected Mode)
    cli                     ; إيقاف المقاطعات
    lgdt [gdt_descriptor]   ; تحميل جدول الذاكرة GDT

    mov eax, cr0
    or eax, 0x1             ; تفعيل بت الحماية 32-bit
    mov cr0, eax

    jmp CODE_SEG:init_pm    ; قفزة بعيدة لتفريغ طابور المعالج

[bits 32]
init_pm:
    ; تهيئة مسجلات البيانات للـ 32-bit
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x90000        ; إعداد المكدس (Stack)

    call 0x1000             ; القفز فوراً لتشغيل النواة عند 0x1000!
    jmp $

BOOT_DRIVE db 0

; --- جدول GDT القياسي ---
gdt_start:
    dd 0x0, 0x0
gdt_code:
    dw 0xffff, 0x0
    db 0x0, 10011010b, 11001111b, 0x0
gdt_data:
    dw 0xffff, 0x0
    db 0x0, 10010010b, 11001111b, 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; حشو الـ 512 بايت والتوقيع السحري
times 510-($-$$) db 0
dw 0xaa55