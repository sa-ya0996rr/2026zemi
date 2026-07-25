#pragma once
#ifndef STAGE_H
#define STAGE_H


#include "Common.h"


class Camera;
class Image;



class Stage
{

public:


    Stage();


    ~Stage();



    void Update();



    void Draw(
        Camera& camera,
        Image& image
    );



    bool CheckCollision(
        Rect rect
    );



private:


    static constexpr int MAP_WIDTH = 40;

    static constexpr int MAP_HEIGHT = 20;



    int map[MAP_HEIGHT][MAP_WIDTH];



    void DrawTile(
        int x,
        int y,
        Camera& camera,
        Image& image
    );



};



#endif