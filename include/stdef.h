#pragma once

#define SCREEN_H        880
#define SCREEN_W        1080
#define SCREEN_FPS      60

extern SDL_Window *g_window;
extern SDL_Renderer *g_renderer;
extern bool g_isAlive;

// Global helper function
int randRange(int lower, int upper);

enum class GameState {
    mainMenu,
    inTutorial,
    inGame,
    pauseMenu,
    killedMenu
};

enum class GameDifficulty {
    easy,
    medium,
    hard
};

struct Circle {
    int x, y, r;
};