#include "libc/stdio.h"
#include "libc/stdint.h"
#include "screen.h"


// Define memory-mapped I/O address for VGA text mode buffer
volatile unsigned short* VGA_BUFFER_SCREEN = (volatile unsigned short*)0xB8000;

// Creates a vga entry which is used to print a character to the terminal 
// using vga text mode buffer with set color and background color
unsigned short vga_entry_screen(unsigned char u_char, unsigned char char_color, unsigned char bgcolor) {
    return (unsigned short)u_char | (unsigned short)char_color << 8 | (unsigned short)bgcolor << 12;
}

// Function to update the hardware cursor position
void update_cursor(int row, int col) {
    uint16_t position = row * VGA_WIDTH + col;

    outb(VGA_CMD_PORT, 0x0F);
    outb(VGA_DATA_PORT, (uint8_t)(position & 0xFF));
    outb(VGA_CMD_PORT, 0x0E);
    outb(VGA_DATA_PORT, (uint8_t)((position >> 8) & 0xFF));
}

// Function to get the current cursor position 
void get_cursor(int *row, int *col) {
    uint16_t position = 0;

    outb(VGA_CMD_PORT, 0x0F);
    position |= inb(VGA_DATA_PORT);
    outb(VGA_CMD_PORT, 0x0E);
    position |= ((uint16_t)inb(VGA_DATA_PORT)) << 8;

    *row = position / VGA_WIDTH;
    *col = position % VGA_WIDTH;
}

// Function to move the cursor to the next line (usually when the user presses enter)
void move_cursor_to_next_line() {
    int row, col;
    get_cursor(&row, &col);
    col = 0;
    row++;
    if (row >= VGA_HEIGHT) {
        row = 0; // Wrap around to the top
    }
    update_cursor(row, col);
}

// Functioin to advance the cursor by one cell (usually when the user types a character)
void advance_cursor() {
    int row, col;
    get_cursor(&row, &col);
    col++;
    if (col >= VGA_WIDTH) {
        col = 0;
        row++;
        if (row >= VGA_HEIGHT) {
            row = 0;
        }
    }
    update_cursor(row, col);
}


