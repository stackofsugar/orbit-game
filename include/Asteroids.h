#pragma once
#include "pch.h"
using namespace std;

class Asteroids : public Entity {
protected:
    int m_accelX, m_accelY, h_frameTimeCount, h_Yoffset;
    int h_vellocCap, h_accelVariable, h_accelTimeCap;
    float h_spinValue, h_spinAcceleration;
    uniform_int_distribution<int> m_dist;
    mt19937 m_gen;
public:
    Asteroids();

    ~Asteroids();

    void initSpawn(int Yoffset, float spinOffset, int randPosX);

    void spawn(int randPosX);

    bool isVanishedFromScreen();

    void processMovement();

    void updateMove();

    void render();

    void stationaryRender();

    void DEBUG_SETXY(int x, int y);

    void setDifficulty(int newVelocity, int newAccelTime, int newAccelVariable, float newSpinAcceleration);
};