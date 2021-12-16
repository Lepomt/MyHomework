#define _CRT_SECURE_NO_WARNINGS

#include "Lottery.hpp"

// We're not changing `iName`, so `const` it.
Lottery::Lottery(std::string& iName, std::time_t iDeadline, int iGiftsPerPerson, int iGiftValue)
{
    // Use initialization list for member fields.
    name = iName;
    deadline = iDeadline;
    giftsPerPerson = iGiftsPerPerson;
    giftValue = iGiftValue;
    moderatorList.insert("admin");
}

Ticket* Lottery::findTicket(std::string& username)
{
    auto ticket = ticketList.find(username);
    if (ticket == ticketList.end())
    {
        return nullptr;
    }
    else
    {
        return &ticket->second;
    }
}

// Are we changing the caller? `const` it. And `username` too.
void Lottery::addParticipant(std::string& caller, std::string& username)  // const
{
    if (!isModerator(caller))
    {
        std::cout << ">insufficient access to lottery\n";
    }

    if (findTicket(username))
    {
        std::cout << ">user already joined the lottery\n";
    }
    else
    {
        ticketList.emplace(std::piecewise_construct, std::forward_as_tuple(username), std::forward_as_tuple(username));
    }

    if (requests.find(username) != requests.end())
    {
        requests.erase(username);
    }
}

// const
void Lottery::drawVictims(std::string& username)  // const
{
    Ticket* userTicket = findTicket(username);

    if (!userTicket)
    {
        std::cout << ">user is not a part of the lottery\n";
        return;
    }

    if (userTicket->giftsGiven >= giftsPerPerson)
    {
        std::cout << ">user already drawn\n";
        return;
    }

    // Extract it as a function, e.g. "getPossibleAssignments".
    std::vector<Ticket*> pool;
    for (auto& ticket : ticketList)
    {
        if (ticket.second.owner != userTicket->owner && ticket.second.giftsRecieved < giftsPerPerson)
        {
            pool.emplace_back(&ticket.second);
        }
    }

    if (pool.size() < giftsPerPerson)
    {
        std::cout << ">not enough participants for the lottery\n";
        return;
    }

    // This is not used outside the following for loop.
    int randomNumber;
    // `i` is fine for that kind of arbitrary counters. (Because you're not using the `counter` for anything.)
    for (int counter = 0; counter < giftsPerPerson; counter++)
    {
        randomNumber = rand() % pool.size();
        userTicket->recipeints.push_back(pool[randomNumber]);
        userTicket->giftsGiven++;
        pool[randomNumber]->giftsRecieved++;
        pool.erase(pool.begin() + randomNumber);
    }

    // Extract a printer method/overloard ostream operator.
    std::cout << ">deadline: " << ctime(&deadline) << ">gift value: " << giftValue << "\n>give gifts to:\n";

    for (auto recipient : userTicket->recipeints)
    {
        std::cout << " -" << recipient->owner << std::endl;
    }
}
// Consider renaming to `canModerate`.
bool Lottery::isModerator(std::string& username) { return moderatorList.find(username) != moderatorList.end(); }

void Lottery::requestToJoin(std::string& username)
{
    if (findTicket(username))
    {
        std::cout << ">you already joined the lottery\n";
    }
    else
    {
        requests.insert(username);
    }
}

// Unused parameter.
void Lottery::showRequests(std::string& username)
{
    for (auto& request : requests)
    {
        std::cout << " -" << request << std::endl;
    }
}
