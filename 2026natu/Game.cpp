#include "DxLib.h"
#include "Game.h"
#include "Block.h"
#include "Player.h"
#include "Draw.h"
#include <stdlib.h>
#include <time.h>

int gameState = 0;
int turn = 0;
int score = 0;
bool gameOver = false;

const float FALL_SPEED = 4.0f;
const float RISE_SPEED = 2.0f;
const float NEW_BLOCK_SPEED = 2.0f;
const float FOOD_SPEED = 2.0f;

int newBlockIndex = -1;

// 好物用
int foodBlockIndex = -1;
bool foodRising = false;


// ==================================================
// 関数の宣言
// ==================================================

void StartGravity();
void UpdateFalling();

void StartRising();
void UpdateRising();

void StartNewBlock();
void UpdateNewBlock();
void AddNewAnimal();

void MakeFavoriteFood();
void UpdateFavoriteFood();
void CheckFavoriteFood();


// ==================================================
// 好物を出す
// ==================================================

void MakeFavoriteFood()
{
    // 3ターン目までは出さない
    if (turn < 3)
        return;

    // 5回に1回くらい出る
    if (rand() % 5 != 0)
        return;

    // 動物がいるか調べる
    bool sheepExist = false;
    bool cowExist = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (blockType[i] == 'S')
            sheepExist = true;

        if (blockType[i] == 'C')
            cowExist = true;
    }

    // 動物がいなければ出さない
    if (sheepExist == false && cowExist == false)
        return;


    // 好物の種類を決める
    char foodType;

    if (sheepExist && cowExist)
    {
        // 羊と牛が両方いる場合はランダム
        if (rand() % 2 == 0)
            foodType = 'W';
        else
            foodType = 'T';
    }
    else if (sheepExist)
    {
        // 羊しかいないなら小麦
        foodType = 'W';
    }
    else
    {
        // 牛しかいないならとうもろこし
        foodType = 'T';
    }


    // 出す場所を決める
    int x = rand() % WIDTH;

    AddBlock(
        foodType,
        x,
        HEIGHT - 1,
        1
    );

    foodBlockIndex = blockCount - 1;

    // 画面の下から出す
    blockDrawY[foodBlockIndex] =
        (float)(BOARD_Y + HEIGHT * CELL);

    foodRising = true;
}


// ==================================================
// 好物を上に動かす
// ==================================================

void UpdateFavoriteFood()
{
    if (foodBlockIndex < 0)
        return;

    if (blockExist[foodBlockIndex] == false)
    {
        foodRising = false;
        foodBlockIndex = -1;
        return;
    }


    // 好物を少しずつ上げる
    blockDrawY[foodBlockIndex] -= FOOD_SPEED;


    // 画面内に入ったら止める
    float targetY =
        (float)(BOARD_Y +
            (HEIGHT - 1) * CELL);


    if (blockDrawY[foodBlockIndex] <= targetY)
    {
        blockDrawY[foodBlockIndex] = targetY;
        foodRising = false;
    }
}


// ==================================================
// 好物と動物をチェック
// ==================================================

void CheckFavoriteFood()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;


        // --------------------------------------------------
        // 小麦 → 羊
        // --------------------------------------------------

        if (blockType[i] == 'W')
        {
            for (int j = 0; j < blockCount; j++)
            {
                if (blockExist[j] == false)
                    continue;

                if (blockType[j] != 'S')
                    continue;


                // 小麦が羊の上にあるか
                if (blockY[i] == blockY[j] - 1)
                {
                    if (IsOverlapping(
                        blockX[i],
                        blockLength[i],
                        blockX[j],
                        blockLength[j]))
                    {
                        score += 100;

                        blockExist[i] = false;

                        if (i == foodBlockIndex)
                        {
                            foodBlockIndex = -1;
                            foodRising = false;
                        }

                        break;
                    }
                }
            }
        }


        // --------------------------------------------------
        // とうもろこし → 牛
        // --------------------------------------------------

        if (blockType[i] == 'T')
        {
            for (int j = 0; j < blockCount; j++)
            {
                if (blockExist[j] == false)
                    continue;

                if (blockType[j] != 'C')
                    continue;


                // とうもろこしが牛の上にあるか
                if (blockY[i] == blockY[j] - 1)
                {
                    if (IsOverlapping(
                        blockX[i],
                        blockLength[i],
                        blockX[j],
                        blockLength[j]))
                    {
                        score += 100;

                        blockExist[i] = false;

                        if (i == foodBlockIndex)
                        {
                            foodBlockIndex = -1;
                            foodRising = false;
                        }

                        break;
                    }
                }
            }
        }
    }
}


// ==================================================
// 新しい動物を追加
// ==================================================

void AddNewAnimal()
{
    char type;

    if (rand() % 2 == 0)
        type = 'S';
    else
        type = 'C';


    int length =
        rand() % 4 + 1;


    int x =
        rand() % (WIDTH - length + 1);


    AddBlock(
        type,
        x,
        HEIGHT - 1,
        length
    );


    newBlockIndex =
        blockCount - 1;


    // 画面の下から出す
    blockDrawY[newBlockIndex] =
        (float)(BOARD_Y + HEIGHT * CELL);


    newBlockRising = true;
}


// ==================================================
// 重力開始
// ==================================================

void StartGravity()
{
    bool someoneFalls = false;


    for (int i = 0; i < blockCount; i++)
        falling[i] = false;


    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (HasSupport(i))
            continue;

        if (blockY[i] >= HEIGHT - 1)
            continue;


        falling[i] = true;
        someoneFalls = true;
    }


    if (someoneFalls)
    {
        if (gameState == 4)
            gameState = 4;
        else
            gameState = 1;
    }
    else
    {
        if (gameState == 4)
        {
            gameState = 0;

            playerMoved = false;
            playerTurnFinished = false;
        }
        else
        {
            StartRising();
        }
    }
}


