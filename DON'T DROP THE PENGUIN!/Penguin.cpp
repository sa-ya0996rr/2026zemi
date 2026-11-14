
#include "Penguin.h"
#include "Ice.h"
#include <tchar.h>

Penguin::Penguin()
    : tileIndex(-1),
    imageHandle(-1)
{}

// ‰æ‘œ‚Ì“Ç‚İ‚İ
bool Penguin::Init()
{
    imageHandle = LoadGraph(_T("images/penguin.png"));

    //printfDx(_T("Penguin Image Handle: %d\n"), imageHandle);

    //if (imageHandle == -1)
    //{
    //    printfDx(_T("Penguin image load failed!\n"));
    //    return false;
    //}

    //printfDx(_T("Penguin image load success!\n"));

    return true;
}

// ‰æ‘œ‚Ì‰ğ•ú
void Penguin::Release()
{
    if (imageHandle != -1)
    {
        DeleteGraph(imageHandle);
        imageHandle = -1;
    }
}

// ƒyƒ“ƒMƒ“‚ÌˆÊ’u‚ğİ’è
void Penguin::SetPosition(int index)
{
    tileIndex = index;
}

// ƒyƒ“ƒMƒ“‚ªw’è‚³‚ê‚½•X‚Ìã‚É‚¢‚é‚©”»’è
bool Penguin::IsOnTile(int index, const Ice& ice) const
{
    if (HasFallen(ice))
    {
        return false;
    }

    return tileIndex == index;
}

// ƒyƒ“ƒMƒ“‚ª—‰º‚µ‚Ä‚¢‚é‚©”»’è
bool Penguin::HasFallen(const Ice& ice) const
{
    if (tileIndex < 0 || tileIndex >= TILE_COUNT)
    {
        return true;
    }

    return !ice.IsTilePresent(tileIndex);
}

// ƒyƒ“ƒMƒ“‚ğ•`‰æ
void Penguin::Draw(const Ice& ice, int player) const
{
    // ƒyƒ“ƒMƒ“‚ª—‰º‚µ‚Ä‚¢‚éê‡‚Í•`‰æ‚µ‚È‚¢
    if (HasFallen(ice))
    {
        return;
    }

    // ‰æ‘œ‚ª“Ç‚İ‚Ü‚ê‚Ä‚¢‚È‚¢ê‡‚Í•`‰æ‚µ‚È‚¢
    if (imageHandle == -1)
    {
        return;
    }

    // ƒ^ƒCƒ‹‚Ì’†SÀ•W
    int cx = ice.TileCenterX(tileIndex);
    int cy = ice.TileCenterY(tileIndex);

    // ƒyƒ“ƒMƒ“‰æ‘œ‚Ì•`‰æƒTƒCƒY
    int width = 60;
    int height = 70;

    // •`‰æˆÊ’u
    int left = cx - width / 2;
    int top = cy - height + 25;
    int right = left + width;
    int bottom = top + height;

    // ƒyƒ“ƒMƒ“‰æ‘œ‚ğ•`‰æ
    DrawExtendGraph(
        left,
        top,
        right,
        bottom,
        imageHandle,
        TRUE
    );
}
