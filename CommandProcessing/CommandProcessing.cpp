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
// Constructors
CommandProcessor::CommandProcessor() : gameEngine(nullptr) {}
CommandProcessor::CommandProcessor(CommandProcessor& processor)
{
    for (const auto& cmd : processor.commands)
    {
        commands.push_back(cmd);
    }
    gameEngine = processor.gameEngine;
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
        if (result != "!!! invalid command !!!")
        {
            cmd.saveEffect(result);
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
    out << "\nThe commands currently stored are: ";
    for(auto i = processor.commands.begin(); i != processor.commands.end(); i++)
    {
        out << "\n" << *i;
    }
    return out;
}
