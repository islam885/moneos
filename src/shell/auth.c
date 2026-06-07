#include "auth.h"
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <stddef.h>

static void get_input(char *buf, bool mask) {
    int i = 0;
    while (1) {
        if (keyboard_has_key()) {
            char c = keyboard_get_char();
            if (c == '\n') {
                buf[i] = '\0';
                break;
            } else if (c == '\b') {
                if (i > 0) i--;
            } else {
                buf[i++] = c;
                if (mask) screen_print("*", 400 + i * 8, 300, 0xFFFFFF);
                else screen_print(&c, 400 + i * 8, 300, 0xFFFFFF);
            }
        }
    }
}

bool run_login(void) {
    screen_clear(0x202020); // Темно-серый фон
    screen_print("Login: ", 320, 280, 0xFFFFFF);
    screen_print("Password: ", 320, 300, 0xFFFFFF);
    
    char user[32], pass[32];
    get_input(user, false);
    get_input(pass, true);
    
    // Сравнение admin:admin
    return (user[0] == 'a' && user[1] == 'd' && user[2] == 'm' && user[3] == 'i' && user[4] == 'n' && user[5] == '\0' &&
            pass[0] == 'a' && pass[1] == 'd' && pass[2] == 'm' && pass[3] == 'i' && pass[4] == 'n' && pass[5] == '\0');
}
