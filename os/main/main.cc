

#include <stdint.h>

#include "cursor.hpp" //
#include "define.hpp"
#include "io.hpp"     // System OS
#include "kernel.hpp" // Core
#include "screen.hpp"
#include "system.hpp"
#include "while.hpp"
#include "window.hpp" // Window management or Estetics

/**
 * This file include -*- C++ -*- for everyone.
 *  @author Contributed by Kpangan ZkxFsd
 * (kpanganbaru@gmail.com)
 *  @file kernel.cpp
 *  ISF (Inpianda Software Foundation)
 *  @date 17 Mei 2026 -> ~~~
 *  @version 1.0
 *  @brief  Operating System.
 *  @copyright (C) 2026-INF (ISF) Inpianda Software Foundation,
 * Inc. This file is part and real of ISF Product. You can use
 * or modify it under ISF terms. All rights reserved.
 */

// 1193182 to 1 second

/***
*  Called in kernel.hpp
*  for efficient code, excellent code,
*  and i write this to telling you.
// extern "C" {
// void outb(unsigned short port, unsigned char data);
// unsigned char inb(unsigned short port);
// char keyboard();
// }
// void outw(unsigned short port, unsigned short val) {
//
//   __asm__ __volatile__("outw %0, %1" : : "a"(val), "Nd"(port));
// }
// extern "C" char keyboard();
**/

void loop() {

  inp::dprintfs("a", 0, 0, 0x7F);

  inp::dprintfs("b", 0, 0, 0x7F);

  inp::dprintfs("c", 0, 0, 0x7F);

  inp::dprintfs("a", 0, 0, 0x7F);

  inp::dprintfs("b", 0, 0, 0x7F);

  inp::dprintfs("c", 0, 0, 0x7F);

  inp::dprintfs("a", 0, 0, 0x7F);

  inp::dprintfs("b", 0, 0, 0x7F);

  inp::dprintfs("c", 0, 0, 0x7F);

  inp::dprintfs("a", 0, 0, 0x7F);

  inp::dprintfs("b", 0, 0, 0x7F);

  inp::dprintfs("c", 0, 0, 0x7F);
}
int cursor_x = 0;
int cursor_y = 0;
volatile char *const video_memory = (volatile char *)0xB8000;
char scancode_terakhir = 0;
char buffer[64]; // Penampung teks perintah (misal max 64 huruf)
int buf_idx = 0; // Penunjuk posisi huruf sekarang

const int TASK = 500000;

// Dipanggil saat Core 1 berhasil bangun dari sleeping state

