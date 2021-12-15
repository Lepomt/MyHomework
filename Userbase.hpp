#pragma once

#include <iostream>
#include <map>
#include <string>

#include "Coordinator.hpp"
#include "Command.hpp"
#include "User.hpp"

class Userbase
{
private:
    enum Commands
    {
        create,
        switchTo,
        join,
        draw
    };

    std::map<std::string, Commands> commandList = {
        {"create", create}, {"switch", switchTo}, {"join", join}, {"draw", draw}};

    std::map<std::string, User> userMap;

public:
    void passCommand(Command command, int argumentsUsed);
};
