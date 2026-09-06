#include "Function.h"
#include "DxLib.h"

Player player;
Map map;
Event event;
Choice choice;
Ending ending;

int gameState;

// 0 = ゲーム中
// 1 = 選択肢
// 2 = エンディング

void GameInit()
{
    player.Init();
    map.Init();
    event.Init();
    choice.Init();
    ending.Init();

    gameState = 0;
}

void GameUpdate()
{
    // --------------------
    // ゲーム中
    // --------------------

    if (gameState == 0)
    {
        player.Update();

        event.Update(
            player.x,
            player.y
        );

        if (event.choiceStart)
        {
            gameState = 1;
        }
    }

    // --------------------
    // 選択肢
    // --------------------

    else if (gameState == 1)
    {
        choice.Update();

        if (CheckHitKey(KEY_INPUT_SPACE))
        {
            ending.SetEnding(choice.selected + 1);

            gameState = 2;
        }
    }

    // --------------------
    // エンディング
    // --------------------

    else if (gameState == 2)
    {
        if (CheckHitKey(KEY_INPUT_R))
        {
            player.Init();
            event.Init();
            choice.Init();

            gameState = 0;
        }
    }
}

void GameDraw()
{
    // ゲーム画面
    if (gameState == 0)
    {
        map.Draw();

        player.Draw();

        event.Draw();
    }

    // 選択肢画面
    else if (gameState == 1)
    {
        map.Draw();

        player.Draw();

        choice.Draw();
    }

    // エンディング
    else if (gameState == 2)
    {
        ending.Draw();
    }
}

void GameEnd()
{
}