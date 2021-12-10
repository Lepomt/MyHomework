#pragma once

#include <iostream>
#include <map>
#include <string>

#include "Lottery.hpp"
#include "Lotterybase.hpp"
#include "Parser.hpp"
#include "User.hpp"
#include "Userbase.hpp"

class Coordinator
{
private:
    enum Commands
    {
        user,
        lottery,
        create,
        set,
        changeto,
        addto,
        join,
        draw,
        help,
        quit,
        name,
        deadline,
        gifts,
        value
    };
    std::map<std::string, Commands> commandList;
    Parser parser;
    Userbase userbase;
    Lotterybase lotterybase;
    User* currentUser = nullptr;
    Lottery* currentLottery = nullptr;

    bool checkConditions(char flags);

    void passCreate();
    void passSet();
    void passChangeTo();

public:
    Coordinator();
    bool getInput();
};
