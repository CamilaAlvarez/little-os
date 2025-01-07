#ifndef _HAL_H
#define _HAL_H

#ifndef ARCH_X86
#error "HAL not implemented for this platform"
#endif

#include "stdint.h"

#define far
#define near

typedef void far (*irq_handler)();

int hal_initialize();
int hal_shutdown();
void geninterrupt(int n);
void interruptdone(unisigned int intno);
void sound(unsigned frequency);
unsigned char inportb(unsigned shirt portid);
void outportb(unsigned short portid, unsigned char value);
// Enables all hardware interrupts
void enable();
// Disables all hardware interrupts
void disable();
// Sets new interrupt vector
void setvect(int intno, irq_handler vect); // The same as void setvect(int intno, void far *vect());
irq_handler getvect(int intno); // The same as void (far *getvect(int intno))();, that is getvect is a function that receives a number as a param and returns a function pointer
const char* get_cpu_vendor();
int get_tick_count();
#endif
