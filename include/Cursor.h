#pragma once
#include "pch.h"
using namespace std;

class Cursor : public Texture {
protected:
    int m_posX, m_posY;
public:
    Cursor();

    void processAndUpdateMovement(int mouseX, int mouseY);

    void render();
};