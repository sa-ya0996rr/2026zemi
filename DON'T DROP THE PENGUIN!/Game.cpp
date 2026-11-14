
#include "Game.h"
#include <tchar.h>

// コンストラクタ
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
    mouseY(0),
    backgroundHandle(-1)
{}

// 終了処理
Game::~Game()
{
    penguins[0].Release();

    if (backgroundHandle != -1)
    {
        DeleteGraph(backgroundHandle);
        backgroundHandle = -1;
    }
}

// 初期化
void Game::Init()
{
    backgroundHandle = LoadGraph(_T("images/background.png"));

    ice.Init();

    penguins[0].Init();

    penguins[0].SetPosition(15);
}

// 試合開始
void Game::StartMatch()
{
    ice.Init();

    penguins[0].SetPosition(18);

    players[0].Reset();
    players[1].Reset();

    currentPlayer = 0;
    turn = 0;
    winner = -1;

    pendingChance = false;
    pendingTile = -1;

    scene = SCENE_PLAY;
}

// 更新
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
    {
        int mx, my;
        GetMousePoint(&mx, &my);

        // EnterキーはYES
        if (enter && !previousEnter)
        {
            // スキップ札を獲得
            players[currentPlayer].SetTicket(true);

            pendingChance = false;
            pendingTile = -1;

            // YESなのでターン交代
            currentPlayer = 1 - currentPlayer;
            turn++;

            scene = SCENE_PLAY;
        }
        // マウスクリック
        else if (mouse && !previousMouse)
        {
            // 左側をクリック：YES
            if (mx < 500)
            {
                players[currentPlayer].SetTicket(true);

                pendingChance = false;
                pendingTile = -1;

                currentPlayer = 1 - currentPlayer;
                turn++;

                scene = SCENE_PLAY;
            }
            // 右側をクリック：NO
            else
            {
                // スキップ札は獲得しない
                pendingChance = false;
                pendingTile = -1;

                // ターン交代しない
                // 同じプレイヤーが通常の氷を壊す
                scene = SCENE_PLAY;
            }
        }
        break;
    }

    case SCENE_CHANCE_MISS:
        // MISS画面から次のターンへ
        if ((enter && !previousEnter) ||
            (mouse && !previousMouse))
        {
            pendingChance = false;
            pendingTile = -1;

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

    if (CheckHitKey(KEY_INPUT_UP) && !previousUp)
    {
        menuSelect = (menuSelect + 2) % 3;
    }

    if (CheckHitKey(KEY_INPUT_DOWN) && !previousDown)
    {
        menuSelect = (menuSelect + 1) % 3;
    }

    for (int i = 0; i < 3; i++)
    {
        int top = 440 + i * 55;
        int bottom = 485 + i * 55;

        if (mx >= 350 && mx <= 650 &&
            my >= top && my <= bottom)
        {
            menuSelect = i;
        }
    }

    bool decide = enterClick;

    if (mouseClick)
    {
        int top = 440 + menuSelect * 55;
        int bottom = 485 + menuSelect * 55;

        if (mx >= 350 && mx <= 650 &&
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
            StartMatch();
        }
        else if (menuSelect == 1)
        {
            mode = 1;
            scene = SCENE_MODE;
            menuSelect = 0;
        }
    }
}

// CPU難易度選択
void Game::HandleMode()
{
    int mx, my;
    GetMousePoint(&mx, &my);

    int mouse = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    bool mouseClick = mouse && !previousMouse;
    bool enterClick = CheckHitKey(KEY_INPUT_RETURN) && !previousEnter;

    if (CheckHitKey(KEY_INPUT_UP) && !previousUp)
    {
        menuSelect = (menuSelect + 2) % 3;
    }

    if (CheckHitKey(KEY_INPUT_DOWN) && !previousDown)
    {
        menuSelect = (menuSelect + 1) % 3;
    }

    for (int i = 0; i < 3; i++)
    {
        int top = 350 + i * 60;
        int bottom = 395 + i * 60;

        if (mx >= 350 && mx <= 650 &&
            my >= top && my <= bottom)
        {
            menuSelect = i;
        }
    }

    bool decide = enterClick;

    if (mouseClick)
    {
        int top = 350 + menuSelect * 60;
        int bottom = 395 + menuSelect * 60;

        if (mx >= 350 && mx <= 650 &&
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
        }
        else if (menuSelect == 1)
        {
            cpuDifficulty = 1;
        }
        else
        {
            cpuDifficulty = 2;
        }

        StartMatch();
        menuSelect = 0;
    }
}

