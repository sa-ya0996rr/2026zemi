#include "DxLib.h"
#include "Draw.h"
#include "Block.h"
#include <cstdio>

// ==================================================
// ゲーム画面
// ==================================================
void DrawGameScreen(
    int turn,
    int score,
    bool gameOver)
{
    // ==========================================
    // 背景
    // ==========================================
    DrawBox(
        0,
        0,
        1200,
        800,
    GetColor(
        180,
        220,
        180),
        TRUE);

    // ==========================================
    // タイトル
    // ==========================================
    DrawString(
        500,
        20,
        "FARM PUZZLE",
        GetColor(
            0,
            0,
            0));

    // ==========================================
    // TURN
    // ==========================================
    char turnText[50];

    sprintf_s(
        turnText,
        "TURN : %d",
        turn);

    DrawString(
        50,
        20,
        turnText,
        GetColor(
            0,
            0,
            0));

    // ==========================================
    // SCORE
    // ==========================================
    char scoreText[50];

    sprintf_s(
        scoreText,
        "SCORE : %d",
        score);

    DrawString(
        950,
        20,
        scoreText,
        GetColor(
            0,
            0,
            0));

    // ==========================================
    // 説明
    // ==========================================
    DrawString(
        40,
        90,
        "S = SHEEP",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        120,
        "C = COW",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        150,
        "W = WHEAT",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        180,
        "T = CORN",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        220,
        "DRAG ANIMAL",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        250,
        "MOVE 1 CELL = END TURN",
        GetColor(
            0,
            0,
            0));

    // ==========================================
    // マップ
    // ==========================================
    DrawBox(
        BOARD_X,
        BOARD_Y,
        BOARD_X + WIDTH * CELL,
        BOARD_Y + HEIGHT * CELL,
        GetColor(
            220,
            210,
            170),
        TRUE);

    // ==========================================
    // マス目
    // ==========================================
    for (int y = 0; y <= HEIGHT; y++)
    {
        DrawLine(BOARD_X, BOARD_Y + y * CELL, BOARD_X + WIDTH * CELL, BOARD_Y + y * CELL,
            GetColor(
                150,
                150,
                150));
    }

    for (int x = 0; x <= WIDTH; x++)
    {
        DrawLine(BOARD_X + x * CELL, BOARD_Y, BOARD_X + x * CELL, BOARD_Y + HEIGHT * CELL,
            GetColor(
                150,
                150,
                150));
    }

    // ==========================================
    // ブロック
    // ==========================================
    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
        {
            continue;
        }

        int x = BOARD_X + blockX[i] * CELL;

        int y = (int)blockDrawY[i];

        int width = blockLength[i] * CELL;

        int color;

        // 羊
        if (blockType[i] == 'S')
        {
            color = GetColor(
                        245,
                        245,
                        245);
        }

        // 牛
        else if (blockType[i] == 'C')
        {
            color = GetColor(
                        80,
                        80,
                        80);
        }

        // ======================================
        // 好物
        // ======================================
		// 小麦
        else if (blockType[i] == 'W')
        {
            color = GetColor(
                        240,
                        210,
                        100);
        }

		// とうもろこし
        else
        {
            color = GetColor(
                        250,
                        220,
                        80);
        }

        // ======================================
        // ブロック本体
        // ======================================
        DrawBox(
            x + 3,
            y + 3,
            x + width - 3,
            y + CELL - 3,
            color,
            TRUE);

        // ======================================
        // 枠
        // ======================================
        DrawBox(
            x + 3,
            y + 3,
            x + width - 3,
            y + CELL - 3,
            GetColor(
                80,
                80,
                80),
            FALSE);

        // ======================================
        // S / C / W / T
        // ======================================
        char text[2];

        text[0] = blockType[i];

        text[1] = '\0';

        DrawString(
            x + 20,
            y + 15,
            text,
            GetColor(
                0,
                0,
                0));
    }

    // ==========================================
    // GAME OVER
    // ==========================================
    if (gameOver)
    {
        DrawBox(
            350,
            300,
            850,
            450,
            GetColor(
                255,
                255,
                255),
            TRUE);

        DrawString(
            500,
            340,
            "GAME OVER",
            GetColor(
                0,
                0,
                0));

        char finalScore[50];

        sprintf_s(finalScore, "SCORE : %d", score);

        DrawString(
            500,
            390,
            finalScore,
            GetColor(
                0,
                0,
                0));
    }
}

/*
この Draw.cpp で使っているもの

先生に説明するときに整理すると、こんな感じ。

種類	        このファイルでの使用例	    目的
if文	        if (gameOver)	            ゲームオーバー表示などの条件判定
else if文	    blockType[i] == 'C'など	    ブロック種類ごとの処理
else文	        とうもろこしの処理	        それ以外の種類を処理
for文	        マス目・ブロックの描画	    同じ処理を繰り返す
continue	    存在しないブロックを飛ばす	不要な描画をしない
関数	        DrawGameScreen()	        描画処理をまとめる
配列	        turnText[50]など	        文字列を保存
変数	        x, y, width, color	        描画に必要な情報を保存
int型	        座標、スコア、ターン	    整数を扱う
bool型	        gameOver	                ON/OFF状態を扱う
char型	        text[2]	                    1文字・文字列を扱う
型変換	        (int)blockDrawY[i]	        floatをintに変換
DxLibの関数	    DrawBox()など	            実際に画面へ描画

特に覚えておくといいところ

このDraw.cppは、

ゲームの中身を計算するファイルではなく、計算された結果を「見える形」にするファイル。

例えば、

Block.cpp
「羊は横3マス、縦8段にいる」
        ↓
Draw.cpp
「じゃあ画面のこの場所に羊のブロックを描こう」

という関係。

だから、

Block.cpp → ブロックの管理・判定
Player.cpp → プレイヤーの操作
Game.cpp → ゲーム全体の進行
Draw.cpp → 画面への表示

という役割分担になっている。

そして今回のDraw.cppは、if文・else if文・else文・for文・関数・配列・変数・int・bool・char・型変換などを実際に使っているファイルだよ。
*/