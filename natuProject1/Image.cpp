#include "Image.h"



// =========================================
// コンストラクタ
// =========================================

Image::Image()
{

    playerImage = -1;

    enemyImage = -1;

    tileImage = -1;

}



// =========================================
// デストラクタ
// =========================================

Image::~Image()
{

    Release();

}



// =========================================
// 読み込み
// =========================================

bool Image::Load()
{

    playerImage =
        LoadGraph(
            TEXT("player.png")
        );


    enemyImage =
        LoadGraph(
            TEXT("enemy.png")
        );


    tileImage =
        LoadGraph(
            TEXT("tiles.png")
        );


    if (playerImage == -1)
    {
        OutputDebugString(
            TEXT("player 読み込み失敗\n")
        );
    }


    return true;

}



// =========================================
// プレイヤー画像
// =========================================

int Image::GetPlayerImage()
{

    return playerImage;

}



// =========================================
// 敵画像
// =========================================

int Image::GetEnemyImage()
{

    return enemyImage;

}



// =========================================
// タイル画像
// =========================================

int Image::GetTileImage()
{

    return tileImage;

}



// =========================================
// 解放
// =========================================

void Image::Release()
{


    if (playerImage != -1)
    {

        DeleteGraph(playerImage);

    }



    if (enemyImage != -1)
    {

        DeleteGraph(enemyImage);

    }



    if (tileImage != -1)
    {

        DeleteGraph(tileImage);

    }


}