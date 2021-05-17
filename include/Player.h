#pragma once
#include "pch.h"
using namespace std;

class Player : public Entity {
protected:
    int m_mouseDistanceXY, m_accelX, m_accelY;
public:
    Player();

    void updateToInitialPosition(int mouseX, int mouseY);

    void render();

    void calculateMouseDistance(int mouseX, int mouseY);

    void processMovement(int mouseX, int mouseY);

    void updateMove();
};