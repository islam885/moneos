#include <multiboot.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cod/animation.h>
#include <shell/auth.h>
#include <shell/shell.h>

void kernel_main(struct multiboot_info *mbi) {
    gdt_init();
    idt_init();
    keyboard_init();
    screen_init(mbi);
    
    start_boot_animation();
    
    while (!run_login()) {
        screen_print("Invalid login!", 320, 350, 0xFF0000);
    }
    
    screen_clear(0x000000);
    shell_run();
    
    for (;;) asm volatile("hlt");
}
