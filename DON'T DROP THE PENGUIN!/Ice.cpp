#include "Ice.h"
#include <tchar.h>

Ice::Ice() { Init(); }

void Ice::Init()
{
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        tiles[i].durability = 100;
        tiles[i].exists = true;
        tiles[i].chance = false;
    }
}

int Ice::TileX(int i) const { return i % BOARD_COLS; }
int Ice::TileY(int i) const { return i / BOARD_COLS; }
int Ice::TileCenterX(int i) const { return 105 + TileX(i) * 85 + (TileY(i) % 2) * 42; }
int Ice::TileCenterY(int i) const { return 95 + TileY(i) * 75; }
bool Ice::IsTilePresent(int i) const { return i >= 0 && i < TILE_COUNT && tiles[i].exists; }
bool Ice::IsGone(int i) const { return !IsTilePresent(i); }
bool Ice::IsChance(int i) const { return i >= 0 && i < TILE_COUNT && tiles[i].chance; }

void Ice::Draw()
{
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        if (!tiles[i].exists) continue;
        int x = TileCenterX(i), y = TileCenterY(i);
        int color = tiles[i].chance ? GetColor(180, 245, 255) : GetColor(130, 210, 245);
        DrawCircle(x, y, 38, color, TRUE);
        DrawCircle(x, y, 38, GetColor(40, 100, 150), FALSE);
        if (tiles[i].durability < 100)
        {
            TCHAR str[16];

            _stprintf_s(
                str,
                _T("%d"),
                tiles[i].durability
            );

            DrawString(
                x - 12,
                y - 8,
                str,
                GetColor(20, 50, 80)
            );
        }
        if (tiles[i].chance) DrawString(x - 8, y - 28, _T("*"), GetColor(255, 220, 60));
    }
}

int Ice::HitTest(int mx, int my) const
{
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        if (!tiles[i].exists) continue;
        int dx = mx - TileCenterX(i), dy = my - TileCenterY(i);
        if (dx * dx + dy * dy <= 38 * 38) return i;
    }
    return -1;
}

void Ice::ApplyDamage(int i, int damage)
{
    if (!IsTilePresent(i)) return;
    tiles[i].durability -= damage;
    if (tiles[i].durability < 80) tiles[i].exists = false;
}

void Ice::RemoveTile(int i)
{
    if (i >= 0 && i < TILE_COUNT) tiles[i].exists = false;
}

bool Ice::HasChanceIce() const
{
    for (int i = 0; i < TILE_COUNT; ++i)
        if (tiles[i].exists && tiles[i].chance) return true;
    return false;
}

void Ice::SpawnChanceIce()
{
    int candidates[TILE_COUNT], count = 0;
    for (int i = 0; i < TILE_COUNT; ++i)
        if (tiles[i].exists && !tiles[i].chance) candidates[count++] = i;
    if (count > 0) tiles[candidates[GetRand(count - 1)]].chance = true;
}

void Ice::GetNeighbors(int i, int out[6], int& count) const
{
    count = 0;
    int x = TileX(i), y = TileY(i);
    const int dxEven[6] = { -1, 1, -1, 0, -1, 0 };
    const int dyEven[6] = { 0, 0, -1, -1, 1, 1 };
    const int dxOdd[6] = { -1, 1, 0, 1, 0, 1 };
    const int dyOdd[6] = { 0, 0, -1, -1, 1, 1 };
    for (int k = 0; k < 6; ++k)
    {
        int nx = x + (y % 2 ? dxOdd[k] : dxEven[k]);
        int ny = y + (y % 2 ? dyOdd[k] : dyEven[k]);
        if (nx >= 0 && nx < BOARD_COLS && ny >= 0 && ny < BOARD_ROWS)
            out[count++] = ny * BOARD_COLS + nx;
    }
}

bool Ice::HasStablePath(int i) const
{
    // ‰º’[‚Ì—ñ‚É“ž’B‚Å‚«‚é•X‚ðŽx‚¦‚ ‚è‚Æ‚·‚é
    bool visited[TILE_COUNT] = {};
    int queue[TILE_COUNT], head = 0, tail = 0;
    queue[tail++] = i; visited[i] = true;
    while (head < tail)
    {
        int cur = queue[head++];
        if (TileY(cur) == BOARD_ROWS - 1) return true;
        int n[6], count; GetNeighbors(cur, n, count);
        for (int k = 0; k < count; ++k)
            if (tiles[n[k]].exists && !visited[n[k]])
            {
                visited[n[k]] = true; queue[tail++] = n[k];
            }
    }
    return false;
}

void Ice::ResolveCollapse(Penguin penguins[2])
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        bool fall[TILE_COUNT] = {};
        for (int i = 0; i < TILE_COUNT; ++i)
            if (tiles[i].exists && !HasStablePath(i)) fall[i] = true;
        for (int i = 0; i < TILE_COUNT; ++i)
            if (fall[i]) { tiles[i].exists = false; changed = true; }
    }
}
