#include "../include/pch.h"
using namespace std;

Cursor::Cursor() {
    m_posX = m_posY = 0;
}

void Cursor::processAndUpdateMovement(int mouseX, int mouseY) {
    m_posX = mouseX - (m_width / 2);
    m_posY = mouseY - (m_height / 2);
}

void Cursor::render() {
    Texture::render(m_posX, m_posY);
}