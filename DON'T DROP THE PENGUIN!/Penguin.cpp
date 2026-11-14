
#include "Penguin.h"
#include "Ice.h"
#include <tchar.h>

Penguin::Penguin()
    : x(0),
    y(0),
    imageHandle(-1)
{
}

// 画像の読み込み
bool Penguin::Init()
{
    imageHandle = LoadGraph(_T("images/penguin.png"));

    printfDx(_T("Penguin Image Handle: %d\n"), imageHandle);

    if (imageHandle == -1)
    {
        printfDx(_T("Penguin image load failed!\n"));
        return false;
    }

    printfDx(_T("Penguin image load success!\n"));

    return true;
}

// 画像の解放
void Penguin::Release()
{
    if (imageHandle != -1)
    {
        DeleteGraph(imageHandle);
        imageHandle = -1;
    }
}

// ペンギンの位置を設定
void Penguin::SetPosition(int px, int py)
{
    x = px;
    y = py;
}

// ペンギンが指定された氷のタイルの上にいるか判定
bool Penguin::IsOnTile(int i, const Ice& ice) const
{
    return i == y * BOARD_COLS + x;
}

// ペンギンが落下しているか判定
bool Penguin::HasFallen(const Ice& ice) const
{
    return !ice.IsTilePresent(y * BOARD_COLS + x);
}

// ペンギンを描画
void Penguin::Draw(const Ice& ice, int player) const
{
    // ペンギンが落下している場合は描画しない
    if (HasFallen(ice))
    {
        return;
    }

    // 画像が読み込まれていない場合は描画しない
    if (imageHandle == -1)
    {
        return;
    }

    // ペンギンの位置に対応するタイルのインデックス
    int i = y * BOARD_COLS + x;

    // タイルの中心座標
    int cx = ice.TileCenterX(i);
    int cy = ice.TileCenterY(i);

    // ペンギン画像の描画サイズ
    int width = 60;
    int height = 70;

    // 描画位置
    int left = cx - width / 2;
    int top = cy - height + 25;
    int right = left + width;
    int bottom = top + height;

    // ペンギン画像を描画
    DrawExtendGraph(
        left,
        top,
        right,
        bottom,
        imageHandle,
        TRUE
    );
}
