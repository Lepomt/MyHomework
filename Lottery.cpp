#include "Lottery.hpp"

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