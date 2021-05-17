#include "../include/pch.h"
using namespace std;

Text::Text() {
    m_color = {};
    m_font = NULL;
    m_posX = m_posY = 0;
}

bool Text::loadText(string text) {
    free();

    SDL_Surface *textSurface = TTF_RenderText_Blended(m_font, text.c_str(), m_color);
    if (!textSurface) {
        return false;
    }

    m_texture = SDL_CreateTextureFromSurface(g_renderer, textSurface);
    if (!m_texture) {
        return false;
    }
    m_width = textSurface->w;
    m_height = textSurface->h;

    SDL_FreeSurface(textSurface);
    textSurface = NULL;
    return true;
}

void Text::render() {
    Texture::render(m_posX, m_posY);
}

void Text::setProperties(TTF_Font *font, int r, int g, int b, int a, int posX, int posY) {
    SDL_Color textColor = { r, g, b, a };
    m_color = textColor;
    m_font = font;

    m_posX = posX;
    m_posY = posY;
}

void Text::setPosition(int posX, int posY) {
    m_posX = posX;
    m_posY = posY;
}