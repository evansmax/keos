#include "console.h"
#include "io.h"

static uint16_t *video_memory = (uint16_t *)0xB8000;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void move_cursor(void)
{
	uint16_t cursorLocation = cursor_y * 80 + cursor_x;

	outb(0x3D4, 14);					// high
	outb(0x3D5, cursorLocation >> 8);
	outb(0x3D4, 15);					// low
	outb(0x3D5, cursorLocation);
}

static void scroll(void)
{
	uint8_t attribute_byte = (rc_black << 4) | (rc_white & 0x0F);
	uint16_t blank = 0x20 | (attribute_byte << 8);

	if (cursor_y >= 25) {
		int i;
		for (i = 0 * 80; i < 24 * 80; i++) {
			video_memory[i] = video_memory[i+80];
		}

		for (i = 24 * 80; i < 25 * 80; i++) {
			video_memory[i] = blank;
		}

		cursor_y = 24;
	}
}

void console_clear(void)
{
	uint8_t attribute_byte = (rc_black >> 4) | (rc_white & 0x0F);
	uint16_t blank = 0x20 | (attribute_byte << 8);
	
	int i;
	for (i = 0; i < 25*80; i++) {
		video_memory[i] = blank;
	}

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	uint8_t back_color = (uint8_t)back;
	uint8_t fore_color = (uint8_t)fore;

	uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
	uint16_t attribute = attribute_byte << 8;

	// 0x08 Backspace
	// 0x09 Tab
	if (c == 0x08 && cursor_x) {
		cursor_x -= 1;
	} else if (c == 0x09) {
		cursor_x = (cursor_x+8) & ~(8-1);
	} else if (c == '\r') {
		cursor_x =  0;
	} else if (c == '\n') {
		cursor_x =  0;
		cursor_y += 1;
	} else if (c >= ' ') {
		video_memory[cursor_y*80 + cursor_x] = c | attribute;
		cursor_x += 1;
	}

	if (cursor_x >= 80) {
		cursor_x =  0;
		cursor_y += 1;
	}

	scroll();

	move_cursor();
}

void console_write(char *cstr)
{
	while (*cstr) {
		console_putc_color(*csrt++, rc_black, rc_white);
	}
}

void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
	while (*cstr) {
		console_write_color(*cstr++, back, fore);
	}
}

void console_write_hex(uint32_t n, real_color_t back, real_color_t fore)
{
	console_write_color("0x", back, fore);

	int tmp;
	int noZeroes = 1;

	int i;
	for (i = 32-4; i >= 0; i -= 4) {
		tmp = (n >> i) & 0x0F;
		if (tmp ==0 && noZeroes) {
			continue;
		}
		noZeroes = 0;
		if (tmp >= 0x0A) {
			console_putc_color(tmp-0x0A+'a', back, fore);
		} else {
			console_putc_color(tmp+'0', back, fore);
		}
	}
}

void console_write_dec(uint32_t n, real_color_t back, real_color_t fore)
{
	if (n == 0) {
		console_putc_color('0', back, fore);
	}

	uint32_t tmp = n;
	char c[32];
	int i = 0;
	while (tmp > 0) {
		c[i] = (tmp % 10) + '0';
		tmp /= 10;
		i += 1;
	}
	c[i] = 0;

	char c2[32];
	c2[i] = 0;
	i -= 1;

	int j = 0;
	while (i >= 0) {
		c2[i] = c[j];
		i -= 1;
		j += 1;
	}

	console_write_color(c2, back, fore);
}