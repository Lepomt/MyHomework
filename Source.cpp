#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<map>

#define SECONDS_IN_DAY 84600

enum privilages { banned, basicAccess, moderator, admin };

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

class User
{
private:
	std::vector<std::string> lotteriesEntered;
	std::string username;
	privilages accessType;

public:
	User(std::string& setUsername, privilages setAccessType) :
		username{ setUsername },
		accessType{ setAccessType }{}

	std::string& getName() { return username; }
	privilages getAccessType() { return accessType; }
	std::vector<std::string>& getLotteriesEntered() { return lotteriesEntered; }

	bool isSubcribedToLottery(std::string& lotteryname); // may be unecessary
	void subscribeToLottery(std::string& lotteryname);
	void unsubscribeToLottery(std::string& lotteryname);
};

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

void Ticket::unlinkRecipient(std::string& username)
{
	for (std::vector<std::string>::iterator iterator = recipients.begin(); iterator != recipients.end(); iterator++)
	{
		if (*iterator == username)
		{
			giftsToGive--;
			recipients.erase(iterator);
			return;
		}
	}
}

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

bool Lottery::hasExpired()
{
	if (isLotteryOpen && deadline < std::time(0))
	{
		std::cout << ">lottery has ended\n";
		return true;
	}
	return false;
}

bool Lottery::isOpen()
{
	if (!isLotteryOpen)
	{
		std::cout << ">lottery is closed\n"; return false;
	}
	else return true;
}

bool Lottery::isModerator(std::string& username)
{
	for (auto& moderator : moderatorList)
	{
		if (moderator == username) return true;
	}

	return false;
}

void Lottery::openLottery()
{
	if (isLotteryOpen)
	{
		std::cout << ">lottery is already open\n";
	}
	else if (deadline > std::time(0) && giftValue > 0 && giftsPerPerson > 0)
	{
		isLotteryOpen = true;
		std::cout << ">lottery opened\n";
	}
	else
	{
		std::cout << ">set correct values for the lottery\n";
	}
}

void Lottery::addModerator(std::string& username)
{
	for (auto& moderator : moderatorList)
	{
		if (moderator == username) return;
	}

	moderatorList.emplace_back(username);
}

void Lottery::listModerators()
{
	for (auto& moderator : moderatorList)
	{
		std::cout << moderator << std::endl;
	}
}

bool Lottery::ticketExist(std::string& username)
{
	if (ticketList.find(username) != ticketList.end()) return true;
	else
	{
		std::cout << ">ticket dosen't exist\n";
		return false;
	}
}

void Lottery::addTicket(std::string& username)
{
	if (!ticketExist(username))
	{
		std::cout << ">creating ticket\n";
		ticketList.emplace(std::piecewise_construct, std::forward_as_tuple(username), std::forward_as_tuple(username));
	}
}

void Lottery::printTicket(std::string& username)
{
	if (ticketExist(username))
	{
		std::cout << "  ~deadline: " << std::ctime(&deadline)
			<< "  ~gift value: " << giftValue << std::endl
			<< "  ~give presents to:\n";

		for (auto& recipiant : ticketList.at(username).recipients)
		{
			std::cout << "   -" << recipiant << std::endl;
		}
	}
}

void Lottery::listTickets()
{
	for (auto& ticket : ticketList)
	{
		std::cout << ticket.second.owner << std::endl
			<< ">to give: " << ticket.second.giftsToGive << std::endl
			<< ">to recieve: " << ticket.second.giftsToRecieve << std::endl
			<< ">gives presents to:\n";

		for (auto& recipiant : ticket.second.recipients)
		{
			std::cout << " -" << recipiant << std::endl;
		}

		std::cout << std::endl;
	}
}

void Lottery::removeTicket(std::string& username)
{
	if (ticketExist(username))
	{
		// unlink gifts to the user
		for (auto& ticket : ticketList)
		{
			ticket.second.unlinkRecipient(username);
		}

		// unlink gifts from recipients
		Ticket& currentTicket = ticketList.at(username);

		for (auto& recipiat : currentTicket.recipients)
		{
			ticketList.at(recipiat).giftsToRecieve--;
		}

		ticketList.erase(username);
	}
}

void Lottery::roll(std::string& username)
{
	std::vector<std::string*> pool;

	if (!ticketExist(username)) return;

	Ticket& currentTicket = ticketList.at(username);

	if (currentTicket.giftsToGive >= giftsPerPerson) return;

	// create a private pool
	for (auto& ticket : ticketList)
	{
		if (ticket.first != username && ticket.second.giftsToRecieve < giftsPerPerson)
		{
			pool.push_back(&ticket.second.owner);
		}
	}

	// roll from pool
	while (pool.size() > 0 && currentTicket.giftsToGive < giftsPerPerson)
	{
		int randomNumber = rand() % pool.size();
		currentTicket.giftsToGive++;
		currentTicket.recipients.emplace_back(*pool[randomNumber]);
		ticketList.at(*pool[randomNumber]).giftsToRecieve++;
		pool.erase(pool.begin() + randomNumber);
	}
}

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

int main()
{
	Coordinator coordinator;

	while (coordinator.getInput()) {}

	return 0;
}
