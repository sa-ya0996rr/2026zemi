
#include "Ice.h"
#include <cstdlib>
#include <tchar.h>

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Ice::Ice()
{
    Init();
}

// •X‚Ì‰Šú‰»
void Ice::Init()
{
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        // ‘S‚Ä‚Ì•X‚ğ‘¶İ‚³‚¹‚é
        tiles[i].exists = true;

        // ‘Ï‹v’l‚ğ100‚Éİ’è
        tiles[i].durability = 100;

        // ƒ`ƒƒƒ“ƒX•X‚Å‚Í‚È‚¢ó‘Ô‚É‚·‚é
        tiles[i].chance = false;
    }
}

// •X‚ÌX•ûŒü‚ÌˆÊ’u‚ğæ“¾‚·‚é
int Ice::TileX(int i) const
{
    return i % BOARD_COLS;
}

// •X‚ÌY•ûŒü‚ÌˆÊ’u‚ğæ“¾‚·‚é
int Ice::TileY(int i) const
{
    return i / BOARD_COLS;
}

// •X‚Ì’†SXÀ•W‚ğæ“¾‚·‚é
int Ice::TileCenterX(int i) const
{
    int x = TileX(i);
    int y = TileY(i);

    // Še’i‚Ì‰¡•ûŒü‚Ì‚¸‚ê
    const int rowOffsetX[BOARD_ROWS] =
    {
        0, 3, -2, 2, -3, 1
    };

    // ˜ZŠpŒ`‚ğ–§W‚³‚¹‚È‚ª‚çA”z’u‚ğ­‚µ•s‹K‘¥‚É‚·‚é
    return 105 + x * 85 + (y % 2) * 42 + rowOffsetX[y];
}

// •X‚Ì’†SYÀ•W‚ğæ“¾‚·‚é
int Ice::TileCenterY(int i) const
{
    int y = TileY(i);

    // ‚‚³‚Í‘S‚Ä“ˆê‚·‚é
    return 95 + y * 75;
}

// •X‚ª‘¶İ‚·‚é‚©
bool Ice::IsTilePresent(int i) const
{
    return i >= 0 && i < TILE_COUNT && tiles[i].exists;
}

// •X‚ªÁ‚¦‚Ä‚¢‚é‚©
bool Ice::IsGone(int i) const
{
    return !IsTilePresent(i);
}

// ƒ`ƒƒƒ“ƒX•X‚©
bool Ice::IsChance(int i) const
{
    return i >= 0 && i < TILE_COUNT && tiles[i].chance;
}

