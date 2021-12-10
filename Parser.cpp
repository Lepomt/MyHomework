#include "Parser.hpp"

Parser::Parser(const Parser& other)
{
    for (auto& element : other.tokens)
    {
        tokens.emplace_back(element.data, element.type);
    }
}

Parser& Parser::operator=(Parser& other)
{
    clear();

    for (auto& element : other.tokens)
    {
        tokens.emplace_back(element.data, element.type);
    }
}

void Parser::clear()
{
    for (auto& element : tokens)
    {
        delete element.data;
    }
    tokens.clear();
}

bool Parser::isNumber(std::string& string)
{
    for (auto symbol : string)
    {
        if (!std::isdigit(symbol))
            return false;
    }
    return true;
}

std::string& Parser::getCommandName()
{
    std::string line;
    std::string word;
    std::stringstream stream;

    clear();
    argumentCount = 0;

    getline(std::cin, line, '\n');
    stream.str(line);

    if (!line[0])
        return blank;

    while (!stream.eof())
    {
        getline(stream, word, ' ');
        if (!word.empty())
        {
            if (isNumber(word))
            {
                tokens.emplace_back(new int(std::stoi(word)), intType);
            }
            else
            {
                tokens.emplace_back(new std::string(word), stringType);
            }
            argumentCount++;
        }
    }

    argumentCount -= 1;
    return *(std::string*)tokens[0].data;
}

bool Parser::expectArguments(int count, ...)
{
    va_list arguments;

    if (argumentCount < count)
    {
        std::cout << ">not enough arguments\n";
        return false;
    }

    va_start(arguments, count);
    for (int index = 1; index <= count; index++)
    {
        if (tokens[index].type != va_arg(arguments, Datatype))
        {
            std::cout << ">invalid argument " << index << std::endl;
            va_end(arguments);
            return false;
        }
    }
    va_end(arguments);
    return true;
}
