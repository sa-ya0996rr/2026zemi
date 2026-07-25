#pragma once
#ifndef GAME_H
#define GAME_H


#include "Player.h"
#include "Stage.h"
#include "Camera.h"
#include "Enemy.h"
#include "Image.h"
#include "Input.h"



class Game
{

public:


    Game();


    ~Game();



    void Update();


    void Draw();



private:


    Player player;


    Stage stage;


    Camera camera;


    Enemy enemy;


    Image image;


    Input input;



    // “G‚Æ‚ÌÚG”»’è

    bool CheckEnemyCollision();



};



#endif