// •X‚Ì“y•U‚ÆŠC‚ğ•`‰æ‚·‚é
void Ice::Draw(int selectedIndex)
{
    // ŠC‚Ì”wŒi
    DrawBox(0, 0, 800, 600, GetColor(8, 55, 105), TRUE);
    DrawBox(0, 300, 800, 600, GetColor(5, 35, 85), TRUE);

    // ŠC–Ê‚Ì”g
    for (int i = 0; i < 18; ++i)
    {
        int waveX = (i * 73 + 19) % 780;
        int waveY = 35 + (i * 47 + 11) % 465;

        DrawLine(
            waveX, waveY,
            waveX + 24, waveY,
            GetColor(45, 125, 190)
        );

        DrawLine(
            waveX + 8, waveY + 3,
            waveX + 30, waveY + 3,
            GetColor(20, 90, 155)
        );
    }

    const int radius = 42;
    const int sideDepth = 105;

    int DrawExtendGraph;

    // •X‚Ì‘¤–Ê‚ğ•`‰æ‚·‚é
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        if (!tiles[i].exists)
        {
            continue;
        }

        // •X‚Ì’†SÀ•W
        int cx = TileCenterX(i);
        int cy = TileCenterY(i);

        // ˜ZŠpŒ`‚Ì‰º‘¤‚ÌÀ•W
        int x1 = cx - radius / 2;
        int y1 = cy + radius * 3 / 4;

        int x2 = cx + radius / 2;
        int y2 = cy + radius * 3 / 4;

        int x3 = cx + radius;
        int y3 = cy;

        int x4 = cx - radius;
        int y4 = cy;

        // ³–Ê‘¤
        DrawTriangle(
            x1, y1,
            x2, y2,
            x2, y2 + sideDepth,
            GetColor(35, 145, 210), TRUE
        );

        DrawTriangle(
            x1, y1,
            x2, y2 + sideDepth,
            x1, y1 + sideDepth,
            GetColor(25, 115, 185), TRUE
        );

        // ¶‘¤–Ê
        DrawTriangle(
            x4, y4,
            x1, y1,
            x1, y1 + sideDepth,
            GetColor(40, 165, 220), TRUE
        );

        DrawTriangle(
            x4, y4,
            x1, y1 + sideDepth,
            x4, y4 + sideDepth,
            GetColor(25, 125, 195), TRUE
        );

        // ‰E‘¤–Ê
        DrawTriangle(
            x2, y2,
            x3, y3,
            x3, y3 + sideDepth,
            GetColor(30, 135, 205), TRUE
        );

        DrawTriangle(
            x2, y2,
            x3, y3 + sideDepth,
            x2, y2 + sideDepth,
            GetColor(20, 105, 175), TRUE
        );
    }

    // •X‚Ìã–Ê‚ğ•`‰æ‚·‚é
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        if (!tiles[i].exists)
        {
            continue;
        }

        // •X‚Ì’†SÀ•W
        int cx = TileCenterX(i);
        int cy = TileCenterY(i);

        // ˜ZŠpŒ`‚Ì’¸“_À•W
        int vx[6] =
        {
            cx,
            cx + radius,
            cx + radius,
            cx,
            cx - radius,
            cx - radius
        };

        int vy[6] =
        {
            cy - radius,
            cy - radius / 2,
            cy + radius / 2,
            cy + radius,
            cy + radius / 2,
            cy - radius / 2
        };

        // •X‚ÌF‚ÍÂF‚Æ…F‚É‹ß‚¢”’F‚Ì2F‚É“ˆê
        // ƒ`ƒƒƒ“ƒX•X‚àF‚Í•ÏX‚µ‚È‚¢
        int topColor;

        if (i % 2 == 0)
        {
            topColor = GetColor(155, 225, 250);
        }
        else
        {
            topColor = GetColor(220, 245, 255);
        }

        // •X‚Ì—ÖŠsF
        int edgeColor = GetColor(65, 155, 210);

        // ‘I‘ğ’†‚Ì•X‚Í—ÖŠs‚ğ–¾‚é‚­‚·‚é
        if (i == selectedIndex)
        {
            edgeColor = GetColor(255, 255, 255);
        }

        // ˜ZŠpŒ`‚Ìã–Ê‚ğ•`‰æ
        for (int k = 0; k < 6; ++k)
        {
            int next = (k + 1) % 6;

            DrawTriangle(
                cx, cy,
                vx[k], vy[k],
                vx[next], vy[next],
                topColor, TRUE
            );
        }

        // ˜ZŠpŒ`‚Ì—ÖŠs‚ğ•`‰æ
        for (int k = 0; k < 6; ++k)
        {
            int next = (k + 1) % 6;

            DrawLine(
                vx[k], vy[k],
                vx[next], vy[next],
                edgeColor
            );
        }

        // ‘Ï‹v’l‚ğ•\¦
        if (tiles[i].durability < 100)
        {
            TCHAR str[16];

            _stprintf_s(
                str,
                _T("%d"),
                tiles[i].durability
            );

            DrawString(
                cx - 12,
                cy - 8,
                str,
                GetColor(20, 50, 80)
            );
        }

        // ƒ`ƒƒƒ“ƒX•X‚ÌƒLƒ‰ƒLƒ‰‰‰o
        // •X–{‘Ì‚ÌF‚Í•ÏX‚µ‚È‚¢
        if (tiles[i].chance)
        {
            int sparkleColor = GetColor(255, 255, 255);

            // ’†‰›‚ÌƒLƒ‰ƒLƒ‰
            DrawLine(
                cx - 8, cy - 28,
                cx + 8, cy - 28,
                sparkleColor
            );

            DrawLine(
                cx, cy - 36,
                cx, cy - 20,
                sparkleColor
            );

            // Î‚ß‚ÌŒõ
            DrawLine(
                cx - 5, cy - 33,
                cx + 5, cy - 23,
                sparkleColor
            );

            DrawLine(
                cx + 5, cy - 33,
                cx - 5, cy - 23,
                sparkleColor
            );

            // ¬‚³‚ÈƒLƒ‰ƒLƒ‰
            DrawLine(
                cx + 20, cy - 12,
                cx + 28, cy - 12,
                sparkleColor
            );

            DrawLine(
                cx + 24, cy - 16,
                cx + 24, cy - 8,
                sparkleColor
            );
        }
    }
}

