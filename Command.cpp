#include "Command.hpp"

Command::Command(std::istream& stream)
{
    std::string line;
    std::string word;
    std::stringstream subStream;

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

bool Command::isNumber(std::string& string)
{
    for (auto symbol : string)
    {
        if (!std::isdigit(symbol))
            return false;
    }
    return true;
}

bool Command::expectArguments(int offset, int count, ...)
{
    va_list arguments;
    int index;

    if (words.size() < count && words.size() > offset)
    {
        std::cout << ">not enough arguments\n";
        return false;
    }

    va_start(arguments, count);
    for (index = offset; index < count; index++)
    {
        switch (va_arg(arguments, Datatype))
        {
            case intType:
                if (!isNumber(words[index]))
                    goto loop_break;

            case stringType: break;

            default: goto loop_break;
        }
    }
    va_end(arguments);
    return true;

loop_break:

    std::cout << ">invalid argument " << index + 1 << std::endl;
    va_end(arguments);
    return false;
}
