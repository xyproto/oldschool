#ifndef H_OLDSCHOOL
#define H_OLDSCHOOL

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "SDL2/SDL.h"

SDL_Window *win;
SDL_Renderer *renderer;

float spixelw; // width of a "pixel" in the native resolution
float spixelh; // height of a "pixel" in the native resolution

// the palette
uint8_t reds[256];
uint8_t greens[256];
uint8_t blues[256];

// the framebuffer
uint8_t screen[64000];
// copy of the framebuffer, used for comparison to avoid unnecessary drawing
uint8_t oldscreen[64000];

int init(); // initialize everything (fullscreen graphics)
void clear(uint8_t colorindex); // clear the framebuffer with the given color index
void setcolor(uint8_t colorindex, uint8_t r, uint8_t g, uint8_t b); // define a color index with red, green and blue
void putpixel(int x, int y, uint8_t colorindex); // place a pixel at the given coordinate, with the given color index
uint8_t getpixel(int x, int y); // get the color index of the given coordinate
void flip(); // draw the pixels
void quit(); // quit
void sleep(uint32_t ms); // wait for the given number of milliseconds
bool keypressed(); // is a key down?
SDL_Keycode getkey(); // get the keycode for the last pressed key
void wfk(); // wait for a key to be released (or pressed and then released)
bool escpressed(); // check if esc has been pressed

#ifdef __cplusplus
} /* End of extern "C" { */
#endif

#endif /* H_NEWT */
