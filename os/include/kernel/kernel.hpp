#ifndef INP_KERNEL_HPP
#define INP_KERNEL_HPP

/**
* This file include -*- C++ -*- for everyone.
 *  @author Contributed by Kpangan ZkxFsd
 * (kpanganbaru@gmail.com)
 *  @file kernel.hpp
 *  ISF (Inpianda Software Foundation)
 *  @date 17 Mei 2026  -> ~~~
 *  @version 1.0
 *  @brief Component of Operating Systems.
 *  @copyright (C) 2026-INF (ISF) Inpianda Software Foundation,
 * Inc. This file is part and real of ISF Product. You can use
 * or modify it under ISF terms. All rights reserved.
 */
/**
 * Terms of Inpianda Software Foundation (ISF)
 * You can change it or modify it under ISF Terms.
 * You should have received a copy of GNU General Public
 * License, and a copy of (ISF) Inpianda Software
 * Foundation along with this code. Real Website :
 * Main                         : https://www.inpianda.org/index.html.
 * License                      : https://www.inpianda.org/license/license.html. 
 * Terms (ISF terms, GNU terms) : https://www.inpianda.org/terms/terms.html.  
 * - You can modify the code if you can.
 * - You can change the code under GNU License.
 * - You can use the code to your project, \ 
 *   but remember under ISF terms and GNU License.
 *   " use You can !!! " 
 */

#include "define.hpp"

extern "C" {
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
char prt_keyboard();
} 
inline void outw(unsigned short port, unsigned short val) {
  // Pake "outw" karena nilainya 16-bit (short), bukan "outb" (char)
  __asm__ __volatile__("outw %0, %1" : : "a"(val), "Nd"(port));
}
extern "C" char keyboard();



