
#include "Game.h"
#include <tchar.h>

Game::Game()
    : scene(SCENE_TITLE),
    mode(0),
    cpuDifficulty(1),
    currentPlayer(0),
    menuSelect(0),
    winner(-1),
    turn(0),
    cpuTurn(false),
    pendingChance(false),
    pendingTile(-1),
    previousEnter(0),
    previousUp(0),
    previousDown(0),
    previousSpace(0),
    previousMouse(0),
    mouseX(0),
    mouseY(0)
{
}

void Game::Init()
{
    ice.Init();
    penguins[0].SetPosition(2, 5);
    penguins[1].SetPosition(4, 5);
}

void Game::StartMatch()
{
    ice.Init();

    penguins[0].SetPosition(2, 5);
    penguins[1].SetPosition(4, 5);

    players[0].Reset();
    players[1].Reset();

    currentPlayer = 0;
    turn = 0;
    winner = -1;
    pendingChance = false;

    scene = SCENE_PLAY;
}

void Game::Update()
{
    int enter = CheckHitKey(KEY_INPUT_RETURN);
    int up = CheckHitKey(KEY_INPUT_UP);
    int down = CheckHitKey(KEY_INPUT_DOWN);
    int space = CheckHitKey(KEY_INPUT_SPACE);

    int mouse = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    switch (scene)
    {
    case SCENE_TITLE:
        HandleTitle();
        break;

    case SCENE_MODE:
        HandleMode();
        break;

    case SCENE_PLAY:
        HandlePlay();
        break;

    case SCENE_CHANCE_CHOICE:
        if ((enter && !previousEnter) ||
            (mouse && !previousMouse))
        {
            int mx, my;
            GetMousePoint(&mx, &my);

            if (enter || mx < 400)
            {
                players[currentPlayer].SetTicket(true);
            }

            pendingChance = false;
            currentPlayer = 1 - currentPlayer;
            turn++;

            scene = SCENE_PLAY;
        }
        break;

    case SCENE_GAMEOVER:
        if (enter && !previousEnter)
        {
            scene = SCENE_TITLE;
            menuSelect = 0;
        }
        break;
    }

    previousEnter = enter;
    previousUp = up;
    previousDown = down;
    previousSpace = space;
    previousMouse = mouse;
}

// タイトル画面
void Game::HandleTitle()
{
    int mx, my;
    GetMousePoint(&mx, &my);

    int mouse = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
    bool mouseClick = mouse && !previousMouse;
    bool enterClick = CheckHitKey(KEY_INPUT_RETURN) && !previousEnter;

    // キーボード操作
    if (CheckHitKey(KEY_INPUT_UP) && !previousUp)
    {
        menuSelect = (menuSelect + 2) % 3;
    }

    if (CheckHitKey(KEY_INPUT_DOWN) && !previousDown)
    {
        menuSelect = (menuSelect + 1) % 3;
    }

    // マウスの位置で選択項目を変更
    for (int i = 0; i < 3; i++)
    {
        int top = 300 + i * 55;
        int bottom = 345 + i * 55;

        if (mx >= 245 && mx <= 555 &&
            my >= top && my <= bottom)
        {
            menuSelect = i;
        }
    }

    // マウスクリックまたはEnterで決定
    bool decide = enterClick;

    if (mouseClick)
    {
        int top = 300 + menuSelect * 55;
        int bottom = 345 + menuSelect * 55;

        if (mx >= 245 && mx <= 555 &&
            my >= top && my <= bottom)
        {
            decide = true;
        }
    }

    if (decide)
    {
        if (menuSelect == 0)
        {
            mode = 0;
            scene = SCENE_MODE;
            menuSelect = 0;
        }
        else if (menuSelect == 1)
        {
            mode = 1;
            scene = SCENE_MODE;
            menuSelect = 0;
        }
        else
        {
            scene = SCENE_GAMEOVER;
            winner = -2;
        }
    }
}

