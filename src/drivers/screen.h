#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include <multiboot.h>

void screen_init(struct multiboot_info *mbi);
void screen_put_pixel(uint32_t x, uint32_t y, uint32_t color);
void screen_clear(uint32_t color);
void screen_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
void screen_putc(char c, uint32_t x, uint32_t y, uint32_t color);
void screen_print(const char *s, uint32_t x, uint32_t y, uint32_t color);

uint32_t screen_get_width(void);
uint32_t screen_get_height(void);

#endif
