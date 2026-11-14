
#pragma once

#include "DxLib.h"
#include "Ice.h"
#include "Penguin.h"
#include "Player.h"
#include "ChargeImpact.h"
#include "CPU.h"
#include "Item.h"
#include "Effect.h"
#include "Window.h"

enum GameScene
{
    SCENE_TITLE,
    SCENE_MODE,
    SCENE_PLAY,
    SCENE_CHANCE_CHOICE,
    SCENE_CHANCE_MISS,
    SCENE_GAMEOVER
};

class Game
{
public:
    Game();
    ~Game();

    void Init();
    void Update();
    void Draw();

private:
    GameScene scene;

    int mode;
    int cpuDifficulty;
    int currentPlayer;
    int menuSelect;
    int winner;
    int turn;

    bool cpuTurn;
    bool pendingChance;

    int pendingTile;

    int previousEnter;
    int previousUp;
    int previousDown;
    int previousSpace;
    int previousMouse;

    int mouseX;
    int mouseY;

    int backgroundHandle;

    Ice ice;
    Penguin penguins[2];
    Player players[2];

    ChargeImpact charge;
    CPU cpu;
    Item item;
    Effect effect;
    Window window;

    void StartMatch();

    void HandleTitle();
    void HandleMode();
    void HandlePlay();

    void HandleChargeResult(int power, bool perfect);
    void HandleChanceResult(int gauge);

    void CheckGameOver();

    bool CheckUnsafeLastLine();
};
