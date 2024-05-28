#include "libc/stdio.h"
#include "libc/stdint.h"
#include "../include/screen.h"

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
        row = 0;
    }
    update_cursor(row, col);
}

// Function advances cursor by one cell (usually when the user types)
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
