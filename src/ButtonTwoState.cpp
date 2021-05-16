#include "../include/pch.h"
using namespace std;

ButtonTwoState::ButtonTwoState() {
    m_posX = m_posY = m_width = m_height = 0;
    activeness = false;
    m_texRectHoverInactive = m_texRectHoverActive = {};
}

// should be side by side, left is the inactive one
bool ButtonTwoState::initSheet(bool isSideBySide, int posCenteredX, int posCenteredY) {
    cout << "old: " << m_width << ' ' << m_height << '\n';

    m_width /= 2;

    cout << "new: " << m_width << ' ' << m_height << '\n';

    if (isSideBySide) {
        m_texRectHoverInactive = { 0, 0, m_width, m_height };
        m_texRectHoverActive = { m_width, 0, m_width, m_height };
    }
    else {
        m_texRectHoverInactive = { 0, 0, m_width, m_height };
        m_texRectHoverActive = { 0, m_height, m_width, m_height };
    }

    m_posX = posCenteredX - (m_width / 2);
    m_posY = posCenteredY - (m_height / 2);
    return true;
}

void ButtonTwoState::setPosition(int posX, int posY) {
    m_posX = posX;
    m_posY = posY;
}

bool ButtonTwoState::watchForMouseHover(int mouseX, int mouseY) {
    if ((mouseX >= m_posX && mouseX <= (m_posX + m_width)) &&
        (mouseY >= m_posY && mouseY <= (m_posY + m_height))) {
        activeness = true;
        return true;
    }
    else {
        activeness = false;
        return false;
    }
}

void ButtonTwoState::render() {
    SDL_Rect renderTarget = { m_posX, m_posY, m_width, m_height };
    SDL_Rect renderSource = {};

    if (activeness) {
        renderSource = m_texRectHoverActive;
    }
    else {
        renderSource = m_texRectHoverInactive;
    }

    SDL_RenderCopy(g_renderer, m_texture, &renderSource, &renderTarget);
}