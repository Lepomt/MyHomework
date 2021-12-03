#include <vector>
#include <string>

enum privilages { banned, basicAccess, moderator, admin };

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