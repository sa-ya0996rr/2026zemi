#pragma once
#ifndef PLAYER_H
#define PLAYER_H


#include "Common.h"


class Stage;
class Input;
class Camera;
class Image;



class Player
{

public:

    Player();

    ~Player();



    void Update(
        Stage& stage,
        Input& input
    );



    void Draw(
        Camera& camera,
        Image& image
    );



    Vector2 GetPosition();



    Rect GetRect();



private:


    Vector2 position;


    float velocityY;


    bool isGround;


    Direction direction;



    void Jump();



    void ApplyGravity();



};



#endif