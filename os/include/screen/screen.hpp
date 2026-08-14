#ifndef INP_SCREEN_HPP
#define INP_SCREEN_HPP

#include <define.hpp>
#include <config.hpp>

//  supported,
//  to print screen with one color,
//  normal to print screen ALL ,
//  do not return because void().
ATT_NOHOT_
INP_SCREEN
void screen_all_not(char color);


//  supported,
//  to print screen with one color,
//  normal to print screen ALL (FORCE),
//  do not return because void().
ATT_NOHOT_
INP_SCREEN
void screen_all_(char color);

//  supported,
//  to print screen with one color (FORCE)
//  normal to print screen with dependence int x, int y
//  do not return beacuse void()
ATT_NOHOT_
INP_SCREEN
void screen_depend_(char color, int x, int y);

//  supported,
//  to print screen with one color
//  normal to print screen with dependence int x, int y
//  do not return beacuse void()
ATT_NOHOT_
INP_SCREEN
void screen_depend_not(char color, int x, int y);

// void fpscreen(char color);
// void pscreen(char color);

#endif
