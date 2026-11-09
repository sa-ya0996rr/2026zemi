#include "CPU.h"
#include "Ice.h"
#include "Penguin.h"
#include <tchar.h>

int CPU::ChooseTile(const Ice& ice, Penguin penguins[2], int difficulty)
{
    int list[TILE_COUNT], count = 0;
    for (int i = 0; i < TILE_COUNT; ++i)
        if (ice.IsTilePresent(i) && !penguins[0].IsOnTile(i, ice) && !penguins[1].IsOnTile(i, ice))
            list[count++] = i;
    if (!count) return -1;
    return list[GetRand(count - 1)];
}
int CPU::RollPower(int d)
{
    int r = GetRand(99);
    if (d == 0) { if (r < 5) return 100; if (r < 20) return 75; if (r < 55) return 50; return 20; }
    if (d == 1) { if (r < 20) return 100; if (r < 50) return 75; if (r < 80) return 50; return 20; }
    if (r < 40) return 100; if (r < 80) return 75; if (r < 95) return 50; return 20;
}
