#include "Coordinator.hpp"
#include "Command.hpp"

int main()
{   
    Coordinator coordinator;

    do
    {
        coordinator.printCurrent();
    } while (coordinator.dispatch(Parser::getNewCommand()));

    return 0;
}
