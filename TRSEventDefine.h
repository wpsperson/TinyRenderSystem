#pragma once


// mouse button
#define TRS_MOUSE_BUTTON_LEFT         0
#define TRS_MOUSE_BUTTON_RIGHT        1
#define TRS_MOUSE_BUTTON_MIDDLE       2

/* The unknown key */
#define TRS_KEY_UNKNOWN            -1

/* Printable keys */
#define TRS_KEY_SPACE              32
#define TRS_KEY_APOSTROPHE         39  /* ' */
#define TRS_KEY_COMMA              44  /* , */
#define TRS_KEY_MINUS              45  /* - */
#define TRS_KEY_PERIOD             46  /* . */
#define TRS_KEY_SLASH              47  /* / */
#define TRS_KEY_0                  48
#define TRS_KEY_1                  49
#define TRS_KEY_2                  50
#define TRS_KEY_3                  51
#define TRS_KEY_4                  52
#define TRS_KEY_5                  53
#define TRS_KEY_6                  54
#define TRS_KEY_7                  55
#define TRS_KEY_8                  56
#define TRS_KEY_9                  57
#define TRS_KEY_SEMICOLON          59  /* ; */
#define TRS_KEY_EQUAL              61  /* = */
#define TRS_KEY_A                  65
#define TRS_KEY_B                  66
#define TRS_KEY_C                  67
#define TRS_KEY_D                  68
#define TRS_KEY_E                  69
#define TRS_KEY_F                  70
#define TRS_KEY_G                  71
#define TRS_KEY_H                  72
#define TRS_KEY_I                  73
#define TRS_KEY_J                  74
#define TRS_KEY_K                  75
#define TRS_KEY_L                  76
#define TRS_KEY_M                  77
#define TRS_KEY_N                  78
#define TRS_KEY_O                  79
#define TRS_KEY_P                  80
#define TRS_KEY_Q                  81
#define TRS_KEY_R                  82
#define TRS_KEY_S                  83
#define TRS_KEY_T                  84
#define TRS_KEY_U                  85
#define TRS_KEY_V                  86
#define TRS_KEY_W                  87
#define TRS_KEY_X                  88
#define TRS_KEY_Y                  89
#define TRS_KEY_Z                  90
#define TRS_KEY_LEFT_BRACKET       91  /* [ */
#define TRS_KEY_BACKSLASH          92  /* \ */
#define TRS_KEY_RIGHT_BRACKET      93  /* ] */
#define TRS_KEY_GRAVE_ACCENT       96  /* ` */
#define TRS_KEY_WORLD_1            161 /* non-US #1 */
#define TRS_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define TRS_KEY_ESCAPE             256
#define TRS_KEY_ENTER              257
#define TRS_KEY_TAB                258
#define TRS_KEY_BACKSPACE          259
#define TRS_KEY_INSERT             260
#define TRS_KEY_DELETE             261
#define TRS_KEY_RIGHT              262
#define TRS_KEY_LEFT               263
#define TRS_KEY_DOWN               264
#define TRS_KEY_UP                 265
#define TRS_KEY_PAGE_UP            266
#define TRS_KEY_PAGE_DOWN          267
#define TRS_KEY_HOME               268
#define TRS_KEY_END                269
#define TRS_KEY_CAPS_LOCK          280
#define TRS_KEY_SCROLL_LOCK        281
#define TRS_KEY_NUM_LOCK           282
#define TRS_KEY_PRINT_SCREEN       283
#define TRS_KEY_PAUSE              284
#define TRS_KEY_F1                 290
#define TRS_KEY_F2                 291
#define TRS_KEY_F3                 292
#define TRS_KEY_F4                 293
#define TRS_KEY_F5                 294
#define TRS_KEY_F6                 295
#define TRS_KEY_F7                 296
#define TRS_KEY_F8                 297
#define TRS_KEY_F9                 298
#define TRS_KEY_F10                299
#define TRS_KEY_F11                300
#define TRS_KEY_F12                301
#define TRS_KEY_F13                302
#define TRS_KEY_F14                303
#define TRS_KEY_F15                304
#define TRS_KEY_F16                305
#define TRS_KEY_F17                306
#define TRS_KEY_F18                307
#define TRS_KEY_F19                308
#define TRS_KEY_F20                309
#define TRS_KEY_F21                310
#define TRS_KEY_F22                311
#define TRS_KEY_F23                312
#define TRS_KEY_F24                313
#define TRS_KEY_F25                314
#define TRS_KEY_KP_0               320
#define TRS_KEY_KP_1               321
#define TRS_KEY_KP_2               322
#define TRS_KEY_KP_3               323
#define TRS_KEY_KP_4               324
#define TRS_KEY_KP_5               325
#define TRS_KEY_KP_6               326
#define TRS_KEY_KP_7               327
#define TRS_KEY_KP_8               328
#define TRS_KEY_KP_9               329
#define TRS_KEY_KP_DECIMAL         330
#define TRS_KEY_KP_DIVIDE          331
#define TRS_KEY_KP_MULTIPLY        332
#define TRS_KEY_KP_SUBTRACT        333
#define TRS_KEY_KP_ADD             334
#define TRS_KEY_KP_ENTER           335
#define TRS_KEY_KP_EQUAL           336
#define TRS_KEY_LEFT_SHIFT         340
#define TRS_KEY_LEFT_CONTROL       341
#define TRS_KEY_LEFT_ALT           342
#define TRS_KEY_LEFT_SUPER         343
#define TRS_KEY_RIGHT_SHIFT        344
#define TRS_KEY_RIGHT_CONTROL      345
#define TRS_KEY_RIGHT_ALT          346
#define TRS_KEY_RIGHT_SUPER        347
#define TRS_KEY_MENU               348

// modifier keys
#define TRS_MOD_SHIFT           0x0001
#define TRS_MOD_CONTROL         0x0002
#define TRS_MOD_ALT             0x0004
#define TRS_MOD_SUPER           0x0008
#define TRS_MOD_CAPS_LOCK       0x0010
#define TRS_MOD_NUM_LOCK        0x0020