#pragma once

class Event
{
public:

    bool choiceStart;

    void Init();

    void Update(int playerX, int playerY);

    void Draw();
};