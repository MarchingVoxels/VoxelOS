#include "timer.h"
#include "../drivers/screen.h"

#include "../libc/string.h"
#include "../libc/function.h"
#include "../libc/mem.h"

#include "ports.h"

#include "isr.h"

u32 tick = 0;

static void timer_callback(registers_t regs){
    tick++;
    kprint("tick: ");
    char tick_ascii[256]; 
    int_to_ascii(tick, tick_ascii);
    kprint(tick_ascii);
    kprint("\n");
    UNUSED(regs);
}

void init_timer(u32 freq){
    register_interrupt_handler(IRQ0, timer_callback); // registers the timer callback as an interrupt
    u32 divisor = 1193180 / freq; // hardware clock at 1193180 Hz
    u8 low = (u8)(divisor & 0xFF); 
    u8 high = (u8)((divisor >> 8) & 0xFF);
    port_byte_out(0x43, 0x36); // PIT
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}