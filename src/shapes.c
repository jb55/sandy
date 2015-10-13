
#include <stdlib.h>
#include "shapes.h"

void line(void *data, int x0, int y0, int x1, int y1, int (*setPixel)(void *, int, int)) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    if (setPixel(data, x0, y0) == STOP_LINE) break;
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}
