#pragma once

#include <iostream>
#include <string>
#include <map>

template<class T>
class Database
{
public:
    virtual void createRecord(std::string& name);
    T* findRecord(std::string& name);

private:
    std::map<std::string, T> recordList;
};

void Database<class T>::createRecord(std::string& name)
{
    if (recordList.find(name) == recordList.end())
    {
        recordList.emplace(name, name);
    }
    else
    {
        std::cout << ">user already exists\n";
    }
}

T* Database<class T>::findRecord(std::string& name)
{
    auto record = recordList.find(name);
    if (record == recordList.end())
    {
        return nullptr;
    }
    else
    {
        return &record->second;
    }
}
