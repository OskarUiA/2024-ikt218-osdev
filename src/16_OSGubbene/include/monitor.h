// monitor.h -- Defines the interface for monitor.h
//              From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"
#include "libc/stdint.h"
#include "libc/stddef.h"


#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CMD_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

// Function to update the hardware cursor position
void update_cursor(int row, int col);

// Function to get the current cursor position
void get_cursor(int *row, int *col);

// Function to move the cursor to the next line
void move_cursor_to_next_line();

// Function to advance the cursor
void advance_cursor();


#endif // MONITOR_H