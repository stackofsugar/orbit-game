#include "../include/pch.h"
using namespace std;

Asteroids::Asteroids() {
    m_accelX = m_accelY = h_frameTimeCount = h_vellocCap = h_accelTimeCap = h_accelVariable = 0;
}

Asteroids::~Asteroids() {
    free();
}

void Asteroids::spawn() {
    m_accelX = m_accelY = m_velX = m_velY = 0;
    collider.r = m_width / 2;

    m_posY = -m_height;
    m_posX = randRange(5, SCREEN_W - m_width - 5);
}

void Asteroids::initSpawn() {
    spawn();
}

bool Asteroids::isVanishedFromScreen() {
    if (m_posY >= SCREEN_W) {
        return true;
    }
    else if (m_posX + m_width <= 0) {
        return true;
    }
    else if (m_posX >= SCREEN_W) {
        return true;
    }

    return false;
}

void Asteroids::processMovement() {
    h_frameTimeCount++;
    
    if (h_frameTimeCount >= h_accelTimeCap) {
        m_velY += h_accelVariable;

        h_frameTimeCount = 0;
    }

    if (m_velX > h_vellocCap) {
        m_velX = h_vellocCap;
    }
    if (m_velY > h_vellocCap) {
        m_velY = h_vellocCap;
    }

    collider.x = m_posX + (m_width / 2);
    collider.y = m_posY + (m_height / 2);

    if (isVanishedFromScreen()) {
        spawn();
    }
}

void Asteroids::updateMove() {
    m_posX += m_velX;
    m_posY += m_velY;
}

void Asteroids::render() {
    Texture::render(m_posX, m_posY);
}

void Asteroids::DEBUG_SETXY(int x, int y) {
    m_posX = x;
    m_posY = y;

    collider.x = m_posX + (m_width / 2);
    collider.y = m_posY + (m_height / 2);
}

/*
 \param newVelocity 50 - 96
 \param newAccelTime 20 - 0
 \param newAccelVariable 1 - 4
*/
void Asteroids::setDifficulty(int newVelocity, int newAccelTime, int newAccelVariable) {
    h_vellocCap = newVelocity;
    h_accelTimeCap = newAccelTime;
    h_accelVariable = newAccelVariable;
}