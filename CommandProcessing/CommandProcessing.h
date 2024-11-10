#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H


#include <iostream>
#include <string>
#include <vector>
#include "../LoggingObserver/LoggingObserver.h"
#include "../Game_Engine/GameEngine.h"

using std::string;
using std::ostream;

class Command /*: public Subject, public Iloggable*/
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
};

class CommandProcessor /*: public Subject, public Iloggable*/
{
private:
    vector<Command> commands;
    GameEngine *gameEngine;
    string readCommand();

public:

    // Constructors
    CommandProcessor();
    CommandProcessor(GameEngine *engine);
    CommandProcessor(CommandProcessor &processor);
    
    // other methods
    void saveCommand(const Command& cmd);
    Command getCommand();
    string validate();

    friend ostream& operator << (ostream &out, const CommandProcessor& processor);

    /*string stringToLog() const override {
        return "CommandProcessor: " + to_string(commands.size()) + "commands processed.";
    }*/
};

#endif