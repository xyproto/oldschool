#include "oldschool.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "vgapal.h"

const int OSC_WIDTH = 320;
const int OSC_HEIGHT = 200;

int init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    fprintf(stderr,
	    "\nCould not initialize SDL: %s\n",
	    SDL_GetError()
	   );
    return 1;
  }

  win = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, OSC_WIDTH, OSC_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
  if (win == NULL){
    fprintf(stderr,
	    "\nCould not create window: %s\n",
	    SDL_GetError()
	   );
    return 1;
  }

  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL){
    fprintf(stderr,
	    "\nCould not create renderer: %s\n",
	    SDL_GetError()
	   );
    return 1;
  }

  int width = 0;
  int height = 0;
  SDL_GetWindowSize(win, &width, &height);

  //SDL_RendererFlags(SDL_RENDERER_PRESENTVSYNC);

  screen = SDL_CreateRGBSurface(0, OSC_WIDTH, OSC_HEIGHT, 32, 0, 0, 0, 0);
  if (screen == NULL) {
    fprintf(stderr,
	    "\nCould not create surface: %s\n",
	    SDL_GetError()
	   );
  }

  sdlTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, OSC_WIDTH, OSC_HEIGHT);
  if (sdlTexture == NULL) {
    fprintf(stderr,
	    "\nCould not create texture: %s\n",
	    SDL_GetError()
	   );
  }

  // Turn off alpha
  SDL_SetSurfaceBlendMode(screen, SDL_BLENDMODE_NONE);
  SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_NONE);

  // Set up the palette like for mode13h
  for (int i=0; i<256; i++) {
    setcolor(i, vgapal[i*3], vgapal[i*3+1], vgapal[i*3+2]);
  }

  return 0;
}

// Changing the palette for an "indexed color" on an RGB surface is a pretty slow operation
void setcolor(uint8_t colorindex, uint8_t r, uint8_t g, uint8_t b) {
  reds[colorindex] = r;
  greens[colorindex] = g;
  blues[colorindex] = b;
  // TODO: Find a way to fix all the old pixels when a color changes

/*
  //SDL_SetPaletteColors(pal, 
  SDL_LockSurface(screen);
  SDL_Color color = screen->format->palette->colors[colorindex];
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = 255;
  SDL_UnlockSurface(screen);
*/
}

void clear(uint8_t colorindex) {
  Uint32 pixel = SDL_MapRGB(screen->format, reds[colorindex], greens[colorindex], blues[colorindex]);
  Uint32* pixels = screen->pixels;
  SDL_LockSurface(screen);
  for (int i=0; i < screen->h*screen->w; i++) {
    pixels[i] = pixel;
  }
  SDL_UnlockSurface(screen);
  // TODO: Use SDL functions to clear the surface instead
}

// TODO: Draw to a buffer instead, only lock/unluck the surface when flipping
void putpixel(int x, int y, uint8_t colorindex) {
  Uint32 pixel = SDL_MapRGB(screen->format, reds[colorindex], greens[colorindex], blues[colorindex]);
  if ((x >= 0) && (y >= 0) && (x < OSC_WIDTH) && (y < OSC_HEIGHT)) {

    //SDL_LockSurface(screen);
    SDL_LockTexture(sdlTexture, NULL, &screen->pixels, &screen->pitch);
    ((Uint32*)screen->pixels)[x + y*OSC_WIDTH] = pixel;
    SDL_UnlockTexture(sdlTexture);
    //SDL_UnlockSurface(screen);
  }
  //SDL_FillRect(screen, NULL, pixel);
  //SDL_LockSurface(screen);
  //if ((x >= 0) && (y >= 0) && (x < OSC_WIDTH) && (y < OSC_HEIGHT)) {
  //  ((Uint32*)screen->pixels)[x + y*screen->pitch] = SDL_MapRGB(screen->format, reds[colorindex], greens[colorindex], blues[colorindex]);
  //}
  //SDL_UnlockSurface(screen);
}

uint8_t getpixel(int x, int y) {
  // return screen[x + y*OSC_WIDTH];
  uint8_t r;
  uint8_t g;
  uint8_t b;
  //SDL_LockSurface(screen);
  SDL_LockTexture(sdlTexture, NULL, &screen->pixels, &screen->pitch);
  Uint32 packedColor = ((Uint32*)screen->pixels)[x + y*screen->pitch];
  SDL_GetRGB(packedColor, screen->format, &r, &g, &b);
  SDL_UnlockTexture(sdlTexture);
  //SDL_UnlockSurface(screen);
  for (int i=0; i<256; i++) {
    if ((reds[i] == r) && (greens[i] == g) && (blues[i] == b)) {
      return i;
    }
  }
  return 0;
}

void flip() {
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, OSC_WIDTH * sizeof(Uint32));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
  SDL_RenderPresent(renderer);
  //SDL_UpdateWindowSurfaceRects(win, rects, numrects);
}

void sleep(uint32_t ms) {
  SDL_Delay(ms);
}

// has a key been pressed?
bool keypressed() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      return true;
    }
  }
  return false;
}

// has ESC been pressed?
bool escpressed() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if ((event.type == SDL_KEYDOWN) && (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
      return true;
    }
  }
  return false;
}

// wait for a key to be released
void wfk() {
  SDL_Event event;
  while (SDL_WaitEvent(&event)) {
    if (event.type == SDL_KEYUP) {
      return;
    }
  }
}

const char* getkey() {
  SDL_Event event;
  SDL_Keycode keycode = 0;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      keycode = event.key.keysym.sym;
    }
  }
  return SDL_GetKeyName(keycode);
}

uint8_t* getframebuffer() {
  return (uint8_t*)(screen->pixels);
}

void quit() {
  SDL_DestroyRenderer(renderer);
  //SDL_DestroySurface(screen);
  SDL_DestroyTexture(sdlTexture);
  SDL_DestroyWindow(win);
  atexit(SDL_Quit);
}
