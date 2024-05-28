// Based on the teachers solution guide file system.h

#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/limits.h"
#include "libc/stdio.h"
#include "string.h"

// Custom Functions
void panic(const char* reason);
char* hex32_to_str(char buffer[], unsigned int val);
char* int32_to_str(char buffer[], int val);

