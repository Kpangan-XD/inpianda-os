#include "config.hpp"
#include <define.hpp>
#include <stdint.h>
#include <system.hpp>

#include <io.hpp>

extern "C" void switch_to_stack(volatile uint32_t *old_esp, uint32_t new_esp);

namespace sys { // system or CORE/THREAD
CORE_0 void yield_core0() {
  int prev = core0_current_th;
  core0_current_th = (core0_current_th == 0) ? 1 : 0;
  switch_to_stack(&core0_threads[prev].esp,
                  core0_threads[core0_current_th].esp);
}

CORE_1 void yield_core1() {
  int prev = core1_current_th;
  // FIX BUG 1: Diganti ke core1_current_th!
  core1_current_th = (core1_current_th == 0) ? 1 : 0; 
  switch_to_stack(&core1_threads[prev].esp,
                  core1_threads[core1_current_th].esp);
}

THREAD_CORE_0A void task_thread_a() {
  while (true) {
    int countA = 0;
    inp::dprintfs("Core 0 - Thread A", 5, 5, 0x0A); // Warna Hijau
    yield_core0();
    countA++;
  }
}

THREAD_CORE_0B void task_thread_b() {
  while (true) {
    inp::dprintfs("Core 0 - Thread B", 5, 6, 0x0E); // Warna Kuning
    yield_core0();
  }
}

THREAD_CORE_1A void task_thread_c() {
  while (true) {
    inp::dprintfs("Core 1 - Thread C", 5, 7, 0x0C); // Warna Merah
    yield_core1();
  }
}

THREAD_CORE_1B void task_thread_d() {
  while (true) {
    inp::dprintfs("Core 1 - Thread D", 5, 8, 0x0B); // Warna Biru Muda
    yield_core1();
  }
}

// SETUP STACK FIX
void setup_thread_stack(ThreadState* th, uint8_t* stack_mem, void (*func_ptr)()){
    uintptr_t top = (uintptr_t)&stack_mem[4096];
    
    // FIX BUG 2: Alignment 16-byte biar x86/C++ gak GPF Triple Fault
    top &= ~((uintptr_t)0x0F);

    uint32_t* ptr = (uint32_t*)top;

    *(--ptr) = (uint32_t)func_ptr; // Return address
    ptr -= 8;                      // Allocate space buat pusha

    th->esp = (uint32_t)ptr;
}

// LAPIC STARTUP
void init_lapic(){
    LAPIC_SPURIOUS = 0x1FF;
}

// void wakeup_core1(){
//     uint8_t sipi_vector = 0x08; // Pastikan real mode vector sesuai alamat entry

//     LAPIC_ICR_HIGH = (1 << 24);
//     LAPIC_ICR_LOW  = 0x00004500; // INIT IPI
    
//     for (volatile int i = 0; i < 100000; i++); // Delay

//     LAPIC_ICR_HIGH = (1 << 24);
//     LAPIC_ICR_LOW  = 0x00004600 | sipi_vector; // SIPI
// }

inline void wait_icr_ready() {
    // Bit 12 = Delivery Status (0 = Idle, 1 = Send Pending)
    while (LAPIC_ICR_LOW & (1 << 12)) {
        asm volatile("pause");
    }
}

void wakeup_core1() {
    uint8_t sipi_vector = 0x08; // Vector 0x08 -> Entry di RAM 0x8000 (0x08 * 4KB)

    // 1. KIRIM INIT IPI
    wait_icr_ready();
    LAPIC_ICR_HIGH = (1 << 24);                     // Target: Core 1 (APIC ID 1)
    LAPIC_ICR_LOW  = 0x00004500;                    // INIT, Assert, Physical, Edge

    // Delay ~10ms (pake loop volatile)
    for (volatile int i = 0; i < 1000000; i++) asm volatile("nop");

    // 2. KIRIM SIPI PERTAMA (SIPI 1)
    wait_icr_ready();
    LAPIC_ICR_HIGH = (1 << 24);                     // Target: Core 1
    LAPIC_ICR_LOW  = 0x00004600 | sipi_vector;      // Start-up IPI + Vector

    // Delay singkat ~200 microseconds
    for (volatile int i = 0; i < 100000; i++) asm volatile("nop");

    // 3. KIRIM SIPI KEDUANNYA (SIPI 2 - Wajib menurut Intel SDM!)
    wait_icr_ready();
    LAPIC_ICR_HIGH = (1 << 24);                     // Target: Core 1
    LAPIC_ICR_LOW  = 0x00004600 | sipi_vector;      // Start-up IPI + Vector
}

} // namespace sys

namespace inp {

char get_color(char video_memory) {
  char getcol = video_memory;
  return getcol;
}

void sleep(unsigned short ticks) {
  // 1. Setel chip PIT ke Mode 0 (Interrupt on Terminal Count)
  outb(0x43, 0x30);

  // 2. Kirim takaran detak (Low byte dulu, baru High byte)
  outb(0x40, (unsigned char)(ticks & 0xFF));
  outb(0x40, (unsigned char)((ticks >> 8) & 0xFF));

  // 3. Looping nungguin chip PIT selesai berdetak mundur sampe nol!
  while (true) {
    outb(0x43, 0x00); // Latch status
    unsigned char status = inb(0x40);
    if ((status & 0x80) != 0) { // Bit paling atas nyala = selesai!
      break;
    }
  }
}

void Sleep(unsigned int ms) {
  for (unsigned int i = 0; i < ms; i++) {
    sleep(1193); // Panggil delay 1 milidetik berulang-ulang, cok!
  }
}
} // namespace inp