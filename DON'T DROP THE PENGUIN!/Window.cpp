
#include "Window.h"
#include "DxLib.h"
#include <tchar.h>

// 背景画像を描画する
void Window::DrawBackground()
{
    // 背景画像を最初の1回だけ読み込む
    static int backgroundHandle = -1;
    static bool loaded = false;

    if (!loaded)
    {
        backgroundHandle = LoadGraph(_T("images/background.png"));
        loaded = true;
    }

    // 背景画像が読み込めた場合
    if (backgroundHandle != -1)
    {
        DrawExtendGraph(
            0, 0,
            800, 600,
            backgroundHandle,
            TRUE
        );
    }
    else
    {
        // 画像が読み込めない場合の背景
        DrawBox(
            0, 0,
            800, 600,
            GetColor(30, 80, 130),
            TRUE
        );
    }
}

// 半透明パネルを描画する
void Window::DrawPanel(
    int x1, int y1,
    int x2, int y2,
    int alpha)
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    // 暗い青色の半透明パネル
    DrawBox(
        x1, y1,
        x2, y2,
        GetColor(15, 45, 80),
        TRUE
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // パネルの枠線
    DrawBox(
        x1, y1,
        x2, y2,
        GetColor(150, 210, 240),
        FALSE
    );
}

// 文字を中央に描画する
void Window::Center(int y, const TCHAR* text, int color)
{
    // 文字列が存在しない場合は処理しない
    if (text == nullptr)
    {
        return;
    }

    // 文字列の幅を取得
    int w = GetDrawStringWidth(
        text,
        (int)_tcslen(text),
        24
    );

    // 画面中央に描画
    DrawString((800 - w) / 2, y, text, color);
}
// タイトル画面
void Window::DrawTitle(int select)
{
    // 背景
    DrawBackground();

    // タイトル部分のパネル
    DrawPanel(140, 40, 660, 235, 190);

    // メニュー部分のパネル
    DrawPanel(220, 280, 580, 470, 190);

    // タイトル文字
    Center(
        65,
        _T("DON'T DROP THE PENGUIN!"),
        GetColor(220, 245, 255)
    );

    Center(
        180,
        _T("PENGUIN ICE BATTLE"),
        GetColor(190, 225, 255)
    );

    // マウスカーソルの位置を取得
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    // 選択肢
    const TCHAR* options[] =
    {
        _T("2 PLAYER"),
        _T("CPU BATTLE"),
        _T("EXIT")
    };

    for (int i = 0; i < 3; i++)
    {
        int top = 300 + i * 55;
        int bottom = 345 + i * 55;

        // カーソルが選択肢の枠内にあるか確認
        bool mouseInside =
            mouseX >= 245 &&
            mouseX <= 555 &&
            mouseY >= top &&
            mouseY <= bottom;

        // selectが一致し、さらにカーソルが枠内にある場合だけ色を表示
        if (i == select && mouseInside)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);

            DrawBox(
                245,
                top,
                555,
                bottom,
                GetColor(80, 175, 225),
                TRUE
            );

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        // 選択肢の文字
        Center(
            305 + i * 55,
            options[i],
            GetColor(240, 250, 255)
        );
    }
}

// CPU難易度選択画面
void Window::DrawMode(int select)
{
    // 背景
    DrawBackground();

    // メニュー全体のパネル
    DrawPanel(220, 80, 580, 500, 200);

    Center(
        100,
        _T("CPU DIFFICULTY"),
        GetColor(220, 245, 255)
    );

    // マウスカーソルの位置を取得
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    const TCHAR* options[] =
    {
        _T("EASY"),
        _T("NORMAL"),
        _T("HARD")
    };

    for (int i = 0; i < 3; i++)
    {
        int top = 250 + i * 60;
        int bottom = 295 + i * 60;

        // カーソルが選択肢の枠内にあるか確認
        bool mouseInside =
            mouseX >= 250 &&
            mouseX <= 550 &&
            mouseY >= top &&
            mouseY <= bottom;

        // selectが一致し、さらにカーソルが枠内にある場合だけ色を表示
        if (i == select && mouseInside)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);

            DrawBox(
                250,
                top,
                550,
                bottom,
                GetColor(80, 175, 225),
                TRUE
            );

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        // 選択肢の文字
        Center(
            255 + i * 60,
            options[i],
            GetColor(240, 250, 255)
        );
    }

    Center(
        500,
        _T("SELECT CPU DIFFICULTY"),
        GetColor(190, 225, 255)
    );
}

// ゲーム画面の情報表示
void Window::DrawPlay(
    int current,
    Player players[2],
    int turn,
    ChargeImpact& charge)
{
    // 情報表示部分のパネル
    DrawPanel(0, 455, 800, 590, 190);

    // プレイヤーのターン
    DrawString(
        10, 510,
        current == 0
        ? _T("PLAYER 1 TURN")
        : _T("PLAYER 2 / CPU TURN"),
        GetColor(240, 250, 255)
    );

    // ターン数
    DrawFormatString(
        10, 535,
        GetColor(240, 250, 255),
        _T("TURN: %d"),
        turn + 1
    );

    // チケット情報
    DrawFormatString(
        10, 560,
        GetColor(240, 250, 255),
        _T("P1 TICKET: %s"),
        players[0].HasTicket()
        ? _T("YES")
        : _T("NO")
    );

    DrawFormatString(
        250, 560,
        GetColor(240, 250, 255),
        _T("P2 TICKET: %s"),
        players[1].HasTicket()
        ? _T("YES")
        : _T("NO")
    );

    // チャージ中
    if (charge.IsRunning())
    {
        DrawPanel(230, 460, 570, 530, 220);

        // チャージゲージの背景
        DrawBox(
            250, 500,
            550, 520,
            GetColor(40, 40, 40),
            TRUE
        );

        // チャージゲージ
        DrawBox(
            250, 500,
            250 + charge.GetGauge() / 3,
            520,
            GetColor(255, 220, 40),
            TRUE
        );

        DrawString(
            250, 475,
            _T("PRESS SPACE TO HIT!"),
            GetColor(240, 250, 255)
        );
    }
}

// チャンス氷選択画面
void Window::DrawChanceChoice()
{
    // 画面中央のパネル
    DrawPanel(120, 200, 680, 420, 230);

    Center(
        230,
        _T("CHANCE ICE!"),
        GetColor(255, 255, 255)
    );

    Center(
        280,
        _T("GET SKIP TICKET? ENTER = YES"),
        GetColor(230, 245, 255)
    );

    Center(
        330,
        _T("CLICK RIGHT SIDE = NO"),
        GetColor(230, 245, 255)
    );
}

// ゲームオーバー画面
void Window::DrawGameOver(int winner)
{
    // 背景
    DrawBackground();

    // メッセージ部分のパネル
    DrawPanel(140, 180, 660, 400, 210);

    if (winner == -2)
    {
        Center(
            260,
            _T("EXIT"),
            GetColor(255, 255, 255)
        );
    }
    else if (winner == 0)
    {
        Center(
            230,
            _T("PLAYER 1 WINS!"),
            GetColor(255, 255, 255)
        );
    }
    else
    {
        Center(
            230,
            _T("PLAYER 2 / CPU WINS!"),
            GetColor(255, 255, 255)
        );
    }

    Center(
        330,
        _T("PRESS ENTER TO RETURN TO TITLE"),
        GetColor(230, 245, 255)
    );
}
