
#include "Ice.h"

#include <cstdlib>
#include <cmath>
#include <tchar.h>

// 氷画像の描画サイズ
const int ICE_IMAGE_SIZE = 84;

// 盤面の中央位置
const int ICE_CENTER_X = 500;
const int ICE_CENTER_Y = 400;

// 横方向・縦方向の配置間隔
const int ICE_SPACE_X = 63;
const int ICE_SPACE_Y = 63;

// 左から縦一列の氷の個数
// 1・4・5・6・5・6・5・4・1
const int COLUMN_LENGTHS[BOARD_COLS] =
{
    1, 4, 5, 6, 5, 6, 5, 4, 1
};

// 各列の開始インデックス
const int COLUMN_STARTS[BOARD_COLS] =
{
    0, 1, 5, 10, 16, 21, 27, 32, 36
};

// 隣接判定の縦方向の許容値
const int NEIGHBOR_Y_LIMIT = 32;


// コンストラクタ
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

        // 色をランダムに決定
        tiles[i].blue = (rand() % 2 == 0);
    }
}


// 初期化
void Ice::Init()
{
    iceWhiteHandle =
        LoadGraph(_T("images/ice_white.png"));

    iceBlueHandle =
        LoadGraph(_T("images/ice_blue.png"));

    iceWhiteCrackedHandle =
        LoadGraph(_T("images/ice_white_cracked.png"));

    iceBlueCrackedHandle =
        LoadGraph(_T("images/ice_blue_cracked.png"));

    for (int i = 0; i < TILE_COUNT; i++)
    {
        // 初期耐久値はすべて100
        tiles[i].durability = 100;

        tiles[i].exists = true;
        tiles[i].chance = false;

        // 氷の色をランダムに決定
        tiles[i].blue = (rand() % 2 == 0);
    }
}

// 氷の描画
void Ice::Draw(int selectedIndex)
{
    static int sparkleHandle =
        LoadGraph(_T("images/chance_sparkle.png"));

    // 2回に分けて描画
    // 1回目：選択されていない氷
    // 2回目：選択中の氷を最後に描画
    for (int pass = 0; pass < 2; pass++)
    {
        for (int i = 0; i < TILE_COUNT; i++)
        {
            if (!tiles[i].exists)
            {
                continue;
            }

            // 選択中の氷を最後に描画する
            if (selectedIndex >= 0)
            {
                if (pass == 0 && i == selectedIndex)
                {
                    continue;
                }

                if (pass == 1 && i != selectedIndex)
                {
                    continue;
                }
            }
            else
            {
                // 選択中の氷がない場合、2回目は描画しない
                if (pass == 1)
                {
                    continue;
                }
            }

            int centerX = TileCenterX(i);
            int centerY = TileCenterY(i);

            int imageSize = ICE_IMAGE_SIZE;

            // 選択中の氷を少し大きくする
            if (i == selectedIndex)
            {
                imageSize = 92;
            }

            int drawX = centerX - imageSize / 2;
            int drawY = centerY - imageSize / 2;

            int imageHandle = -1;

            // ひび割れ画像
            if (tiles[i].durability < 100)
            {
                if (tiles[i].blue)
                {
                    imageHandle = iceBlueCrackedHandle;
                }
                else
                {
                    imageHandle = iceWhiteCrackedHandle;
                }
            }
            // 通常画像
            else
            {
                if (tiles[i].blue)
                {
                    imageHandle = iceBlueHandle;
                }
                else
                {
                    imageHandle = iceWhiteHandle;
                }
            }

            // 氷画像の描画
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
                DrawCircle(
                    centerX,
                    centerY,
                    40,
                    GetColor(180, 230, 255),
                    TRUE
                );
            }

            // 耐久値表示
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

            // チャンス氷のキラキラ演出
            if (tiles[i].chance)
            {
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

            // 選択中の氷を黄色で囲む
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
}


// マウス位置から氷を判定
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

        int dx = abs(mx - centerX);
        int dy = abs(my - centerY);

        // 六角形の範囲判定
        if (dx <= 42 && dy <= 36)
        {
            if (dx + dy * 0.577 <= 42)
            {
                return i;
            }
        }
    }

    return -1;
}

// 氷にダメージを与える
void Ice::ApplyDamage(int index, int damage)
{
    // 番号が範囲外なら処理しない
    if (index < 0 || index >= TILE_COUNT)
    {
        return;
    }

    // すでに消えている氷なら処理しない
    if (!tiles[index].exists)
    {
        return;
    }

    // チャージインパクトの数値をダメージとして減算
    tiles[index].durability -= damage;

    // 現在耐久値が20以下になったら氷を崩す
    if (tiles[index].durability <= 20)
    {
        RemoveTile(index);
    }
}

// 氷を消す
void Ice::RemoveTile(int index)
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return;
    }

    tiles[index].exists = false;
    tiles[index].chance = false;
}


// 氷が消えているか
bool Ice::IsGone(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return true;
    }

    return !tiles[index].exists;
}


// チャンス氷か
bool Ice::IsChance(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return false;
    }

    return tiles[index].chance;
}


// チャンス氷が存在するか
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


