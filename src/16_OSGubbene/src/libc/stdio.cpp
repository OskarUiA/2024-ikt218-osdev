#include "../../include/libc/stdio.h"


// Define memory-mapped I/O address for VGA text mode buffer
volatile unsigned short* VGA_BUFFER = (volatile unsigned short*)0xB8000;

// VGA color codes
enum vga_colors {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GRAY = 7,
    VGA_COLOR_DARK_GRAY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Creates a vga entry which is used to print a character to the terminal 
// using vga text mode buffer with set color and background color
unsigned short vga_entry(unsigned char u_char, unsigned char char_color, unsigned char bgcolor) {
    return (unsigned short) u_char | (unsigned short) char_color << 8 | (unsigned short) bgcolor << 12;
}

int putchar(int int_char) {
    int row, col;
    get_cursor(&row, &col);

    if (int_char == '\n' || int_char == '\r') {
        col = 0;
        row++;
        if (row >= VGA_HEIGHT) {
            row = 0; // Wrap around
        }
    } else if (int_char == '\b') { // Handle backspace
        if (col == 0) {
            if (row > 0) {
                row--;
                col = VGA_WIDTH - 1;
            }
        } else {
            col--;
        }
        VGA_BUFFER[row * VGA_WIDTH + col] = vga_entry(' ', VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    } else {
        unsigned short entry = vga_entry(int_char, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
        VGA_BUFFER[row * VGA_WIDTH + col] = entry;
        col++;
        if (col >= VGA_WIDTH) {
            col = 0;
            row++;
            if (row >= VGA_HEIGHT) {
                row = 0; // Wrap around
            }
        }
    }

    update_cursor(row, col);
    
    return int_char;
}

// Helper function to convert an integer to a string
void itoa(int value, char* buffer, int base) {
    char *ptr = buffer, *ptr1 = buffer, tmp_char;
    int tmp_value;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    if (value < 0 && base == 10) {
        *ptr++ = '-';
        ptr1++;
        value = -value;
    }

    while (value != 0) {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789"[tmp_value - (value * base)];
    }

    *ptr = '\0';

    // Reverse the string
    while (ptr1 < --ptr) {
        tmp_char = *ptr;
        *ptr = *ptr1;
        *ptr1 = tmp_char;
        ptr1++;
    }
}

int printf(const char* __restrict__ format_string, ...) {
    va_list args;
    va_start(args, format_string);
    
    int printed = 0;

    // Loop through format string
    for (const char* char_ptr = format_string; *char_ptr != '\0'; char_ptr++) {
        // If current character is not a format specifier, just print it
        if (*char_ptr != '%') {
            if (putchar(*char_ptr) == EOF) {
                va_end(args);
                return EOF;
            } else {
                printed++;
            }
            continue;
        }

        // Handle format specifiers
        switch (*(++char_ptr)) {
            case 's': {
                const char* string_argument = va_arg(args, const char*);
                size_t len = strlen(string_argument);
                for (size_t i = 0; i < len; i++) {
                    if (putchar(string_argument[i]) == EOF) {
                        va_end(args);
                        return EOF;
                    }
                    printed++;
                }
                break;
            }
            case 'c': {
                int int_char = va_arg(args, int);
                if (int_char != '\0') { // Check for the null character
                    if (putchar(int_char) == EOF) {
                        va_end(args);
                        return EOF;
                    } else {
                        printed++;
                    }
                }
                break;
            }
            case 'd': {
                int int_value = va_arg(args, int);
                char buffer[20]; // Enough to hold all numbers up to 64-bits
                itoa(int_value, buffer, 10);
                for (char *ptr = buffer; *ptr != '\0'; ptr++) {
                    if (putchar(*ptr) == EOF) {
                        va_end(args);
                        return EOF;
                    }
                    printed++;
                }
                break;
            }
            default: {
                // If format string includes an unsupported format specifier, print the '%'
                if (putchar('%') == EOF) {
                    va_end(args);
                    return EOF;
                } else {
                    printed++;
                }
                if (putchar(*char_ptr) == EOF) {
                    va_end(args);
                    return EOF;
                } else {
                    printed++;
                }
                break;
            }
        }
    }
    
    // Clear arguments list from memory
    va_end(args);
    return printed;
}

extern "C" void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

extern "C" uint8_t inb(uint16_t port)
{
   uint8_t ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

extern "C" uint16_t inw(uint16_t port)
{
   uint16_t ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}
