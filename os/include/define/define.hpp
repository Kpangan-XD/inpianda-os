#ifndef INP_DEFINE_HPP
#define INP_DEFINE_HPP


// DEFINE
// Macro

// SYSTEMS

#define OS_VIDEO_B ((char *)0xB8000)

// Pressed --------------------------------------------------------------------

// QWERTY KEYBOARD, ALPHABET --------------------------------------------------------------------
#define OS_Q 0x10
#define OS_W 0x11
#define OS_E 0x12
#define OS_R 0x13
#define OS_T 0x14
#define OS_Y 0x15
#define OS_U 0x16
#define OS_I 0x17
#define OS_O 0x18
#define OS_P 0x19

#define OS_A 0x1E
#define OS_S 0x1F
#define OS_D 0x20
#define OS_F 0x21
#define OS_G 0x22
#define OS_H 0x23
#define OS_J 0x24
#define OS_K 0x25
#define OS_L 0x26

#define OS_Z 0x2C
#define OS_X 0x2D
#define OS_C 0x2E
#define OS_V 0x2F
#define OS_B 0x30
#define OS_N 0x31
#define OS_M 0x32

// NUMBER --------------------------------------------------------------------

#define OS_1 0x02
#define OS_2 0x03
#define OS_3 0x04
#define OS_4 0x05
#define OS_5 0x06
#define OS_6 0x07
#define OS_7 0x08
#define OS_8 0x09
#define OS_9 0x0A
#define OS_0 0x0B

// ADDITIONAL KEYBOARD --------------------------------------------------------------------

#define OS_SCL 0x27     // ;
#define OS_FSL 0x35     // /
#define OS_BSL 0x2B     // \'
#define OS_SQUOTE 0x28  // '
#define OS_BCKTICK 0x29 // `

#define OS_COMA 0x33
#define OS_DOT 0x34

#define OS_SCR_1 0x1A // SQUARE CURLY BRACKETS [
#define OS_SCR_2 0x1B // SQUARE CURLY BRACKETS ]

#define OS_MIN 0x0C // - min
#define OS_SAM 0x0D // = sam

// SPECIAL --------------------------------------------------------------------

#define OS_CAP 0x3A
#define OS_ESC 0x01
#define OS_SPACE 0x39
#define OS_ENT 0x1C
#define OS_LCNT 0x1D
#define OS_RCNT 0x1D
#define OS_LSHT 0x2A
#define OS_RSHT 0x36
#define OS_NULL '\0'
#define OS_BACK 0x0E // BACKSPACE
#define OS_TAB 0x0F  // TAB
#define OS_EMPTY ' '

#define OS_ARR_UP 0x48
#define OS_ARR_DOWN 0x50

#define OS_ARR_LEFT 0x4B
#define OS_ARR_RIGHT 0x4D

// F1-F12 --------------------------------------------------------------------

#define OS_F1 0x3B
#define OS_F2 0x3C
#define OS_F3 0x3D
#define OS_F4 0x3E
#define OS_F5 0x3F
#define OS_F6 0x40
#define OS_F7 0x41
#define OS_F8 0x42
#define OS_F9 0x43
#define OS_F10 0x44
#define OS_F11 0x57
#define OS_F12 0x58

// Release --------------------------------------------------------------------

// QWERTY, ALPHABET --------------------------------------------------------------------

#define ROS_Q 0x90
#define ROS_W 0x91
#define ROS_E 0x92
#define ROS_R 0x93
#define ROS_T 0x94
#define ROS_Y 0x95
#define ROS_U 0x96
#define ROS_I 0x97
#define ROS_O 0x98
#define ROS_P 0x99

#define ROS_A 0x9E
#define ROS_S 0x9F
#define ROS_D 0x99
#define ROS_F 0x99
#define ROS_G 0x99
#define ROS_H 0x99
#define ROS_J 0x99
#define ROS_K 0x99
#define ROS_L 0x99
#define ROS_Z 0xAC
#define ROS_X 0xAD
#define ROS_C 0xAE
#define ROS_V 0xAF
#define ROS_B 0xB0
#define ROS_N 0xB1
#define ROS_M 0xB2

// ADDITIONAL --------------------------------------------------------------------
#define ROS_MIN 0x8C
#define ROS_SAM 0x8D

#define ROS_SCR_1 0x9A
#define ROS_SCR_2 0x9B

#define ROS_SCL 0xA7     // ;
#define ROS_FSL 0xB5     // /
#define ROS_BSL 0xAB     // \'
#define ROS_SQUOTE 0xA8  // '
#define ROS_BCKTICK 0xA9 // `

// SPECIAL --------------------------------------------------------------------
#define ROS_ESC 0x81
#define ROS_BACK 0x8E
#define ROS_TAB 0x8F
#define ROS_ENT 0x9C
#define ROS_LCNT 0x9D
#define ROS_RCNT 0x9D
#define ROS_LSHT 0xAA
#define ROS_RSHT 0xB6

// NUMBER --------------------------------------------------------------------

#define ROS_1 0x82
#define ROS_2 0x83
#define ROS_3 0x84
#define ROS_4 0x85
#define ROS_5 0x86
#define ROS_6 0x87
#define ROS_7 0x88
#define ROS_8 0x89
#define ROS_9 0x8A
#define ROS_0 0x8B

// SYSTEM

#define INP_SCREEN
#define INP_WINDOW
#define INP_IO
#define INP_CURSOR
#define INP_SYSTEM

// SYSTEM (CORE/THREAD) multithreading multicore !

#define CORE_THREAD

#define CORE_0
#define CORE_1

#define THREAD_CORE_0A 
#define THREAD_CORE_0B 
#define THREAD_CORE_1A 
#define THREAD_CORE_1B 

#endif // end
