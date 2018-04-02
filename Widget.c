#include "Widget.h"

void destroy_widget(widget_t *src) {
    if (src != NULL)
        src->destroy(src);
}
