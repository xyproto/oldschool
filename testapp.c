#include <stdio.h>
#include "oldschool.h"

int main(void) {
  init();
  setcolor(0, 0, 0, 0);
  setcolor(1, 0, 0, 100);
  setcolor(2, 255, 0, 0);
  setcolor(3, 120, 255, 200);
  setcolor(4, 255, 255, 255);
  clear(1);
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
  while (!keypressed()) {
    putpixel(x, 0, 3);
    x++;
    flip();
    sleep(10);
  }

  while (!escpressed()) {
    sleep(10);
  }

  quit();
  return 0;
}
