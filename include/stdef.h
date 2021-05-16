#pragma once

#define SCREEN_H            880
#define SCREEN_W            1080
#define SCREEN_FPS          60
#define INTENDED_PLAYTIME   3600    // frames

#ifdef _MSC_VER
# pragma warning(disable: 26812)
#endif

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

enum CauseOfDeath {
    outOfBorder,
    asteroidHit,
    cursorHit,
    Default
};
extern CauseOfDeath g_causeofdeath;

enum ButtonRect {
    hoverInactive = 0,
    hoverActive
};

struct Circle {
    int x, y, r;
};