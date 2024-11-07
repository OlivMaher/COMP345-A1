#include "CommandProcessing.h"
#include "../Game_Engine/GameEngine.h"

// ---- Command class Definition ----
Command::Command(const string& cmd) : command(cmd), effect("") {};
Command::Command(const Command& otherCommand) : command(otherCommand.command), effect(otherCommand.effect) {};

ostream & operator<< (ostream &out, const Command &cmd)
{
    out << "The name of this command is: " << cmd.command << "\n";
    out << "The effect of this command is: " << cmd.effect;
    return out;
}

// ---- CommandProcessor class definition ----
// Deep copy of vectors for constructors
CommandProcessor::CommandProcessor(std::vector<Command> cmds)
{
    copy(cmds.begin(), cmds.end(), back_inserter(commands));
}
CommandProcessor::CommandProcessor(const CommandProcessor& processor)
{
    copy(processor.commands.begin(), processor.commands.end(), back_inserter(processor.commands));
}

// Private methods
Command* CommandProcessor::readCommand()
{
    string someCommand;
    std::cout << "Enter Command: ";
    std::getline(std::cin, someCommand);
    return new Command(someCommand);
}

void CommandProcessor::saveCommand(const Command& cmd)
{
    commands.push_back(cmd);
    notify();
}

// Public methods
Command* CommandProcessor::getCommand()
{
    Command *aComand = readCommand();
    saveCommand(*aComand);
}

bool CommandProcessor::validate(const Command& cmd)
{
}

ostream& operator <<(ostream &out,const CommandProcessor& processor)
{
    for(auto i = processor.commands.begin(); i != processor.commands.end(); i++)
    {
        std::cout << "\nThe commands currently stored are: " << *i;
    }
}
