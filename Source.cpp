#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "Coordinator.hpp"

int main()
{
    std::srand(std::time(0));

    Coordinator coordinator;

    std::cout << "\n --- Secret Santa MK III\n\n";

    while (coordinator.getInput())
    {
    }

    return 0;
}
