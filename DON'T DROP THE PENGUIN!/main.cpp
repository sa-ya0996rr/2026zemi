//shft+ALT+↑or↓
#include "DxLib.h"
#include "Game.h"
#include <tchar.h>


// ゲーム画面のサイズ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // 準備
    ChangeWindowMode(FALSE);
    SetDrawScreen(DX_SCREEN_BACK);
    //SetGraphMode(1920, 1080, 32);
    SetGraphMode(1000, 800, 32);
    DxLib_Init();
    // DxLibの初期
    if (DxLib_Init() == -1)return-1;
    // 本編
    Game();

    DxLib_End();
    return 0;
}

int main()
{
    // ウィンドウタイトル
    SetMainWindowText(_T("DON'T DROP THE PENGUIN!"));

    // DxLib初期化
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // マウスカーソルを表示
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
        game.Update();

        ClearDrawScreen();

        game.Draw();

        ScreenFlip();
    }

    // DxLib終了
    DxLib_End();

    return 0;
}
