#include "CommandProcessing.h"

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
CommandProcessor::CommandProcessor(GameEngine* engine) : gameEngine(engine){}

// Private methods
string CommandProcessor::readCommand()
{
    string cmd;
    cout << "Enter command: ";
    getline(std::cin, cmd);
    return cmd;
}

void CommandProcessor::saveCommand(const Command& cmd)
{
    commands.push_back(cmd);
    notify();
}

// Public methods
Command CommandProcessor::getCommand()
{
    string cmdString = readCommand();
    Command cmd(cmdString);
    saveCommand(cmd);
    return cmd;
}

bool CommandProcessor::validate(Command &cmd)
{
    if (gameEngine)
    {
        std::string result = gameEngine->handleCommand(cmd.getCommand());
        if (result == "Command Valid")
        {
            cmd.saveEffect("Command executed successfully");
            return true;
        }
        else
        {
            cmd.saveEffect(result); // Save the error or status message from the GameEngine
            return false;
        }
    }
    else
    {
        cmd.saveEffect("Game engine is not initialized");
        return false;
    }
}

ostream& operator <<(ostream &out,const CommandProcessor& processor)
{
    for(auto i = processor.commands.begin(); i != processor.commands.end(); i++)
    {
        std::cout << "\nThe commands currently stored are: " << *i;
    }
}
