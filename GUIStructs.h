#ifndef GUI_STRUCTS_H_
#define GUI_STRUCTS_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>

typedef struct widget_t Widget;
typedef struct button_t Button;
typedef struct window_t Window;

typedef enum _GAME_WINDOW{
    MAIN_MENU,
    GAME_MODE,
    ONE_PLAYER_OPTIONS,
    LOAD_GAME,
    GAME,
    QUIT
} GAME_WINDOW;

struct widget_t {
	struct window_t* window;
	void (*draw)(struct widget_t*);
	void (*handleEvent)(struct widget_t*, SDL_Event*);
	void (*destroy)(struct widget_t*);
	void* data;
};

struct button_t {
    SDL_Texture* texture;
    SDL_Rect location;
    GAME_WINDOW next_window;
    void (*action)(Widget *src);
};

struct window_t {
    SDL_Window* window;
    int height;
    int width;
	SDL_Renderer* renderer;
	SDL_Texture** textures;
	SDL_Rect** texRects;
    struct widget_t** widgets;
    int numOfTex;
    int numOfWidgets;
	bool isShown;
    GAME_WINDOW nextWindow;
};

#endif /* GUI_STRUCTS_H_ */
