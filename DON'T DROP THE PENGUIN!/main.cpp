
#include "DxLib.h"
#include "Game.h"
#include <tchar.h>

int main()
{
    // ウィンドウ設定
    ChangeWindowMode(FALSE);
    SetGraphMode(1000, 800, 32);
    SetMainWindowText(_T("DON'T DROP THE PENGUIN!"));

    // DxLib初期化
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // マウスカーソルを表示
    SetMouseDispFlag(TRUE);

    // 裏画面に描画
    SetDrawScreen(DX_SCREEN_BACK);

    // ゲーム初期化
    Game game;
    game.Init();

    // ゲームループ
    while (ProcessMessage() == 0 &&
        !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        ClearDrawScreen();

        game.Update();
        game.Draw();

        ScreenFlip();
    }

    // DxLib終了
    DxLib_End();

    return 0;
}
