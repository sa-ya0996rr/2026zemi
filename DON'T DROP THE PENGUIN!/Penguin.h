#pragma once
#include "DxLib.h"
class Ice;
class Penguin
{
public:
    Penguin();
    void SetPosition(int x, int y);
    bool IsOnTile(int index, const Ice& ice) const;
    bool HasFallen(const Ice& ice) const;
    void Draw(const Ice& ice, int player) const;
private:
    int x, y;
};
