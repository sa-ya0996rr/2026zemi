
#pragma once
#include <tchar.h>
#include "Player.h"
#include "ChargeImpact.h"

class Window
{
public:
    void DrawTitle(int select);
    void DrawMode(int select);
    void DrawPlay(int current, Player players[2], int turn, ChargeImpact& charge);
    void DrawChanceChoice();
    void DrawGameOver(int winner);

private:
    void Center(int y, const TCHAR* text, int color);
};
