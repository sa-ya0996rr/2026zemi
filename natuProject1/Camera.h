#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include "Common.h"



class Camera
{

public:


    Camera();


    ~Camera();



    void Update(
        Vector2 playerPosition
    );



    void SetStageSize(
        int width,
        int height
    );



    int GetScreenX(
        float worldX
    );


    int GetScreenY(
        float worldY
    );



    Vector2 GetPosition();



private:


    Vector2 position;



    float minX;

    float maxX;


    float minY;

    float maxY;


};



#endif