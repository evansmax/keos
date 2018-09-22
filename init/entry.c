#include "console.h"

int kern_entry()
{
	console_clear();

	console_write_color("Hello, keos!\n", rc_black, rc_white);

	return 0;
}