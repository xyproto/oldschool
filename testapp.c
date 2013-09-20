#include <stdio.h>
#include "oldschool.h"

int main(void) {
  init();

  clear(0);
  for (int x=0; x < 256; x++) {
    putpixel(x, 80, x);
  }
  flip();
  wfk();

  setcolor(0, 0, 0, 0);
  setcolor(1, 0, 0, 100);
  setcolor(2, 255, 0, 0);
  setcolor(3, 120, 255, 200);
  setcolor(4, 255, 255, 255);
  clear(3);
  putpixel(0, 0, 2);
  putpixel(1, 1, 2);
  flip();
  //SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
  //SDL_RenderDrawLine(renderer, 0, 0, 319, 199);
  wfk();
  
  putpixel(319, 199, 4);
  flip();
  wfk();

  int x = 1;
  int direction = 1;
  while (!keypressed()) {
    clear(0);
    for (int y=0; y < 200; y++) {
	putpixel(x, y, 3);
    }
    x += direction;
    if (x >= 320) {
	direction = -1;
	x = 319;
    } else if (x <= 0) {
	direction = 1;
	x = 0;
    }
    flip();
    sleep(10);
  }

  while (!escpressed()) {
    sleep(10);
  }

  quit();
  return 0;
}
