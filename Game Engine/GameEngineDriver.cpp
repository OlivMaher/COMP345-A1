#include "GameEngine.h"
#include <iostream>
#include <string>

using namespace std;

void testGameStates()
{
    GameEngine gameEngine(new StartState());

    string command;

    while(true)
    {
        cout << "\ncurrent state: " << *(gameEngine.getCurrentState());
        cout << "Enter command: ";
        getline(cin, command);

        if (gameEngine.getCurrentState()->getStateName() == "WinState" && command == "end")
        {
            cout << "\nthank you for playing!" << endl;
            break;
        }

        gameEngine.handleCommand(command);
    }
}

int main()
{
    testGameStates();
    return 0;
}