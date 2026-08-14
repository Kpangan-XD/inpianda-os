#ifndef INP_SYSTEM_HPP
#define INP_SYSTEM_HPP
#include "define.hpp"
#include <stdint.h>

extern "C" {
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
char prt_keyboard();
}
namespace sys {
  // thread / core

  // global settings 

  struct ThreadState {
      uint32_t esp;  
  };

  inline ThreadState core0_threads[2];
  inline ThreadState core1_threads[2];

  inline volatile int core0_current_th = 0;
  inline volatile int core1_current_th = 0;

  inline uint8_t stack_thA[4096];
  inline uint8_t stack_thB[4096];
  inline uint8_t stack_thC[4096];
  inline uint8_t stack_thD[4096];
  

// Core 0 (A,B)
THREAD_CORE_0A alignas(64) inline volatile uint32_t thread_a = 0;
THREAD_CORE_0B alignas(64) inline volatile uint32_t thread_b = 0;
// Core 1 (A,B) -> (C,D) (same position)
THREAD_CORE_1A alignas(64) inline volatile uint32_t thread_c = 0;
THREAD_CORE_1B alignas(64) inline volatile uint32_t thread_d = 0;

// IND : YIELD = gantian thread per core
CORE_0 void yield_core0();
CORE_1 void yield_core1();

//  Core 0 = (A, B), Core 1 = (C, D) (2 cores) (4 threads)
THREAD_CORE_0A void task_thread_a();
THREAD_CORE_0B void task_thread_b();
THREAD_CORE_1A void task_thread_c();
THREAD_CORE_1B void task_thread_d();

void setup_thread_stack(ThreadState* th, uint8_t* stack_mem, void (*func_ptr)());

// start
void init_lapic();
void wakeup_core1();

// thread / core
}

namespace inp {


char get_color(char video_memory);

// Bad for sleep, old than inline void Sleep()
void sleep(unsigned short ticks);


// Seconds, new than inline void sleep()
void Sleep(unsigned int ms);




} // namespace inp

#endif
