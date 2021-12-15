#pragma once

#include <cstdarg>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Command
{
    public:
        enum Datatype
        {
            intType,
            stringType
        };

        Command(std::istream& stream);

        bool expectArguments(int offset, int count, ...);
        int extractInt(int index) { return std::stoi(words[index]); }
        std::string extractString(int index) { return words[index]; }

    private:
        std::vector<std::string> words;

        bool isNumber(std::string& string);
};

namespace Parser
{
inline Command getNewCommand() { return Command(std::cin); }
}