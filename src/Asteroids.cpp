#include "../include/pch.h"
using namespace std;

Asteroids::Asteroids() {
    m_accelX = m_accelY = h_frameTimeCount = h_vellocCap = h_accelTimeCap = h_accelVariable = h_Yoffset = 0;
    h_spinAcceleration = h_spinValue = 0.0f;

    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_NONE);

    random_device randev;
    mt19937 gen(randev());
    uniform_int_distribution<int> dist(5, SCREEN_W - m_width - 5);
    m_dist = dist;
    m_gen = gen;
}

Asteroids::~Asteroids() {
    free();
}

void Asteroids::spawn(int randPosX) {
    m_accelX = m_accelY = m_velX = 0;
    collider.r = m_width / 2;

    m_posY = -m_height - h_Yoffset;
    m_posX = randPosX;

    m_velY = h_vellocCap / 6;
}

void Asteroids::initSpawn(int Yoffset, float spinOffset, int randPosX) {
    h_Yoffset = Yoffset;
    h_spinValue = spinOffset;
    h_vellocCap = 50;
    h_accelTimeCap = 20;
    h_accelVariable = 1;
    spawn(randPosX);
}

bool Asteroids::isVanishedFromScreen() {
    if (m_posY >= SCREEN_W + 5) {
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
}

void Asteroids::updateMove() {
    m_posX += m_velX;
    m_posY += m_velY;
}

void Asteroids::render() {
    SDL_Rect renderTarget = { m_posX, m_posY, m_width, m_height };
    h_spinValue += h_spinAcceleration;
    if (h_spinValue >= 360.0f) {
        h_spinValue = 0.0f;
    }

    SDL_RenderCopyEx(g_renderer, m_texture, NULL, &renderTarget, h_spinValue, NULL, SDL_FLIP_NONE);
}

void Asteroids::stationaryRender() {
    SDL_Rect renderTarget = { m_posX, m_posY, m_width, m_height };
    SDL_RenderCopyEx(g_renderer, m_texture, NULL, &renderTarget, h_spinValue, NULL, SDL_FLIP_NONE);
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
void Asteroids::setDifficulty(int newVelocity, int newAccelTime, int newAccelVariable, float newSpinAcceleration) {
    h_vellocCap = newVelocity;
    h_accelTimeCap = newAccelTime;
    h_accelVariable = newAccelVariable;
    h_spinAcceleration = newSpinAcceleration;
}