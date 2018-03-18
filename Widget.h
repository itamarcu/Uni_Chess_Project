#ifndef WIDGET_H_
#define WIDGET_H_

#include <SDL.h>
#include <SDL_video.h>

#include "GUIStructs.h"

// useful function for NULL-safe destroy
void destroyWidget(Widget *src);

#endif /* WIDGET_H_ */
