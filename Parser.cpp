#include "Parser.hpp"

void Parser::getNextLine(std::istream& stream)
{
    // Declare variables directly before their usage.
    std::string line;
    std::string word;
    std::stringstream subStream;

    depth = 0;
    words.clear();

    getline(stream, line, '\n');
    subStream.str(line);

    while (!subStream.eof())
    {
        getline(subStream, word, ' ');
        if (!word.empty())
            words.push_back(word);
    }
}

bool Parser::expectArguments(int count, ...)
{
    va_list arguments;
    int index;

    if (words.size() < count + depth)
    {
        std::cout << ">not enough arguments\n";
        return false;
    }

    va_start(arguments, count);
    for (index = depth; index < count + depth; index++)
    {
        switch (va_arg(arguments, Datatype))
        {
            case intType:
                if (!isNumber(words[index]))
                    goto loop_break;
                break;

            case stringType: break;

            default:
            loop_break:
                std::cout << ">invalid argument " << index + 1 << std::endl;
                va_end(arguments);
                return false;
        }
    }
    va_end(arguments);

    return true;
}

int Parser::extractInt() { return std::stoi(words[depth++]); }

std::string Parser::extractString() { return words[depth++]; }

bool Parser::isNumber(std::string& string)
{
    for (auto symbol : string)
    {
        if (!std::isdigit(symbol))
            return false;
    }
    return true;
}
