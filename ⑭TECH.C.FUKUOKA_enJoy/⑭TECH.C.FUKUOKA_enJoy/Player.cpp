#include "Player.h"
#include "DxLib.h"

void Player::Init()
{
    // プレイヤーの初期位置
    x = 600;
    y = 400;

    // 画像読み込み
    rightImage[0] = LoadGraph("Images/player_right_1.png");
    rightImage[1] = LoadGraph("Images/player_right_2.png");

    leftImage[0] = LoadGraph("Images/player_left_1.png");
    leftImage[1] = LoadGraph("Images/player_left_2.png");

    backImage[0] = LoadGraph("Images/player_back_1.png");
    backImage[1] = LoadGraph("Images/player_back_2.png");

    frontImage = LoadGraph("Images/player_front.png");

    // 最初は正面
    direction = 0;

    walkFrame = 0;
    animationTimer = 0;
}

void Player::Update()
{
    bool moving = false;

    // --------------------
    // 上
    // --------------------
    if (CheckHitKey(KEY_INPUT_W))
    {
        y -= 4;

        direction = 2;
        moving = true;
    }

    // --------------------
    // 下
    // --------------------
    if (CheckHitKey(KEY_INPUT_S))
    {
        y += 4;

        direction = 0;
        moving = true;
    }

    // --------------------
    // 左
    // --------------------
    if (CheckHitKey(KEY_INPUT_A))
    {
        x -= 4;

        direction = 3;
        moving = true;
    }

    // --------------------
    // 右
    // --------------------
    if (CheckHitKey(KEY_INPUT_D))
    {
        x += 4;

        direction = 1;
        moving = true;
    }

    // --------------------
    // 画面外に出ないようにする
    // --------------------

    if (x < 50)
    {
        x = 50;
    }

    if (x > 1150)
    {
        x = 1150;
    }

    if (y < 100)
    {
        y = 100;
    }

    if (y > 650)
    {
        y = 650;
    }

    // --------------------
    // 歩きアニメーション
    // --------------------

    if (moving)
    {
        animationTimer++;

        if (animationTimer >= 10)
        {
            animationTimer = 0;

            walkFrame++;

            if (walkFrame >= 2)
            {
                walkFrame = 0;
            }
        }
    }
    else
    {
        walkFrame = 0;
        animationTimer = 0;
    }
}

void Player::Draw()
{
    int image = -1;

    // --------------------
    // 正面
    // --------------------

    if (direction == 0)
    {
        image = frontImage;
    }

    // --------------------
    // 右
    // --------------------

    else if (direction == 1)
    {
        if (walkFrame == 0)
        {
            image = rightImage[0];
        }
        else
        {
            image = rightImage[1];
        }
    }

    // --------------------
    // 後ろ
    // --------------------

    else if (direction == 2)
    {
        if (walkFrame == 0)
        {
            image = backImage[0];
        }
        else
        {
            image = backImage[1];
        }
    }

    // --------------------
    // 左
    // --------------------

    else if (direction == 3)
    {
        if (walkFrame == 0)
        {
            image = leftImage[0];
        }
        else
        {
            image = leftImage[1];
        }
    }

    // --------------------
    // キャラクター表示
    // --------------------

    if (image != -1)
    {
        DrawExtendGraph(
            x,
            y,
            x + 100,
            y + 140,
            image,
            TRUE
        );
    }
}