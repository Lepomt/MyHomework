#include "Contest.h"

Contest::Contest(float iPrizePool, std::time_t iDeadline, int iWinnersToRollAmount) : prizePool{ iPrizePool }, deadline{ iDeadline }, amountOfWinnersToRoll{ iWinnersToRollAmount }{}

void Contest::clear()
{
	amountOfWinnersToRoll = 1;
	prizePool = 0.0f;
	deadline = time(0);

	if (!participants.empty())
	{
		for (auto participant : participants)
		{
			delete participant;
		}
		participants.clear();
	}

	if (!winners.empty())
	{
		for (auto winner : winners)
		{
			delete winner;
		}
		winners.clear();
	}

	std::cout << ">pool cleared\n";
}

void Contest::setDeadline(int year, int month, int day)
{
	std::tm date = { 0 };
	std::time_t time;
	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = day;

	if ((time = mktime(&date)) == -1)
	{
		std::cout << ">invalid date\n";
	}
	else
	{
		deadline = time;
	}
	
}

void Contest::showDeadline()
{
	std::cout << ctime(&deadline);
}

void Contest::addParticipant(std::string& name)
{
	if (findParticipantIndex(winners, name) != INVALID_INDEX)
	{
		std::cout << ">\"" << name << "\" already won\n";
	}
	else if (findParticipantIndex(participants, name) != INVALID_INDEX)
	{
		std::cout << ">name already taken\n";
	}
	else
	{
		participants.push_back(new Participant(name));
	}
}

void Contest::removeParticipant(int index)
{
	if (index > 0 && index <= participants.size())
	{
		participants.erase(participants.begin() + index - 1);
	}
	else
	{
		std::cout << ">wrong index\n";
	}
}

void Contest::removeParticipant(std::string& name)
{
	removeParticipant(findParticipantIndex(participants, name));
}

void Contest::findParticipant(std::string& name)
{
	int index = findParticipantIndex(winners, name);

	if (index != INVALID_INDEX)
	{
		std::cout << '>' << name << " is a winner" << std::endl;
	}
	else if ((index = findParticipantIndex(participants, name)) != INVALID_INDEX)
	{
		std::cout << '>' << name << " is a praticipant, index number: " << index << std::endl;
	}
	else
	{
		std::cout << '>' << name << " is not enlisted\n";
	}
}

void Contest::showParticipants()
{
	printPool(participants);
}

void Contest::showWinners()
{
	printPool(winners);
}

void Contest::rollWinners()
{
	if (participants.size() < amountOfWinnersToRoll)
	{
		std::cout << ">not enough participants\n";
	}
	else
	{
		for (int index = 0; index < amountOfWinnersToRoll; index++)
		{
			int randomNumber = std::rand() % participants.size();
		}
	}
}

// private

void Contest::printPool(std::vector<Participant*> vector)
{
	for (int index = 0; index < vector.size(); index++)
	{
		Participant* participant = vector[index];
		std::cout << index + 1 << ". " << participant->getName() << std::endl;
	}
}

int Contest::findParticipantIndex(std::vector<Participant*>& pool, std::string& name)
{
	for (int index = 0; index < pool.size(); index++)
	{
		if (pool[index]->getName() == name)
		{
			return index;
		}
	}

	return INVALID_INDEX;
}

void Contest::moveParticipant(std::vector<Participant*>& originalPool, std::vector<Participant*> targetPool, int index)
{
	targetPool.push_back(participants[index]);
	originalPool.erase(participants.begin() + index);
}
