#pragma once

class Player
{
public:

    int x;
    int y;

    // 画像
    int rightImage[2];
    int leftImage[2];
    int backImage[2];
    int frontImage;

    // 0 = 正面
    // 1 = 右
    // 2 = 後ろ
    // 3 = 左
    int direction;

    int walkFrame;
    int animationTimer;

    void Init();
    void Update();
    void Draw();
};