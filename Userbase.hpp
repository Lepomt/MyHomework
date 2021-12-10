#pragma once

#include <iostream>
#include <string>
#include <map>

#include "User.hpp"

class Userbase
{
public:
    void createUser(std::string& name, User::Privilages);
    User* findUser(std::string& name);

private:
    std::map<std::string, User> userList;
};
