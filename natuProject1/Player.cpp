#include "Player.h"

#include "Stage.h"
#include "Input.h"
#include "Camera.h"
#include "Image.h"

#include <DxLib.h>



// =========================================
// コンストラクタ
// =========================================

Player::Player()
{

    position.x = 100;

    position.y = 400;


    velocityY = 0;


    isGround = false;


    direction = Direction::Right;

}



// =========================================
// デストラクタ
// =========================================

Player::~Player()
{

}



// =========================================
// 更新
// =========================================

void Player::Update(
    Stage& stage,
    Input& input
)
{

    // -------------------------
    // 横移動
    // -------------------------

    float moveX = 0;


    if (input.Left())
    {

        moveX -= PLAYER_MOVE_SPEED;

        direction = Direction::Left;

    }


    if (input.Right())
    {

        moveX += PLAYER_MOVE_SPEED;

        direction = Direction::Right;

    }



    position.x += moveX;



    // 横方向の当たり判定

    if (stage.CheckCollision(GetRect()))
    {

        position.x -= moveX;

    }





    // -------------------------
    // ジャンプ
    // -------------------------

    if (input.Jump() && isGround)
    {

        Jump();

    }





    // -------------------------
    // 縦移動
    // -------------------------

    ApplyGravity();




    position.y += velocityY;



    // 縦方向の当たり判定

    if (stage.CheckCollision(GetRect()))
    {

        // 落下中

        if (velocityY > 0)
        {

            position.y -= velocityY;


            isGround = true;

        }


        // 上昇中

        else if (velocityY < 0)
        {

            position.y -= velocityY;

        }



        velocityY = 0;

    }
    else
    {

        isGround = false;

    }


}



// =========================================
// 描画
// =========================================

void Player::Draw(
    Camera& camera,
    Image& image
)
{

    int handle =
        image.GetPlayerImage();



    if (handle == -1)
    {

        DrawBox(

            camera.GetScreenX(position.x),

            camera.GetScreenY(position.y),


            camera.GetScreenX(
                position.x + PLAYER_WIDTH
            ),

            camera.GetScreenY(
                position.y + PLAYER_HEIGHT
            ),


            GetColor(255, 0, 0),

            TRUE

        );


        return;

    }




    DrawGraph(

        camera.GetScreenX(position.x),

        camera.GetScreenY(position.y),


        handle,

        TRUE

    );


}



// =========================================
// ジャンプ
// =========================================

void Player::Jump()
{

    velocityY =
        PLAYER_JUMP_POWER;


    isGround = false;

}



// =========================================
// 重力
// =========================================

void Player::ApplyGravity()
{

    velocityY += GRAVITY;



    if (velocityY > MAX_FALL_SPEED)
    {

        velocityY = MAX_FALL_SPEED;

    }

}



// =========================================
// 座標取得
// =========================================

Vector2 Player::GetPosition()
{

    return position;

}



// =========================================
// Rect取得
// =========================================

Rect Player::GetRect()
{

    Rect rect;



    rect.left =
        position.x;


    rect.top =
        position.y;



    rect.right =
        position.x + PLAYER_WIDTH;


    rect.bottom =
        position.y + PLAYER_HEIGHT;



    return rect;

}