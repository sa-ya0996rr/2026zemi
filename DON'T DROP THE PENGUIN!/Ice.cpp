
#include "Ice.h"

#include <cstdlib>
#include <cmath>
#include <tchar.h>

// •X‚Ì‰æ‘œƒTƒCƒY
const int ICE_IMAGE_SIZE = 84;

// •X‚Ì”z’uˆÊ’ui1000~800‰æ–Ê‚Ì’†‰›Šñ‚èj
const int ICE_START_X = 225;
const int ICE_START_Y = 125;

// •X“¯m‚ÌŠÔŠu
const int ICE_SPACE_X = 85;
const int ICE_SPACE_Y = 75;

// s‚²‚Æ‚Ì‰¡ˆÊ’u’²®
const int ROW_OFFSET_X[BOARD_ROWS] =
{
    0, 3, -2, 2, -3, 1
};

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Ice::Ice()
    : iceWhiteHandle(-1),
    iceBlueHandle(-1),
    iceWhiteCrackedHandle(-1),
    iceBlueCrackedHandle(-1)
{
    for (int i = 0; i < TILE_COUNT; i++)
    {
        tiles[i].durability = 100;
        tiles[i].exists = true;
        tiles[i].chance = false;
    }
}

// ‰Šú‰»
void Ice::Init()
{
    // •X‰æ‘œ‚ğ“Ç‚İ‚Ş
    iceWhiteHandle = LoadGraph(_T("images/ice_white.png"));
    iceBlueHandle = LoadGraph(_T("images/ice_blue.png"));

    // ‚Ğ‚ÑŠ„‚ê‚½•X‰æ‘œ‚ğ“Ç‚İ‚Ş
    iceWhiteCrackedHandle =
        LoadGraph(_T("images/ice_white_cracked.png"));

    iceBlueCrackedHandle =
        LoadGraph(_T("images/ice_blue_cracked.png"));

    // •X‚Ìó‘Ô‚ğ‰Šú‰»
    for (int i = 0; i < TILE_COUNT; i++)
    {
        tiles[i].durability = 100;
        tiles[i].exists = true;
        tiles[i].chance = false;
    }
}

// •X‚Ì•`‰æ
void Ice::Draw(int selectedIndex)
{
    for (int i = 0; i < TILE_COUNT; i++)
    {
        // ‘¶İ‚µ‚È‚¢•X‚Í•`‰æ‚µ‚È‚¢
        if (!tiles[i].exists)
        {
            continue;
        }

        int centerX = TileCenterX(i);
        int centerY = TileCenterY(i);

        int imageSize = ICE_IMAGE_SIZE;

        // ‘I‘ğ’†‚Ì•X‚Í­‚µ‘å‚«‚­‚·‚é
        if (i == selectedIndex)
        {
            imageSize = 92;
        }

        int drawX = centerX - imageSize / 2;
        int drawY = centerY - imageSize / 2;

        // ”’‚¢•X‚ÆÂ‚¢•X‚ğŒğŒİ‚Ég‚¤
        int imageHandle;

        // ‘Ï‹v’l‚ª80ˆÈ‰º‚È‚ç‚Ğ‚ÑŠ„‚ê‰æ‘œ
        if (tiles[i].durability <= 80)
        {
            if (i % 2 == 0)
            {
                imageHandle = iceBlueCrackedHandle;
            }
            else
            {
                imageHandle = iceWhiteCrackedHandle;
            }
        }
        // ‘Ï‹v’l‚ª100‚È‚ç’Êí‰æ‘œ
        else
        {
            if (i % 2 == 0)
            {
                imageHandle = iceBlueHandle;
            }
            else
            {
                imageHandle = iceWhiteHandle;
            }
        }

        // •X‚Ì‰æ‘œ‚ğ•`‰æ
        if (imageHandle != -1)
        {
            DrawExtendGraph(
                drawX,
                drawY,
                drawX + imageSize,
                drawY + imageSize,
                imageHandle,
                TRUE
            );
        }
        else
        {
            // ‰æ‘œ‚ª“Ç‚İ‚ß‚È‚©‚Á‚½ê‡‚Ì‰¼•\¦
            DrawCircle(
                centerX,
                centerY,
                40,
                GetColor(180, 230, 255),
                TRUE
            );
        }

        // ‘Ï‹v’l‚ª100–¢–‚Ìê‡‚É•\¦
        if (tiles[i].durability < 100)
        {
            DrawFormatString(
                centerX - 15,
                centerY - 10,
                GetColor(20, 50, 80),
                _T("%d"),
                tiles[i].durability
            );
        }

        // ƒ`ƒƒƒ“ƒX•X‚ÌƒLƒ‰ƒLƒ‰‰‰o
        if (tiles[i].chance)
        {
            // ƒLƒ‰ƒLƒ‰‰æ‘œ‚ğ“Ç‚İ‚Şi‰‰ñ‚Ì‚İj
            static int sparkleHandle =
                LoadGraph(_T("images/chance_sparkle.png"));

            // ƒLƒ‰ƒLƒ‰‰æ‘œ‚ğ•X‚Ì’†‰›‚É•`‰æ
            if (sparkleHandle != -1)
            {
                DrawExtendGraph(
                    centerX - 36,
                    centerY - 36,
                    centerX + 36,
                    centerY + 36,
                    sparkleHandle,
                    TRUE
                );
            }
        }

        // ‘I‘ğ’†‚Ì•X‚ğˆÍ‚Ş
        if (i == selectedIndex)
        {
            DrawCircle(
                centerX,
                centerY,
                44,
                GetColor(255, 230, 50),
                FALSE
            );
        }
    }
}

