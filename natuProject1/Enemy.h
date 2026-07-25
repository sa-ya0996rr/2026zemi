#pragma once
#ifndef ENEMY_H
#define ENEMY_H


#include "Common.h"


class Camera;
class Image;



class Enemy
{

public:


    Enemy(
        float x,
        float y
    );


    ~Enemy();



    void Update();



    void Draw(
        Camera& camera,
        Image& image
    );



    Vector2 GetPosition();



    Rect GetRect();



    bool IsAlive();


    void Dead();



private:


    Vector2 position;


    float speed;


    Direction direction;



    int width;

    int height;



    bool alive;


};



#endif