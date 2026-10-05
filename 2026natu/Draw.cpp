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
        BOARD_X +
        WIDTH * CELL,
        BOARD_Y +
        HEIGHT * CELL,
        GetColor(
            220,
            210,
            170),
        TRUE);


    // ==========================================
    // マス目
    // ==========================================

    for (int y = 0;
        y <= HEIGHT;
        y++)
    {
        DrawLine(
            BOARD_X,
            BOARD_Y +
            y * CELL,
            BOARD_X +
            WIDTH * CELL,
            BOARD_Y +
            y * CELL,
            GetColor(
                150,
                150,
                150));
    }


    for (int x = 0;
        x <= WIDTH;
        x++)
    {
        DrawLine(
            BOARD_X +
            x * CELL,
            BOARD_Y,
            BOARD_X +
            x * CELL,
            BOARD_Y +
            HEIGHT * CELL,
            GetColor(
                150,
                150,
                150));
    }


    // ==========================================
    // ブロック
    // ==========================================

    for (int i = 0;
        i < blockCount;
        i++)
    {
        if (blockExist[i] == false)
        {
            continue;
        }


        int x =
            BOARD_X +
            blockX[i] * CELL;


        int y =
            (int)blockDrawY[i];


        int width =
            blockLength[i] * CELL;


        int color;


        // ======================================
        // 羊
        // ======================================

        if (blockType[i] == 'S')
        {
            color =
                GetColor(
                    245,
                    245,
                    245);
        }


        // ======================================
        // 牛
        // ======================================

        else if (blockType[i] == 'C')
        {
            color =
                GetColor(
                    80,
                    80,
                    80);
        }


        // ======================================
        // 小麦
        // ======================================

        else if (blockType[i] == 'W')
        {
            color =
                GetColor(
                    240,
                    210,
                    100);
        }


        // ======================================
        // とうもろこし
        // ======================================

        else
        {
            color =
                GetColor(
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


        text[0] =
            blockType[i];


        text[1] =
            '\0';


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


        sprintf_s(
            finalScore,
            "SCORE : %d",
            score);


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