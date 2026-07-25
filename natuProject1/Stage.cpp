#include "Stage.h"

#include "Camera.h"
#include "Image.h"

#include <DxLib.h>



// =========================================
// コンストラクタ
// =========================================

Stage::Stage()
{


    for (int y = 0; y < MAP_HEIGHT; y++)
    {

        for (int x = 0; x < MAP_WIDTH; x++)
        {

            map[y][x] = 0;

        }

    }



    // 地面

    for (int x = 0; x < MAP_WIDTH; x++)
    {

        map[18][x] = 1;

        map[19][x] = 1;

    }



    // 足場

    map[15][5] = 1;
    map[15][6] = 1;
    map[15][7] = 1;



    map[12][10] = 1;
    map[12][11] = 1;
    map[12][12] = 1;


}



// =========================================
// デストラクタ
// =========================================

Stage::~Stage()
{

}



// =========================================
// 更新
// =========================================

void Stage::Update()
{

}



// =========================================
// 描画
// =========================================

void Stage::Draw(
    Camera& camera,
    Image& image
)
{

    for (int y = 0; y < MAP_HEIGHT; y++)
    {

        for (int x = 0; x < MAP_WIDTH; x++)
        {

            if (map[y][x] == 1)
            {

                DrawTile(
                    x,
                    y,
                    camera,
                    image
                );

            }

        }

    }

}



// =========================================
// タイル描画
// =========================================

void Stage::DrawTile(
    int x,
    int y,
    Camera& camera,
    Image& image
)
{

    int worldX =
        x * TILE_SIZE;


    int worldY =
        y * TILE_SIZE;



    int handle =
        image.GetTileImage();



    if (handle == -1)
    {

        DrawBox(

            camera.GetScreenX(worldX),

            camera.GetScreenY(worldY),


            camera.GetScreenX(
                worldX + TILE_SIZE
            ),

            camera.GetScreenY(
                worldY + TILE_SIZE
            ),


            GetColor(0, 200, 0),

            TRUE

        );


        return;

    }



    DrawGraph(

        camera.GetScreenX(worldX),

        camera.GetScreenY(worldY),


        handle,

        TRUE

    );


}



// =========================================
// 当たり判定
// =========================================

bool Stage::CheckCollision(
    Rect rect
)
{


    int left =
        (int)(rect.left / TILE_SIZE);


    int right =
        (int)(rect.right / TILE_SIZE);



    int top =
        (int)(rect.top / TILE_SIZE);


    int bottom =
        (int)(rect.bottom / TILE_SIZE);





    for (int y = top; y <= bottom; y++)
    {

        for (int x = left; x <= right; x++)
        {

            if (x < 0 ||
                x >= MAP_WIDTH ||
                y < 0 ||
                y >= MAP_HEIGHT)
            {
                continue;
            }



            if (map[y][x] == 1)
            {

                return true;

            }


        }

    }



    return false;

}