#include "ChargeImpact.h"
#include "DxLib.h"
#include <tchar.h>

ChargeImpact::ChargeImpact() : running(false), fastMode(false), startTime(0), speed(1) {}
void ChargeImpact::Start(bool fast)
{
    running = true;
    fastMode = fast;
    startTime = GetNowCount();
    speed = fast ? 3 : 2;
}
bool ChargeImpact::IsRunning() const { return running; }
int ChargeImpact::GetGauge() const
{
    if (!running) return 0;
    int elapsed = (GetNowCount() - startTime) * speed;
    return elapsed % 2000 < 1000 ? elapsed % 1000 : 1000 - (elapsed % 1000);
}
int ChargeImpact::GetSpeed() const { return speed; }
int ChargeImpact::Stop()
{
    // Gauge value must be captured before setting running=false.
    int elapsed = (GetNowCount() - startTime) * speed;
    int phase = elapsed % 2000;
    int gauge = (phase < 1000) ? phase : 1000 - phase;
    running = false;

    if (gauge >= 900) return 100; // PERFECT
    if (gauge >= 700) return 75;  // GOOD
    if (gauge >= 450) return 50;  // NORMAL
    return 20;                    // MISS
}
