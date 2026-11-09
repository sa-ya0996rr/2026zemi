#pragma once
class Player
{
public:
    Player();
    void Reset();
    bool HasTicket() const;
    void SetTicket(bool value);
    bool UseTicket();
private:
    bool ticket;
};