extern "C" void main_() {

  clear_screen(0x0F);

  draw_text_rect(0, 0, 80, 24,
                 0x1F); // Tingginya cuma 24, biar baris 25 buat taskbar
  inp::Sleep(100000);

  update_cursor(cursor_x, cursor_y);
  Window_ Terminal("Terminal", 10, 10, 30, 16, true, false, 0x07, 0x70);
  Window_ tas("Terminal", 10, 14, 30, 16, true, false, 0x07, 0x70);
  inp::dprintfs("hello world", 40, 11, 0x70);
  Taskbar_ Taskbar(0x50);
  Taskbar_ Start(0x20);
  Terminal.window();
  tas.window();
  Taskbar.Taskbar();
  Start.startmenu();

  inp::dprintfs("hauddddddddddddddddddddddddddddddddddddddddddd", 10, 8, 0x7F);
  inp::dprintfs("maka", 10, 9, 0x7F);
  while (true) {
    loop();
    char scancode = keyboard();

    if (scancode != 0) {

      // 1. SATPAM A: JALUR TOMBOL DIPENCET (Press Mode)
      if ((scancode & 0x80) == 0) {

        // Biar gak double-trigger pas ditahan, cek status terakhir
        if (scancode == scancode_terakhir) {
          continue; // Kalau masih tombol yang sama dan belum dilepas, skip!
        }

        scancode_terakhir = scancode; // Catat tombol aktif sekarang

        char huruf = scancode_(scancode);

        // --- [SISA KODE PENGETIKAN HURUF LO YANG SUDAH WORK] ---
        if (huruf) {
          if (huruf != ' ' && huruf != '\n') {
            buffer[buf_idx] = huruf;
          }
          if (huruf != '\n') {
            inp::rprint(huruf, 0x0F);
            cursor_x++;
            if (cursor_x >= 57) {
              cursor_x = 0;
              cursor_y++;
            }
          }

          // 3. PAS TOMBOL ENTER DIPENCET -> BARU CEK ISINYA!
          if (huruf == OS_ENT) {
            buffer[buf_idx] = '\0'; // Kunci akhir string

            // Cek apakah huruf pertamanya 'Q'
            if (buffer[0] == 'Q') {
              inp::dprintfs("hello world", 10, 10, 0x0F);
            }

            // Reset buffer buat ketikan selanjutnya
            buf_idx = 0;
            cursor_x = 0;
            cursor_y++;
          }

          update_cursor(cursor_x, cursor_y);
          // inp::rprint(huruf, 0x0F);
          // cursor_x++;
          // if (cursor_x >= 57) {
          //   cursor_x = 0;
          //   cursor_y++;
          // }
          // update_cursor(cursor_x, cursor_y);
        }

        // switch, if didn't need if else hierarcy, or complex.
        switch (scancode) {
        case OS_ARR_UP:
          // for moving cursor
          cursor_y--;
          update_cursor(cursor_x, cursor_y);
          continue;
        case OS_ARR_DOWN:
          // for moving cursor
          cursor_y++;
          update_cursor(cursor_x, cursor_y);
          continue;
        case OS_ARR_LEFT:
          // for moving cursor
          cursor_x--;
          update_cursor(cursor_x, cursor_y);
          continue;
        case OS_ARR_RIGHT:
          // for moving cursor
          cursor_x++;
          update_cursor(cursor_x, cursor_y);
          continue;
        case OS_ENT:
          cursor_x = 0;
          if (cursor_y > 80)
            cursor_y = 0;
          cursor_y++;
          update_cursor(cursor_x, cursor_y);
          continue;
        case OS_BACK:
          cursor_x--;
          update_cursor(cursor_x, cursor_y);
          inp::rprint(' ', 0x0F);
          continue;
        case OS_F1: // shutdown
          clear_screen(0x70);
          screen(0x70, 25, 80);
          inp::dprintfs("GOODBYE...", 25, 12, 0x6E);
          inp::Sleep(100000);
          outw(0x604, 0x2000);
          continue;
        default:
          break;
        }

        // for complex if else hierarcy

        // Window management
        // bool drag = false;
        // if (scancode == OS_LSHT &&
        //     (cursor_x >= Terminal.getX() ||
        //      cursor_x <= (Terminal.getX() + Terminal.getLebar())) ||
        //     cursor_y == Terminal.getY()) {
        //   drag = true;
        // }
        // if (drag && scancode == OS_ARR_LEFT) {
        //   draw_text_rect(Terminal.getX(), Terminal.getY(),
        //   Terminal.getLebar(),
        //                  Terminal.getTinggi(), 0x1F);
        //   Terminal.setX(Terminal.getX() - 1);
        //   Terminal.window();
        //   cursor_x--;
        //   update_cursor(cursor_x, cursor_y);
        // }

      } else {
        // release mode?
        // get real scancode for = scancode - 0x80?
        char scancode_asli = scancode - 0x80;

        // if real scancode == scancode_after, will reset scancode_after;
        if (scancode_asli == scancode_terakhir) {
          scancode_terakhir = 0;
          // and scancode will get again.
        }
      }

    } // SELECTED SCANCODE
  } // WHILE
}
extern "C" void core1_entry();

extern "C" void core1_main() {
  // 1. Siapkan Stack palsu untuk Thread D di Core 1
  sys::setup_thread_stack(&sys::core1_threads[1], sys::stack_thD,
                          sys::task_thread_d);

  // 2. Jalankan Thread C sebagai thread pertama di Core 1!
  sys::task_thread_c();
}
/************
 * 1 - 9 = BLACK
 * 10 = DARK BLUE
 *
 */

// core

//     sys::setup_thread_stack(&sys::core0_threads[0], sys::stack_thA,
//     sys::task_thread_a); sys::setup_thread_stack(&sys::core0_threads[1],
//     sys::stack_thB, sys::task_thread_b);
//     sys::setup_thread_stack(&sys::core1_threads[0], sys::stack_thC,
//     sys::task_thread_c); sys::setup_thread_stack(&sys::core1_threads[1],
//     sys::stack_thD, sys::task_thread_d);

//     sys::init_lapic();

// sys::wakeup_core1();
//     sys::task_thread_a();
// core
