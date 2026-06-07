#include "animation.h"
#include <drivers/screen.h>
#include <stddef.h>

// Простая задержка
static void delay(uint32_t count) {
    for (volatile uint32_t i = 0; i < count * 1000000; i++);
}

void start_boot_animation(void) {
    screen_clear(0x000000); // Черный экран
    
    // Рисуем простую анимацию: квадрат, который "растет"
    for (int i = 0; i < 100; i += 2) {
        screen_draw_rect(400 - i, 300 - i, i * 2, i * 2, 0x0000FF * (i / 10)); // Синеватый квадрат
        delay(5);
    }
    
    screen_print("MoneOS v2.0 - LOADING...", 320, 400, 0xFFFFFF);
    delay(50);
}
