#include "Userbase.hpp"

void Userbase::createUser(std::string& name, User::Privilages access)
{
    if (userList.find(name) == userList.end())
    {
        userList.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name, access));
    }
    else
    {
        std::cout << ">user already exists\n";
    }
}

User* Userbase::findUser(std::string& name)
{
    auto user = userList.find(name);
    if (user == userList.end())
    {
        return nullptr;
    }
    else
    {
        return &user->second;
    }
}
