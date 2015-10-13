#ifndef SANDY_SHAPES_H
#define SANDY_SHAPES_H

#define CONTINUE_LINE 0
#define STOP_LINE 1

void line(void *data, int x0, int y0, int x1, int y1,
          int (*setPixel)(void *, int, int));

#endif /* SANDY_SHAPES_H */
