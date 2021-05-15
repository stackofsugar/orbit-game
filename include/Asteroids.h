#pragma once
#include "pch.h"
using namespace std;

class Asteroids : public Entity {
protected:
    int m_accelX, m_accelY, h_frameTimeCount;
    int h_vellocCap, h_accelVariable, h_accelTimeCap;
public:
    Asteroids();

    ~Asteroids();

    void initSpawn();

    void spawn();

    bool isVanishedFromScreen();

    void processMovement();

    void updateMove();

    void render();

    void DEBUG_SETXY(int x, int y);

    void setDifficulty(int newVelocity, int newAccelTime, int newAccelVariable);
};