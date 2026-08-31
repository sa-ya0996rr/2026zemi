#pragma once


// ==================================================
// マップ
// ==================================================

const int WIDTH = 9;

const int HEIGHT = 10;

const int CELL = 70;

const int BOARD_X = 280;

const int BOARD_Y = 50;


// ==================================================
// ブロック
// ==================================================

const int MAX_BLOCK = 100;


extern char blockType[MAX_BLOCK];

extern int blockX[MAX_BLOCK];

extern int blockY[MAX_BLOCK];

extern int blockLength[MAX_BLOCK];

extern bool blockExist[MAX_BLOCK];


// ==================================================
// 描画位置
// ==================================================

extern float blockDrawY[MAX_BLOCK];


// ==================================================
// アニメーション
// ==================================================

extern bool falling[MAX_BLOCK];

extern bool rising[MAX_BLOCK];

extern bool newBlockRising;


// ==================================================
// ブロック数
// ==================================================

extern int blockCount;


// ==================================================
// 関数
// ==================================================

void AddBlock(
    char type,
    int x,
    int y,
    int length);


void MakeInitialBlocks();


bool IsOverlapping(
    int x1,
    int length1,
    int x2,
    int length2);


bool HasSupport(
    int number);


void StartRising();