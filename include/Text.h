#pragma once
#include "pch.h"
using namespace std;

class Text : public Texture {
protected:
    SDL_Color m_color;
    TTF_Font *m_font;
    int m_posX, m_posY;
public:
    Text();

    bool loadText(string text);

    void render();

    void setProperties(TTF_Font *font, int r, int g, int b, int a, int posX, int posY);

    void setPosition(int posX, int posY);
};