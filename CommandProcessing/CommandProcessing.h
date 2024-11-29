#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H

#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../LoggingObserver/LoggingObserver.h"
#include "../Game_Engine/GameEngine.h"

using std::string;
using std::ostream;

class Command : public Subject, public Iloggable
{
private:
    string command;
    string effect;

public:
    // Constructors
    Command(const string &cmd);
    Command(const Command &otherCommand);

    // Stream insertion operator
    friend ostream & operator<< (ostream &out, const Command& cmd);

    void saveEffect(const string &eff);    

    // Getter methods
    string getCommand() const;

    string getEffect() const;
    string stringToLog() const override
    {
        return "Command: " + command + ", Effect: " + effect;
    }
};

class CommandProcessor : public Subject, public Iloggable
{
private:
    vector<Command> commands;
    GameEngine *gameEngine;

protected:
    virtual string readCommand();

public:

    // Constructors
    CommandProcessor();
    CommandProcessor(GameEngine *engine);
    CommandProcessor(CommandProcessor &processor);
    
    // other methods
    void saveCommand(const Command& cmd);
    Command getCommand();
    string validate();
    bool processTournamentCommand(const Command& cmd);

    friend ostream& operator << (ostream &out, const CommandProcessor& processor);

    string stringToLog() const override{
        return "CommandProcessor: " + to_string(commands.size()) + " commands processed.";
    }
};

class FileLineReader
{
private:
    ifstream file;

public:
    // Constructors
    FileLineReader(const string &name);
    explicit FileLineReader(const FileLineReader &reader);

    // Destructor
    ~FileLineReader();

    // Other methods
    string readLineFromFile();

    bool hasMoreCommands();

};

class FileCommandProcessorAdapter : public CommandProcessor
{
private:
    FileLineReader *fileReader;

protected:
    // Overrides the readCommand() method to read from the file instead of the console
    string readCommand() override
    {
    if (fileReader && fileReader->hasMoreCommands())
    {
        return fileReader->readLineFromFile();
    }
    return ""; // No more commands to read
}

public:
    // Constructor to initialize FileLineReader with a filename
    FileCommandProcessorAdapter(const std::string &filename, GameEngine *engine);
    ~FileCommandProcessorAdapter();

};

#endif
