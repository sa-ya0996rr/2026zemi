#include "DxLib.h"
#include "Player.h"
#include "Block.h"


// ==================================================
// プレイヤー情報
// ==================================================

bool dragging = false;

int dragBlock = -1;

bool playerMoved = false;

bool playerTurnFinished = false;


// ==================================================
// マウス情報
// ==================================================

int mouseStartX = 0;

int blockStartX = 0;

int oldMouse = 0;


// ==================================================
// 実際に動かしたか
// ==================================================

bool actuallyMoved = false;


// ==================================================
// マウスからブロックを探す
// ==================================================

int GetBlockFromMouse(
    int mouseX,
    int mouseY)
{
    // 後から追加したものを優先
    for (int i = blockCount - 1;
        i >= 0;
        i--)
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


        if (mouseX >= x &&
            mouseX < x + width &&
            mouseY >= y &&
            mouseY < y + CELL)
        {
            return i;
        }
    }


    return -1;
}


// ==================================================
// 移動できるか
// ==================================================

bool CanMoveBlock(
    int number,
    int newX)
{
    // ==============================================
    // 左端
    // ==============================================

    if (newX < 0)
    {
        return false;
    }


    // ==============================================
    // 右端
    // ==============================================

    if (newX +
        blockLength[number] >
        WIDTH)
    {
        return false;
    }


    // ==============================================
    // 他のブロックと重ならないか
    // ==============================================

    for (int i = 0;
        i < blockCount;
        i++)
    {
        if (i == number)
        {
            continue;
        }


        if (blockExist[i] == false)
        {
            continue;
        }


        // 同じ段だけ確認
        if (blockY[i] !=
            blockY[number])
        {
            continue;
        }


        if (IsOverlapping(
            newX,
            blockLength[number],
            blockX[i],
            blockLength[i]))
        {
            return false;
        }
    }


    return true;
}


// ==================================================
// ブロックを移動
// ==================================================

void MoveBlockByMouse(
    int number)
{
    int mouseX;
    int mouseY;


    GetMousePoint(
        &mouseX,
        &mouseY);


    int difference =
        mouseX -
        mouseStartX;


    int moveCell =
        difference /
        CELL;


    int newX =
        blockStartX +
        moveCell;


    // ==============================================
    // 移動できる場合
    // ==============================================

    if (CanMoveBlock(
        number,
        newX))
    {
        // 元の位置と違う
        if (newX !=
            blockX[number])
        {
            blockX[number] =
                newX;


            // 本当に動かした
            actuallyMoved = true;
        }
    }
}


// ==================================================
// プレイヤー更新
// ==================================================

void UpdatePlayer()
{
    int mouseX;
    int mouseY;


    GetMousePoint(
        &mouseX,
        &mouseY);


    int mouse =
        GetMouseInput();


    // ==============================================
    // 押した瞬間
    // ==============================================

    if ((mouse &
        MOUSE_INPUT_LEFT) &&
        !(oldMouse &
            MOUSE_INPUT_LEFT))
    {
        // このターンまだ動かしていない
        if (playerMoved == false)
        {
            int number =
                GetBlockFromMouse(
                    mouseX,
                    mouseY);


            if (number != -1)
            {
                // ==================================
                // 動物だけ動かせる
                // ==================================

                if (blockType[number] == 'S' ||
                    blockType[number] == 'C')
                {
                    dragging = true;

                    dragBlock =
                        number;


                    mouseStartX =
                        mouseX;


                    blockStartX =
                        blockX[number];


                    actuallyMoved =
                        false;
                }
            }
        }
    }


    // ==============================================
    // 押している間
    // ==============================================

    if ((mouse &
        MOUSE_INPUT_LEFT) &&
        dragging)
    {
        MoveBlockByMouse(
            dragBlock);
    }


    // ==============================================
    // 離した瞬間
    // ==============================================

    if (!(mouse &
        MOUSE_INPUT_LEFT) &&
        (oldMouse &
            MOUSE_INPUT_LEFT))
    {
        if (dragging)
        {
            dragging = false;


            // ======================================
            // 本当に動かした場合だけ
            // ターン終了
            // ======================================

            if (actuallyMoved)
            {
                playerMoved = true;

                playerTurnFinished =
                    true;
            }
            else
            {
                // 動かしていない
                // ターンは終了しない

                playerMoved = false;

                playerTurnFinished =
                    false;
            }


            dragBlock = -1;
        }
    }


    // ==============================================
    // マウス状態保存
    // ==============================================

    oldMouse =
        mouse;
}