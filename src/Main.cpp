#include "../include/pch.h"

const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

bool g_isAlive = true;                  // global extern
using namespace std;

SDL_Window *g_window;                   // global extern
SDL_Renderer *g_renderer;               // global extern
CauseOfDeath g_causeofdeath;            // global extern

bool loadMedia() {

    return true;
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("bepis", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_W, SCREEN_H, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

    SDL_ShowCursor(SDL_DISABLE);

    return true;
}

void close() {
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

void handleErrors() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Program Error", SDL_GetError(), g_window);
    close();
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (!init()) {
        handleErrors();
    }

    Player player;
    if (!player.loadFromFile("res2//redSmaller.png")) {
        handleErrors();
    }

    Asteroids asteroid;
    if (!asteroid.loadFromFile("res2//asteroid.png")) {
        handleErrors();
    }

    ScrollingBg scrollingbg;
    if (!scrollingbg.loadFromFile("res2//scrlbg.png")) {
        handleErrors();
    }

    Cursor cursor;
    if (!cursor.loadFromFile("res2//cursor.png")) {
        handleErrors();
    }

    srand((unsigned)(time(0)));
    SDL_Event ev = {};
    bool running = true;

    SDL_RenderClear(g_renderer);
    SDL_RenderPresent(g_renderer);

    player.updateToInitialPosition();
    asteroid.initSpawn();
    g_causeofdeath = CauseOfDeath::asteroidHit;
    //asteroid.DEBUG_SETXY(SCREEN_W / 2, SCREEN_H / 2 - 200);

    GameState gameState = GameState::mainMenu;
    int mouseStateX = 0, mouseStateY = 0, elapsedFrameInGame = 0,
        velocityValue = 0, accelTimeValue = 0, accelVariableValue = 0;

    while (running) {
        SDL_RenderClear(g_renderer);

        while (SDL_PollEvent(&ev) != 0) {
            SDL_GetMouseState(&mouseStateX, &mouseStateY);
            cursor.processAndUpdateMovement(mouseStateX, mouseStateY);
            if (ev.type == SDL_QUIT) {
                running = false;
            }
            else if (ev.type == SDL_KEYDOWN) {
                if (gameState == GameState::inGame) {
                    if (ev.key.keysym.sym == SDLK_ESCAPE) {
                        gameState = GameState::pauseMenu;
                        cout << "pausing\n";
                    }
                }
                else if (gameState == GameState::inTutorial) {
                    if (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER) {
                        gameState = GameState::inGame;
                        cout << "entering game\n";
                        scrollingbg.setIngameLoopStatus(true);
                    }
                }
                else if (gameState == GameState::mainMenu) {
                    if (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER) {
                        gameState = GameState::inTutorial;
                        cout << "entering tutorial\n";
                    }
                }
                else if (gameState == GameState::pauseMenu) {
                    if (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER) {
                        gameState = GameState::inGame;
                        cout << "entering game\n";
                    }
                }
                else if (gameState == GameState::killedMenu) {
                    if (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER) {
                        gameState = GameState::inGame;
                        cout << "entering game\n";
                        scrollingbg.setIngameLoopStatus(true);
                    }
                }
            }
        }

        if (gameState == GameState::inGame) {
            elapsedFrameInGame++;

            scrollingbg.calculateMove();
            scrollingbg.render();

            player.processMovement(mouseStateX, mouseStateY);
            player.updateMove();
            player.render();

            velocityValue = 50 + (int)(((float)elapsedFrameInGame / INTENDED_PLAYTIME) * (120 - 50));
            accelTimeValue = 20 + (int)(((float)elapsedFrameInGame / INTENDED_PLAYTIME) * (0 - 20));
            accelVariableValue = 1 + (int)(((float)elapsedFrameInGame / INTENDED_PLAYTIME) * (5 - 1));

            asteroid.setDifficulty(velocityValue, accelTimeValue, accelVariableValue);

            asteroid.processMovement();
            asteroid.updateMove();
            asteroid.render();

            if (player.isCollided(asteroid)) {
                g_causeofdeath = CauseOfDeath::asteroidHit;
                g_isAlive = false;
            }

            if (!g_isAlive) {
                cout << "you're dead\n";
                g_isAlive = true;

                player.updateToInitialPosition();
                asteroid.initSpawn();
                elapsedFrameInGame = 0;

                gameState = GameState::killedMenu;
                scrollingbg.setIngameLoopStatus(false);
            }
        }
        else if (gameState == GameState::mainMenu) {
            scrollingbg.calculateMove();
            scrollingbg.render();
        }
        else if (gameState == GameState::inTutorial) {
            scrollingbg.calculateMove();
            scrollingbg.render();
        }
        else if (gameState == GameState::pauseMenu) {
            scrollingbg.render();
        }
        else if (gameState == GameState::killedMenu) {
            scrollingbg.render();
        }

        cursor.render();
        SDL_RenderPresent(g_renderer);
    }

    close();
    return 0;
}