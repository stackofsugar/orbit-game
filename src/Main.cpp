#include "../include/pch.h"

bool g_isAlive = true;                  // global extern
using namespace std;

SDL_Window *g_window;                   // global extern
SDL_Renderer *g_renderer;               // global extern
CauseOfDeath g_causeofdeath;            // global extern

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Orbit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
    return (lower + ((lower + rand() % (upper - lower + 1)) ^ (lower + rand() % (upper - lower + 1))) % (upper - lower + 1));
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
    Asteroids asteroid[2];
    ScrollingBg scrollingbg;
    Cursor cursor;
    GameMenu mainMenu;
    GameMenu tutorialMenu;
    GameMenu pauseMenu;
    GameMenu killedMenu[3];
    GameMenu ingameOverlay;
    ButtonTwoState respawnButton;
    ButtonTwoState spawnButton;

    if (!player.loadFromFile("res//entities//player.png") ||
        !asteroid[0].loadFromFile("res//entities//asteroid.png") ||
        !asteroid[1].loadFromFile("res//entities//asteroid.png") ||
        !scrollingbg.loadFromFile("res//scrlbg.png") ||
        !cursor.loadFromFile("res//entities//cursor.png") ||
        !mainMenu.loadFromFile("res//menus//mainMenu.png") ||
        !pauseMenu.loadFromFile("res//menus//pauseMenu.png") ||
        !respawnButton.loadFromFile("res//buttons//respawnButton.png") ||
        !spawnButton.loadFromFile("res//buttons//spawnButton.png") ||
        !killedMenu[0].loadFromFile("res//menus//killedMenu//outOfBounds.png") ||
        !killedMenu[1].loadFromFile("res//menus//killedMenu//crashedToAsteroids.png") ||
        !killedMenu[2].loadFromFile("res//menus//killedMenu//crashedToMoon.png") ||
        !tutorialMenu.loadFromFile("res//menus//tutorialMenu.png") ||
        !ingameOverlay.loadFromFile("res//menus//ingameOverlaySmall.png")
        ) {
        handleErrors();
    }

    respawnButton.initSheet(true, SCREEN_W / 2, 544);
    spawnButton.initSheet(true, SCREEN_W / 2, 727);

    SDL_Event ev = {};
    bool running = true;

    SDL_RenderClear(g_renderer);
    SDL_RenderPresent(g_renderer);

    srand((unsigned)time(0));

    asteroid[0].initSpawn(0, (float)randRange(0, 360),
                          randRange(10, (SCREEN_W / 2) - (int)(asteroid[0].getWidth() * (float)(1.5f))));
    asteroid[1].initSpawn(randRange(10, 100), (float)randRange(0, 360),
                          randRange((SCREEN_W / 2) + (asteroid[1].getWidth() / 2), SCREEN_W - asteroid[1].getWidth() - 10));

    g_causeofdeath = CauseOfDeath::Default;
    GameState gameState = GameState::mainMenu;

    int mouseStateX = 0, mouseStateY = 0, elapsedFrameInGame = 0,
        velocityValue = 0, accelTimeValue = 0, accelVariableValue = 0;
    float spinAcceleration = 0.0f;

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
                        player.updateToInitialPosition(mouseStateX, mouseStateY);
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
                        cout << "respawned\n";
                        player.updateToInitialPosition(mouseStateX, mouseStateY);
                        scrollingbg.setIngameLoopStatus(true);
                    }
                }
            }
            else if (ev.type == SDL_MOUSEBUTTONUP) {
                if (gameState == GameState::mainMenu) {
                    gameState = GameState::inTutorial;
                    cout << "entering tutorial\n";
                }
                else if (gameState == GameState::pauseMenu) {
                    gameState = GameState::inGame;
                    cout << "entering game\n";
                }
                else if (gameState == GameState::killedMenu && respawnButton.watchForMouseHover(mouseStateX, mouseStateY)) {
                    gameState = GameState::inGame;
                    cout << "respawned\n";
                    scrollingbg.setIngameLoopStatus(true);
                }
                else if (gameState == GameState::inTutorial && spawnButton.watchForMouseHover(mouseStateX, mouseStateY)) {
                    gameState = GameState::inGame;
                    cout << "spawned\n";
                    player.updateToInitialPosition(mouseStateX, mouseStateY);
                    scrollingbg.setIngameLoopStatus(true);
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
            spinAcceleration = 1.0f + ((float)elapsedFrameInGame / (float)INTENDED_PLAYTIME) * (5.0f - 1);

            for (int i = 0; i < 2; i++) {
                asteroid[i].setDifficulty(velocityValue, accelTimeValue, accelVariableValue, spinAcceleration);
                asteroid[i].processMovement();
                asteroid[i].updateMove();
                asteroid[i].render();
                if (player.isCollided(asteroid[i])) {
                    g_causeofdeath = CauseOfDeath::asteroidHit;
                    g_isAlive = false;
                }
            }
            if (asteroid[0].isVanishedFromScreen()) {
                asteroid[0].spawn(randRange(10, (SCREEN_W / 2) - (int)(asteroid[0].getWidth() * (float)(1.5f))));
            }
            if (asteroid[1].isVanishedFromScreen()) {
                asteroid[1].spawn(randRange((SCREEN_W / 2) + (asteroid[1].getWidth() / 2), SCREEN_W - asteroid[1].getWidth() - 10));
            }


            if (!g_isAlive) {
                cout << "you're dead\n";
                g_isAlive = true;

                player.updateToInitialPosition(mouseStateX, mouseStateY);

                srand((unsigned)time(0));

                asteroid[0].initSpawn(0, (float)randRange(0, 360),
                                      randRange(10, (SCREEN_W / 2) - (int)(asteroid[0].getWidth() * (float)(1.5f))));
                asteroid[1].initSpawn(randRange(10, 100), (float)randRange(0, 360),
                                      randRange((SCREEN_W / 2) + (asteroid[1].getWidth() / 2), SCREEN_W - asteroid[1].getWidth() - 10));

                elapsedFrameInGame = 0;

                gameState = GameState::killedMenu;
                scrollingbg.setIngameLoopStatus(false);
            }
            ingameOverlay.render();
        }
        else if (gameState == GameState::mainMenu) {
            scrollingbg.calculateMove();
            scrollingbg.render();
            mainMenu.render();
        }
        else if (gameState == GameState::inTutorial) {
            scrollingbg.calculateMove();
            spawnButton.watchForMouseHover(mouseStateX, mouseStateY);

            scrollingbg.render();
            tutorialMenu.render();
            spawnButton.render();
        }
        else if (gameState == GameState::pauseMenu) {
            scrollingbg.render();
            player.render();
            asteroid[0].stationaryRender();
            asteroid[1].stationaryRender();
            pauseMenu.render();
        }
        else if (gameState == GameState::killedMenu) {
            scrollingbg.render();
            respawnButton.watchForMouseHover(mouseStateX, mouseStateY);
            respawnButton.render();
            if (g_causeofdeath == CauseOfDeath::outOfBorder) {
                killedMenu[CauseOfDeath::outOfBorder].render();
            }
            else if (g_causeofdeath == CauseOfDeath::asteroidHit) {
                killedMenu[CauseOfDeath::asteroidHit].render();
            }
            else if (g_causeofdeath == CauseOfDeath::cursorHit) {
                killedMenu[CauseOfDeath::cursorHit].render();
            }
        }
        cursor.render();
        SDL_RenderPresent(g_renderer);
    }

    close();
    return 0;
}