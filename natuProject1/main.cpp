#include <DxLib.h>

#include "Common.h"
#include "Game.h"


int WINAPI WinMain(
    HINSTANCE,
    HINSTANCE,
    LPSTR,
    int)
{

    // ウィンドウモード
    ChangeWindowMode(TRUE);


    // 画面サイズ
    SetGraphMode(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        32
    );


    // タイトル
    SetMainWindowText(
        TEXT("Side Scroll Action Game")
    );


    // DxLib初期化
    if (DxLib_Init() == -1)
    {
        return -1;
    }


    // 裏画面設定
    SetDrawScreen(
        DX_SCREEN_BACK
    );


    // ゲーム生成
    Game game;



    // メインループ
    while (
        ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0
        )
    {

        // 更新
        game.Update();



        // 描画準備
        ClearDrawScreen();



        // 描画
        game.Draw();



        // 表示
        ScreenFlip();

    }



    // DxLib終了
    DxLib_End();


    return 0;
}