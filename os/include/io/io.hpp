#ifndef INP_IO_HPP
#define INP_IO_HPP

#include "config.hpp"
#include "define.hpp"
#include <stdarg.h>

/**
 * This file include -*- C++ -*- for everyone.
 *  @author Contributed by Kpangan ZkxFsd
 * (kpanganbaru@gmail.com)
 *  @file io.hpp
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

extern int cursor_x;
extern int cursor_y;
extern volatile char* const video_memory;
extern char scancode_terakhir;
extern char scancode;

extern "C" {
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
char keyboard();
}

namespace inp {


// raw scancode, to write TEXT in screen, with color
// OLD print, dont use where you need x, y anything.
ATT_NOTHROW ATT_COLD 
void rprint(char raw, char color);

// have x, y to coor your TEXT.
ATT_NOTHROW ATT_COLD 
void mrprint(char raw, char color, int x, int y);

//  deprecated,
//  NOT support arg, only single.
//  normal to print text in screen
//  do not return because void()
// __attribute__((__nothrow__, __leaf__, __cold__,__deprecated__("use printfs
// instead, this function deprecated") ))
ATT_NOHOT_DEPR("use printfs instead, this function deprecated")
void dprintfs(const char *text, int x, int y, char color);

//  deprecated,
//  support arg, multiplex.
//  normal to print text in screen
//  do not return because void()
ATT_NOHOT_DEPR("use printf instead, this function deprecated")
ATT_FORMAT(printf, 1, 5)
void dprintf(const char* text, int x, int y, char color, ...);

ATT_COLD ATT_LEAF ATT_NOTHROW ATT_INLINE 
ATT_FORMAT(printf, 1, 5)
void printf(const char* text, int x, int y, char color, ...);


}
    
#endif //

// __attribute__((warn_unused_result)) __attribute__((always_inline))

/*
void fungsi_auto_bersih(int* ptr_variabel) {
    printf("[KERNEL LOG] Variabel di alamat %p otomatis keluar dari scope!\n",
(void*)ptr_variabel); printf("[KERNEL LOG] Nilai terakhirnya adalah: %d\n",
*ptr_variabel); printf("[KERNEL LOG] Memori dibersihkan di level
hardware.\n\n");
}

void jalankan_logika_game() {
    printf("--- Memulai Fungsi Logika Game ---\n");

    // --- 2. CARA PAKENYA ---
    // Kita buat variabel biasa, tapi ditempelin atribut cleanup
    int darah_bot __attribute__((cleanup(fungsi_auto_bersih))) = 100;

    printf("Darah bot saat ini di dalam memori: %d\n", darah_bot);

    // Iseng kita ubah nilainya di tengah jalan
    darah_bot -= 34;
    printf("Bot kena hit! Darah berkurang jadi: %d\n", darah_bot);

    printf("--- Akhir dari Fungsi Logika Game (Menyentuh Tutup Kurung) ---\n");
    // <-- TEPAT DI SINI, compiler bakal otomatis nyisipin
fungsi_auto_bersih(&darah_bot) secara ghaib!
}
*/