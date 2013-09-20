#include "oldschool.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

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
  
  screen = SDL_CreateRGBSurface(0, OSC_WIDTH, OSC_HEIGHT, 8, 0, 0, 0, 0);
  //SDL_SetSurfaceBlendMode(screen, SDL_BLENDMODE_NONE);
  sdlTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_INDEX8, SDL_TEXTUREACCESS_STREAMING, OSC_WIDTH, OSC_HEIGHT);
  //SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_NONE);
  return 0;
}

void setcolor(uint8_t colorindex, uint8_t r, uint8_t g, uint8_t b) {
  //SDL_SetPaletteColors(pal, 
  SDL_LockSurface(screen);
  SDL_Color color = screen->format->palette->colors[colorindex];
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = 255;
  SDL_UnlockSurface(screen);
}

void clear(uint8_t colorindex) {
	for (int y=0; y < screen->h; y++) {
			for (int x=0; x < screen->w; x++) {
        ((uint8_t*)screen->pixels)[x + y*screen->pitch] = colorindex;
			}
   }
}

void putpixel(int x, int y, uint8_t colorindex) {
  SDL_LockSurface(screen);
  if ((x >= 0) && (y >= 0) && (x < OSC_WIDTH) && (y < OSC_HEIGHT)) {
    // screen[x + y*OSC_WIDTH] = colorindex;
    ((uint8_t*)screen->pixels)[x + y*screen->pitch] = colorindex;
  }
  SDL_UnlockSurface(screen);
}

uint8_t getpixel(int x, int y) {
  // return screen[x + y*OSC_WIDTH];
  SDL_LockSurface(screen);
  uint8_t color = ((uint8_t*)screen->pixels)[x + y*screen->pitch];
  SDL_UnlockSurface(screen);
	return color;
}

void flip() {
  SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
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
  SDL_Keycode keycode;
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
  SDL_DestroyWindow(win);
  atexit(SDL_Quit);
}
