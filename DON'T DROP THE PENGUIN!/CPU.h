#pragma once
class Ice;
class Penguin;
class CPU
{
public:
    int ChooseTile(const Ice& ice, Penguin penguins[2], int difficulty);
    int RollPower(int difficulty);
};
