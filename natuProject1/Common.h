#pragma once
#ifndef COMMON_H
#define COMMON_H


// =========================================
// 画面設定
// =========================================

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

constexpr int GAME_FPS = 60;


// =========================================
// プレイヤー設定
// =========================================

constexpr int PLAYER_WIDTH = 32;
constexpr int PLAYER_HEIGHT = 32;

constexpr float PLAYER_MOVE_SPEED = 5.0f;

constexpr float PLAYER_JUMP_POWER = -15.0f;

constexpr float GRAVITY = 1.0f;

constexpr float MAX_FALL_SPEED = 20.0f;


// =========================================
// タイル設定
// =========================================

constexpr int TILE_SIZE = 32;


// =========================================
// カメラ設定
// =========================================

constexpr float CAMERA_SPEED = 0.15f;


// =========================================
// 敵設定
// =========================================

constexpr int ENEMY_WIDTH = 32;
constexpr int ENEMY_HEIGHT = 32;

constexpr float ENEMY_SPEED = 2.0f;


// =========================================
// ベクトル
// =========================================

struct Vector2
{
    float x;
    float y;
};


// =========================================
// 矩形
// =========================================

struct Rect
{
    float left;
    float top;
    float right;
    float bottom;
};


// =========================================
// 向き
// =========================================

enum class Direction
{
    Left,
    Right
};


// =========================================
// ゲームシーン
// =========================================

enum class Scene
{
    Title,
    Game,
    Clear,
    GameOver
};


#endif