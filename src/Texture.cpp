#include "../include/pch.h"
using namespace std;

Texture::Texture() {
    m_texture = NULL;
    m_height = m_width = 0;
}

Texture::~Texture() {
    free();
}

bool Texture::loadFromFile(string path) {
    free();
    SDL_Texture *newTexture = NULL;
    SDL_Surface *newSurface = IMG_Load(path.c_str());

    if (newSurface == NULL) {
        return false;
    }

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

void Texture::free() {
    if (m_texture != NULL) {
        SDL_DestroyTexture(m_texture);
        m_texture = NULL;
    }
}

void Texture::render(int x, int y) {
    SDL_Rect renderTarget = { x, y, m_width, m_height };
    SDL_RenderCopy(g_renderer, m_texture, NULL, &renderTarget);
}

int Texture::getHeight() {
    return m_height;
}

int Texture::getWidth() {
    return m_width;
}

void Texture::overrideWidth(int newWidth) {
    m_width = newWidth;
}

void Texture::overrideHeight(int newHeight) {
    m_height = newHeight;
}
