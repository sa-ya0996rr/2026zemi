#include "Map.h"
#include "DxLib.h"

void Map::Init()
{
    roomImage = LoadGraph("Images/room.png");
}

void Map::Draw()
{
    DrawExtendGraph(
        0,
        0,
        1280,
        720,
        roomImage,
        TRUE
    );
}