// ƒ}ƒEƒX‚Å‘I‘ğ‚³‚ê‚½•X‚Ì”Ô†‚ğæ“¾‚·‚é
int Ice::HitTest(int mx, int my) const
{
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        // ‘¶İ‚µ‚È‚¢•X‚Í‘I‘ğ‚Å‚«‚È‚¢
        if (!tiles[i].exists)
        {
            continue;
        }

        // •X‚Ì’†SÀ•W
        int cx = TileCenterX(i);
        int cy = TileCenterY(i);

        // ƒ}ƒEƒX‚Æ•X‚Ì’†S‚Æ‚Ì‹——£
        int dx = mx - cx;
        int dy = my - cy;

        // ‹——£‚Ìâ‘Î’l
        int absX = abs(dx);
        int absY = abs(dy);

        const int radius = 42;

        // ˜ZŠpŒ`‚Ì“à‘¤‚©”»’è
        bool inside = false;

        if (absY <= radius / 2)
        {
            if (absX <= radius)
            {
                inside = true;
            }
        }
        else if (absY <= radius)
        {
            int maxX = 2 * (radius - absY);

            if (absX <= maxX)
            {
                inside = true;
            }
        }

        // •X‚Ì“à‘¤‚È‚ç”Ô†‚ğ•Ô‚·
        if (inside)
        {
            return i;
        }
    }

    return -1;
}

// w’è‚µ‚½•X‚Éƒ_ƒ[ƒW‚ğ—^‚¦‚é
void Ice::ApplyDamage(int i, int damage)
{
    if (!IsTilePresent(i))
    {
        return;
    }

    // ‘Ï‹v’l‚ğŒ¸‚ç‚·
    tiles[i].durability -= damage;

    // ‘Ï‹v’l‚ª80–¢–‚É‚È‚Á‚½‚ç•X‚ğÁ‚·
    if (tiles[i].durability < 80)
    {
        tiles[i].exists = false;
    }
}

// w’è‚³‚ê‚½•X‚ğæ‚èœ‚­
void Ice::RemoveTile(int i)
{
    if (i >= 0 && i < TILE_COUNT)
    {
        tiles[i].exists = false;
    }
}

// ƒ`ƒƒƒ“ƒX•X‚ª‘¶İ‚·‚é‚©
bool Ice::HasChanceIce() const
{
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        if (tiles[i].exists && tiles[i].chance)
        {
            return true;
        }
    }

    return false;
}

// ƒ`ƒƒƒ“ƒX•X‚ğƒ‰ƒ“ƒ_ƒ€‚ÉoŒ»‚³‚¹‚é
void Ice::SpawnChanceIce()
{
    // ”z’u‚Å‚«‚é•X‚ÌŒó•â
    int candidates[TILE_COUNT];
    int count = 0;

    // ‘¶İ‚·‚é’Êí‚Ì•X‚ğ’T‚·
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        if (tiles[i].exists && !tiles[i].chance)
        {
            candidates[count] = i;
            count++;
        }
    }

    // ”z’u‚Å‚«‚é•X‚ª‚ ‚ê‚Îƒ‰ƒ“ƒ_ƒ€‚É‘I‚Ô
    if (count > 0)
    {
        int randomIndex = GetRand(count - 1);

        tiles[candidates[randomIndex]].chance = true;
    }
}