// CPUモード選択画面
void Game::HandleMode()
{
    int mx, my;
    GetMousePoint(&mx, &my);

    int mouse = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
    bool mouseClick = mouse && !previousMouse;
    bool enterClick = CheckHitKey(KEY_INPUT_RETURN) && !previousEnter;

    // キーボード操作
    if (CheckHitKey(KEY_INPUT_UP) && !previousUp)
    {
        menuSelect = (menuSelect + 2) % 3;
    }

    if (CheckHitKey(KEY_INPUT_DOWN) && !previousDown)
    {
        menuSelect = (menuSelect + 1) % 3;
    }

    // マウスの位置で選択項目を変更
    for (int i = 0; i < 3; i++)
    {
        int top = 250 + i * 60;
        int bottom = 295 + i * 60;

        if (mx >= 250 && mx <= 550 &&
            my >= top && my <= bottom)
        {
            menuSelect = i;
        }
    }

    // マウスクリックまたはEnterで決定
    bool decide = enterClick;

    if (mouseClick)
    {
        int top = 250 + menuSelect * 60;
        int bottom = 295 + menuSelect * 60;

        if (mx >= 250 && mx <= 550 &&
            my >= top && my <= bottom)
        {
            decide = true;
        }
    }

    if (decide)
    {
        if (menuSelect == 0)
        {
            cpuDifficulty = 0;
            StartMatch();
        }
        else if (menuSelect == 1)
        {
            cpuDifficulty = 1;
            StartMatch();
        }
        else
        {
            cpuDifficulty = 2;
            StartMatch();
        }

        menuSelect = 0;
    }
}

void Game::HandlePlay()
{
    cpuTurn = (mode == 1 && currentPlayer == 1);

    if (cpuTurn)
    {
        int tile = cpu.ChooseTile(ice, penguins, cpuDifficulty);

        if (tile < 0)
        {
            winner = 0;
            scene = SCENE_GAMEOVER;
            return;
        }

        if (ice.IsChance(tile))
        {
            ice.RemoveTile(tile);
            players[1].SetTicket(true);
        }

        int power = cpu.RollPower(cpuDifficulty);

        ice.ApplyDamage(tile, power);
        ice.ResolveCollapse(penguins);

        effect.Trigger();

        CheckGameOver();

        if (scene != SCENE_GAMEOVER)
        {
            currentPlayer = 0;
            turn++;
        }

        return;
    }

    int mx, my;
    GetMousePoint(&mx, &my);

    int tile = ice.HitTest(mx, my);

    if ((GetMouseInput() & MOUSE_INPUT_LEFT) &&
        !previousMouse &&
        tile >= 0)
    {
        if (penguins[0].IsOnTile(tile, ice) ||
            penguins[1].IsOnTile(tile, ice))
        {
            return;
        }

        pendingTile = tile;
        charge.Start(false);
        scene = SCENE_PLAY;
    }

    if (charge.IsRunning())
    {
        if (CheckHitKey(KEY_INPUT_SPACE) && !previousSpace)
        {
            int power = charge.Stop();
            HandleChargeResult(power, power >= 100);
        }
    }
    else
    {
        if (turn > 0 &&
            !ice.HasChanceIce() &&
            GetRand(99) < 20)
        {
            ice.SpawnChanceIce();
        }
    }
}

void Game::HandleChargeResult(int power, bool perfect)
{
    if (pendingTile < 0)
    {
        return;
    }

    bool chance = ice.IsChance(pendingTile);

    ice.ApplyDamage(pendingTile, power);
    ice.ResolveCollapse(penguins);

    effect.Trigger();

    if (chance && ice.IsGone(pendingTile))
    {
        pendingChance = true;
        scene = SCENE_CHANCE_CHOICE;
        return;
    }

    CheckGameOver();

    if (scene != SCENE_GAMEOVER)
    {
        currentPlayer = 1 - currentPlayer;
        turn++;
    }

    pendingTile = -1;
}

void Game::CheckGameOver()
{
    if (penguins[0].HasFallen(ice))
    {
        winner = 1;
        scene = SCENE_GAMEOVER;
    }
    else if (penguins[1].HasFallen(ice))
    {
        winner = 0;
        scene = SCENE_GAMEOVER;
    }
}

void Game::Draw()
{
    if (scene == SCENE_TITLE)
    {
        window.DrawTitle(menuSelect);
    }
    else if (scene == SCENE_MODE)
    {
        window.DrawMode(menuSelect);
    }
    else if (scene == SCENE_PLAY ||
        scene == SCENE_CHANCE_CHOICE)
    {
        ice.Draw();

        penguins[0].Draw(ice, 0);
        penguins[1].Draw(ice, 1);

        window.DrawPlay(currentPlayer, players, turn, charge);

        if (scene == SCENE_CHANCE_CHOICE)
        {
            window.DrawChanceChoice();
        }
    }
    else
    {
        window.DrawGameOver(winner);
    }

    effect.Draw();
}
