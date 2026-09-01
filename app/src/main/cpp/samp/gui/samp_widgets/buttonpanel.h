#pragma once

// ใช้ include หลักของโปรเจกต์ที่น่าจะมีประกาศคลาส Layout และ Button ไว้แล้ว
#include "../gui.h"

class ButtonPanel : public Layout
{
public:
    ButtonPanel();


    CButton* m_bH;
private:
    CButton* m_bAlt;
    CButton* m_bY;
    CButton* m_bN;
};