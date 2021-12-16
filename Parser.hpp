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
            intType,
            stringType
        };

        void getNextLine(std::istream& stream);
        bool expectArguments(int count, ...);
        int extractInt();
        std::string extractString();

    private:
        int depth = 0;
        std::vector<std::string> words;

        bool isNumber(std::string& string);
};
