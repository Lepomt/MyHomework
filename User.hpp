#pragma once

#include <string>
#include <vector>

class User
{
public:
    enum Privilages
    {
        basicAccess,
        admin
    };

    User(std::string setUsername, Privilages setAccessType) : username{setUsername}, accessType{setAccessType} {}

    std::string& getName() { return username; }
    Privilages getAccessType() { return accessType; }

private:
    // It is redundant to name it `username` as it already is in the `User` class. `name` is fine.
    std::string username;
    Privilages accessType;
};
