#include "../include/pch.h"
using namespace std;

const int GRAVITY_CONSTANT = 500;
int ACCEL_CAP = 1;
int VELOC_CAP = 10;

Player::Player() {
    m_mouseDistanceXY = m_accelX = m_accelY = 0;
}

void Player::updateToInitialPosition(int mouseX, int mouseY) {
    m_posX = (mouseX - (m_width / 2) - 50);
    m_posY = (mouseY - (m_height / 2) - 150);

    if (m_posY <= 0) {
        m_posY = 1;
    }
    if (m_posX <= 0) {
        m_posX = 1;
    }

    m_accelX = m_accelY = m_velY = 0;
    m_velX = 5;

    collider.r = m_width / 2;
}

void Player::render() {
    Texture::render(m_posX, m_posY);
}

void Player::calculateMouseDistance(int mouseX, int mouseY) {
    //int centerPosX = m_posX + (m_width / 2),
    //    centerPosY = m_posY + (m_height / 2),
    //    Xdistance = abs(centerPosX - m_mouseX),
    //    Ydistance = abs(centerPosY - m_mouseY);
    m_mouseDistanceXY = sqrt(pow((m_posX + (m_width / 2) - mouseX), 2) +
                             pow((m_posY + (m_height / 2) - mouseY), 2));
}

void Player::processMovement(int mouseX, int mouseY) {

    calculateMouseDistance(mouseX, mouseY);
    if (isCollidingWithBorder()) {
        g_isAlive = false;
        g_causeofdeath = CauseOfDeath::outOfBorder;
        return;
    }
    else if (m_mouseDistanceXY <= 25) {
        g_isAlive = false;
        g_causeofdeath = CauseOfDeath::cursorHit;
        return;
    }

    // calculate acceleration
    // - calculate X acceleration
    if (mouseX < m_posX + (m_width / 2) - 10) {
        m_accelX = -GRAVITY_CONSTANT / m_mouseDistanceXY;
    }
    else if (mouseX > m_posX + (m_width / 2) + 10) {
        m_accelX = GRAVITY_CONSTANT / m_mouseDistanceXY;
    }
    else {
        m_accelX = 0;
    }

    // - calculate Y acceleration
    if (mouseY < m_posY + (m_height / 2) - 10) {
        m_accelY = -GRAVITY_CONSTANT / m_mouseDistanceXY;
    }
    else if (mouseY > m_posY + (m_height / 2) + 10) {
        m_accelY = GRAVITY_CONSTANT / m_mouseDistanceXY;
    }
    else {
        m_accelY = 0;
    }

    // capping acceleration and velocity
    // - capping acceleration
    if (m_accelX > ACCEL_CAP) {
        m_accelX = ACCEL_CAP;
    }
    else if (m_accelX < -ACCEL_CAP) {
        m_accelX = -ACCEL_CAP;
    }

    if (m_accelY > ACCEL_CAP) {
        m_accelY = ACCEL_CAP;
    }
    else if (m_accelY < -ACCEL_CAP) {
        m_accelY = -ACCEL_CAP;
    }

    // - applying acceleration
    m_velX += m_accelX;
    m_velY += m_accelY;

    // - capping velocity
    if (m_velX > VELOC_CAP) {
        m_velX = VELOC_CAP;
    }
    else if (m_velX < -VELOC_CAP) {
        m_velX = -VELOC_CAP;
    }

    if (m_velY > VELOC_CAP) {
        m_velY = VELOC_CAP;
    }
    else if (m_velY < -VELOC_CAP) {
        m_velY = -VELOC_CAP;
    }

    // adjust colliders
    collider.x = m_posX + (m_width / 2);
    collider.y = m_posY + (m_height / 2);
}

void Player::updateMove() {
    m_posX += m_velX;
    m_posY += m_velY;
}