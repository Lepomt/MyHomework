#include "Contest.h"

class App
{
	private:
		Contest* contest;

		int stringIsNumber(std::string& string);
		void passSetCommand(std::stringstream& stream, std::string& token);

	public:
		App();
		~App() { delete contest; }

        int getInput();
};
