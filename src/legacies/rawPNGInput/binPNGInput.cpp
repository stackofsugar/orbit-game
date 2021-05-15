#include <SDL.h>
#include <string>
#define SCREEN_H    680
#define SCREEN_W    680
using namespace std;

enum fileNames {
    Fdefault,
    Fdown,
    Fleft,
    Fright,
    Fup,
    Ftotal
};

enum surfaceKeyPresses {
    kpDEFAULT,
    kpDOWN,
    kpLEFT,
    kpRIGHT,
    kpUP,
    kpTOTAL
};

class Application {
private:
    SDL_Window *main_window;
    SDL_Surface *this_surface;
    SDL_Surface *screen_surface;
    SDL_Surface *keypress_surface[kpTOTAL];

    std::string m_window_title;

public:
    Application(std::string window_title) {
        main_window = NULL;
        this_surface = NULL;
        screen_surface = NULL;
        for (int i = 0; i < kpTOTAL; i++) {
            keypress_surface[i] = NULL;
        }
        m_window_title = window_title;
    }
};

int main() {

}