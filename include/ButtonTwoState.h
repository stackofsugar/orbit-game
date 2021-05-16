#pragma once
#include "pch.h"
using namespace std;

class ButtonTwoState : public Texture {
protected:
    int m_posX, m_posY;
    bool activeness;
    SDL_Rect m_texRectHoverInactive, m_texRectHoverActive;
public:
    ButtonTwoState();

    bool initSheet(bool isSideBySide, int posCenteredX, int posCenteredY);

    void setPosition(int posX, int posY);

    bool watchForMouseHover(int mouseX, int mouseY);

    void render();
};