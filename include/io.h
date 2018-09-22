#ifndef _IO_H
#define _IO_H

#include "type.h"

/*
 * ==============================================================================================
 *
 *		video input/output
 *		index: 0x3d4
 *		data: 0x3d5
 *
 */
static inline void outb(uint8_t data, uint16_t port)
{
	asm volatile ("outb %0, %1" : : "a" (data), "dN" (port));
}

static inline void outw(uint16_t data, uint16_t port)
{
	asm volatile ("outw %0, %1" : : "a" (data), "dN" (port));
}

static inline void outl(uint32_t data, uint16_t port)
{
	asm volatile ("outl %0, %1" : : "a" (data), "dN" (port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t data;
	asm volatile ("inb %1, %0" : "=a" (data) : "dN" (port));
	return data;
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t data;
	asm volatile ("inw %1, %0" : "=a" (data) : "dN" (port));
	return data;
}

static inline uint32_t inl(uint32_t port)
{
	uint32_t data;
	asm volatile ("inl %1, %0" : "=a" (data) : "dN" (port));
	return data;
}


/*
 *============================================
 *
 *		keyboard input
 *		port: 
 *
 */

#endif
