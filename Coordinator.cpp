#include "Coordinator.hpp"

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

bool Coordinator::dispatch(Command command)
{
    if (!command.expectArguments(0, 1, Command::stringType))
        return false;

    switch (commandList.find(command.extractString(0))->second)
    {
        case help: printHelp(); break;

        case user: //userbase.passCommand(command, 1);
            break;

        case lottery:  // currentLottery->passCommand(command, 1);
            break;

        case quit: return false;

        default: std::cout << ">invalid command\n";
    }

    return true;
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