// チャンス氷をランダムに生成
void Ice::SpawnChanceIce()
{
    if (HasChanceIce())
    {
        return;
    }

    int candidates[TILE_COUNT];
    int candidateCount = 0;

    for (int i = 0; i < TILE_COUNT; i++)
    {
        if (tiles[i].exists)
        {
            candidates[candidateCount] = i;
            candidateCount++;
        }
    }

    if (candidateCount == 0)
    {
        return;
    }

    int selected = candidates[rand() % candidateCount];

    tiles[selected].chance = true;
}


// 氷の列番号
int Ice::TileX(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return -1;
    }

    for (int x = 0; x < BOARD_COLS; x++)
    {
        int start = COLUMN_STARTS[x];
        int end = start + COLUMN_LENGTHS[x];

        if (index >= start && index < end)
        {
            return x;
        }
    }

    return -1;
}


// 氷の行番号
int Ice::TileY(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return -1;
    }

    for (int x = 0; x < BOARD_COLS; x++)
    {
        int start = COLUMN_STARTS[x];
        int end = start + COLUMN_LENGTHS[x];

        if (index >= start && index < end)
        {
            return index - start;
        }
    }

    return -1;
}


// 氷の中心X座標
int Ice::TileCenterX(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return 0;
    }

    int x = TileX(index);

    // 9列全体を中央に配置
    int startX =
        ICE_CENTER_X - (BOARD_COLS - 1) * ICE_SPACE_X / 2;

    return startX + x * ICE_SPACE_X;
}


// 氷の中心Y座標
int Ice::TileCenterY(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return 0;
    }

    int x = TileX(index);
    int y = TileY(index);

    int columnLength = COLUMN_LENGTHS[x];

    // 各列を中央に配置
    int startY =
        ICE_CENTER_Y - (columnLength - 1) * ICE_SPACE_Y / 2;

    return startY + y * ICE_SPACE_Y;
}


// 氷が存在するか
bool Ice::IsTilePresent(int index) const
{
    if (index < 0 || index >= TILE_COUNT)
    {
        return false;
    }

    return tiles[index].exists;
}


// 隣接する氷を取得
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

    // 同じ列の上
    if (y > 0)
    {
        out[count++] = index - 1;
    }

    // 同じ列の下
    if (y < COLUMN_LENGTHS[x] - 1)
    {
        out[count++] = index + 1;
    }

    // 左の列
    if (x > 0)
    {
        int targetX = x - 1;

        for (int i = 0; i < COLUMN_LENGTHS[targetX]; i++)
        {
            int candidate = COLUMN_STARTS[targetX] + i;

            int dy =
                abs(TileCenterY(index) - TileCenterY(candidate));

            if (dy <= NEIGHBOR_Y_LIMIT && count < 6)
            {
                out[count++] = candidate;
            }
        }
    }

    // 右の列
    if (x < BOARD_COLS - 1)
    {
        int targetX = x + 1;

        for (int i = 0; i < COLUMN_LENGTHS[targetX]; i++)
        {
            int candidate = COLUMN_STARTS[targetX] + i;

            int dy =
                abs(TileCenterY(index) - TileCenterY(candidate));

            if (dy <= NEIGHBOR_Y_LIMIT && count < 6)
            {
                out[count++] = candidate;
            }
        }
    }
}


// 下側の支えまで安定して繋がっているか
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

        int currentX = TileX(current);
        int currentY = TileY(current);

        // 最も下に位置する列の末端に到達したら安定
        if (COLUMN_LENGTHS[currentX] == 6 &&
            currentY == COLUMN_LENGTHS[currentX] - 1)
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

// 支えがなくなった氷を落下させる
void Ice::ResolveCollapse(Penguin penguins[2])
{
    // ペンギンの落下・敗北判定はGame側で行う
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

            // 支えまで繋がっていない氷を消す
            if (!HasStablePath(i))
            {
                RemoveTile(i);
                changed = true;
            }
        }
    }
}
// 最後の一列が危険な形か判定
bool Ice::IsUnsafeLastLine() const
{
    int selected[BOARD_COLS];

    // 各列に残っている氷を調べる
    for (int x = 0; x < BOARD_COLS; x++)
    {
        int count = 0;
        selected[x] = -1;

        int start = COLUMN_STARTS[x];
        int end = start + COLUMN_LENGTHS[x];

        for (int i = start; i < end; i++)
        {
            if (tiles[i].exists)
            {
                count++;
                selected[x] = i;
            }
        }

        // 各列に氷が1枚ずつ残っていなければ対象外
        if (count != 1)
        {
            return false;
        }
    }

    // ペンギンが乗っている氷が中央に残っているか
    if (!tiles[18].exists || selected[4] != 18)
    {
        return false;
    }

    // 隣の列同士がつながっているか確認
    for (int x = 0; x < BOARD_COLS - 1; x++)
    {
        int leftTile = selected[x];
        int rightTile = selected[x + 1];

        int dy = abs(
            TileCenterY(leftTile) -
            TileCenterY(rightTile)
        );

        // つながっていない場所があれば危険
        if (dy > NEIGHBOR_Y_LIMIT)
        {
            return true;
        }
    }

    // すべてつながっている場合は36通りの安全な形
    return false;
}


// 盤面の氷をすべて消す
void Ice::RemoveAllTiles()
{
    for (int i = 0; i < TILE_COUNT; i++)
    {
        if (tiles[i].exists)
        {
            RemoveTile(i);
        }
    }
}