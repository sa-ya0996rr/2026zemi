#include "DxLib.h"
#include "Game.h"

int WINAPI WinMain(
    HINSTANCE,
    HINSTANCE,
    LPSTR,
    int
)
{
    ChangeWindowMode(TRUE);

    SetGraphMode(
        1280,
        720,
        32
    );

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    GameInit();

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        GameUpdate();

        GameDraw();

        ScreenFlip();
    }

    GameEnd();

    DxLib_End();

    return 0;
}