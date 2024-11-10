#include "CommandProcessing.h"

void testCommandProcessor()
{
    GameEngine engine(new StartState());
    GameEngine* engineptr = &engine;
    CommandProcessor cmdProcessor(engineptr);

    while(true)
    {
        cout << "\ncurrent state: " << *(engine.getCurrentState());

        cmdProcessor.getCommand();
        string result = cmdProcessor.validate();
        if (engine.getCurrentState()->getStateName() == "WinState" && result == "ending")
        {
            cout << "\nThank you for playing!" << endl;
            break;
        }
        else if (result == "invalid")
        {
            cout << "\nTry again: ";
            continue;
        }

        cout << cmdProcessor << endl;
    }

    engineptr = NULL;
}

int main()
{
    testCommandProcessor();
    return 0;
}