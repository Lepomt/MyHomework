#include <iostream>

#include "Coordinator.hpp"

int main()
{   
    Coordinator coordinator;
    
    while (coordinator.dispatchCommands()) {}

    return 0;
}

// FORMAT THE FILES
