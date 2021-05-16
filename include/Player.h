#pragma once
#include "pch.h"
using namespace std;

class Player : public Entity {
protected:
    int m_mouseX, m_mouseY, m_mouseDistanceXY, m_accelX, m_accelY;
public:
    Player();

    void updateToInitialPosition();

    void render();

    void calculateMouseDistance();

    void processMovement(int mouseX, int mouseY);

    void updateMove();
};