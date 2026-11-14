
#include "Window.h"
#include "DxLib.h"
#include <tchar.h>

// 背景画像
void Window::DrawBackground()
{
    static int backgroundHandle = -1;
    static bool loaded = false;

    if (!loaded)
    {
        backgroundHandle = LoadGraph(_T("images/background.png"));
        loaded = true;
    }

    if (backgroundHandle != -1)
    {
        DrawExtendGraph(
            0, 0,
            1000, 800,
            backgroundHandle,
            TRUE
        );
    }
    else
    {
        DrawBox(
            0, 0,
            1000, 800,
            GetColor(30, 80, 130),
            TRUE
        );
    }
}

// 半透明パネル
void Window::DrawPanel(
    int x1, int y1,
    int x2, int y2,
    int alpha)
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    DrawBox(
        x1, y1,
        x2, y2,
        GetColor(15, 45, 80),
        TRUE
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawBox(
        x1, y1,
        x2, y2,
        GetColor(150, 210, 240),
        FALSE
    );
}

// 文字を画面中央に表示
void Window::Center(int y, const TCHAR* text, int color)
{
    if (text == nullptr)
    {
        return;
    }

    // 文字サイズを30に設定
    SetFontSize(30);

    int w = GetDrawStringWidth(
        text,
        (int)_tcslen(text),
        30
    );

    DrawString(
        (1000 - w) / 2,
        y,
        text,
        color
    );
}

// タイトル画面
void Window::DrawTitle(int select)
{
    DrawBackground();

    // タイトル部分のパネル
    DrawPanel(180, 150, 820, 370, 190);

    // メニュー部分のパネル
    DrawPanel(300, 410, 700, 590, 190);

    Center(
        250,
        _T("DON'T DROP THE PENGUIN!"),
        GetColor(220, 245, 255)
    );

    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    const TCHAR* options[] =
    {
        _T("2 PLAYER"),
        _T("CPU BATTLE"),
    };

    for (int i = 0; i < 2; i++)
    {
        int top = 435 + i * 70;     //  top　 = 枠の上の位置　枠を30ピクセル下に動かしたのなら440→470　450×-2
        int bottom = 500 + i * 70;  // bottom = 枠の下の位置　枠を30ピクセル下に動かしたのなら485→515　505×-2

        bool mouseInside =
            mouseX >= 350 &&
            mouseX <= 650 &&
            mouseY >= top &&
            mouseY <= bottom;

        if (i == select && mouseInside)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);  // 枠の透明度

            DrawBox(
                350, top,                   // 350→枠の左端　  top →枠の上端
                650, bottom,                // 650→枠の右端　bottom→枠の下端
                GetColor(80, 175, 225),     // 枠の色
                TRUE
            );

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        Center(
            455 + i * 65,// 455→文字の現在のY座標　数字上→文字下、数字下→文字上　i*55→2 PLAYERとCPU BATTLEの間隔
            options[i],
            GetColor(240, 250, 255)
        );
    }
}

// CPU難易度選択
void Window::DrawMode(int select)
{
    DrawBackground();

    DrawPanel(300, 260, 700, 550, 200);

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
        int top = 358 + i * 60;
        int bottom = 402 + i * 60;

        bool mouseInside =
            mouseX >= 350 &&
            mouseX <= 650 &&
            mouseY >= top &&
            mouseY <= bottom;

        if (i == select && mouseInside)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);

            DrawBox(
                350, top,
                650, bottom,
                GetColor(80, 175, 225),
                TRUE
            );

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        Center(
            365 + i * 60,
            options[i],
            GetColor(240, 250, 255)
        );
    }

    Center(
        300,// 590→文字の現在のY座標　数字上→文字下、数字下→文字上
        _T("SELECT CPU DIFFICULTY"),
        GetColor(190, 225, 255)
    );
}

