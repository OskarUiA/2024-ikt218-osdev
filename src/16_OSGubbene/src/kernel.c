#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "pit.h"
#include "memory.h"
#include "../gdt/gdt.h"
#include "idt.h"
#include <multiboot2.h>



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

// End of the kernel image, defined elsewhere.
extern uint32_t end;

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    gdt_init();
    init_idt();
    init_irq();


    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();

    init_pit();

    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}