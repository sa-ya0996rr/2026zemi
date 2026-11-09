
#include "DxLib.h"
#include "Game.h"
#include <tchar.h>

int main()
{
    // ゲーム画面のサイズ
    SetGraphMode(800, 600, 32);

    // ウィンドウタイトル
    SetMainWindowText(_T("DON'T DROP THE PENGUIN!"));

    // フルスクリーンモードに設定(FALSE = フルスクリーンモード)
    //if (ChangeWindowMode(FALSE) != 0)
    //{
    //    printf("ChangeWindowMode failad.\n");
    //    return -1;
    //}
    ChangeWindowMode(FALSE);

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
