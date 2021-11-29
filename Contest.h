#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

class Participant
{
	private:
		float winnings = 0.0f;
		std::string name;

	public:
		Participant(std::string& iName) { name = iName; }
		std::string& getName() { return name; }
		float setWinnings(float iWinnings) { winnings = iWinnings; }
		float getWinnings() { return winnings; }
};

class Contest
{
	private:
		const int INVALID_INDEX = -1;

		int amountOfWinnersToRoll;
		float prizePool;
		std::time_t deadline;

		std::vector<Participant*> participants;
		std::vector<Participant*> winners;

		void printPool(std::vector<Participant*> vector);
		int findParticipantIndex(std::vector<Participant*>& pool, std::string& name);
		void moveParticipant(std::vector<Participant*>& pool, std::vector<Participant*> newPool, int index);

	public:
		Contest(float iPrizePool, std::time_t iDeadline, int iWinnersToRollAmount);
		~Contest() { clear(); }

		void setPrize(float amount) { prizePool = amount; }
		void showPrize() { std::cout << prizePool << std::endl; }
		void setAmountOfWinnersToRoll(int amount) { amountOfWinnersToRoll = amount; };

		void clear();
		void setDeadline(int year, int month, int day);
		void showDeadline();
		void addParticipant(std::string& name);
		void removeParticipant(int index);
		void removeParticipant(std::string& name);
		void findParticipant(std::string& name);
		void showParticipants();
		void showWinners();
		void rollWinners();
};