// ==================================================
// ブロックを落とす
// ==================================================

void UpdateFalling()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (falling[i] == false)
            continue;


        int targetY =
            blockY[i] + 1;


        if (targetY >= HEIGHT)
        {
            falling[i] = false;
            continue;
        }


        bool canFall = true;


        for (int j = 0; j < blockCount; j++)
        {
            if (i == j)
                continue;

            if (blockExist[j] == false)
                continue;

            if (blockY[j] != targetY)
                continue;


            if (IsOverlapping(
                blockX[i],
                blockLength[i],
                blockX[j],
                blockLength[j]))
            {
                canFall = false;
                break;
            }
        }


        if (canFall == false)
        {
            falling[i] = false;
            continue;
        }


        float targetDrawY =
            (float)(
                BOARD_Y +
                targetY * CELL
                );


        blockDrawY[i] += FALL_SPEED;


        if (blockDrawY[i] >= targetDrawY)
        {
            blockDrawY[i] =
                targetDrawY;

            blockY[i] =
                targetY;

            falling[i] = false;
        }
    }


    bool stillFalling = false;


    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (falling[i])
        {
            stillFalling = true;
            break;
        }
    }


    if (stillFalling)
        return;


    bool needMoreGravity = false;


    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (HasSupport(i))
            continue;


        if (blockY[i] < HEIGHT - 1)
        {
            needMoreGravity = true;
            break;
        }
    }


    if (needMoreGravity)
    {
        StartGravity();
    }
    else
    {
        if (gameState == 1)
        {
            StartRising();
        }
        else if (gameState == 4)
        {
            gameState = 0;

            playerMoved = false;
            playerTurnFinished = false;
        }
    }
}


// ==================================================
// 既存ブロックを1段上げる
// ==================================================

void StartRising()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;


        if (blockY[i] <= 0)
        {
            gameOver = true;
            return;
        }
    }


    bool hasBlock = false;


    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;


        hasBlock = true;

        blockY[i]--;

        rising[i] = true;
    }


    if (hasBlock)
        gameState = 2;
    else
        StartNewBlock();
}


// ==================================================
// ブロックを上げるアニメーション
// ==================================================

void UpdateRising()
{
    bool stillRising = false;


    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (rising[i] == false)
            continue;


        float targetY =
            (float)(
                BOARD_Y +
                blockY[i] * CELL
                );


        blockDrawY[i] -= RISE_SPEED;


        if (blockDrawY[i] <= targetY)
        {
            blockDrawY[i] =
                targetY;

            rising[i] = false;
        }
        else
        {
            stillRising = true;
        }
    }


    if (!stillRising)
        StartNewBlock();
}


// ==================================================
// 新しいブロックを出す
// ==================================================

void StartNewBlock()
{
    AddNewAnimal();

    gameState = 3;
}


// ==================================================
// 新しい動物を上げる
// ==================================================

void UpdateNewBlock()
{
    if (newBlockIndex < 0)
    {
        gameState = 4;
        StartGravity();
        return;
    }


    if (blockExist[newBlockIndex] == false)
    {
        gameState = 4;
        StartGravity();
        return;
    }


    float targetY =
        (float)(
            BOARD_Y +
            blockY[newBlockIndex] * CELL
            );


    blockDrawY[newBlockIndex] -=
        NEW_BLOCK_SPEED;


    if (blockDrawY[newBlockIndex] <= targetY)
    {
        blockDrawY[newBlockIndex] =
            targetY;

        newBlockRising = false;


        gameState = 4;

        StartGravity();
    }
}


// ==================================================
// ゲーム初期化
// ==================================================

void InitGame()
{
    srand((unsigned int)time(NULL));


    blockCount = 0;

    turn = 0;

    score = 0;

    gameOver = false;

    gameState = 0;

    newBlockIndex = -1;

    foodBlockIndex = -1;

    foodRising = false;


    dragging = false;

    dragBlock = -1;

    playerMoved = false;

    playerTurnFinished = false;

    newBlockRising = false;


    MakeInitialBlocks();
}


// ==================================================
// ゲーム更新
// ==================================================

void UpdateGame()
{
    // Escキーで終了
    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
        DxLib_End();
        exit(0);
    }


    if (gameOver)
        return;


    // ==================================================
    // プレイヤーのターン
    // ==================================================

    if (gameState == 0)
    {
        UpdatePlayer();


        if (playerTurnFinished)
        {
            turn++;

            playerTurnFinished = false;


            // 好物をチェック
            CheckFavoriteFood();


            // 重力開始
            StartGravity();
        }
    }


    // ==================================================
    // 1回目の重力
    // ==================================================

    else if (gameState == 1)
    {
        UpdateFalling();
    }


    // ==================================================
    // 既存ブロックを上げる
    // ==================================================

    else if (gameState == 2)
    {
        UpdateRising();
    }


    // ==================================================
    // 新しい動物を上げる
    // ==================================================

    else if (gameState == 3)
    {
        UpdateNewBlock();
    }


    // ==================================================
    // 2回目の重力
    // ==================================================

    else if (gameState == 4)
    {
        UpdateFalling();
    }


    // ==================================================
    // 好物の処理
    // ==================================================

    if (foodRising)
    {
        UpdateFavoriteFood();
    }


    // 3ターン目以降、好物を出す
    // すでに好物が出ている場合は出さない
    if (turn >= 3 &&
        foodBlockIndex == -1)
    {
        MakeFavoriteFood();
    }
}


// ==================================================
// ゲーム描画
// ==================================================

void DrawGame()
{
    DrawGameScreen(
        turn,
        score,
        gameOver
    );
}