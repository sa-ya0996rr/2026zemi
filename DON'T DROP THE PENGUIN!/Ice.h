#pragma once

#include "DxLib.h"
#include "Penguin.h"

const int BOARD_COLS = 7;
const int BOARD_ROWS = 6;
const int TILE_COUNT = BOARD_COLS * BOARD_ROWS;

struct IceTile
{
    int durability;
    bool exists;
    bool chance;
};

class Ice
{
public:
    Ice();

    void Init();
    void Draw(int selectedIndex = -1);

    int HitTest(int mx, int my) const;

    void ApplyDamage(int index, int damage);
    void RemoveTile(int index);

    bool IsGone(int index) const;
    bool IsChance(int index) const;
    bool HasChanceIce() const;

    void SpawnChanceIce();

    void ResolveCollapse(Penguin penguins[2]);

    int TileX(int index) const;
    int TileY(int index) const;

    int TileCenterX(int index) const;
    int TileCenterY(int index) const;

    bool IsTilePresent(int index) const;

private:
    IceTile tiles[TILE_COUNT];

    // í èÌÇÃïXâÊëú
    int iceWhiteHandle;
    int iceBlueHandle;

    // Ç–Ç—äÑÇÍÇΩïXâÊëú
    int iceWhiteCrackedHandle;
    int iceBlueCrackedHandle;

    bool HasStablePath(int index) const;

    void GetNeighbors(
        int index,
        int out[6],
        int& count
    ) const;
};