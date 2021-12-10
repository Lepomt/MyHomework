#pragma once

#include <cstdarg>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Parser
{
public:
    enum Datatype
    {
        nullType,
        charType,
        intType,
        floatType,
        stringType
    };

    Parser(){};
    Parser(const Parser& other);
    Parser& operator=(Parser& other);
    ~Parser() { clear(); }

    std::string& getCommandName();
    bool expectArguments(int count, ...);
    template<typename T>
    T extractArgument(int index)
    {
        return *static_cast<T*>(tokens[index + 1].data);
    }

private:
    struct token
    {
        void* data;
        Datatype type;

        token(void* iData, Datatype iType) : data{iData}, type{iType} {}
    };

    std::vector<token> tokens;
    std::string blank = "";
    int argumentCount = 0;

    void clear();
    bool isNumber(std::string& string);
};
