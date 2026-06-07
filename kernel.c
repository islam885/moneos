#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

// VGA цвета
#define COLOR_BLACK   0x0
#define COLOR_BLUE    0x1
#define COLOR_GREEN   0x2
#define COLOR_CYAN    0x3
#define COLOR_RED     0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN   0x6
#define COLOR_LGRAY   0x7
#define COLOR_DGRAY   0x8
#define COLOR_LBLUE   0x9
#define COLOR_LGREEN  0xA
#define COLOR_LCYAN   0xB
#define COLOR_LRED    0xC
#define COLOR_LMAGENTA 0xD
#define COLOR_YELLOW  0xE
#define COLOR_WHITE   0xF

#define MAKE_COLOR(fg, bg) ((bg << 4) | fg)

static uint16_t *const VGA = (uint16_t *)0xB8000;
static size_t row = 0, col = 0;
static uint8_t color = MAKE_COLOR(COLOR_LGRAY, COLOR_BLACK);

static inline uint16_t entry(char c, uint8_t col) {
    return (uint16_t)c | (uint16_t)col << 8;
}

static void clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            VGA[y * VGA_WIDTH + x] = entry(' ', color);
    row = col = 0;
}

static void set_color(uint8_t fg, uint8_t bg) {
    color = MAKE_COLOR(fg, bg);
}

static void putc(char c) {
    if (c == '\n') {
        col = 0;
        if (++row == VGA_HEIGHT) row = 0;
        return;
    }
    VGA[row * VGA_WIDTH + col] = entry(c, color);
    if (++col == VGA_WIDTH) {
        col = 0;
        if (++row == VGA_HEIGHT) row = 0;
    }
}

static void print(const char *s) {
    for (size_t i = 0; s[i]; i++) putc(s[i]);
}

static void print_colored(const char *s, uint8_t fg, uint8_t bg) {
    uint8_t old_color = color;
    set_color(fg, bg);
    print(s);
    color = old_color;
}

// Простая задержка (не точная, но визуально работает)
static void delay(uint32_t count) {
    for (volatile uint32_t i = 0; i < count * 1000000; i++);
}

static void print_status_ok(void) {
    // Переходим к колонке 70 для выравнивания
    col = 70;
    print_colored("[ ", COLOR_WHITE, COLOR_BLACK);
    print_colored("OK", COLOR_LGREEN, COLOR_BLACK);
    print_colored(" ]", COLOR_WHITE, COLOR_BLACK);
    print("\n");
}

static void print_status_err(void) {
    col = 70;
    print_colored("[ ", COLOR_WHITE, COLOR_BLACK);
    print_colored("ERR", COLOR_LRED, COLOR_BLACK);
    print_colored(" ]", COLOR_WHITE, COLOR_BLACK);
    print("\n");
}

static void print_status_loading(void) {
    col = 70;
    print_colored("[", COLOR_WHITE, COLOR_BLACK);
    print_colored("....", COLOR_YELLOW, COLOR_BLACK);
    print_colored("]", COLOR_WHITE, COLOR_BLACK);
}

static void print_boot_message(const char *msg, int success) {
    print_colored(" * ", COLOR_CYAN, COLOR_BLACK);
    print(msg);
    print_status_loading();
    delay(3); // задержка для эффекта загрузки
    
    // Возвращаемся назад и перезаписываем статус
    col = 70;
    if (success)
        print_status_ok();
    else
        print_status_err();
}

static void print_progress_bar(int percent) {
    const int bar_width = 50;
    int filled = (bar_width * percent) / 100;
    
    print_colored("[", COLOR_LCYAN, COLOR_BLACK);
    for (int i = 0; i < filled; i++)
        print_colored("=", COLOR_LGREEN, COLOR_BLACK);
    if (filled < bar_width)
        print_colored(">", COLOR_YELLOW, COLOR_BLACK);
    for (int i = filled + 1; i < bar_width; i++)
        print_colored(" ", COLOR_BLACK, COLOR_BLACK);
    print_colored("]", COLOR_LCYAN, COLOR_BLACK);
    
    // Процент
    char buf[5];
    buf[0] = ' ';
    buf[1] = '0' + (percent / 10);
    buf[2] = '0' + (percent % 10);
    buf[3] = '%';
    buf[4] = '\0';
    print_colored(buf, COLOR_WHITE, COLOR_BLACK);
}

static void boot_sequence(void) {
    // Заголовок
    print("\n");
    print_colored("  ███╗   ███╗ ██████╗ ███╗   ██╗███████╗ ██████╗ ███████╗\n", 
                  COLOR_LCYAN, COLOR_BLACK);
    print_colored("  ████╗ ████║██╔═══██╗████╗  ██║██╔════╝██╔═══██╗██╔════╝\n", 
                  COLOR_LCYAN, COLOR_BLACK);
    print_colored("  ██╔████╔██║██║   ██║██╔██╗ ██║█████╗  ██║   ██║███████╗\n", 
                  COLOR_CYAN, COLOR_BLACK);
    print_colored("  ██║╚██╔╝██║██║   ██║██║╚██╗██║██╔══╝  ██║   ██║╚════██║\n", 
                  COLOR_CYAN, COLOR_BLACK);
    print_colored("  ██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████╗╚██████╔╝███████║\n", 
                  COLOR_BLUE, COLOR_BLACK);
    print_colored("  ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝ ╚═════╝ ╚══════╝\n", 
                  COLOR_BLUE, COLOR_BLACK);
    print("\n");
    print_colored("  Welcome to MoneOS v0.1.0\n", COLOR_YELLOW, COLOR_BLACK);
    print_colored("  ─────────────────────────────────────────────────────────\n\n", 
                  COLOR_DGRAY, COLOR_BLACK);
    
    // Загрузка модулей
    print_boot_message("Initializing kernel core", 1);
    print_boot_message("Loading memory manager", 1);
    print_boot_message("Setting up VGA display", 1);
    print_boot_message("Initializing interrupt handlers", 1);
    print_boot_message("Loading filesystem driver", 1);
    print_boot_message("Starting timer subsystem", 1);
    print_boot_message("Configuring keyboard driver", 1);
    print_boot_message("Network stack initialization", 0); // пример ошибки
    print_boot_message("Mounting root filesystem", 1);
    
    print("\n");
    print_colored(" Loading system... ", COLOR_WHITE, COLOR_BLACK);
    delay(2);
    print("\n ");
    print_progress_bar(100);
    print("\n\n");
    
    print_colored(" ✓ System ready!\n\n", COLOR_LGREEN, COLOR_BLACK);
    
    // Приглашение консоли
    print_colored(" root@moneos:~# ", COLOR_LGREEN, COLOR_BLACK);
    print_colored("_", COLOR_WHITE, COLOR_BLACK);
}

void kernel_main(void) {
    clear();
    boot_sequence();
    for (;;) __asm__ volatile("hlt");
}