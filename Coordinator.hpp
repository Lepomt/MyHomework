#pragma once

#include <functional>  // What is it used for?
#include <iostream>    // What is it used for?
#include <map>
#include <set>  // What is it used for?
#include <string>

#include "Lottery.hpp"
#include "Parser.hpp"
#include "User.hpp"

class Coordinator
{
public:
    Coordinator();
    bool dispatchCommands();
    void printCurrent();

private:
    // Prefer enum class over plain enums, to avoid implicit conversions.
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
    // Give `std::string` an alias so it is known what it represents. Give the map a decent name, like
    // `IDENTIFIER_TO_COMMAND`. This map should be constant.
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
    // Same as the other map finding, e.g. `nameToUser`
    std::map<std::string, User> userMapKeyedByName;
    std::map<std::string, Lottery> lotteryMapKeyedByName;

    Commands extractCommand();

    void passCreate();
    void passSwitchTo();
    void passUser();
    void passLottery();

    void printHelp();
};
