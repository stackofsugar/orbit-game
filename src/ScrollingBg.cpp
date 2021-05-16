#include "../include/pch.h"
using namespace std;

ScrollingBg::ScrollingBg() {
    m_isInIngameLoop = m_scrollingOffset = h_frameTimeCount = 0;
    m_scrollingOffsetFrameTime = 1;
}

void ScrollingBg::render() {
    Texture::render(0, m_scrollingOffset - (m_height / 2));
    Texture::render(0, m_scrollingOffset - (m_height / 2) - m_height);
}

void ScrollingBg::calculateMove() {
    if (m_isInIngameLoop) {
        h_frameTimeCount++;
        if (h_frameTimeCount > INTENDED_PLAYTIME / 10) {
            if (m_scrollingOffsetFrameTime < 20) {
                m_scrollingOffsetFrameTime++;
            }
            h_frameTimeCount = 0;
            cout << m_scrollingOffsetFrameTime << '\n';
        }

        m_scrollingOffset += m_scrollingOffsetFrameTime;
        if (m_scrollingOffset >= m_height) {
            m_scrollingOffset = 0;
        }
    }
    else {
        m_scrollingOffset++;
        if (m_scrollingOffset >= m_height) {
            m_scrollingOffset = 0;
        }
    }
}

void ScrollingBg::setIngameLoopStatus(bool status) {
    m_isInIngameLoop = status;
    m_scrollingOffsetFrameTime = 1;
    h_frameTimeCount = 0;
}