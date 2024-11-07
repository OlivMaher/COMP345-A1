#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H


#include <iostream>
#include <string>
#include <vector>
#include "../LoggingObserver/LoggingObserver.h"

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

    void saveEffect(const string &eff)
    {
        effect = eff;
        notify();
    }

    // Getter methods
    string getCommand() const
    {
        return command;
    }

    string getEffect() const
    {
        return effect;
    }
    string stringToLog() const override {
        return "Command: " + command + ", Effect: " + effect;
    }
};

class CommandProcessor : public Subject, public Iloggable
{
private:
    std::vector<Command> commands;
    Command* readCommand();

public:

    // Constructors
    CommandProcessor();
    CommandProcessor(const CommandProcessor& processor);
    CommandProcessor(std::vector<Command> cmds);
    
    // other methods
    void saveCommand(const Command& cmd);
    Command* getCommand();
    bool validate(const Command& cmd);

    friend ostream& operator << (ostream &out, const CommandProcessor& processor);

    string stringToLog() const override {
        return "CommandProcessor: " + to_string(commands.size()) + "commands processed.";
    }
};

#endif
