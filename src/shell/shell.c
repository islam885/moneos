#include "shell.h"
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <common.h>

static char cmd_buf[128];
static int cmd_pos = 0;

static void parse_command(char *cmd) {
    if (cmd[0] == '\0') return;
    
    // Простая симуляция команд
    if (cmd[0] == 'h' && cmd[1] == 'e' && cmd[2] == 'l' && cmd[3] == 'p') {
        screen_print("Commands: help, clear, ping, curl, ip, weather, matrix, stats", 10, 30, 0xFFFFFF);
    } else if (cmd[0] == 'c' && cmd[1] == 'l' && cmd[2] == 'e' && cmd[3] == 'a' && cmd[4] == 'r') {
        screen_clear(0x000000);
    } else if (cmd[0] == 'p' && cmd[1] == 'i' && cmd[2] == 'n' && cmd[3] == 'g') {
        screen_print("Ping 127.0.0.1: time<1ms", 10, 30, 0x00FF00);
    } else {
        screen_print("Command not found.", 10, 30, 0xFF0000);
    }
}

void shell_run(void) {
    screen_print("root@moneOS:/# ", 10, 400, 0x00FF00);
    
    while (1) {
        if (keyboard_has_key()) {
            char c = keyboard_get_char();
            if (c == '\n') {
                cmd_buf[cmd_pos] = '\0';
                parse_command(cmd_buf);
                cmd_pos = 0;
                screen_print("root@moneOS:/# ", 10, 400, 0x00FF00);
            } else if (c == '\b') {
                if (cmd_pos > 0) cmd_pos--;
            } else {
                cmd_buf[cmd_pos++] = c;
                // Рисуем символ
                char s[2] = {c, '\0'};
                screen_print(s, 140 + cmd_pos * 8, 400, 0xFFFFFF);
            }
        }
    }
}
