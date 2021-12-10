#include "Lotterybase.hpp"

void Lotterybase::createLottery(std::string& name, std::time_t deadline, int giftsPerPerson, int giftValue)
{
    if (lotteryList.find(name) == lotteryList.end())
    {
        lotteryList.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, deadline, giftsPerPerson, giftValue));
    }
    else
    {
        std::cout << ">lottery already exists\n";
    }
}

Lottery* Lotterybase::findLottery(std::string& name)
{
    auto lottery = lotteryList.find(name);
    if (lottery == lotteryList.end())
    {
        return nullptr;
    }
    else
    {
        return &lottery->second;
    }
}
