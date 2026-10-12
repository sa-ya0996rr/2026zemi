#pragma once

const int WIDTH = 9;
const int HEIGHT = 10;

const int CELL = 60;
const int BOARD_X = 330;
const int BOARD_Y = 100;
const int MAX_BLOCK = 100;

extern char blockType[MAX_BLOCK];

extern int blockX[MAX_BLOCK];
extern int blockY[MAX_BLOCK];

extern int blockLength[MAX_BLOCK];

extern bool blockExist[MAX_BLOCK];

extern float blockDrawY[MAX_BLOCK];

extern bool falling[MAX_BLOCK];

extern bool rising[MAX_BLOCK];

extern bool newBlockRising;

extern int blockCount;

void AddBlock(char type, int x, int y, int length);
void MakeInitialBlocks();
bool IsOverlapping(int x1, int length1, int x2, int length2);
bool HasSupport(int number);
void StartRising();

/*
今の Block.h で使っているもの
今回先生に「どんなものを使っている？」と説明する場合、このファイルだけでも、

定数 → WIDTH, HEIGHT, CELL, BOARD_X, BOARD_Y, MAX_BLOCK
配列 → blockType[], blockX[] など
変数 → blockCount
関数 → AddBlock()など
char型 → 'S', 'C'など
int型 → 座標や長さなど
bool型 → 状態管理
float型 → 描画位置
extern → 他のcppファイルでも同じ変数を使う
const → 値を変更しないようにする

というものを使っている。
*/