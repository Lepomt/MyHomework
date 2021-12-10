#pragma once

#include <iostream>
#include <string>
#include <map>

#include "Lottery.hpp"

class Lotterybase
{
public:
    void createLottery(std::string& name, std::time_t deadline, int giftsPerPerson, int giftValue);
    Lottery* findLottery(std::string& name);

private:
    std::map<std::string, Lottery> lotteryList;
};
