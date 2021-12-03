#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Ticket
{
public:
	std::vector<std::string> recipients;
	std::string owner;
	int giftsToGive = 0;
	int giftsToRecieve = 0;

	Ticket(std::string& username) : owner{ username } {}

	void unlinkRecipient(std::string& username);
};

class Lottery
{
	private:
		std::map<std::string, Ticket> ticketList;
		std::vector<std::string> moderatorList;
		std::time_t deadline;
		std::string lotteryname;
		int giftValue;
		int giftsPerPerson;
		bool isLotteryOpen = false;

	public:
		Lottery(std::time_t setDeadline, std::string& setLoterryname, int setGiftValue, int setGiftsPerPerson) :
			deadline{ setDeadline },
			lotteryname{ setLoterryname },
			giftValue{ setGiftValue },
			giftsPerPerson{ setGiftsPerPerson } {}

		bool getIsLotteryOpen() { return isLotteryOpen; }
		std::string& getName() { return lotteryname; }
		std::time_t getDeadline() { return deadline; }
		int getGiftValue() { return giftValue; }
		int getGiftsPerPerson() { return giftsPerPerson; }

		void setName(std::string& newLotteryname) { lotteryname = newLotteryname; }
		void setDeadline(std::time_t newDeadline) { deadline = newDeadline; }
		void setGiftValue(int newGiftValue) { giftValue = newGiftValue; }
		void setGiftsPerPerson(int newGiftsPerPerson) { giftsPerPerson = newGiftsPerPerson; }

		bool isOpen();
		bool hasExpired();
		bool isModerator(std::string& username);

		void openLottery();
		void addModerator(std::string& username);
		void listModerators();
		bool ticketExist(std::string& username);
		void addTicket(std::string& username);
		void printTicket(std::string& username);
		void listTickets();
		void removeTicket(std::string& username);
		void roll(std::string& username);
};
