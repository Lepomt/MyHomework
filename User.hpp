#pragma once

#include <vector>
#include <string>

class User
{
public:
    enum Privilages
    {
        basicAccess,
        admin
    };

	User(std::string& setUsername, Privilages setAccessType) : username{ setUsername }, accessType{ setAccessType }{}

	std::string& getName() { return username; }
	Privilages getAccessType() { return accessType; }

private:
	std::string username;
	Privilages accessType;
};
