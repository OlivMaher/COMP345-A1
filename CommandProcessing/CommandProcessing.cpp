#include "CommandProcessing.h"

// ---- Command class Definition ----
Command::Command(const string& cmd) : command(cmd), effect("") {};
Command::Command(const Command& otherCommand) : command(otherCommand.command), effect(otherCommand.effect) {};

string Command::getCommand() const
{
    return command;
}

string Command::getEffect() const
{
    return effect;
}

void Command::saveEffect(const string &eff)
{
    effect = eff;
    notify();
}

ostream & operator<< (ostream &out, const Command &cmd)
{
    out << cmd.command << "\n";
    out << "The effect of this command is: " << cmd.effect << endl;
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
    return this->commands.back();
}

string CommandProcessor::validate()
{
    if (gameEngine && !commands.empty()) // Ensure commands is not empty
    {
        std::string result = gameEngine->handleCommand(commands.back().getCommand());
        if (result == "Ending game")
        {
            commands.back().saveEffect(result);
            return "ending";
        }
        else if (result != "!!! invalid command !!!")
        {
            commands.back().saveEffect(result);
            return "valid";
        }
        else
        {
            commands.back().saveEffect(result);
            return "invalid";
        }
    }
    else
    {
        return "invalid";
    }
}

ostream& operator <<(ostream &out,const CommandProcessor& processor)
{
    out << "\nThe commands currently stored are: ";
    for (const auto& cmd : processor.commands)
    {
        out << "\n" <<cmd; // This will call the operator<< for Command
    }
    return out;
}


// ---- FileLineReader class definition ----
FileLineReader::FileLineReader(const string& filename)
{
    file.open(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }
}

FileLineReader::~FileLineReader()
{
    if (file.is_open())
    {
        file.close();
    }
}

string FileLineReader::readLineFromFile()
{
    string line;
    if (getline(file, line))
    {
        return line;
    }
    return "";
}

bool FileLineReader::hasMoreCommands()
{
    return !file.eof();
}

// ---- FileCommandProcessorAdapter class definition ----
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string &filename, GameEngine *engine)
        : CommandProcessor(engine), fileReader(new FileLineReader(filename)) {}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    delete fileReader;
}