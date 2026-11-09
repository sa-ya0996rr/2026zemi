
#include "Window.h"
#include "DxLib.h"
#include <tchar.h>

void Window::Center(int y, const TCHAR* text, int color)
{
    int w = GetDrawStringWidth(text, (int)_tcslen(text), 24);
    DrawString((800 - w) / 2, y, text, color);
}


void Window::DrawTitle(int select)
{

    // タイトル文字
    Center(65, _T("DON'T DROP THE PENGUIN!"),
        GetColor(25, 75, 130));

    Center(180, _T("PENGUIN ICE BATTLE"),
        GetColor(30, 50, 70));

    // 選択肢
    const TCHAR* options[] =
    {
        _T("2 PLAYER"),
        _T("CPU BATTLE"),
        _T("EXIT")
    };

    for (int i = 0; i < 3; ++i)
    {
        if (i == select)
        {
            DrawBox(
                245,
                300 + i * 55,
                555,
                345 + i * 55,
                GetColor(110, 190, 230),
                TRUE
            );
        }

        Center(
            305 + i * 55,
            options[i],
            GetColor(20, 35, 50)
        );
    }

    // 操作説明
    Center(
        525,
        _T("UP/DOWN: SELECT   ENTER: DECIDE"),
        GetColor(40, 60, 80)
    );
}

void Window::DrawMode(int select)
{
    DrawBox(0, 0, 800, 600, GetColor(175, 225, 245), TRUE);

    Center(100, _T("CPU DIFFICULTY"), GetColor(25, 75, 130));

    const TCHAR* options[] =
    {
        _T("EASY"),
        _T("NORMAL"),
        _T("HARD")
    };

    for (int i = 0; i < 3; ++i)
    {
        if (i == select)
        {
            DrawBox(250, 250 + i * 60, 550, 295 + i * 60,
                GetColor(110, 190, 230), TRUE);
        }

        Center(255 + i * 60, options[i], GetColor(20, 35, 50));
    }

    Center(500, _T("SELECT CPU DIFFICULTY"),
        GetColor(40, 60, 80));
}

void Window::DrawPlay(int current, Player players[2], int turn, ChargeImpact& charge)
{
    DrawString(10, 510,
        current == 0 ? _T("PLAYER 1 TURN") : _T("PLAYER 2 / CPU TURN"),
        GetColor(20, 40, 70));

    DrawFormatString(10, 535, GetColor(20, 40, 70),
        _T("TURN: %d"), turn + 1);

    DrawFormatString(10, 560, GetColor(20, 40, 70),
        _T("P1 TICKET: %s"),
        players[0].HasTicket() ? _T("YES") : _T("NO"));

    DrawFormatString(250, 560, GetColor(20, 40, 70),
        _T("P2 TICKET: %s"),
        players[1].HasTicket() ? _T("YES") : _T("NO"));

    if (charge.IsRunning())
    {
        DrawBox(250, 500, 550, 520, GetColor(40, 40, 40), TRUE);

        DrawBox(250, 500, 250 + charge.GetGauge() / 3, 520,
            GetColor(255, 220, 40), TRUE);

        DrawString(250, 475, _T("PRESS SPACE TO HIT!"),
            GetColor(20, 40, 70));
    }
}

void Window::DrawChanceChoice()
{
    DrawBox(120, 200, 680, 420, GetColor(40, 80, 120), TRUE);

    Center(230, _T("CHANCE ICE!"), GetColor(255, 255, 255));
    Center(280, _T("GET SKIP TICKET? ENTER = YES"),
        GetColor(255, 255, 255));
    Center(330, _T("CLICK RIGHT SIDE = NO"),
        GetColor(255, 255, 255));
}

void Window::DrawGameOver(int winner)
{
    DrawBox(0, 0, 800, 600, GetColor(175, 225, 245), TRUE);

    if (winner == -2)
    {
        Center(260, _T("EXIT"), GetColor(255, 255, 255));
    }
    else if (winner == 0)
    {
        Center(230, _T("PLAYER 1 WINS!"), GetColor(255, 255, 255));
    }
    else
    {
        Center(230, _T("PLAYER 2 / CPU WINS!"),
            GetColor(255, 255, 255));
    }

    Center(330, _T("PRESS ENTER TO RETURN TO TITLE"),
        GetColor(255, 255, 255));
}
