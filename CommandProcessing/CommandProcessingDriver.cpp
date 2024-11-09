#include "CommandProcessing.h"

void testCommandProcessor()
{
    GameEngine engine(new StartState());
    GameEngine* engineptr = &engine;
    CommandProcessor cmdProcessor(engineptr);

    while(true)
    {
        cout << "\ncurrent state: " << *(engine.getCurrentState());
        cout << "Enter command: ";

        Command newCommand = cmdProcessor.getCommand();
        if (!cmdProcessor.validate(newCommand))
        {
            cout << "\nTry again: ";
            continue;
        }
    }

    delete engineptr;
    engineptr = NULL;
}

int main()
{
    testCommandProcessor();
    return 0;
}