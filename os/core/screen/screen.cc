#include "define.hpp"

#include "screen.hpp"

extern char *video_memory;

namespace inp {
/***
 * f = force your set color or anything to display
 * pscreen = screen function || no force and no move, basic
 * cpscreen = function to help pscreen, abstraction
 * fcpscreen || cpscreen || mcpscreen
 * m = move screen
 *
 */

//   help function, DO NOT USE !!
inline void mcpscreen(int row, int column, char color) {}

//   help function, DO NOT USE !!
inline void fcpscreen(char color) {
  for (int i = 0; i < 80 * 25; i++) {
    video_memory[i * 2] = ' ';
    video_memory[i * 2 + 1] = color;
  }
}
//   help function, DO NOT USE !!
inline void cpscreen(char color) {}

// FORCE to Clear screen ALL, and repaint it with one color hexadecimal
// (0xXX)
inline void fpscreen(char color) {}

// NOT FORCE to Clear screen only, and repaint it with one color hexadecimal
// (0xXX)
inline void pscreen(char color) {}

// FORCE TO Move Screen ALL, and u can set x/y to set color in your
// display (0xXX)
void fmpscreen(int x, int y, char color) {}

// NOT FORCE TO Move Screen only, and u can set x/y to set color in your
// display (0xXX)
void mpscreen(int x, int y, char color) {}

} // namespace inp
