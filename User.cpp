#include "User.hpp"

bool User::isSubcribedToLottery(std::string& lotteryname)
{
	for (auto& lottery : lotteriesEntered)
	{
		if (lottery == lotteryname) return true;
	}

	return false;
}

void User::subscribeToLottery(std::string& lotteryname)
{
	if (!isSubcribedToLottery(lotteryname)) lotteriesEntered.emplace_back(lotteryname);
}

void User::unsubscribeToLottery(std::string& lotteryname)
{
	for (std::vector<std::string>::iterator iterator = lotteriesEntered.begin(); iterator != lotteriesEntered.end(); iterator++)
	{
		if (*iterator == lotteryname)
		{
			lotteriesEntered.erase(iterator);
			return;
		}
	}
}