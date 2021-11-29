#include "App.h"
#include <sstream>

constexpr size_t hash(const char* str)
{
    const long long p = 131;
    const long long m = 4294967291;
    long long total = 0;
    long long current_multiplier = 1;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        total = (total + current_multiplier * str[i]) % m;
        current_multiplier = (current_multiplier * p) % m;
    }
    return total;
}

App::App()
{
    std::srand(time(0));
    contest = new Contest(0.0f, std::time(0), 1);
    std::cout << "\n --- *** SECRET SANTA MK II *** ---\n Type \"help\" to show command list. Pass an empty line, or type \"quit\" in order to exit.\n\n";
}

int App::getInput()
{
    std::string buffer;
    std::string token;

    std::getline(std::cin, buffer);
    std::stringstream stream(buffer);
    std::getline(stream, token, ' ');

    if (!buffer[0]) return 0;

    switch (hash(&token[0]))
    {
        case hash("help"):
            std::cout << ">add NAME\n -add a participant to the contest, each one must have a uniqe NAME\n>set prize NUMBER\n>set winners NUMBER\n -change amount of possible winners (one by default)\n>set deadline YEAR/MONTH/DAY (date has to be all numeric with slashes)\n>prize\n -show the current prize\n>deadline\n -show the current deadline\n>find NAME\n -show if the NAME is a participant\n>participants\n -show the lists of participants\n>winners\n -show the list of winners\n>roll\n -randomly choose a set amount of winners from participants\n>remove NUMBER or remove NAME\n -remove a participant from the contest by his NAME or his NUMBER on the list\n>clear\n -reset the contest\n>quit\n";
            break;

        case hash("add"):
            std::getline(stream, token, '\n');
            contest->addParticipant(token);
            break;

        case hash("set"):
            std::getline(stream, token, ' ');
            passSetCommand(stream, token);
            break;

        case hash("find"):
            std::getline(stream, token, '\n');
            contest->findParticipant(token);
            break;

        case hash("participants"):
            contest->showParticipants();
            break;

        case hash("winners"):
            contest->showWinners();
            break;

        case hash("roll"):
            contest->rollWinners();
            break;

        case hash("deadline"):
            contest->showDeadline();
            break;

        case hash("prize"):
            contest->showPrize();
            break;

        case hash("remove"):
            getline(stream, token, '\n');
            if (stringIsNumber(token))
            {
                contest->removeParticipant(std::stoi(token));
            }
            else
            {
                contest->removeParticipant(token);
            }
            break;

        case hash("clear"):
            contest->clear();
            break;

        case hash("quit"):
            return 0;

        default:
            std::cout << ">invalid command" << std::endl;
            break;
    }

    std::cout << std::endl;
    return 1;
}

int App::stringIsNumber(std::string& string)
{
    for (auto letter : string)
    {
        if (!std::isdigit(letter)) return 0;
    }

    return 1;
}

void App::passSetCommand(std::stringstream& stream, std::string& token)
{
    switch (hash(&token[0]))
    {
        case hash("prize"):
            getline(stream, token, '\n');
            if (stringIsNumber(token))
            {
                contest->setPrize(std::stof(token));
            }
            else 
            {
                std::cout << ">invalid value";
            }

            break;

        case hash("winners"):
            getline(stream, token, '\n');
            if (stringIsNumber(token))
            {
                contest->setAmountOfWinnersToRoll(std::stoi(token));
            }
            else
            {
                std::cout << ">invalid value";
            }
            break;

        case hash("deadline"):
            int year = 1900, month = 1, day = 1;

            std::getline(stream, token, '/');
            if (stringIsNumber(token)) year = std::stoi(token);
            std::getline(stream, token, '/');
            if (stringIsNumber(token)) month = std::stoi(token);
            std::getline(stream, token, '/');
            if (stringIsNumber(token)) day = std::stoi(token);
 
            if (year > 1900 && month > 0 && month < 13 && day > 0 && day < 32)
            {
                contest->setDeadline(year, month, day);
            }
            else
            {
                std::cout << ">incorect date";
            }

            break;
    }
}