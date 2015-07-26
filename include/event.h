
#ifndef SANDY_EVENT_H
#define SANDY_EVENT_H

#include "graphics.h"

void process_events(const SDL_Renderer *renderer,
                    const SDL_Window *window,
                    SDL_Surface **surface);

#endif /* SANDY_EVENT_H */