// —×Ú‚·‚é•X‚Ì”Ô†‚ğæ“¾‚·‚é
void Ice::GetNeighbors(int i, int out[6], int& count) const
{
    count = 0;

    // Œ»İ‚Ì•X‚ÌˆÊ’u
    int x = TileX(i);
    int y = TileY(i);

    // ‹ô”s‚Ì—×ÚÀ•W
    const int dxEven[6] = { -1, 1, -1, 0, -1, 0 };
    const int dyEven[6] = { 0, 0, -1, -1, 1, 1 };

    // Šï”s‚Ì—×ÚÀ•W
    const int dxOdd[6] = { -1, 1, 0, 1, 0, 1 };
    const int dyOdd[6] = { 0, 0, -1, -1, 1, 1 };

    // üˆÍ6•ûŒü‚ğ’²‚×‚é
    for (int k = 0; k < 6; ++k)
    {
        // nx‚Æny‚ğif‚ÌŠO‚ÅéŒ¾‚·‚é
        int nx;
        int ny;

        if (y % 2 == 1)
        {
            nx = x + dxOdd[k];
            ny = y + dyOdd[k];
        }
        else
        {
            nx = x + dxEven[k];
            ny = y + dyEven[k];
        }

        // ”Õ–Ê‚Ì”ÍˆÍ“à‚È‚ç“o˜^‚·‚é
        if (nx >= 0 && nx < BOARD_COLS &&
            ny >= 0 && ny < BOARD_ROWS)
        {
            out[count] = ny * BOARD_COLS + nx;
            count++;
        }
    }
}

// •X‚ªÅ‰º’i‚Ü‚ÅŒq‚ª‚Á‚Ä‚¢‚é‚©’²‚×‚é
bool Ice::HasStablePath(int i) const
{
    // ’²¸Ï‚İ‚Ì•X
    bool visited[TILE_COUNT] = {};

    // ’²¸—p‚ÌƒLƒ…[
    int queue[TILE_COUNT];

    int head = 0;
    int tail = 0;

    // ’²¸ŠJn’n“_
    queue[tail] = i;
    tail++;

    visited[i] = true;

    // ƒLƒ…[‚É•X‚ªc‚Á‚Ä‚¢‚éŠÔ’²‚×‚é
    while (head < tail)
    {
        int cur = queue[head];
        head++;

        // Å‰º’i‚Ü‚ÅŒq‚ª‚Á‚Ä‚¢‚ê‚ÎˆÀ’è
        if (TileY(cur) == BOARD_ROWS - 1)
        {
            return true;
        }

        // —×Ú‚·‚é•X‚ğæ“¾
        int n[6];
        int count = 0;

        GetNeighbors(cur, n, count);

        // —×Ú‚·‚é•X‚ğ’²‚×‚é
        for (int k = 0; k < count; ++k)
        {
            int next = n[k];

            if (tiles[next].exists && !visited[next])
            {
                visited[next] = true;

                queue[tail] = next;
                tail++;
            }
        }
    }

    // Å‰º’i‚ÉŒq‚ª‚Á‚Ä‚¢‚È‚¢
    return false;
}

// x‚¦‚ª‚È‚­‚È‚Á‚½•X‚ğ•ö—‚³‚¹‚é
void Ice::ResolveCollapse(Penguin penguins[2])
{
    // penguins‚ÍGame‘¤‚Å—‰º”»’è‚Ég—p‚·‚é
    (void)penguins;

    // •ö—‚·‚é•X‚ğ‹L˜^‚·‚é
    bool fall[TILE_COUNT] = {};

    // ‘S‚Ä‚Ì•X‚ÌˆÀ’è«‚ğ’²‚×‚é
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        if (tiles[i].exists && !HasStablePath(i))
        {
            fall[i] = true;
        }
    }

    // •ö—‚·‚é•X‚ğæ‚èœ‚­
    for (int i = 0; i < TILE_COUNT; ++i)
    {
        if (fall[i])
        {
            tiles[i].exists = false;
        }
    }
}
