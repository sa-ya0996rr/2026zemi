#include "Camera.h"



// =========================================
// コンストラクタ
// =========================================

Camera::Camera()
{

    position.x = 0;

    position.y = 0;



    minX = 0;

    minY = 0;



    maxX = 0;

    maxY = 0;

}



// =========================================
// デストラクタ
// =========================================

Camera::~Camera()
{

}



// =========================================
// ステージサイズ設定
// =========================================

void Camera::SetStageSize(
    int width,
    int height
)
{


    maxX =
        width - SCREEN_WIDTH;



    maxY =
        height - SCREEN_HEIGHT;



    if (maxX < 0)
    {

        maxX = 0;

    }



    if (maxY < 0)
    {

        maxY = 0;

    }

}



// =========================================
// 更新
// =========================================

void Camera::Update(
    Vector2 playerPosition
)
{


    float targetX =
        playerPosition.x -
        SCREEN_WIDTH / 2;



    float targetY =
        playerPosition.y -
        SCREEN_HEIGHT / 2;




    position.x +=
        (targetX - position.x)
        *
        CAMERA_SPEED;



    position.y +=
        (targetY - position.y)
        *
        CAMERA_SPEED;




    if (position.x < minX)
    {

        position.x = minX;

    }



    if (position.x > maxX)
    {

        position.x = maxX;

    }




    if (position.y < minY)
    {

        position.y = minY;

    }



    if (position.y > maxY)
    {

        position.y = maxY;

    }



}



// =========================================
// 画面座標変換
// =========================================

int Camera::GetScreenX(
    float worldX
)
{

    return (int)
        (worldX - position.x);

}



int Camera::GetScreenY(
    float worldY
)
{

    return (int)
        (worldY - position.y);

}



// =========================================
// 座標取得
// =========================================

Vector2 Camera::GetPosition()
{

    return position;

}