#include "Block.h" 

// ==================================================
// ブロックの変数
// ==================================================
char blockType[MAX_BLOCK];
/**/

int blockX[MAX_BLOCK];
int blockY[MAX_BLOCK];
/**/

int blockLength[MAX_BLOCK];
/**/

bool blockExist[MAX_BLOCK];
/**/

// ==================================================
// 描画位置
// ==================================================
float blockDrawY[MAX_BLOCK];
/**/

// ==================================================
// アニメーション
// ==================================================
bool falling[MAX_BLOCK];
bool rising[MAX_BLOCK];
/**/

bool newBlockRising = false;
/**/

// ==================================================
// ブロック数
// ==================================================
int blockCount = 0;
/**/

// ==================================================
// ブロック追加
// ==================================================
void AddBlock(
    char type, 
    int x, 
    int y, 
    int length)
    /**/
{
    if (blockCount >= MAX_BLOCK)
    {
        return;
    }
    /**/

    blockType[blockCount] = type;
    blockX[blockCount] = x;
    blockY[blockCount] = y;
    blockLength[blockCount] = length;
    blockExist[blockCount] = true;
    /**/

    // 描画位置
    blockDrawY[blockCount] = (float)(BOARD_Y + y * CELL);
    /**/
    
    falling[blockCount] = false;
    rising[blockCount] = false;
    
    blockCount++;
    /**/
}

// ==================================================
// 最初のブロック
// ==================================================
void MakeInitialBlocks()
/**/
{
    // ==========================================
    // 8段目
    // ==========================================
    AddBlock('S', 0, 8, 2);
    AddBlock('C', 3, 8, 3);
    AddBlock('S', 7, 8, 2);

    // ==========================================
    // 9段目
    // ==========================================
    AddBlock('C', 1, 9, 2);
    AddBlock('S', 4, 9, 3);
    AddBlock('C', 8, 9, 1);
}

// ==================================================
// 横に重なっているか
// ==================================================
bool IsOverlapping(
    int x1,
    int length1,
    int x2,
    int length2)
    /**/
{
    int left1 = x1;
    int right1 = x1 + length1 - 1;
    /**/

    int left2 = x2;
    int right2 = x2 + length2 - 1;

    if (right1 >= left2 && right2 >= left1)
    {
        return true;
    }
    /**/

    return false;
}

// ==================================================
// 下に支えがあるか
// ==================================================
bool HasSupport(int number)
{
    int belowY = blockY[number] + 1;
    /**/

    // ------------------------------------------
    // 一番下
    // ------------------------------------------
    if (belowY >= HEIGHT)
    {
        return true;
    }

    // ------------------------------------------
    // 他のブロック
    // ------------------------------------------
    for (int i = 0; i < blockCount; i++)
    {
        if (i == number)
        {
            continue;
        }
        if (blockExist[i] == false)
        {
            continue;
        }

        // 真下の段
        if (blockY[i] != belowY)
        {
            continue;
        }

        // 横に1マスでも重なっている
        if (IsOverlapping(
            blockX[number],
            blockLength[number],
            blockX[i],
            blockLength[i]))
        {
            return true;
        }
    }

    return false;
}

/*
このファイルで使っているものをまとめると
この Block.cpp では、今回話していたものの中からかなり多く使っている。

種類	    使っている場所	目的
if文	    AddBlock()など	条件を判定
for文	    HasSupport()	全ブロックを調べる
continue	HasSupport()	条件に合わないブロックを飛ばす
return	    各関数	        結果を返す・関数を終了
関数	    AddBlock()など	処理をまとめる
配列	    blockType[]など	複数のブロックを管理
変数	    blockCountなど	データを保存
定数	    MAX_BLOCKなど	変わらない値を管理
bool型	    blockExistなど	Yes / Noを管理
int型	    blockXなど	    整数を管理
float型	    blockDrawY	    小数を含む位置を管理
char型	    blockType	    S/Cなど1文字を管理

このBlock.cpp の一番大事な考え方

簡単に言うと、

① 配列でブロックの情報を保存する
↓
② 関数でブロックを追加・判定する
↓
③ if文で条件を判定する
↓
④ for文で全部のブロックを調べる
↓
⑤ boolで「ある・ない」「支えがある・ない」を返す

という構造になっている。

だから、このファイルは**「ブロックのデータを持っておいて、ブロックを追加したり、重なりや支えを判定したりする」**役割になっているよ。
*/