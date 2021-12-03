#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "User.hpp"
#include "Lottery.hpp"

#define SECONDS_IN_DAY 84600

class Coordinator
{
	private:
		std::map<std::string, User> userbase;
		std::map<std::string, Lottery> lotterybase;
		User* currentUser = nullptr;
		Lottery* currentLottery = nullptr;
		privilages UserAccess;
		privilages LotteryAccess;

		bool userExists(std::string& username);
		bool lotteryExists(std::string& username);
		bool isCurrentUser();
		bool isCurrentLottery();
		bool currentUserHasAccess(privilages access);
		bool currentUserHasLotteryAccess();

	public:
		Coordinator();

		void createUser(std::string& username, privilages accessType);
		void listUsers();
		void deleteUser(std::string& username);
		void switchUser(std::string& username);
		void printUserInfo(std::string& username);
		void printUserInfo();

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
