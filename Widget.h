#ifndef WIDGET_H_
#define WIDGET_H_

#include <SDL.h>
#include <SDL_video.h>

#include "GUIStructs.h"

/*
 * Frees a widget pointer (NULL-safe).
 */
void destroy_widget(widget_t *src);

#endif /* WIDGET_H_ */
