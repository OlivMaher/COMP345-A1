#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::ostream;

class Command
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
};

class CommandProcessor
{
private:
    std::vector<Command> commands;
    Command* readCommand();

protected:
    void saveCommand(const Command& cmd);

public:

    // Constructors
    CommandProcessor(const CommandProcessor& processor);
    CommandProcessor(std::vector<Command> cmds);
    
    // other methods
    Command* getCommand();
    bool validate(const Command& cmd);

    friend ostream& operator << (ostream &out, const CommandProcessor& processor);
};

#endif
