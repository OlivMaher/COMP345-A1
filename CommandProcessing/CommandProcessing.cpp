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

bool CommandProcessor::processTournamentCommand(const Command& cmd) {
    istringstream iss(cmd.getCommand());
    string token;
    vector<string> tokens;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() < 9 || tokens[0] != "tournament") {
        cout << "Invalid tournament command format." << endl;
        return false;
    }

    vector<string> mapFiles;
    vector<string> playerStrategies;
    int tokenIndex = 2;

    // Parse map files
    for (; tokenIndex < tokens.size() && tokens[tokenIndex] != "-P"; ++tokenIndex) {
        mapFiles.push_back(tokens[tokenIndex]);
    }

    if (tokenIndex >= tokens.size() || tokens[tokenIndex] != "-P") {
        return false;
    }
    ++tokenIndex;

    // Parse player strategies
    for (; tokenIndex < tokens.size() && tokens[tokenIndex] != "-G"; ++tokenIndex) {
        playerStrategies.push_back(tokens[tokenIndex]);
    }

    if (tokenIndex >= tokens.size() || tokens[tokenIndex] != "-G") {
        return false;
    }
    ++tokenIndex;

    if (tokenIndex >= tokens.size()) {
        return false;
    }
    int numOfGames = stoi(tokens[tokenIndex++]);

    if (tokenIndex >= tokens.size() || tokens[tokenIndex] != "-D") {
        return false;
    }
    ++tokenIndex;

    if (tokenIndex >= tokens.size()) {
        return false;
    }
    int maxTurns = stoi(tokens[tokenIndex]);


    // Validate number of maps
    if (mapFiles.size() < 1 || mapFiles.size() > 5) {
        cout << "Invalid number of maps. Must be between 1 and 5." << endl;
        return false;
    }

    // Validate number of player strategies
    if (playerStrategies.size() < 2 || playerStrategies.size() > 4) {
        cout << "Invalid number of player strategies. Must be between 2 and 4." << endl;
        return false;
    }

    // Validate number of games
    if (numOfGames < 1 || numOfGames > 5) {
        cout << "Invalid number of games. Must be between 1 and 5." << endl;
        return false;
    }

    // Validate maximum number of turns
    if (maxTurns < 10 || maxTurns > 50) {
        cout << "Invalid number of turns. Must be between 10 and 50." << endl;
        return false;
    }

    // Call start tournament function
    gameEngine->startTournament(mapFiles, playerStrategies, numOfGames, maxTurns);

    return true;
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