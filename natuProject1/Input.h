#pragma once
#ifndef INPUT_H
#define INPUT_H


class Input
{

public:

    // コンストラクタ
    Input();


    // 更新
    void Update();


    // キー入力取得

    bool Right();

    bool Left();

    bool Jump();

    bool Escape();



private:

    // 現在のキー状態
    char key[256];

};



#endif