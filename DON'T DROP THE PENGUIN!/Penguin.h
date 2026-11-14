
#pragma once

#include "DxLib.h"

class Ice;

class Penguin
{
public:
    Penguin();

    // 画像の読み込み
    bool Init();

    // 画像の解放
    void Release();

    // ペンギンの位置を設定
    void SetPosition(int x, int y);

    // 指定された氷のタイルの上にいるか判定
    bool IsOnTile(int index, const Ice& ice) const;

    // ペンギンが落下しているか判定
    bool HasFallen(const Ice& ice) const;

    // ペンギンを描画
    void Draw(const Ice& ice, int player) const;

private:
    int x;
    int y;

    // ペンギン画像のハンドル
    int imageHandle;
};
