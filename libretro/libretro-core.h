#ifndef LIBRETRO_CORE_H
#define LIBRETRO_CORE_H 1

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>

// DEVICE retrow
#define RETRO_DEVICE_AMSTRAD_KEYBOARD RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_KEYBOARD, 0)
#define RETRO_DEVICE_AMSTRAD_JOYSTICK RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 1)

extern unsigned amstrad_devices[ 2 ];

#define TEX_MAX_WIDTH 800
#define TEX_MAX_HEIGHT 600

#define EMU_SND_FREQ 2 //2 - 44k
#define LIBRETRO_SND_FREQ 44100

//LOG
#if  defined(__ANDROID__) || defined(ANDROID)
#include <android/log.h>
#define LOG_TAG "RetroArch.Frodo"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#else
#define LOGI printf
#endif

//TYPES

#define UINT16 uint16_t
#define UINT32 uint32_t
#define uint32 uint32_t
#define uint8 uint8_t

#define SCREEN_16BITS

//SCREEN 16BITS
#ifdef SCREEN_16BITS
#define PIXEL_BYTES 2
#define PIXEL_TYPE UINT16
#define M16B
#endif

//SCREEN 32BITS
#ifdef SCREEN_32BITS
#define PIXEL_BYTES 4
#define PIXEL_TYPE UINT32
#endif

#define WINDOW_MAX_SIZE (800*600)
extern PIXEL_TYPE *Retro_Screen;

//VKBD
#define NPLGN 12
#define NLIGN 5
#define NLETT 5

typedef struct {
	char norml[NLETT];
	char shift[NLETT];
	int val;
} Mvk;

extern Mvk MVk[NPLGN*NLIGN*2];

//STRUCTURES
typedef struct{
     signed short x, y;
     unsigned short w, h;
} retro_Rect;

typedef struct{
     unsigned char *pixels;
     unsigned short w, h,pitch;
} retro_Surface;

typedef struct{
     unsigned char r,g,b;
} retro_pal;

#define MAX_INPUTS 2
#define MAX_BUTTONS 16
#define MAX_PADCFG 4
#define ID_PLAYER1 0
#define ID_PLAYER2 1

//VARIABLES
extern int pauseg;
extern PIXEL_TYPE * retro_getScreenPtr();
extern unsigned int retro_getStyle();
extern unsigned int retro_getGfxBpp();
extern unsigned int retro_getGfxBps();
extern void gui_input();
extern void poll_input();

#ifdef __ARM_NEON__
extern void armcpy(void *dst, const void *src, int bytes);
#endif

#endif