// ゲーム画面UI
void Window::DrawPlay(
    int current,
    int mode,
    Player players[2],
    int turn,
    ChargeImpact& charge) 
{
    DrawPanel(0, 720, 1000, 790, 190);

    // ターン表示（左側）
    DrawString(
        30, 740,
        current == 0
        ? _T("PLAYER 1 TURN")
        : (mode == 0
            ? _T("PLAYER 2 TURN")
            : _T("CPU TURN")),
        GetColor(240, 250, 255)
    );

    // P1のターン
    if (current == 0)
    {
        // P1がチケットを持っているときだけ表示
        if (players[0].HasTicket())
        {
            DrawString(
                650, 740,
                _T("P1 TICKET: YES"),
                GetColor(240, 250, 255)
            );
        }
    }
    // P2 / CPUのターン
    else
    {
        // P2がチケットを持っているときだけ表示
        if (players[1].HasTicket())
        {
            DrawString(
                650, 740,
                _T("P2 TICKET: YES"),
                GetColor(240, 250, 255)
            );
        }
    }

    // チャージインパクト
    // チャージインパクトが実行中か確認する
    // 実行中の時だけ、ゲージや説明文字を表示する
    if (charge.IsRunning())
    {
        // チャージインパクトの背景パネル
        DrawPanel(150, 630, 850, 710, 220); // 左端のX座標,上側のY座標,右側のX座標,下端のY座標,パネルの透明度

        // ゲージの最大値
        const int MAX_GAUGE = 1000;

        // ゲージの中央位置
        const int CENTER_X = 500;

        // ゲージの最大幅（左右それぞれ）
        const int MAX_WIDTH = 320;

        // ゲージの高さ
        const int GAUGE_TOP = 680;
        const int GAUGE_BOTTOM = 700;

        // 現在のゲージ値を取得
        int gauge = charge.GetGauge();

        // ゲージの値を幅に変換
        int gaugeWidth = gauge * MAX_WIDTH / MAX_GAUGE;

        // ゲージの背景（黒）
        DrawBox(
            CENTER_X - MAX_WIDTH,
            GAUGE_TOP,
            CENTER_X + MAX_WIDTH,
            GAUGE_BOTTOM,
            GetColor(40, 40, 40),// 黒に近い灰色
            TRUE                 // 塗りつぶし
        );

        // --------------------------------
        // ゲージの色を設定
        // 白 → 黄色 → 橙色 → 赤色
        // --------------------------------

        int red = 255;
        int green = 255;
        int blue = 255;

        // 50％以下：白から黄色へ
        if (gauge <= 500)
        {
            red = 255;
            green = 255 - (35 * gauge / 500);
            blue = 255 - (215 * gauge / 500);
        }
        // 50％～70％：黄色から橙色へ
        else if (gauge <= 700)
        {
            int rate = gauge - 500;

            red = 255;
            green = 220 - (90 * rate / 200);
            blue = 40 - (40 * rate / 200);
        }
        // 70％～100％：橙色から赤色へ
        else
        {
            int rate = gauge - 700;

            red = 255;
            green = 130 - (130 * rate / 300);
            blue = 0;
        }

        // ゲージの色
        int gaugeColor = GetColor(red, green, blue);

        // 中央から左右に広がるゲージ
        DrawBox(
            CENTER_X - gaugeWidth,
            GAUGE_TOP,
            CENTER_X + gaugeWidth,
            GAUGE_BOTTOM,
            gaugeColor,
            TRUE
        );

        const TCHAR* text = _T("PRESS SPACE TO HIT!");

        int textWidth = GetDrawStringWidth(
            text,
            (int)_tcslen(text),
            30
        );

        // 説明文字
        DrawString(
            (1000 - textWidth) / 2,
            645,
            text,
            GetColor(240, 250, 255) // 白に近い色
        );
    }
}

// チャンス氷の選択画面
void Window::DrawChanceChoice()
{
    DrawPanel(180, 250, 820, 550, 230);

    Center(
        290,
        _T("CHANCE ICE!"),
        GetColor(255, 255, 255)
    );

    Center(
        360,
        _T("ENTER = GET SKIP TICKET"),
        GetColor(230, 245, 255)
    );

    Center(
        430,
        _T("CLICK RIGHT SIDE = NO"),
        GetColor(230, 245, 255)
    );
}

// ゲームオーバー
void Window::DrawGameOver(int winner)
{
    DrawBackground();

    DrawPanel(180, 250, 820, 550, 210);

    if (winner == -2)
    {
        Center(
            330,
            _T("EXIT"),
            GetColor(255, 255, 255)
        );
    }
    else if (winner == 0)
    {
        Center(
            320,
            _T("PLAYER 1 WINS!"),
            GetColor(255, 255, 255)
        );
    }
    else
    {
        Center(
            320,
            _T("PLAYER 2 / CPU WINS!"),
            GetColor(255, 255, 255)
        );
    }

    Center(
        430,
        _T("PRESS ENTER TO RETURN TO TITLE"),
        GetColor(230, 245, 255)
    );
}
