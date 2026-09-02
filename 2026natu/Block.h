#pragma once

const int WIDTH = 9;
const int HEIGHT = 10;
const int CELL = 60;
const int BOARD_X = 280;
const int BOARD_Y = 50;

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