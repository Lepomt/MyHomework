#include "Coordinator.h"

constexpr size_t hash(const char* str)
{
	const long long p = 131;
	const long long m = 4294967291; // 2^32 - 5, largest 32 bit prime
	long long total = 0;
	long long current_multiplier = 1;

	for (int i = 0; str[i] != '\0'; ++i)
	{
		total = (total + current_multiplier * str[i]) % m;
		current_multiplier = (current_multiplier * p) % m;
	}

	return total;
}


Coordinator::Coordinator()
{
	std::string name = "admin";

	createUser(name, admin);
	switchUser(name);

	std::cout << std::endl;
}

bool Coordinator::userExists(std::string& username)
{
	if (userbase.find(username) != userbase.end()) return true;
	else
	{
		std::cout << ">user dosen't exist\n";
		return false;
	}
}

bool Coordinator::lotteryExists(std::string& lotteryname)
{
	if (lotterybase.find(lotteryname) != lotterybase.end()) return true;
	else
	{
		std::cout << ">lottery dosen't exists\n";
		return false;
	}
}

bool Coordinator::isCurrentUser()
{
	if (currentUser != nullptr) return true;
	else
	{
		std::cout << ">must switch to a user before using this command\n";
		return false;
	}
}

bool Coordinator::isCurrentLottery()
{
	if (currentLottery != nullptr) return true;
	else
	{
		std::cout << ">must switch to a lottery before using this command\n";
		return false;
	}
}

bool Coordinator::currentUserHasAccess(privilages access)
{
	if (isCurrentUser())
	{
		if (currentUser->getAccessType() >= access) return true;
		else
		{
			std::cout << ">insufficient access\n";
			return false;
		}
	}
}

bool Coordinator::currentUserHasLotteryAccess()
{
	if (isCurrentUser() && isCurrentLottery())
	{
		if (currentLottery->isModerator(currentUser->getName())) return true;
		else if (currentUserHasAccess(admin)) return true;
		else return false;
	}
}

void Coordinator::createUser(std::string& username, privilages accessType)
{
	if (!userExists(username))
	{
		std::cout << ">creating user\n";
		userbase.emplace(std::piecewise_construct, std::forward_as_tuple(username), std::forward_as_tuple(username, accessType));
	}
	else
	{
		std::cout << ">user already exists\n";
	}
}

void Coordinator::listUsers()
{
	for (auto& user : userbase)
	{
		std::cout << " -" << user.second.getName() << std::endl;
	}
}

void Coordinator::deleteUser(std::string& username)
{
	if (userExists(username))
	{
		User& user = userbase.at(username);

		if (currentUser->getName() == username) currentUser = nullptr;

		for (auto& lottery : user.getLotteriesEntered())
		{
			lotterybase.at(lottery).removeTicket(username);
		}
		userbase.erase(username);
	}
}

void Coordinator::switchUser(std::string& username)
{
	if (userExists(username))
	{
		currentUser = &userbase.at(username);
		std::cout << ">current user: " << currentUser->getName() << std::endl;
	}
}

void Coordinator::printUserInfo(std::string& username)
{
	User& user = userbase.at(username);
	std::vector<std::string>& subscriptions = user.getLotteriesEntered();

	std::cout << ">username: " << user.getName() << std::endl;
	if (subscriptions.empty())
	{
		std::cout << ">not lotteries entered\n";
	}
	else
	{
		std::cout << ">lotteries entered:\n";
		for (auto& subscription : subscriptions)
		{
			std::cout << " -" << subscription << std::endl;
			lotterybase.at(subscription).printTicket(user.getName());
		}
	}
}

void Coordinator::printUserInfo()
{
	printUserInfo(currentUser->getName());
}

void Coordinator::createLottery(std::string& loterryname)
{
	if (!lotteryExists(loterryname))
	{
		std::cout << ">creating lottery\n";
		lotterybase.emplace(std::piecewise_construct, std::forward_as_tuple(loterryname), std::forward_as_tuple(std::time(0) + 86400, loterryname, 1, 1));
	}
	else
	{
		std::cout << ">lottery already exists\n";
	}
}

void Coordinator::listLotteries(bool hiddenFlag)
{
	if (lotterybase.empty())
	{
		std::cout << ">no lotteries found\n";
	}
	else
	{
		for (auto& lottery : lotterybase)
		{
			if (lottery.second.getIsLotteryOpen()) std::cout << " -" << lottery.second.getName() << std::endl;
			else if (hiddenFlag) std::cout << " -" << lottery.second.getName() << " (closed)\n";
		}
	}
}

void Coordinator::deleteLottery(std::string& lotteryname)
{
	if (lotteryExists(lotteryname))
	{
		if (currentLottery->getName() == lotteryname) currentLottery = nullptr;

		for (auto& user : userbase)
		{
			user.second.unsubscribeToLottery(lotteryname);
		}
		lotterybase.erase(lotteryname);
	}
}

void Coordinator::switchLottery(std::string& lotteryname)
{
	if (lotteryExists(lotteryname))
	{
		currentLottery = &lotterybase.at(lotteryname);
		std::cout << ">current lottery: " << currentLottery->getName() << std::endl;
	}
}

