
#include "ChargeImpact.h"
#include "DxLib.h"
#include <tchar.h>

ChargeImpact::ChargeImpact()
    : running(false),
    fastMode(false),
    startTime(0),
    speed(1)
{}

void ChargeImpact::Start(bool fast)
{
    running = true;
    fastMode = fast;
    startTime = GetNowCount();

    speed = fast ? 3 : 2;
}

bool ChargeImpact::IsRunning() const
{
    return running;
}

int ChargeImpact::GetGauge() const
{
    if (!running)
    {
        return 0;
    }

    int elapsed = (GetNowCount() - startTime) * speed;
    int phase = elapsed % 2000;

    return (phase < 1000)
        ? phase
        : 1000 - phase;
}

// 実際のゲージ値を取得して停止
int ChargeImpact::StopGauge()
{
    if (!running)
    {
        return 0;
    }

    int elapsed = (GetNowCount() - startTime) * speed;
    int phase = elapsed % 2000;

    int gauge = (phase < 1000)
        ? phase
        : 1000 - phase;

    running = false;

    return gauge;
}

// 通常氷用：4段階のダメージを返す
int ChargeImpact::Stop()
{
    int gauge = StopGauge();

    if (gauge >= 900)
    {
        return 100;
    }

    if (gauge >= 700)
    {
        return 75;
    }

    if (gauge >= 450)
    {
        return 50;
    }

    return 20;
}

int ChargeImpact::GetSpeed() const
{
    return speed;
}
