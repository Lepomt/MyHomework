#pragma once

#include <iostream>
#include <map>
#include <string>
#include <set>
#include <functional>

#include "Parser.hpp"
#include "Lottery.hpp"
#include "User.hpp"

class Coordinator
{
public:
    Coordinator();
    bool dispatchCommands();
    void printCurrent();

private:
    enum Commands
    {
        error,
        help,
        user,
        lottery,
        create,
        switchTo,
        add,
        join,
        draw,
        set,
        name,
        deadline,
        gifts,
        value,
        requests,
        promote,
        quit
    };

    std::map<std::string, Commands> commands = {
        {"help", help},
        {"user", user},
        {"lottery", lottery},
        {"create", create},
        {"switch", switchTo},
        {"add", add},
        {"join", join},
        {"draw", draw},
        {"set", set},
        {"name", name},
        {"deadline", deadline},
        {"gifts", gifts},
        {"value", value},
        {"requests", requests},
        {"promote", promote},
        {"quit", quit}};

    Parser parser;

    User* currentUser;
    Lottery* currentLottery;

    std::map<std::string, User> userMapKeyedByName;
    std::map<std::string, Lottery> lotteryMapKeyedByName;

    Commands extractCommand();

    void passCreate();
    void passSwitchTo();
    void passUser();
    void passLottery();

    void printHelp();
};
