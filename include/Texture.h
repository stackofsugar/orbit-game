#pragma once
#include "pch.h"
using namespace std;

class Texture {
protected:
    SDL_Texture *m_texture;
    int m_width, m_height;
public:
    Texture();

    ~Texture();

    bool loadFromFile(string path);

    void free();

    void render(int x, int y);

    int getHeight();

    int getWidth();

    void overrideWidth(int newWidth);

    void overrideHeight(int newHeight);
};