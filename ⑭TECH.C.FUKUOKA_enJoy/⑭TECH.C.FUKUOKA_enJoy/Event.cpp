#include "Event.h"
#include "DxLib.h"

void Event::Init()
{
    choiceStart = false;
}

void Event::Update(int playerX, int playerY)
{
    // 部屋の中央付近に来たらイベント可能
    if (playerX > 500 &&
        playerX < 750 &&
        playerY > 300 &&
        playerY < 500)
    {
        if (CheckHitKey(KEY_INPUT_E))
        {
            choiceStart = true;
        }
    }
}

void Event::Draw()
{
    if (!choiceStart)
    {
        DrawString(
            20,
            680,
            "WASD：移動　中央でE：イベント",
            GetColor(255, 255, 255)
        );
    }
}