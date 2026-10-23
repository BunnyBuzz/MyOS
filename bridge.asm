[bits 32]
[extern _kmain]    ; استدعاء دالة C (مع الشرطة السفلية الخاصة بـ Windows GCC)

call _kmain        ; القفز إلى كود C
jmp $              ; حلقة توقف إذا انتهت الدالة