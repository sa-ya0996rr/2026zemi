#pragma once

#include "Block.h"


// ==================================================
// ƒvƒŒƒCƒ„[î•ñ
// ==================================================

extern bool dragging;

extern int dragBlock;

extern bool playerMoved;

extern bool playerTurnFinished;


// ==================================================
// ŠÖ”
// ==================================================

void UpdatePlayer();

int GetBlockFromMouse(
    int mouseX,
    int mouseY);

bool CanMoveBlock(
    int number,
    int newX);

void MoveBlockByMouse(
    int number);