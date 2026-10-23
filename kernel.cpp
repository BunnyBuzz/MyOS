#define VGA_ADDRESS 0xB8000

class KernelScreen {
private:
    char* vga = (char*)VGA_ADDRESS;
    int width = 80;
    int height = 25;
    int col = 0;
    int row = 0;

public:
    void clear() {
        for (int i = 0; i < width * height; i++) {
            vga[i * 2]     = ' ';
            vga[i * 2 + 1] = 0x07;
        }
        col = 0;
        row = 0;
    }

    void put_char(char c, char color = 0x0F) {
        if (c == '\n') {
            col = 0;
            row++;
        } else {
            int offset = (row * width + col) * 2;
            vga[offset] = c;
            vga[offset + 1] = color;
            col++;
            if (col >= width) {
                col = 0;
                row++;
            }
        }
    }

    void print(const char* text, char color = 0x0F) {
        for (int i = 0; text[i] != '\0'; i++) {
            put_char(text[i], color);
        }
    }
};

const char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,
    '*',
    0,
    ' ',
};

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

char get_char(void) {
    while ((inb(0x64) & 1) == 0);
    unsigned char code = inb(0x60);
    if (code < 128) {
        return keyboard_map[code];
    }
    return 0;
}

KernelScreen screen;

extern "C" void kmain(void) {
    screen.clear();

    screen.print("=========================================\n", 0x0B);
    screen.print("        Welcome to My 32-bit OS!         \n", 0x0A);
    screen.print("=========================================\n\n", 0x0B);

    screen.print("System initialized successfully in C++.\n", 0x0F);
    screen.print("Type anything on your keyboard:\n> ", 0x0E);

    while (1) {
        char c = get_char();
        if (c != 0) {
            screen.put_char(c, 0x0A);
        }
    }
}
