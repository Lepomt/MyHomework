#pragma once

#include <cstdarg>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class InputCollector
{
public:
    enum Datatype
    {
        intType,
        stringType
    };

    InputCollector() {}
    ~InputCollector() { clear(); }

    int getCommand(std::istream& stream);
    bool expectArguments(int offset, int count, ...);
    template<typename T>
    T extractArgument(int index)
    {
        return *static_cast<T*>(argumentList[index].data);
    }

    void clear();

private:
    struct Argument
    {
        void* data;
        Datatype type;

        Argument(void* iData, Datatype iType) : data{iData}, type{iType} {}
    };

    std::vector<Argument> argumentList;
    int argumentCount = 0;

    bool isNumber(std::string& string);
};
