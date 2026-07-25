#include "Game.h"

#include <DxLib.h>



// =========================================
// コンストラクタ
// =========================================

Game::Game()
    :
    enemy(500, 500)
{

    image.Load();



    camera.SetStageSize(
        40 * TILE_SIZE,
        20 * TILE_SIZE
    );

}



// =========================================
// デストラクタ
// =========================================

Game::~Game()
{

}



// =========================================
// 更新
// =========================================

void Game::Update()
{

    input.Update();



    player.Update(
        stage,
        input
    );



    stage.Update();



    enemy.Update();




    // 敵との接触

    if (CheckEnemyCollision())
    {

        OutputDebugString(
            TEXT("Enemy Hit\n")
        );

    }




    camera.Update(
        player.GetPosition()
    );


}



// =========================================
// 描画
// =========================================

void Game::Draw()
{

    stage.Draw(
        camera,
        image
    );


    player.Draw(
        camera,
        image
    );


    enemy.Draw(
        camera,
        image
    );


}



// =========================================
// 敵接触判定
// =========================================

bool Game::CheckEnemyCollision()
{

    Rect playerRect =
        player.GetRect();



    Rect enemyRect =
        enemy.GetRect();




    if (playerRect.right < enemyRect.left)
    {
        return false;
    }


    if (playerRect.left > enemyRect.right)
    {
        return false;
    }


    if (playerRect.bottom < enemyRect.top)
    {
        return false;
    }


    if (playerRect.top > enemyRect.bottom)
    {
        return false;
    }



    return true;

}