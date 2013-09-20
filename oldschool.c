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

  // Calculate the size of a "pixel"
  spixelw = (float)width / (float)OSC_WIDTH;
  spixelh = (float)height / (float)OSC_HEIGHT;

  return 0;
}

void setcolor(uint8_t colorindex, uint8_t r, uint8_t g, uint8_t b) {
  reds[colorindex] = r;
  greens[colorindex] = g;
  blues[colorindex] = b;
}

void clear(uint8_t colorindex) {
  //SDL_SetRenderDrawColor(renderer, reds[colorindex], greens[colorindex], blues[colorindex], SDL_ALPHA_OPAQUE);
  //SDL_RenderClear(renderer);
  for (int i=0; i < OSC_WIDTH*OSC_HEIGHT; i++) {
    screen[i] = colorindex;
  }
}

void putpixel(int x, int y, uint8_t colorindex) {
  if ((x >= 0) && (y >= 0) && (x < OSC_WIDTH) && (y < OSC_HEIGHT)) {
    screen[x + y*OSC_WIDTH] = colorindex;
  }
}

uint8_t getpixel(int x, int y) {
  return screen[x + y*OSC_WIDTH];
}

void drawpixel(int x, int y, uint8_t colorindex) {
  SDL_SetRenderDrawColor(renderer, reds[colorindex], greens[colorindex], blues[colorindex], SDL_ALPHA_OPAQUE);
  const SDL_Rect pixel = {
    (int)((float)x * spixelw),
    (int)((float)y * spixelh),
    (int)spixelw,
    (int)spixelh
  };
  SDL_RenderFillRect(renderer, &pixel);
}

void flip() {
  // TODO: Use a hash map?
  // TODO: Blit instead?
  int i;
  bool changed = false;
  for (int y=0; y < OSC_HEIGHT; y++) {
    for (int x=0; x < OSC_WIDTH; x++) {
      i = x+y*OSC_WIDTH;
      // only draw changed "pixels"
      if (oldscreen[i] != screen[i]) {
  	drawpixel(x, y, screen[i]);
	changed = true;
      }
      oldscreen[i] = screen[i];
    }
  }
  if (changed) {
    SDL_RenderPresent(renderer);
    // TODO: Use this function instead:
    //SDL_UpdateWindowSurfaceRects(win, rects, numrects);
  }
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

SDL_Keycode getkey() {
  SDL_Event event;
  SDL_Keycode keycode;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      keycode = event.key.keysym.sym;
    }
  }
  return keycode;
}

uint8_t* getframebuffer() {
  return (uint8_t*)screen;
}

void quit() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  atexit(SDL_Quit);
}
