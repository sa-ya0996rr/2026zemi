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

int newBlockIndex = -1;

void StartGravity();
void UpdateFalling();
void StartRising();
void UpdateRising();
void StartNewBlock();
void UpdateNewBlock();
void AddNewAnimal();
void MakeFavoriteFood();
void CheckFavoriteFood();

void MakeFavoriteFood()
{
    if (turn < 3) return;
    if (rand() % 5 != 0) return;

    int x = rand() % WIDTH;

    if (rand() % 2 == 0)
        AddBlock('W', x, HEIGHT - 1, 1);
    else
        AddBlock('T', x, HEIGHT - 1, 1);

    newBlockIndex = blockCount - 1;
    blockDrawY[newBlockIndex] = (float)(BOARD_Y + HEIGHT * CELL);
    newBlockRising = true;
}

void CheckFavoriteFood()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (!blockExist[i]) continue;

        if (blockType[i] == 'W')
        {
            for (int j = 0; j < blockCount; j++)
            {
                if (!blockExist[j]) continue;
                if (blockType[j] != 'S') continue;

                if (blockY[i] == blockY[j] - 1)
                {
                    if (IsOverlapping(blockX[i], blockLength[i],
                        blockX[j], blockLength[j]))
                    {
                        score += 100;
                        blockExist[i] = false;
                        break;
                    }
                }
            }
        }

        if (blockType[i] == 'T')
        {
            for (int j = 0; j < blockCount; j++)
            {
                if (!blockExist[j]) continue;
                if (blockType[j] != 'C') continue;

                if (blockY[i] == blockY[j] - 1)
                {
                    if (IsOverlapping(blockX[i], blockLength[i],
                        blockX[j], blockLength[j]))
                    {
                        score += 100;
                        blockExist[i] = false;
                        break;
                    }
                }
            }
        }
    }
}

void AddNewAnimal()
{
    char type;

    if (rand() % 2 == 0)
        type = 'S';
    else
        type = 'C';

    int length = rand() % 4 + 1;
    int x = rand() % (WIDTH - length + 1);

    AddBlock(type, x, HEIGHT - 1, length);

    newBlockIndex = blockCount - 1;
    blockDrawY[newBlockIndex] = (float)(BOARD_Y + HEIGHT * CELL);
    newBlockRising = true;
}

void StartGravity()
{
    bool someoneFalls = false;

    for (int i = 0; i < blockCount; i++)
        falling[i] = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (!blockExist[i]) continue;
        if (HasSupport(i)) continue;
        if (blockY[i] >= HEIGHT - 1) continue;

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

void UpdateFalling()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (!blockExist[i]) continue;
        if (!falling[i]) continue;

        int targetY = blockY[i] + 1;

        if (targetY >= HEIGHT)
        {
            falling[i] = false;
            continue;
        }

        bool canFall = true;

        for (int j = 0; j < blockCount; j++)
        {
            if (i == j) continue;
            if (!blockExist[j]) continue;
            if (blockY[j] != targetY) continue;

            if (IsOverlapping(blockX[i], blockLength[i],
                blockX[j], blockLength[j]))
            {
                canFall = false;
                break;
            }
        }

        if (!canFall)
        {
            falling[i] = false;
            continue;
        }

        float targetDrawY = (float)(BOARD_Y + targetY * CELL);

        blockDrawY[i] += FALL_SPEED;

        if (blockDrawY[i] >= targetDrawY)
        {
            blockDrawY[i] = targetDrawY;
            blockY[i] = targetY;
            falling[i] = false;
        }
    }

    bool stillFalling = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (!blockExist[i]) continue;

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
        if (!blockExist[i]) continue;
        if (HasSupport(i)) continue;

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

void StartRising()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (!blockExist[i]) continue;

        if (blockY[i] <= 0)
        {
            gameOver = true;
            return;
        }
    }

    bool hasBlock = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (!blockExist[i]) continue;

        hasBlock = true;
        blockY[i]--;
        rising[i] = true;
    }

    if (hasBlock)
        gameState = 2;
    else
        StartNewBlock();
}

void UpdateRising()
{
    bool stillRising = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (!blockExist[i]) continue;
        if (!rising[i]) continue;

        float targetY = (float)(BOARD_Y + blockY[i] * CELL);

        blockDrawY[i] -= RISE_SPEED;

        if (blockDrawY[i] <= targetY)
        {
            blockDrawY[i] = targetY;
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

void StartNewBlock()
{
    AddNewAnimal();
    gameState = 3;
}

void UpdateNewBlock()
{
    if (newBlockIndex < 0)
    {
        gameState = 4;
        StartGravity();
        return;
    }

    if (!blockExist[newBlockIndex])
    {
        gameState = 4;
        StartGravity();
        return;
    }

    float targetY = (float)(BOARD_Y + blockY[newBlockIndex] * CELL);

    blockDrawY[newBlockIndex] -= NEW_BLOCK_SPEED;

    if (blockDrawY[newBlockIndex] <= targetY)
    {
        blockDrawY[newBlockIndex] = targetY;
        newBlockRising = false;

        gameState = 4;
        StartGravity();
    }
}

void InitGame()
{
    srand((unsigned int)time(NULL));

    blockCount = 0;
    turn = 0;
    score = 0;
    gameOver = false;
    gameState = 0;
    newBlockIndex = -1;

    dragging = false;
    dragBlock = -1;
    playerMoved = false;
    playerTurnFinished = false;
    newBlockRising = false;

    MakeInitialBlocks();
}

void UpdateGame()
{
    // Escキーでゲーム終了
    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
        DxLib_End();
        exit(0);
    }

    if (gameOver)
        return;

    if (gameState == 0)
    {
        UpdatePlayer();

        if (playerTurnFinished)
        {
            turn++;
            playerTurnFinished = false;

            CheckFavoriteFood();

            StartGravity();
        }
    }
    else if (gameState == 1)
    {
        UpdateFalling();
    }
    else if (gameState == 2)
    {
        UpdateRising();
    }
    else if (gameState == 3)
    {
        UpdateNewBlock();
    }
    else if (gameState == 4)
    {
        UpdateFalling();
    }
}

void DrawGame()
{
    DrawGameScreen(turn, score, gameOver);
}