// ゲーム中
void Game::HandlePlay()
{
    cpuTurn = (mode == 1 && currentPlayer == 1);

    // CPUのターン
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

            if (CheckUnsafeLastLine())
            {
                return;
            }
        }

        int power = cpu.RollPower(cpuDifficulty);

        ice.ApplyDamage(tile, power);

        // 危険な最後の一列か判定
        if (CheckUnsafeLastLine())
        {
            return;
        }

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

    // 氷をクリックしてチャージ開始
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
    }

    // チャージ中
    if (charge.IsRunning())
    {
        if (CheckHitKey(KEY_INPUT_SPACE) && !previousSpace)
        {
            // チャンス氷の場合
            if (pendingTile >= 0 &&
                ice.IsChance(pendingTile))
            {
                int gauge = charge.StopGauge();

                HandleChanceResult(gauge);
            }
            // 通常氷の場合
            else
            {
                int power = charge.Stop();

                HandleChargeResult(power, power >= 100);
            }
        }
    }
    else
    {
        // チャンス氷の出現
        if (turn > 0 &&
            !ice.HasChanceIce() &&
            GetRand(99) < 20)
        {
            ice.SpawnChanceIce();
        }
    }
}

// 通常氷のチャージ結果
void Game::HandleChargeResult(int power, bool perfect)
{
    if (pendingTile < 0)
    {
        return;
    }

    ice.ApplyDamage(pendingTile, power);

    // 危険な最後の一列か判定
    if (CheckUnsafeLastLine())
    {
        pendingTile = -1;
        return;
    }

    ice.ResolveCollapse(penguins);

    effect.Trigger();

    CheckGameOver();

    if (scene != SCENE_GAMEOVER)
    {
        currentPlayer = 1 - currentPlayer;
        turn++;
    }

    pendingTile = -1;
}

// チャンス氷のチャージ結果
void Game::HandleChanceResult(int gauge)
{
    if (pendingTile < 0)
    {
        return;
    }

    // チャンス氷を消す
    ice.RemoveTile(pendingTile);

    // 危険な最後の一列か判定
    if (CheckUnsafeLastLine())
    {
        pendingTile = -1;
        return;
    }

    ice.ResolveCollapse(penguins);

    effect.Trigger();

    CheckGameOver();

    if (scene == SCENE_GAMEOVER)
    {
        pendingTile = -1;
        return;
    }

    // 95%以上ならYES / NOの選択画面へ
    if (gauge >= 950)
    {
        pendingChance = true;

        scene = SCENE_CHANCE_CHOICE;
    }
    // 95%未満ならMISS
    else
    {
        pendingChance = true;

        scene = SCENE_CHANCE_MISS;
    }
}

// ゲームオーバー判定
void Game::CheckGameOver()
{
    if (penguins[0].HasFallen(ice))
    {
        winner = 1 - currentPlayer;
        scene = SCENE_GAMEOVER;
    }
}

// 描画
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
        scene == SCENE_CHANCE_CHOICE ||
        scene == SCENE_CHANCE_MISS)
    {
        window.DrawBackground();

        ice.Draw(charge.IsRunning() ? pendingTile : -1);

        // ペンギン描画
        penguins[0].Draw(ice, 0);

        window.DrawPlay(
            currentPlayer,
            mode,
            players,
            turn,
            charge
        );

        if (scene == SCENE_CHANCE_CHOICE)
        {
            window.DrawChanceChoice();
        }

        if (scene == SCENE_CHANCE_MISS)
        {
            window.DrawChanceMiss();
        }
    }
    else
    {
        window.DrawGameOver(winner);
    }

    effect.Draw();
}
// 危険な最後の一列を判定
bool Game::CheckUnsafeLastLine()
{
    if (!ice.IsUnsafeLastLine())
    {
        return false;
    }

    // 危険な一列なので全氷を崩す
    ice.RemoveAllTiles();

    ice.ResolveCollapse(penguins);

    effect.Trigger();

    // ペンギン落下によるゲームオーバー判定
    CheckGameOver();

    return true;
}