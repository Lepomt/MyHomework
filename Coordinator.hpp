#pragma once

#include <iostream>
#include <map>
#include <string>
#include <set>
#include <functional>

#include "Command.hpp"
#include "Lottery.hpp"
#include "Userbase.hpp"

class Coordinator
{
private:
    enum Commands
    {
        help,
        user,
        lottery,
        quit
    };

    std::map<std::string, Commands> commandList = {
        {"help", help}, {"user", user}, {"lottery", lottery}, {"quit", quit}};

    User* currentUser;
    Lottery* currentLottery;

    //Userbase userbase = Userbase(this);

    void printHelp();

public:

    void setCurrentUser(User* iCurrentUser) { currentUser = iCurrentUser; }
    void setCurrentLottery(Lottery* iCurrentLottery) { currentLottery = iCurrentLottery; }
    User* getCurrentUser() { return currentUser; }
    Lottery* getCurrentLottery() { return currentLottery; }

    bool dispatch(Command command);
    void printCurrent();
};
