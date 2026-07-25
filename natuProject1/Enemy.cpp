#include "Enemy.h"

#include "Camera.h"
#include "Image.h"

#include <DxLib.h>



// =========================================
// コンストラクタ
// =========================================

Enemy::Enemy(
    float x,
    float y
)
{

    position.x = x;

    position.y = y;


    speed = ENEMY_SPEED;


    direction = Direction::Left;


    width = ENEMY_WIDTH;

    height = ENEMY_HEIGHT;


    alive = true;

}



// =========================================
// デストラクタ
// =========================================

Enemy::~Enemy()
{

}



// =========================================
// 更新
// =========================================

void Enemy::Update()
{

    // 左右移動
    if (!alive)
    {
        return;
    }
    if (direction == Direction::Left)
    {

        position.x -= speed;

    }
    else
    {

        position.x += speed;

    }



    // 移動範囲

    if (position.x < 300)
    {

        direction = Direction::Right;

    }



    if (position.x > 600)
    {

        direction = Direction::Left;

    }


}



// =========================================
// 描画
// =========================================

void Enemy::Draw(
    Camera& camera,
    Image& image
)
{
    if (!alive)
    {
        return;
    }
    int handle =
        image.GetEnemyImage();



    // 画像がない場合

    if (handle == -1)
    {

        DrawBox(

            camera.GetScreenX(position.x),

            camera.GetScreenY(position.y),


            camera.GetScreenX(
                position.x + width
            ),

            camera.GetScreenY(
                position.y + height
            ),


            GetColor(255, 100, 0),

            TRUE

        );


        return;

    }



    // 敵画像表示

    DrawGraph(

        camera.GetScreenX(position.x),

        camera.GetScreenY(position.y),


        handle,

        TRUE

    );


}



// =========================================
// 座標取得
// =========================================

Vector2 Enemy::GetPosition()
{

    return position;

}



// =========================================
// Rect取得
// =========================================

Rect Enemy::GetRect()
{

    Rect rect;


    rect.left =
        position.x;


    rect.top =
        position.y;


    rect.right =
        position.x + width;


    rect.bottom =
        position.y + height;



    return rect;

}

bool Enemy::IsAlive()
{

    return alive;

}



void Enemy::Dead()
{

    alive = false;

}