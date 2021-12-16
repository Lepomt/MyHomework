#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Ticket
{
    std::string owner;
    int giftsGiven = 0;
    int giftsRecieved = 0;  // giftsReceived
    // Why are recipients stored as `Ticket*`?
    std::vector<Ticket*> recipeints;  // recipients

    Ticket(std::string username) : owner{username} {}
};

class Lottery
{
private:
    std::string name;
    std::time_t deadline;
    int giftsPerPerson;
    int giftValue;
    std::map<std::string, Ticket> ticketList;  // `ownerToTicket`?

    std::set<std::string> moderatorList;  // `moderators`, this is not a list.
    std::set<std::string> requests;

    Ticket* findTicket(std::string& username);

public:
    Lottery(const std::string& iName, std::time_t iDeadline, int iGiftsPerPerson, int iGiftValue);

    std::string& getName() { return name; }
    void setName(std::string& iName) { name = iName; }
    void setDeadline(std::time_t iDeadline) { deadline = iDeadline; }
    void setGiftsPerPerson(int iGiftsPerPerson) { giftsPerPerson = iGiftsPerPerson; }
    void setGiftValue(int iGiftValue) { giftValue = iGiftValue; }

    void addParticipant(std::string& caller, std::string& username);
    void drawVictims(std::string& username);

    void addModerator(std::string& caller, std::string& username) { moderatorList.insert(username); }  // make outside
    bool isModerator(std::string& username);

    void requestToJoin(std::string& username);
    void showRequests(std::string& username);
};