// ƒ}ƒEƒXˆÊ’u‚©‚ç•X‚ğ”»’è
int Ice::HitTest(int mx, int my) const
{
    for (int i = 0; i < TILE_COUNT; i++)
    {
        if (!tiles[i].exists)
        {
            continue;
        }

        int centerX = TileCenterX(i);
        int centerY = TileCenterY(i);

        int dx = mx - centerX;
        int dy = my - centerY;

        // •X‚Ì”ÍˆÍ“à‚©”»’è
        if (abs(dx) <= 42 && abs(dy) <= 42)
        {
            if (abs(dx) + abs(dy) * 0.6 <= 42)
            {
                return i;
            }
        }
    }

    return -1;
}

// •X‚Éƒ_ƒ[ƒW‚ğ—^‚¦‚é
void Ice::ApplyDamage(int index, int damage)
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return;
    }

    if (!tiles[index].exists)
    {
        return;
    }

    tiles[index].durability -= damage;

    // ‘Ï‹v’l‚ª80–¢–‚É‚È‚é‚Æ•X‚ªÁ‚¦‚é
    if (tiles[index].durability < 80)
    {
        RemoveTile(index);
    }
}

// •X‚ğÁ‚·
void Ice::RemoveTile(int index)
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return;
    }

    tiles[index].exists = false;
    tiles[index].chance = false;
}

// •X‚ªÁ‚¦‚Ä‚¢‚é‚©
bool Ice::IsGone(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return true;
    }

    return !tiles[index].exists;
}

// ƒ`ƒƒƒ“ƒX•X‚©
bool Ice::IsChance(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return false;
    }

    return tiles[index].chance;
}

// ƒ`ƒƒƒ“ƒX•X‚ª‘¶İ‚·‚é‚©
bool Ice::HasChanceIce() const
{
    for (int i = 0; i < TILE_COUNT; i++)
    {
        if (tiles[i].exists && tiles[i].chance)
        {
            return true;
        }
    }

    return false;
}

// ƒ`ƒƒƒ“ƒX•X‚ğƒ‰ƒ“ƒ_ƒ€‚É¶¬
void Ice::SpawnChanceIce()
{
    // Šù‚Éƒ`ƒƒƒ“ƒX•X‚ª‚ ‚éê‡‚Í¶¬‚µ‚È‚¢
    if (HasChanceIce())
    {
        return;
    }

    int candidates[TILE_COUNT];
    int candidateCount = 0;

    // ‘¶İ‚·‚é•X‚ğŒó•â‚É‚·‚é
    for (int i = 0; i < TILE_COUNT; i++)
    {
        if (tiles[i].exists)
        {
            candidates[candidateCount] = i;
            candidateCount++;
        }
    }

    // Œó•â‚ª‚È‚¢ê‡
    if (candidateCount == 0)
    {
        return;
    }

    // Œó•â‚©‚çƒ‰ƒ“ƒ_ƒ€‚É‘I‚Ô
    int selected = candidates[rand() % candidateCount];

    tiles[selected].chance = true;
}

