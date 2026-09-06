#include "Ending.h"
#include "DxLib.h"

void Ending::Init()
{
    endingNumber = 0;
}

void Ending::SetEnding(int number)
{
    endingNumber = number;
}

void Ending::Update()
{
}

void Ending::Draw()
{
    DrawBox(
        0,
        0,
        1280,
        720,
        GetColor(10, 10, 30),
        TRUE
    );

    if (endingNumber == 1)
    {
        DrawString(
            400,
            250,
            "ENDING 1",
            GetColor(255, 255, 255)
        );

        DrawString(
            300,
            330,
            "操縦室を調べ、宇宙船の秘密を知った。",
            GetColor(255, 255, 255)
        );
    }

    else if (endingNumber == 2)
    {
        DrawString(
            400,
            250,
            "ENDING 2",
            GetColor(255, 255, 255)
        );

        DrawString(
            300,
            330,
            "仲間を見つけ、一緒に旅を続けることになった。",
            GetColor(255, 255, 255)
        );
    }

    else if (endingNumber == 3)
    {
        DrawString(
            400,
            250,
            "ENDING 3",
            GetColor(255, 255, 255)
        );

        DrawString(
            300,
            330,
            "宇宙船を脱出し、新しい星へ向かった。",
            GetColor(255, 255, 255)
        );
    }

    DrawString(
        400,
        500,
        "Rキー：もう一度遊ぶ",
        GetColor(200, 200, 200)
    );
}