void Coordinator::printLotteryInfo(std::string& lotteryname)
{
	if (lotteryExists(lotteryname))
	{
		std::time_t date = currentLottery->getDeadline();
		Lottery& lottery = lotterybase.at(lotteryname);
		std::cout << ">lotteryname: " << currentLottery->getName()
			<< "\n>deadline: " << std::ctime(&date)
			<< ">gift value: " << currentLottery->getGiftValue()
			<< "\n>gift per person: " << currentLottery->getGiftsPerPerson()
			<< std::endl << std::endl;
		if (currentUserHasAccess(moderator)) lottery.listTickets();
	}
}

void Coordinator::printLotteryInfo()
{
	printLotteryInfo(currentLottery->getName());
}

void Coordinator::setLotteryDeadline(int daysSinceToday)
{
	if (!currentLottery->getIsLotteryOpen() && !currentLottery->hasExpired()) currentLottery->setDeadline(std::time(0) + daysSinceToday * SECONDS_IN_DAY);
}

void Coordinator::setLotteryGiftsPerPerson(int giftsPerPerson)
{
	if (!currentLottery->getIsLotteryOpen() && !currentLottery->hasExpired()) currentLottery->setGiftsPerPerson(giftsPerPerson);
}

void Coordinator::setLotteryGiftValue(int giftValue)
{
	if (!currentLottery->getIsLotteryOpen() && !currentLottery->hasExpired()) currentLottery->setGiftValue(giftValue);
}

void Coordinator::addUserToLottery(std::string& username)
{
	if (userExists(username) && currentLottery->isOpen() && !currentLottery->hasExpired())
	{
		userbase.at(username).subscribeToLottery(currentLottery->getName());
		currentLottery->addTicket(username);
	}
}

void Coordinator::joinLottery()
{
	if (currentLottery->isOpen() && !currentLottery->hasExpired())
	{
		currentUser->subscribeToLottery(currentLottery->getName());
		currentLottery->addTicket(currentUser->getName());
	}
}

void Coordinator::roll()
{
	if (currentLottery->isOpen() && !currentLottery->hasExpired()) lotterybase.at(currentLottery->getName()).roll(currentUser->getName());
}

bool Coordinator::getInput()
{
	std::string buffer, token, extraToken;

	std::getline(std::cin, buffer);
	std::stringstream stream(buffer);
	std::getline(stream, token, ' ');

	auto tokenIsNumber = [&]()
	{
		for (auto letter : token)
		{
			if (!std::isdigit(letter))
			{
				std::cout << ">argument is not a number\n";
				return false;
			}
		}

		return true;
	};

	auto GetToken = [&]()
	{
		if (!stream.eof())
		{
			std::getline(stream, token, ' ');
			return true;
		}
		else
		{
			std::cout << ">expected an argument\n";
			return false;
		}
	};

	if (!buffer[0]) return false;

	switch (hash(&token[0]))
	{
	case hash("createuser"):
		if (GetToken()) createUser(token, basicAccess); break;

	case hash("userlist"):
		if (currentUserHasAccess(moderator)) listUsers(); break;

	case hash("switchuser"):
		if (GetToken()) switchUser(token); break;

	case hash("removeuser"):
		if (GetToken() && currentUserHasAccess(admin)) deleteUser(token); break;

	case hash("userinfo"):
		if (GetToken() && currentUserHasAccess(moderator)) printUserInfo(token);
		else if (isCurrentUser()) printUserInfo(); break;

	case hash("createlottery"):
		if (GetToken() && currentUserHasAccess(moderator)) createLottery(token); break;

	case hash("setdeadline"):
		if (GetToken() && currentUserHasLotteryAccess())
		{
			if (tokenIsNumber()) setLotteryDeadline(std::stoi(token));
		}
		break;

	case hash("setgifts"):
		if (GetToken() && currentUserHasLotteryAccess())
		{
			if (tokenIsNumber()) setLotteryGiftsPerPerson(std::stoi(token));
		}
		break;

	case hash("setgiftprice"):
		if (GetToken() && currentUserHasLotteryAccess())
		{
			if (tokenIsNumber()) setLotteryGiftValue(std::stoi(token));
		}
		break;

	case hash("addmod"):
		if (GetToken() && currentUserHasAccess(moderator) && isCurrentLottery()) currentLottery->addModerator(token); break;

	case hash("openlottery"):
		if (currentUserHasAccess(moderator) && isCurrentLottery()) currentLottery->openLottery(); break;

	case hash("lotterylist"):
		if (currentUserHasAccess(moderator)) listLotteries(true);
		else listLotteries(false); break;

	case hash("deletelottery"):
		if (GetToken() && currentUserHasAccess(moderator)) deleteLottery(token); break;

	case hash("switchlottery"):
		if (GetToken()) switchLottery(token); break;

	case hash("lotteryinfo"):
		if (GetToken()) printLotteryInfo(token);
		else if (isCurrentUser()) printLotteryInfo(); break;

	case hash("addtolottery"):
		if (GetToken() && isCurrentLottery() && currentUserHasAccess(moderator)) addUserToLottery(token); break;

	case hash("join"):
		if (isCurrentUser() && isCurrentLottery()) joinLottery(); break;

	case hash("roll"):
		if (isCurrentUser() && isCurrentLottery()) roll(); break;

	case hash("quit"):
		return false; break;

	default:
		std::cout << ">invalid command\n";
		break;
	}

	std::cout << std::endl;
	return true;
}