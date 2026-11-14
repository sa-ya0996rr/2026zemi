
#pragma once

class ChargeImpact
{
public:
    ChargeImpact();

    void Start(bool fast);
    bool IsRunning() const;

    int Stop();
    int StopGauge();

    int GetGauge() const;
    int GetSpeed() const;

private:
    bool running;
    bool fastMode;
    int startTime;
    int speed;
};
