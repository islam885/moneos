#include "keyboard.h"
#include <common.h>
#include <cpu/idt.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static char key_buffer[256];
static int head = 0, tail = 0;

extern void keyboard_isr(void);

static const char scancode_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void keyboard_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    if (!(scancode & 0x80)) { // Key press
        if (scancode < sizeof(scancode_ascii) && scancode_ascii[scancode]) {
            key_buffer[head] = scancode_ascii[scancode];
            head = (head + 1) % 256;
        }
    }
    
    outb(0x20, 0x20); // Send EOI to PIC
}

void keyboard_init(void) {
    idt_set_gate(33, (uint32_t)keyboard_isr, 0x08, 0x8E);
}

bool keyboard_has_key(void) {
    return head != tail;
}

char keyboard_get_char(void) {
    if (head == tail) return 0;
    char c = key_buffer[tail];
    tail = (tail + 1) % 256;
    return c;
}
