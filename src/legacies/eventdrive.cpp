#include <SDL.h>
#include <SDL_image.h>
#include <string>
#define SCREEN_H    680
#define SCREEN_W    680

enum surfaceKeyPresses {
    kpDEFAULT,
    kpUP,
    kpDOWN,
    kpLEFT,
    kpRIGHT,
    kpSTRETCH,
    kpTOTAL     // Must be on the bottom
};

class Application {
private:
    SDL_Window *main_window;
    SDL_Surface *this_surface;
    SDL_Surface *screen_surface;
    SDL_Surface *keypress_surface[kpTOTAL];

    std::string m_window_title;
    SDL_Rect m_stretch_rect;
    bool m_have_last_scaled;
public:
    Application(std::string window_title) {
        main_window = NULL;
        this_surface = NULL;
        screen_surface = NULL;
        for (int i = 0; i < kpTOTAL; i++) {
            keypress_surface[i] = NULL;
        }
        m_window_title = window_title;
        
        // Stretch group
        m_stretch_rect.x = 0;
        m_stretch_rect.y = 0;
        m_stretch_rect.w = SCREEN_W;
        m_stretch_rect.h = SCREEN_H;
    }
    ~Application() {
        for (int i = kpDEFAULT; i < kpTOTAL; i++) {
            SDL_FreeSurface(keypress_surface[i]);
            keypress_surface[i] = NULL;
        }
        SDL_DestroyWindow(main_window);
        main_window = NULL;
        SDL_Quit();
    }

    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            return false;
        }
        main_window = SDL_CreateWindow(m_window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        if (main_window == NULL) {
            return false;
        }
        screen_surface = SDL_GetWindowSurface(main_window);

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            return false;
        }

        return true;
    }

    SDL_Surface* loadSurface(std::string path) {
        SDL_Surface *optimized_surface = NULL;
        SDL_Surface *loaded_surface = NULL;

        if (path.compare(path.length() - 3, 3, "bmp") == 0) {
            loaded_surface = SDL_LoadBMP(path.c_str());
        }
        else if (path.compare(path.length() - 3, 3, "png") == 0) {
            loaded_surface = IMG_Load(path.c_str());
        }
        else {
            printf("%s has unknown image format! Error: %s\n", path.c_str(), SDL_GetError());
            exit(EXIT_FAILURE);
        }

        if (loaded_surface == NULL) {
            printf("Failure to create a surface context! Error: %s\n", SDL_GetError());
            return NULL;
        }
        optimized_surface = SDL_ConvertSurface(loaded_surface, screen_surface->format, 0);

        if (optimized_surface == NULL) {
            printf("Unable to optimize blit! Error: %s\n", SDL_GetError());
            SDL_FreeSurface(optimized_surface);
            return loaded_surface;
        }
        else {
            SDL_FreeSurface(loaded_surface);
            return optimized_surface;
        }
    }

    bool loadMedia() {
        std::string resource_name[kpTOTAL] = { "press.bmp", "up.bmp", "down.png", "left.bmp", "right.bmp", "stretche.png" };
        std::string resource_directory = "resources\\";
        for (int i = kpDEFAULT; i < kpTOTAL; i++) {
            keypress_surface[i] = loadSurface(resource_directory + resource_name[i]);
            if (keypress_surface[i] == NULL) {
                printf("Failed to load %s asset! Error: %s\n", resource_name[i].c_str(), SDL_GetError());
                return false;
            }
        }
        this_surface = keypress_surface[kpDEFAULT];
        return true;
    }

    void updateSurfaceOnKeydown(int key) {
        this_surface = keypress_surface[key];

        if (key == kpSTRETCH || key == kpDOWN) {
            m_stretch_rect.x = 0;
            m_stretch_rect.y = 0;
            m_stretch_rect.w = SCREEN_W;
            m_stretch_rect.h = SCREEN_H;
            m_have_last_scaled = true;
        }
        else {
            m_have_last_scaled = false;
        }
    }

    void blitSurface() {
        if (m_have_last_scaled) {
            SDL_BlitScaled(this_surface, NULL, screen_surface, &m_stretch_rect);
        }
        else {
            SDL_BlitSurface(this_surface, NULL, screen_surface, NULL);
        }
    }

    void updateWindowSurface() {
        SDL_UpdateWindowSurface(main_window);
    }
};

int main(int argc, char **argv) {
    SDL_Event sdlev;
    bool is_quit = false;
    Application myApp("This is My App");
    if (!myApp.init()) {
        printf("SDL initialization failed! Error: %s\n", SDL_GetError());
        return 69;
    }
    if (!myApp.loadMedia()) {
        printf("Media loading failed! Error: %s\n", SDL_GetError());
        return 69;
    }
    
    while (!is_quit) {
        while (SDL_PollEvent(&sdlev) != 0) {
            if (sdlev.type == SDL_QUIT) {
                is_quit = true;
            }
            else if (sdlev.type == SDL_KEYDOWN) {
                switch (sdlev.key.keysym.sym) {
                    case SDLK_UP:
                        myApp.updateSurfaceOnKeydown(kpUP);
                        break;
                    case SDLK_DOWN:
                        myApp.updateSurfaceOnKeydown(kpDOWN);
                        break;
                    case SDLK_RIGHT:
                        myApp.updateSurfaceOnKeydown(kpRIGHT);
                        break;
                    case SDLK_LEFT:
                        myApp.updateSurfaceOnKeydown(kpLEFT);
                        break;
                    case SDLK_s:
                        myApp.updateSurfaceOnKeydown(kpSTRETCH);
                        break;
                    default:
                        myApp.updateSurfaceOnKeydown(kpDEFAULT);
                        break;
                }
            }
        }
        myApp.blitSurface();
        myApp.updateWindowSurface();
    }

    return 0;
}