#include "../include/pch.h"
using namespace std;

double distanceSquared(int x1, int y1, int x2, int y2) {
    return (pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

Entity::Entity() {
    m_posX = m_posY = m_velX = m_velY = h_totalRadiusSquared = 0;
    collider.x = collider.y = 0;

    collider.r = m_width / 2;
}

Entity::~Entity() {
    free();
}

bool Entity::isCollidingWithBorder() {
    if (m_posX <= 0 || m_posX >= (SCREEN_W - m_width)) {
        return true;
    }
    else if (m_posY <= 0 || m_posY >= (SCREEN_H - m_height)) {
        return true;
    }
    return false;
}

bool Entity::isCollided(Entity &other) {
    h_totalRadiusSquared = this->collider.r + other.collider.r;
    h_totalRadiusSquared *= h_totalRadiusSquared;

    if (distanceSquared(this->collider.x, this->collider.y, other.collider.x, other.collider.y) < h_totalRadiusSquared) {
        return true;
    }

    return false;
}