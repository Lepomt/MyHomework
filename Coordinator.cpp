#include "Coordinator.hpp"

#define SECONDS_IN_DAY 86400

#define IS_CURRENT_USER 0b00000001
#define IS_CUR_USR_MOD 0b00000011
#define IS_CURRENT_LOTTERY 0b00000100
#define IS_CUR_USR_LOT_MOD 0b00001101

Coordinator::Coordinator()
{
    commandList["user"] = user;
    commandList["lottery"] = lottery;
    commandList["create"] = create;
    commandList["set"] = set;
    commandList["switch"] = changeto;
    commandList["add"] = addto;
    commandList["join"] = join;
    commandList["draw"] = draw;
    commandList["help"] = help;
    commandList["quit"] = quit;
    commandList["name"] = name;
    commandList["deadline"] = deadline;
    commandList["gifts"] = gifts;
    commandList["value"] = value;

    std::string name = "admin";
    userbase.createUser(name, User::admin);
    currentUser = userbase.findUser(name);
}

bool Coordinator::getInput()
{
    if (currentUser)
        std::cout << '$' << currentUser->getName();
    else
        std::cout << "$no_user";

    if (currentLottery)
        std::cout << '/' << currentLottery->getName() << ": ";
    else
        std::cout << "/no_lottery: ";

    switch (commandList.find(parser.getCommandName())->second)
    {
        case create:
            if (parser.expectArguments(2, Parser::stringType, Parser::stringType))
                passCreate();
            break;

        case set:
            if (checkConditions(IS_CUR_USR_LOT_MOD) && parser.expectArguments(1, Parser::stringType))
                passSet();
            break;

        case changeto:
            if (parser.expectArguments(2, Parser::stringType, Parser::stringType))
                passChangeTo();
            break;

        case addto:
            if (checkConditions(IS_CURRENT_LOTTERY) && parser.expectArguments(1, Parser::stringType))
            {
                std::string username = parser.extractArgument<std::string>(0);
                if (userbase.findUser(username))
                {
                    currentLottery->addParticipant(username);
                }
                else
                {
                    std::cout << ">user doesn't exist\n";
                }
            }
            break;

        case join:
            if (checkConditions(IS_CURRENT_USER | IS_CURRENT_LOTTERY))
            {
                currentLottery->addParticipant(currentUser->getName());
            }
            break;

        case draw:
            if (checkConditions(IS_CURRENT_USER | IS_CURRENT_LOTTERY))
            {
                currentLottery->drawVictims(currentUser->getName());
            }
            break;

        case help:
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
            break;

        case quit: return false;

        default: std::cout << ">invalid command\n";
    }

    return true;
}

void Coordinator::passCreate()
{
    switch (commandList.find(parser.extractArgument<std::string>(0))->second)
    {
        case user:
            if (parser.expectArguments(2, Parser::stringType, Parser::stringType))
            {
                std::string username = parser.extractArgument<std::string>(1);
                userbase.createUser(username, User::basicAccess);
            }
            break;

        case lottery:
            if (checkConditions(IS_CUR_USR_MOD)
                && parser.expectArguments(
                    5, Parser::stringType, Parser::stringType, Parser::intType, Parser::intType, Parser::intType))
            {
                std::string lotteryname = parser.extractArgument<std::string>(1);
                lotterybase.createLottery(
                    lotteryname,
                    std::time(0) + SECONDS_IN_DAY * parser.extractArgument<int>(2),
                    parser.extractArgument<int>(3),
                    parser.extractArgument<int>(4));
            }
            break;

        default: std::cout << ">invalid command\n";
    }
}

void Coordinator::passSet()
{
    switch (commandList.find(parser.extractArgument<std::string>(0))->second)
    {
        case name:
            if (parser.expectArguments(2, Parser::stringType, Parser::stringType))
            {
                std::string lotteryname = parser.extractArgument<std::string>(1);
                currentLottery->setName(lotteryname);
            }
            break;

        case deadline:
            if (parser.expectArguments(2, Parser::stringType, Parser::intType))
            {
                currentLottery->setDeadline(std::time(0) + SECONDS_IN_DAY * parser.extractArgument<int>(1));
            }
            break;

        case gifts:
            if (parser.expectArguments(2, Parser::stringType, Parser::intType))
                currentLottery->setGiftsPerPerson(parser.extractArgument<int>(1));
            break;

        case value:
            if (parser.expectArguments(2, Parser::stringType, Parser::intType))
                currentLottery->setGiftValue(parser.extractArgument<int>(1));
            break;

        default: std::cout << ">invalid command\n";
    }
}
void Coordinator::passChangeTo()
{
    switch (commandList.find(parser.extractArgument<std::string>(0))->second)
    {
        case user:
            if (parser.expectArguments(2, Parser::stringType, Parser::stringType))
            {
                std::string username = parser.extractArgument<std::string>(1);
                User* user = userbase.findUser(username);
                if (user)
                {
                    currentUser = user;
                }
                else
                {
                    std::cout << ">user doesn't exist\n";
                }
            }
            break;

        case lottery:
            if (parser.expectArguments(2, Parser::stringType, Parser::stringType))
            {
                std::string lotteryname = parser.extractArgument<std::string>(1);
                Lottery* lottery = lotterybase.findLottery(lotteryname);
                if (lottery)
                {
                    currentLottery = lottery;
                }
                else
                {
                    std::cout << ">lottery doesn't exist\n";
                }
            }
            break;

        default: std::cout << ">invalid command\n";
    }
}

bool Coordinator::checkConditions(char flags)
{
    // is current user assigned
    if (flags & 0b00000001)
    {
        if (!currentUser)
        {
            std::cout << ">switch to a user\n";
            return 0;
        }
    }

    // is current user an admin
    if (flags & 0b00000010)
    {
        if (currentUser->getAccessType() != User::admin)
        {
            std::cout << ">insufficient access rights\n";
            return 0;
        }
    }

    // is current lottery assigned
    if (flags & 0b00000100)
    {
        if (!currentLottery)
        {
            std::cout << ">switch to a lottery\n";
            return 0;
        }
    }

    // is current user lottery's moderator
    if (flags & 0b00001000)
    {
        if (!currentLottery->isModerator(currentUser->getName()) && currentUser->getAccessType() != User::admin)
        {
            std::cout << ">insufficient access rights\n";
            return 0;
        }
    }

    return 1;
}
