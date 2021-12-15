#include "InputCollector.hpp"

void InputCollector::clear()
{
    for (auto it = argumentList.begin(); it != argumentList.end(); it++)
    {
        if (it->type == stringType)
        {
            static_cast<std::string*>(it->data)->~basic_string();
        }
        delete it->data;
    }
    argumentList.clear();
}

bool InputCollector::isNumber(std::string& string)
{
    for (auto symbol : string)
    {
        if (!std::isdigit(symbol))
            return false;
    }
    return true;
}

int InputCollector::getCommand(std::istream& stream)
{
    std::string line;
    std::string word;
    std::stringstream subStream;

    clear();
    argumentCount = 0;

    getline(stream, line, '\n');
    subStream.str(line);

    while (!subStream.eof())
    {
        getline(subStream, word, ' ');
        if (!word.empty())
        {
            if (isNumber(word))
            {
                argumentList.emplace_back(new int(std::stoi(word)), intType);
            }
            else
            {
                argumentList.emplace_back(new std::string(word), stringType);
            }
            argumentCount++;
        }
    }

    return argumentCount;
}

bool InputCollector::expectArguments(int offset, int count, ...)
{
    va_list arguments;

    if (argumentCount - offset < count)
    {
        std::cout << ">not enough arguments\n";
        return false;
    }

    va_start(arguments, count);
    for (int index = offset; index <= count; index++)
    {
        if (argumentList[index].type != va_arg(arguments, Datatype))
        {
            std::cout << ">invalid argument " << index << std::endl;
            va_end(arguments);
            return false;
        }
    }
    va_end(arguments);
    return true;
}