// Cetakan kursor ukuran 12 baris x 12 kolom
const unsigned char cursor_arrow[12][12] = {
    {1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, {1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    {1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2}, {1, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2},
    {1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2}, {1, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2},
    {1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2}, {1, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2},
    {1, 0, 0, 1, 2, 1, 2, 2, 2, 2, 2, 2}, {1, 0, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2},
    {1, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}};

// scan code for keyboard, u can see that?
inline char scancode_(char scancode) {
  switch (scancode) {

    // A to Z
    // Scancode PS/2 Keyboard
  case OS_Q:
    return 'Q';
  case OS_W:
    return 'W';
  case OS_E:
    return 'E';
  case OS_R:
    return 'R';
  case OS_T:
    return 'T';
  case OS_Y:
    return 'Y';
  case OS_U:
    return 'U';
  case OS_I:
    return 'I';
  case OS_O:
    return 'O';
  case OS_P:
    return 'P';
  case OS_A:
    return 'A';
  case OS_S:
    return 'S';
  case OS_D:
    return 'D';
  case OS_F:
    return 'F';
  case OS_G:
    return 'G';
  case OS_H:
    return 'H';
  case OS_J:
    return 'J';
  case OS_K:
    return 'K';
  case OS_L:
    return 'L';
  case OS_Z:
    return 'Z';
  case OS_X:
    return 'X';
  case OS_C:
    return 'C';
  case OS_V:
    return 'V';
  case OS_B:
    return 'B';
  case OS_N:
    return 'N';
  case OS_M:
    return 'M';
  // NUMBER
  case OS_1:
    return '1';
  case OS_2:
    return '2';
  case OS_3:
    return '3';
  case OS_4:
    return '4';
  case OS_5:
    return '5';
  case OS_6:
    return '6';
  case OS_7:
    return '7';
  case OS_8:
    return '8';
  case OS_9:
    return '9';
  case OS_0:
    return '0';

  case OS_SCL:
    return ';';
  case OS_FSL:
    return '/';
  case OS_BSL:
    return '\\';
  case OS_SQUOTE:
    return '\'';
  case OS_BCKTICK:
    return '`';

  case OS_COMA:
    return ',';
  case OS_DOT:
    return '.';

  case OS_SCR_1:
    return '[';
  case OS_SCR_2:
    return ']';

  case OS_MIN:
    return '-';
  case OS_SAM:
    return '=';

  case OS_SPACE:
    return ' '; // Scancode tombol Spacebar
  default:
    return OS_NULL; // Abaikan tombol lain untuk sementara
  }
}

/****** SMALL FUNCTION
 * IND : Fungsi kecil
 * ENG : smalls functions
 */

#pragma region small_function



inline void clear_screen(char warna_latar) {
  char *video_memory = (char *)0xB8000;

  // Total karakter di layar adalah 80 kolom x 25 baris = 2000 karakter
  for (int i = 0; i < 80 * 25; i++) {
    // Isi dengan karakter spasi (ASCII 32) untuk mengosongkan teks
    video_memory[i * 2] = ' ';
    // Berikan warna latar belakang (misal 0x07 untuk hitam biasa)
    video_memory[i * 2 + 1] = warna_latar;
  }
}

inline void screen(char warna_latar, int baris, int kolom) {
  char *video_memory = OS_VIDEO_B;

  // Total karakter di layar adalah 80 kolom x 25 baris = 2000 karakter

  // Screen 20 Column x 25 row

  for (int i = 0; i < baris * kolom; i++) {
    video_memory[i * 2 + 1] = warna_latar;
  }
}

inline void screen_move(char warna_latar, int baris, int kolom) {
  char *video_memory = (char *)0xB8000;

  // Total karakter di layar adalah 80 kolom x 25 baris = 2000 karakter

  // Screen 20 Column x 25 row

  for (int i = 0; i < baris * kolom; i++) {
    // Isi dengan karakter spasi (ASCII 32) untuk mengosongkan teks
    video_memory[i * 2] = ' ';
    // Berikan warna latar belakang (misal 0x07 untuk hitam biasa)
    video_memory[i * 2 + 1] = warna_latar;
  }
}

#pragma endregion small_function

/****** ONE FUNCTION
 * IND : Fungsi kecil - kecil jadi fungsi gede
 * ENG : smalls functions to one function
 */

#pragma region one_function
inline int hitung_panjang(const char *str) {
  int panjang = 0;
  while (str[panjang] != '\0') {
    panjang++;
  }
  return panjang;
}
 inline void print_string(const char *pesan, int baris, int kolom, char warna) {
  // Alamat awal memori teks VGA
  char *video_memory = (char *)0xB8000;

  // Rumus mencari posisi indeks array berdasarkan koordinat Baris dan Kolom
  // 80 adalah jumlah kolom standar layar, dikali 2 karena 1 karakter = 2 byte
  int posisi_memori = (baris * 80 + kolom) * 2;

  int panjang_pesan = hitung_panjang(pesan);

  for (int i = 0; i < panjang_pesan; i++) {
    // Taruh karakter di byte pertama
    video_memory[posisi_memori] = pesan[i];

    // Taruh warna di byte kedua
    video_memory[posisi_memori + 1] = warna;

    // Lompat 2 byte ke depan untuk karakter berikutnya
    posisi_memori += 2;
  }
}

#pragma endregion one_function

/***
 * CLASS
 *
 *
 */

#pragma region CURSOR_LAMA (32BIT)
#if 0
inline void draw_pixel(int px, int py, unsigned char warna_indeks) {
  // Jagain biar gak bocor keluar batas layar 320x200 (Biar gak triple fault!)
  if (px < 0 || px >= 320 || py < 0 || py >= 200)
    return;

  unsigned char *vga_mem = (unsigned char *)0xA0000;
  // Rumus legendaris: X + (Y * Lebar Layar)
  vga_mem[px + (py * 320)] = warna_indeks;
}

inline void draw_cursor(int mouse_x, int mouse_y) {
  for (int baris = 0; baris < 12; baris++) {
    for (int kolom = 0; kolom < 12; kolom++) {
      unsigned char warna_piksel = cursor_arrow[baris][kolom];

      // Logika Transparansi: Kalau kodenya 2, dilewati (gak usah digambar,
      // cok!)
      if (warna_piksel == 2)
        continue;

      // Tentukan warnanya berdasarkan isi cetakan
      unsigned char warna_final =
          (warna_piksel == 1) ? 0x0F : 0x00; // 0x0F putih, 0x00 hitam

      // Gambar per titik piksel sesuai posisi koordinat mouse!
      draw_pixel(mouse_x + kolom, mouse_y + baris, warna_final);
    }
  }
}

// Array buat nyontek 12x12 piksel background yang ketutupan kursor
inline unsigned char kursor_backup[12][12];

// FUNGSI 1: Nyontek background sebelum kursor digambar
inline void simpan_background(int mx, int my) {
  unsigned char *vga_mem = (unsigned char *)0xA0000;
  for (int b = 0; b < 12; b++) {
    for (int k = 0; k < 12; k++) {
      kursor_backup[b][k] = vga_mem[(mx + k) + ((my + b) * 320)];
    }
  }
}

// FUNGSI 2: Ngembaliin contekan (Ngapus kursor lama)
inline void hapus_kursor_lama(int mx, int my) {
  for (int b = 0; b < 12; b++) {
    for (int k = 0; k < 12; k++) {
      // Gambar ulang piksel background aslinya dari array contekan
      draw_pixel(mx + k, my + b, kursor_backup[b][k]);
    }
  }
}

// inline int mouse_x = 160; // Posisi awal mouse di tengah layar (X)
// inline int mouse_y = 100; // Posisi awal mouse di tengah layar (Y)

inline void update_posisi_mouse(int delta_x, int delta_y) {
  // 1. HAPUS KURSOR DI POSISI LAMA DULU, COK!
  hapus_kursor_lama(mouse_x, mouse_y);

  // 2. Update koordinatnya berdasarkan geseran tangan user
  mouse_x += delta_x;
  mouse_y += delta_y;

  // Jagain biar kursor gak lari keluar batas layar 320x200
  if (mouse_x < 0)
    mouse_x = 0;
  if (mouse_x > 308)
    mouse_x = 308; // 320 - 12 (lebar kursor)
  if (mouse_y < 0)
    mouse_y = 0;
  if (mouse_y > 188)
    mouse_y = 188; // 200 - 12 (tinggi kursor)

  // 3. CONTEK BACKGROUND BARU DI POSISI BARU
  simpan_background(mouse_x, mouse_y);

  // 4. GAMBAR KURSOR GANTENG LO DI POSISI BARU!
  draw_cursor(mouse_x, mouse_y);
}
#endif
#pragma endregion CURSOR_LAMA(32BIT)

#endif
