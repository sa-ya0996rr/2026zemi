
#pragma once

#include "DxLib.h"
#include "Penguin.h"

// 37–‡‚Ì˜ZŠpŒ`”Õ–Ê
const int BOARD_COLS = 9;
const int BOARD_ROWS = 7;
const int TILE_COUNT = 37;

struct IceTile
{
    int durability;
    bool exists;
    bool chance;

    // •X‚ÌFitrue = ÂAfalse = ”’j
    bool blue;
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

    // ÅŒã‚Ìˆê—ñ‚ªŠëŒ¯‚ÈŒ`‚©”»’è
    bool IsUnsafeLastLine() const;

    // ”Õ–Ê‚Ì•X‚ğ‚·‚×‚ÄÁ‚·
    void RemoveAllTiles(); 

private:
    IceTile tiles[TILE_COUNT];

    int iceWhiteHandle;
    int iceBlueHandle;

    int iceWhiteCrackedHandle;
    int iceBlueCrackedHandle;

    bool HasStablePath(int index) const;

    void GetNeighbors(
        int index,
        int out[6],
        int& count
    ) const;
};
