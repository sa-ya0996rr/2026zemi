#include "Penguin.h"
#include "Ice.h"
#include <tchar.h>

Penguin::Penguin() : x(0), y(0) {}
// ペンギンの位置を設定
void Penguin::SetPosition(int px, int py) { x = px; y = py; }
// ペンギンが指定された氷のタイルの上にいるかどうかの判定
bool Penguin::IsOnTile(int i, const Ice& ice) const { return i == y * BOARD_COLS + x; }
// ペンギンが落下しているかどうかの判定
bool Penguin::HasFallen(const Ice& ice) const { return !ice.IsTilePresent(y * BOARD_COLS + x); }
// ペンギンを描画　
void Penguin::Draw(const Ice& ice, int player) const
{
    // ペンギンが落下している場合は描画しない
    if (HasFallen(ice)) return;

    // ペンギンの位置に対応するタイルのインデックスを計算
    int i = y * BOARD_COLS + x;
    // ペンギンの位置を取得
    int cx = ice.TileCenterX(i), cy = ice.TileCenterY(i);
    // ペンギンの体の色
    int bodyColor = player == 0 ? GetColor(25, 30, 45) : GetColor(80, 50, 30);

    // ペンギンの体を描画
    DrawCircle(cx, cy - 5, 22, bodyColor, TRUE);

    // ペンギンのお腹を描画
    DrawCircle(
        cx,                         // お腹の中心X座標
        cy + 2,                     // お腹の中心Y座標
        13,                         // 横方向の半径
        17,                         // 縦方向の半径
        GetColor(255, 255, 255),    // お腹の色
        TRUE                        // 塗りつぶす
    );

    // ペンギンの左目を描画
    DrawCircle(cx - 7, cy - 12, 3, GetColor(255, 255, 255), TRUE);

    // ペンギンの右目を描画
    DrawCircle(cx + 7, cy - 12, 3, GetColor(255, 255, 255), TRUE);

    // ペンギンのくちばしを描画
    DrawTriangle(cx - 5, cy - 5, cx + 5, cy - 5, cx, cy + 3, GetColor(255, 180, 30), TRUE);
}
