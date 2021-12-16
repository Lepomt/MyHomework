#include "Coordinator.hpp"

Coordinator::Coordinator()
{
    currentUser =
        &userMapKeyedByName
             .emplace(
                 std::piecewise_construct, std::forward_as_tuple("admin"), std::forward_as_tuple("admin", User::admin))
             .first->second;
}

bool Coordinator::dispatchCommands()
{
    printCurrent();
    parser.getNextLine(std::cin);

    if (!parser.expectArguments(1, Parser::stringType))
        return false;

    switch (extractCommand())
    {
        case quit: return false;

        case help: printHelp(); break;

        case create: passCreate(); break;

        case switchTo: passSwitchTo(); break;

        case user: passUser(); break;

        case lottery: passLottery(); break;

        default: std::cout << ">invalid command\n";
    }

    return true;
}

void Coordinator::passCreate()
{
    if (!parser.expectArguments(1, Parser::stringType))
        return;

    switch (extractCommand())
    {
        case user:
            if (parser.expectArguments(1, Parser::stringType))
            {
                std::string name = parser.extractString();
                userMapKeyedByName.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(name, User::basicAccess));
            }
            break;

        case lottery:
            if (parser.expectArguments(4, Parser::stringType, Parser::intType, Parser::intType, Parser::intType)
                && currentUser->getAccessType() == User::admin)
            {
                std::string name = parser.extractString();
                int deadline = parser.extractInt();
                int gifts = parser.extractInt();
                int value = parser.extractInt();

                lotteryMapKeyedByName.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(name, deadline, gifts, value));
            }
            break;

        default: std::cout << ">invalid command\n";
    }
}

void Coordinator::passSwitchTo()
{
    if (!parser.expectArguments(2, Parser::stringType, Parser::stringType))
        return;

    Commands command = extractCommand();
    std::string name = parser.extractString();

    switch (command)
    {
        case user:
            if (userMapKeyedByName.find(name) != userMapKeyedByName.end())
                currentUser = &userMapKeyedByName.find(name)->second;
            else
            {
                std::cout << ">user \"" << name << "\" doesn't exists\n";
            }
            break;

        case lottery:
            if (lotteryMapKeyedByName.end() != lotteryMapKeyedByName.find(name))
                currentLottery = &lotteryMapKeyedByName.find(name)->second;
            else
            {
                std::cout << ">lottery \"" << name << "\" doesn't exists\n";
            }
            break;

        default: std::cout << ">invalid command\n";
    }
}

void Coordinator::passUser()
{
    if (!parser.expectArguments(2, Parser::stringType, Parser::stringType))
        return;

    if (!currentUser)
    {
        std::cout << ">switch to a user\n";
    }

    Commands command = extractCommand();
    std::string name = parser.extractString();
    auto lottery = lotteryMapKeyedByName.find(name);

    if (lottery == lotteryMapKeyedByName.end())
    {
        std::cout << ">lottery " << name << " doesn't exist\n";
        return;
    }

    switch (command)
    {
        case join: lottery->second.requestToJoin(currentUser->getName()); break;

        case draw: lottery->second.drawVictims(currentUser->getName()); break;

        default: std::cout << ">invalid command\n";
    }
}

void Coordinator::passLottery()
{
    if (!parser.expectArguments(1, Parser::stringType))
        return;

    if (!currentLottery)
    {
        std::cout << ">switch to a lottery\n";
    }

    switch (extractCommand())
    {
        case add:
            if (parser.expectArguments(1, Parser::stringType))
            {
                std::string name = parser.extractString();
                currentLottery->addParticipant(currentUser->getName(), name);
            }
            break;

        case set:
            if (!currentUser)
            {
                std::cout << ">switch to a user\n";
                break;
            }

            if (!currentLottery->isModerator(currentUser->getName()))
            {
                std::cout << ">insufficient access to lottery\n";
                break;
            }

            if (parser.expectArguments(2, Parser::stringType, Parser::intType))
            {
                switch (extractCommand())
                {
                    case deadline: currentLottery->setDeadline(time(0) + parser.extractInt() * 86400); break;
                    case gifts: currentLottery->setGiftsPerPerson(parser.extractInt()); break;
                    case value: currentLottery->setGiftValue(parser.extractInt()); break;
                    default: std::cout << ">invalid command\n";
                }
            }
            break;

        case promote:
            if (parser.expectArguments(1, Parser::stringType))
            {
                if (!currentUser)
                {
                    std::cout << ">switch to a user\n";
                }
                else
                {
                    std::string name = parser.extractString();
                    currentLottery->addModerator(currentUser->getName(), name);
                }
            }
            break;

        case requests: currentLottery->showRequests(currentUser->getName()); break;

        default: std::cout << ">invalid command\n";
    }
}

Coordinator::Commands Coordinator::extractCommand()
{
    auto iterator = commands.find(parser.extractString());
    if (iterator == commands.end())
    {
        return error;
    }
    else
    {
        return iterator->second;
    }
}

void Coordinator::printCurrent()
{
    if (currentUser)
        std::cout << '$' << currentUser->getName();
    else
        std::cout << "$no_user";

    if (currentLottery)
        std::cout << '/' << currentLottery->getName() << ": ";
    else
        std::cout << "/no_lottery: ";
}

void Coordinator::printHelp()
{
    std::cout << ">create user/lottery NAME/NAME, DEADLINE, GIFTS_PER_PERSON, GIFT_VALUE\n"
                 "-Creates a new user/lottery. Deadline is expressed in days since today.\n"
                 ">set deadline/gifts/value DEADLINE/GIFTS_PER_PERSON/GIFT_VALUE\n"
                 "-Sets properties of current lottery.\n"
                 ">switch user/lottery NAME/NAME\n"
                 "-Switches current user/lottery.\n"
                 ">add USERNAME\n"
                 "-Adds user to current lottery.\n"
                 ">join\n"
                 "-Adds current user to current lottery.\n"
                 ">draw\n"
                 "-Draws random users from current lottery.\n"
                 ">quit\n";
}
