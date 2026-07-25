#pragma once
#ifndef IMAGE_H
#define IMAGE_H


#include <DxLib.h>



class Image
{

public:


    Image();


    ~Image();



    // “Ç‚İ‚İ

    bool Load();



    // ‰æ‘œæ“¾

    int GetPlayerImage();


    int GetEnemyImage();


    int GetTileImage();



private:


    // ƒnƒ“ƒhƒ‹

    int playerImage;

    int enemyImage;

    int tileImage;



    // ‰ğ•ú

    void Release();



};



#endif