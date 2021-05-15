#include <SDL.h>
#include <SDL_image.h>
#include <string>
#define SCREEN_H    680
#define SCREEN_W    980
using namespace std;

class App {
private:
    SDL_Window *main_window;
    SDL_Renderer *main_renderer;
    SDL_Texture *main_texture;
    int keydownCounter;

public:
    App() {
        main_window = NULL;
        main_renderer = NULL;
        main_texture = NULL;
        keydownCounter = 0;
    }

    ~App() {
        SDL_DestroyTexture(main_texture);
        SDL_DestroyRenderer(main_renderer);
        SDL_DestroyWindow(main_window);
        main_renderer = NULL;
        main_window = NULL;
        main_texture = NULL;

        IMG_Quit();
        SDL_Quit();
    }

    bool init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            return false;
        }
        main_window = SDL_CreateWindow("penis", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H,
                                       SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
        main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
        if (!main_window || !main_renderer) {
            return false;
        }
        SDL_SetRenderDrawColor(main_renderer, 0xc6, 0xff, 0xc1, 0xff);

        int imgflgs = IMG_INIT_PNG;
        if (!(IMG_Init(imgflgs) & imgflgs)) {
            return false;
        }
        return true;
    }

    SDL_Texture *load_texture(string path) {
        SDL_Texture *ret_texture = NULL;

        SDL_Surface *loaded_surface = IMG_Load(path.c_str());
        if (loaded_surface == NULL) {
            return NULL;
        }

        ret_texture = SDL_CreateTextureFromSurface(main_renderer, loaded_surface);
        if (ret_texture == NULL) {
            return NULL;
        }

        SDL_FreeSurface(loaded_surface);
        return ret_texture;
    }

    bool loadmedia() {
        main_texture = load_texture("resources//stretche.png");
        if (!main_texture) {
            return false;
        }
        return true;
    }

    SDL_Window *getWindow() {
        return main_window;
    }

    void gameLoop() {
        SDL_Event ev;
        bool game_isrunning = true;
        bool hasMousewheelAction = false;
        SDL_RenderClear(main_renderer);
        SDL_RenderCopy(main_renderer, main_texture, NULL, NULL);
        SDL_RenderPresent(main_renderer);

        SDL_Rect rect_mousemotion = {SCREEN_W / 4, SCREEN_H / 4, SCREEN_W / 2, SCREEN_H / 2};
        SDL_Rect rect_mousedown = { SCREEN_W / 6, SCREEN_H / 6, SCREEN_W / 3, SCREEN_H / 3 };
        while (game_isrunning) { 
            while (SDL_PollEvent(&ev) != 0) {
                if (ev.type == SDL_QUIT) {
                    game_isrunning = false;
                }
                else if (ev.type == SDL_MOUSEMOTION) {
                    SDL_SetRenderDrawColor(main_renderer, 0xC6, 0xFF, 0xC1, 0xFF);
                    SDL_RenderFillRect(main_renderer, &rect_mousemotion);
                    SDL_RenderPresent(main_renderer);
                }
                else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_SetRenderDrawColor(main_renderer, 0xFF, 0xFB, 0xDF, 0xFF);
                    SDL_RenderDrawRect(main_renderer, &rect_mousedown);
                    SDL_RenderPresent(main_renderer);
                }
                else if (ev.type == SDL_KEYDOWN) {
                    if (keydownCounter <= SCREEN_H) {
                        SDL_SetRenderDrawColor(main_renderer, 0x33, 0x44, 0x43, 0xFF);
                        SDL_RenderDrawPoint(main_renderer, SCREEN_W / 2, keydownCounter);
                        SDL_RenderPresent(main_renderer);
                        keydownCounter += 5;
                    }
                }
                else if (ev.type == SDL_MOUSEWHEEL) {
                    keydownCounter = 0;
                    SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 0xFF);
                    SDL_RenderClear(main_renderer);
                    SDL_RenderPresent(main_renderer);
                    if (!hasMousewheelAction) {
                        hasMousewheelAction = true;
                        SDL_Rect viewportRect = { SCREEN_W / 4, SCREEN_H / 4, SCREEN_W / 2, SCREEN_H / 2 };
                        SDL_RenderSetViewport(main_renderer, &viewportRect);
                    }
                }
            }
        }
    }
};

int main(int argc, char **argv) {
    App game;
    if (!game.init()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Program exited", "SDL cannot be initialized", game.getWindow());
        return -1;
    }
    if (!game.loadmedia()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Program exited", "Media cannot be loaded", game.getWindow());
        return -1;
    }

    game.gameLoop();
    return 0;
}