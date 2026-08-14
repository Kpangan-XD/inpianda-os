#ifndef INP_OS_WHILE_HPP
#define INP_OS_WHILE_HPP
#include "cursor.hpp" //
#include "define.hpp"
#include "io.hpp"     // System OS
#include "kernel.hpp" // Core
#include "screen.hpp"
#include "system.hpp"
#include "window.hpp" // Window management or Estetics
#if 0
inline void while_(){
     while (true) {
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
          inp::rprint(huruf, 0x70);
          cursor_x++;
          if (cursor_x >= 57) {
            cursor_x = 22;
            cursor_y++;
          }
          update_cursor(cursor_x, cursor_y);
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
          cursor_x = 22;
          cursor_y++;
          if (cursor_y > 15)
            cursor_y = 15;
          update_cursor(cursor_x, cursor_y);
          continue;
        case OS_BACK:
          cursor_x--;
          update_cursor(cursor_x, cursor_y);
          inp::rprint(' ', 0x70);
          continue;
        case OS_F1: // shutdown
          clear_screen(0x70);
          screen(0x70, 25, 80);
          print_text_string("GOODBYE...", 25, 12, 0x6E);
          inp::Sleep(100000);
          outw(0x604, 0x2000);
          continue;
        default:
          break;
        }

        // for complex if else hierarcy

        // Window management
        bool drag = false;
        if (scancode == OS_LSHT &&
            (cursor_x >= Terminal.getX() ||
             cursor_x <= (Terminal.getX() + Terminal.getLebar())) ||
            cursor_y == Terminal.getY()) {
          drag = true;
        }
        if (drag && scancode == OS_ARR_LEFT) {
          draw_text_rect(Terminal.getX(), Terminal.getY(), Terminal.getLebar(),
                         Terminal.getTinggi(), 0x1F);
          Terminal.setX(Terminal.getX() - 1);
          Terminal.window();
          cursor_x--;
          update_cursor(cursor_x, cursor_y);
        }

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
#endif
#endif