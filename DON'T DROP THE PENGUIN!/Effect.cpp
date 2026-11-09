#include "Effect.h"
#include "DxLib.h"
#include <tchar.h>

Effect::Effect() : frames(0) {}
void Effect::Trigger() { frames = 20; }
void Effect::Draw()
{
    if (frames > 0) { DrawString(10, 10, _T("ICE CRACK!"), GetColor(255, 255, 255)); frames--; }
}
