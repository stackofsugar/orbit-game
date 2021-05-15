#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <time.h>

#define SCREEN_H                    480
#define SCREEN_W                    640
#define SCREEN_FPS                  60

const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
using namespace std;

SDL_Window *g_window;
SDL_Renderer *g_renderer;

class Timer {
private:
    Uint32 m_startTicks;
    Uint32 m_pausedTicks;
    bool m_started;
    bool m_paused;

public:
    Timer() {
        m_started = false;
        m_paused = false;
        m_startTicks = 0;
        m_pausedTicks = 0;
    }

    void start() {
        m_started = true;
        m_paused = false;
        m_startTicks = SDL_GetTicks();
        m_pausedTicks = 0;
    }

    void stop() {
        m_started = false;
        m_paused = false;
        m_startTicks = 0;
        m_pausedTicks = 0;
    }

    void pause() {
        if (m_started && !m_paused) {
            m_paused = true;
            m_pausedTicks = SDL_GetTicks() - m_startTicks;
            m_startTicks = 0;
        }
    }

    void unpause() {
        if (m_started && m_paused) {
            m_paused = false;
            m_startTicks = SDL_GetTicks() - m_pausedTicks;
            m_pausedTicks = 0;
        }
    }

    Uint32 getTicks() {
        if (m_started) {
            if (m_paused) {
                return m_pausedTicks;
            }
            else {
                return (SDL_GetTicks() - m_startTicks);
            }
        }

        return 0;
    }
};

class Texture {
private:
    SDL_Texture *m_texture;
    int m_width;
    int m_height;

public:
    Texture() {
        m_texture = NULL;
        m_height = 0;
        m_width = 0;
    }

    ~Texture() {
        free();
    }

    bool loadFromFile(string path) {
        free();
        SDL_Texture *newTexture = NULL;
        SDL_Surface *newSurface = IMG_Load(path.c_str());

        if (newSurface == NULL) {
            return false;
        }

        SDL_SetColorKey(newSurface, SDL_TRUE, SDL_MapRGB(newSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(g_renderer, newSurface);

        if (newTexture == NULL) {
            return false;
        }

        m_width = newSurface->w;
        m_height = newSurface->h;
        SDL_FreeSurface(newSurface);
        m_texture = newTexture;

        return true;
    }

    void free() {
        if (m_texture != NULL) {
            SDL_DestroyTexture(m_texture);
            m_texture = NULL;
        }
    }

    void render(int x, int y) {
        SDL_Rect renderTarget = { x, y, m_width, m_height };
        SDL_RenderCopy(g_renderer, m_texture, NULL, &renderTarget);
    }

    int getHeight() {
        return m_height;
    }

    int getWidth() {
        return m_width;
    }
};

Texture g_player;
Texture g_background;

bool loadMedia() {
    if (!g_player.loadFromFile("res2//foo.png")) {
        return false;
    }
    
    if (!g_background.loadFromFile("res2//background.png")) {
        return false;
    }

    return true;
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("penis", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                     SCREEN_W, SCREEN_H, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

    if (!g_renderer || !g_window) {
        return false;
    }

    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0xFF);
    int imgFlags = IMG_INIT_PNG;

    if (!(IMG_Init(imgFlags) & imgFlags)) {
        return false;
    }

    if (!loadMedia()) {
        return false;
    }

    return true;
}

void close() {
    g_player.free();
    g_background.free();

    SDL_DestroyWindow(g_window);
    SDL_DestroyRenderer(g_renderer);
    g_window = NULL;
    g_renderer = NULL;
    IMG_Quit();
    SDL_Quit();
}

// [lower, upper]
int randRange(int lower, int upper) {
    return (lower + rand() % (upper - lower + 1));
}

int main(int argc, char **argv) {
    if (!init()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Program Error", SDL_GetError(), g_window);
        return -1;
    }

    srand((unsigned)(time(0)));
    SDL_Event ev = {};
    bool running = true;
    Timer capTimer;


    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(g_renderer);
    g_background.render(0, 0);
    g_player.render(240, 190);
    SDL_RenderPresent(g_renderer);

    while (running) {
        capTimer.start();

        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
            else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                SDL_RenderClear(g_renderer);
                g_background.render(0, 0);
                g_player.render(randRange(0, SCREEN_W - g_player.getWidth()),
                                randRange(0, SCREEN_H - g_player.getHeight()));
                SDL_RenderPresent(g_renderer);
            }
        }

        int frameTicks = capTimer.getTicks();
        if (frameTicks < SCREEN_TICKS_PER_FRAME) {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }

    close();
    return 0;
}