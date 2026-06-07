[bits 32]
extern keyboard_handler

global keyboard_isr
keyboard_isr:
    pushad
    call keyboard_handler
    popad
    iretd
