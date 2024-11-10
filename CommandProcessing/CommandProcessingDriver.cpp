#include "CommandProcessing.h"

void testCommandProcessor(bool useFile, const string& filename = "")
{
    GameEngine engine(new StartState());
    GameEngine* engineptr = &engine;
    CommandProcessor *cmdProcessor;

    if (useFile)
    {
        cmdProcessor = new FileCommandProcessorAdapter(filename, engineptr);
    }
    else
    {
        cmdProcessor = new CommandProcessor(engineptr);
    }
    //CommandProcessor cmdProcessor(engineptr);

    while(true)
    {
        cout << "\ncurrent state: " << *(engine.getCurrentState());

        Command cmd = cmdProcessor->getCommand();

        if (cmd.getCommand().empty()) {
        std::cout << "\nNo more commands to process. Exiting.\n";
        break;
    }

        string result = cmdProcessor->validate();
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

        cout << *cmdProcessor << endl;
    }

    delete cmdProcessor;
    engineptr = NULL;
}

int main(int argc, char *argv[])
{
    if (argc > 1 && string(argv[1]) == "-file" && argc == 3)
    {
        testCommandProcessor(true, argv[2]);
    }
    else
    {
        testCommandProcessor(false);
    }
    return 0;
}