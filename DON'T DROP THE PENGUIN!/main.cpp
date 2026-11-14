
#include "DxLib.h"
#include "Game.h"
#include <tchar.h>

int main()
{
    // フルスクリーン表示
    ChangeWindowMode(FALSE);

    // ゲームの内部画面サイズ
    SetGraphMode(1000, 800, 32);

    // タイトル
    SetMainWindowText(_T("DON'T DROP THE PENGUIN!"));

    // DxLib初期化
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // マウス表示
    SetMouseDispFlag(TRUE);

    // 裏画面描画
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

    DxLib_End();

    return 0;
}
