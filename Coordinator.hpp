#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Lottery.hpp"
#include "User.hpp"

// Only used in this class' .cpp file, no need to expose it in header.
#define SECONDS_IN_DAY 84600

class Coordinator
{
private:
    /*
    What does the string represent here?
    You should give it an alias with `using` that represents its meaning.
    Or reconsider using it at all, instead couple it with `User` and change the type to `std::set<User> users;`
    The same applies to `lotterybase`.
    */
    std::map<std::string, User> userbase;
    std::map<std::string, Lottery> lotterybase;
    // Consider smart pointer usage for both of the below raw ptr. Use `const` if possible/desired, for the underlying
    // type.
    User* currentUser = nullptr;
    Lottery* currentLottery = nullptr;
    // Types should start with a capital letter -> Privilages
    privilages UserAccess;
    privilages LotteryAccess;

    /*
    Use `const` for all the methods that are not supposed to change an object's internal values.
    e.g: `bool userExists(std::string& username) const;`
    Is `username` changed within the method? Pass non-primitives as const& whenever possible, that includes
    `std::string`.
    */
    bool userExists(std::string& username) const;
    bool lotteryExists(std::string& username);
    bool isCurrentUser();
    bool isCurrentLottery();
    bool currentUserHasAccess(privilages access);
    bool currentUserHasLotteryAccess();

public:
    /*
    Adhere to the rule-of-three, in this case to avoid a shallow copy.
    https://en.cppreference.com/w/cpp/language/rule_of_three
    */
    Coordinator();
    /*
    Do all of the below really need to be public?
    The coordinator should only have an exposed interface for the requested command and process it internally, if it
    isn't already.
    */
    void createUser(std::string& username, privilages accessType);
    void listUsers();
    void deleteUser(std::string& username);
    void switchUser(std::string& username);
    void printUserInfo(std::string& username);
    void printUserInfo();

    /*
    Having to group so many methods like that is a cue that you should extract a class out of those.
    Consider creating dedicated Coordinators for Lotteries and Users.
    */
    void createLottery(std::string& setLoterryname);
    void listLotteries(bool hiddenFlag);
    void deleteLottery(std::string& lotteryname);
    void switchLottery(std::string& lotteryname);
    void printLotteryInfo(std::string& lotteryname);
    void printLotteryInfo();

    void setLotteryDeadline(int daysSinceToday);
    void setLotteryGiftsPerPerson(int giftsPerPerson);
    void setLotteryGiftValue(int giftValue);
    void addUserToLottery(std::string& username);

    void joinLottery();
    void roll();

    bool getInput();
};
