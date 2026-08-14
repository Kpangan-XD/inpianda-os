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

#include "io.hpp"
#include "config.hpp"

namespace inp {
  // it only use if you location on cursor_x and cursor_y not determite third party
ATT_NOTHROW ATT_COLD ATT_LEAF
void rprint(char raw, char color) {
  int indeks = (cursor_y * 80) + cursor_x;
  video_memory[indeks * 2] = raw;
  video_memory[indeks * 2 + 1] = color;
}
  // it only use if you location on cursor_x and cursor_y not determite third party
ATT_NOTHROW ATT_COLD ATT_LEAF
void rprintm(char raw, char color, int location) {
  video_memory[location * 2] = raw;
  video_memory[location * 2 + 1] = color;
}
// it only use if you determite location, not anything. 
ATT_NOTHROW ATT_COLD ATT_LEAF
void mrprint(char raw, char color, int x, int y, int i){
  int position_text = ((y * 80) + (x + i));
  video_memory[position_text * 2] = raw;
  video_memory[position_text * 2 + 1] = color;
}
ATT_NOHOT_DEPR("use printfs instead, this function deprecated")
 void dprintfs(const char *text, int x, int y, char color) {
  for (int i = 0; text[i] != OS_NULL; i++) {
    mrprint(text[i], color, x, y, i);
  }
}

}