#include "DxLib.h"
#include "Game.h"


int WINAPI WinMain(
    HINSTANCE,
    HINSTANCE,
    LPSTR,
    int)
{
    ChangeWindowMode(TRUE);

    SetGraphMode(
        1200,
        800,
        32);


    if (DxLib_Init() == -1)
    {
        return -1;
    }


    SetDrawScreen(
        DX_SCREEN_BACK);


    InitGame();


    // ==========================================
    // ÉQÅ[ÉÄÉãÅ[Év
    // ==========================================

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();


        UpdateGame();


        DrawGame();


        ScreenFlip();
    }


    DxLib_End();


    return 0;
}