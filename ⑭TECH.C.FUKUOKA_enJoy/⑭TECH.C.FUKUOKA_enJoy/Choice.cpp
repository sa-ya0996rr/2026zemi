#include "Choice.h"
#include "DxLib.h"

void Choice::Init()
{
    selected = 0;
}

void Choice::Update()
{
    if (CheckHitKey(KEY_INPUT_UP))
    {
        selected--;

        if (selected < 0)
        {
            selected = 2;
        }
    }

    if (CheckHitKey(KEY_INPUT_DOWN))
    {
        selected++;

        if (selected > 2)
        {
            selected = 0;
        }
    }
}

void Choice::Draw()
{
    // 選択肢の背景
    DrawBox(
        200,
        430,
        1080,
        680,
        GetColor(20, 20, 40),
        TRUE
    );

    DrawFormatString(
        250,
        460,
        GetColor(255, 255, 255),
        "どうする？"
    );

    // 選択肢1
    DrawFormatString(
        300,
        510,
        selected == 0 ? GetColor(255, 220, 50) : GetColor(255, 255, 255),
        "%s  船の操縦室を調べる",
        selected == 0 ? ">" : " "
    );

    // 選択肢2
    DrawFormatString(
        300,
        560,
        selected == 1 ? GetColor(255, 220, 50) : GetColor(255, 255, 255),
        "%s  仲間を探す",
        selected == 1 ? ">" : " "
    );

    // 選択肢3
    DrawFormatString(
        300,
        610,
        selected == 2 ? GetColor(255, 220, 50) : GetColor(255, 255, 255),
        "%s  宇宙船から脱出する",
        selected == 2 ? ">" : " "
    );

    DrawFormatString(
        300,
        650,
        GetColor(180, 180, 180),
        "↑↓：選択　SPACE：決定"
    );
}