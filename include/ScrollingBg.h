#pragma once
#include "pch.h"
using namespace std;

class ScrollingBg : public Texture {
protected:
    int m_scrollingOffset, m_scrollingOffsetFrameTime, m_isInIngameLoop, h_frameTimeCount;
public:
    ScrollingBg();

    void calculateMove();

    void setIngameLoopStatus(bool status);

    void render();
};