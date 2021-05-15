#pragma once
#include "pch.h"
using namespace std;

class Entity : public Texture {
protected:
    int m_posX, m_posY, m_velX, m_velY, h_totalRadiusSquared;
    Circle collider;
public:
    Entity();

    ~Entity();

    bool isCollidingWithBorder();

    bool isCollided(Entity &other_entity);
};