// •X‚Ì—ñ”Ô†
int Ice::TileX(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return -1;
    }

    return index % BOARD_COLS;
}

// •X‚Ìs”Ô†
int Ice::TileY(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return -1;
    }

    return index / BOARD_COLS;
}

// •X‚Ì’†SXÀ•W
int Ice::TileCenterX(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return 0;
    }

    int x = TileX(index);
    int y = TileY(index);

    return ICE_START_X
        + x * ICE_SPACE_X
        + (y % 2) * 42
        + ROW_OFFSET_X[y];
}

// •X‚Ì’†SYÀ•W
int Ice::TileCenterY(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return 0;
    }

    int y = TileY(index);

    return ICE_START_Y + y * ICE_SPACE_Y;
}

// •X‚ª‘¶İ‚·‚é‚©
bool Ice::IsTilePresent(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return false;
    }

    return tiles[index].exists;
}

// —×Ú‚·‚é•X‚ğæ“¾
void Ice::GetNeighbors(
    int index,
    int out[6],
    int& count
) const
{
    count = 0;

    if (index < 0 || index >= TILE_COUNT)
    {
        return;
    }

    int x = TileX(index);
    int y = TileY(index);

    // “¯‚¶s‚Ì¶‰E
    if (x > 0)
    {
        out[count++] = index - 1;
    }

    if (x < BOARD_COLS - 1)
    {
        out[count++] = index + 1;
    }

    // ã‰º‚Ìs
    if (y > 0)
    {
        if (y % 2 == 0)
        {
            // ‹ô”s
            if (x > 0)
            {
                out[count++] = index - BOARD_COLS - 1;
            }

            out[count++] = index - BOARD_COLS;
        }
        else
        {
            // Šï”s
            out[count++] = index - BOARD_COLS;

            if (x < BOARD_COLS - 1)
            {
                out[count++] = index - BOARD_COLS + 1;
            }
        }
    }

    if (y < BOARD_ROWS - 1)
    {
        if (y % 2 == 0)
        {
            // ‹ô”s
            if (x > 0)
            {
                out[count++] = index + BOARD_COLS - 1;
            }

            out[count++] = index + BOARD_COLS;
        }
        else
        {
            // Šï”s
            out[count++] = index + BOARD_COLS;

            if (x < BOARD_COLS - 1)
            {
                out[count++] = index + BOARD_COLS + 1;
            }
        }
    }
}

// ‰º‚Ìs‚Ü‚ÅˆÀ’è‚µ‚ÄŒq‚ª‚Á‚Ä‚¢‚é‚©
bool Ice::HasStablePath(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return false;
    }

    if (!tiles[index].exists)
    {
        return false;
    }

    bool visited[TILE_COUNT] = {};

    int queue[TILE_COUNT];
    int head = 0;
    int tail = 0;

    queue[tail++] = index;
    visited[index] = true;

    while (head < tail)
    {
        int current = queue[head++];

        // Å‰º’i‚É“’B‚µ‚½‚çˆÀ’è
        if (TileY(current) == BOARD_ROWS - 1)
        {
            return true;
        }

        int neighbors[6];
        int count = 0;

        GetNeighbors(current, neighbors, count);

        for (int i = 0; i < count; i++)
        {
            int next = neighbors[i];

            if (next < 0 || next >= TILE_COUNT)
            {
                continue;
            }

            if (!tiles[next].exists)
            {
                continue;
            }

            if (visited[next])
            {
                continue;
            }

            visited[next] = true;
            queue[tail++] = next;
        }
    }

    return false;
}

// x‚¦‚ª‚È‚­‚È‚Á‚½•X‚ğ—‰º‚³‚¹‚é
void Ice::ResolveCollapse(Penguin penguins[2])
{
    // penguins‚Ì—‰ºE”s–k”»’è‚ÍGame‘¤‚Ås‚¤
    (void)penguins;

    bool changed = true;

    while (changed)
    {
        changed = false;

        for (int i = 0; i < TILE_COUNT; i++)
        {
            if (!tiles[i].exists)
            {
                continue;
            }

            // ‰º‚Ì’i‚Ü‚ÅŒq‚ª‚Á‚Ä‚¢‚È‚¢•X‚ğÁ‚·
            if (!HasStablePath(i))
            {
                RemoveTile(i);
                changed = true;
            }
        }
    }
}
