#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <iostream>
#include <string>

using namespace std;

class GameEngine;

// State parent class
class State
{
protected:
    string name;

public:
    // Constructors
    State(string newName);
    State(const State& someState);
    State();

    // Virtual function for polymorphic behavior
    virtual void print(ostream& out) const {
        out << "Base State: " << name;
    }

    // Stream insertion operator
    friend ostream & operator<< (ostream &out, const State& state);

    // Abstract methods
    virtual string handleCommand(GameEngine& engine, const string& command) = 0;
    string getStateName() const;
};

// Game Engine, handles transitions and state validation
class GameEngine
{
private:
    State* currentState;

public:
    // Constructors and destructor
    GameEngine(State* initialState);
    GameEngine(GameEngine& engine);
    ~GameEngine();

    // Setter and Getter
    void setCurrentState(State* newState);
    State* getCurrentState() const;

    // Stream insertion operator
    friend ostream & operator<< (ostream &out, State* state);

    string handleCommand(const string& command);
};

// State declarations
class StartState : public State
{
public:
    // Constructors
    StartState();
    StartState(StartState& someState);

    // Overriden methods
    void print(ostream& out) const override;
    string handleCommand(GameEngine& engine, const string& command) override;
};

class MapLoadedState : public State
{
public:
    // Constructors
    MapLoadedState();
    MapLoadedState(MapLoadedState& someState);

    // Overriden methods
    void print(ostream& out) const override;
    string handleCommand(GameEngine& engine, const string& command) override;
};

class MapValidatedState : public State
{
public:
    // Constructors
    MapValidatedState();
    MapValidatedState(MapValidatedState& someState);

    // Overriden methods
    void print(ostream& out) const override;
    string handleCommand(GameEngine& engine, const string& command) override;
};

class PlayersAddedState : public State
{
public:
    // Constructors
    PlayersAddedState();
    PlayersAddedState(PlayersAddedState& someState);

    // Overriden methods
    void print(ostream& out) const override;
    string handleCommand(GameEngine& engine, const string& command) override;
};

class AssignReinforcementsState : public State
{
public:
    // Constructors
    AssignReinforcementsState();
    AssignReinforcementsState(AssignReinforcementsState& someState);

    // Overriden methods
    void print(ostream& out) const override;
    string handleCommand(GameEngine& engine, const string& command) override;
};

class IssueOrdersState : public State
{
public:
    // Constructors
    IssueOrdersState();
    IssueOrdersState(IssueOrdersState& someState);

    // Overriden methods
    void print(ostream& out) const override;
    string handleCommand(GameEngine& engine, const string& command) override;
};

class ExecuteOrdersState : public State
{
public:
    // Constructors
    ExecuteOrdersState();
    ExecuteOrdersState(ExecuteOrdersState& someState);

    // Overriden methods
    void print(ostream& out) const override;
    string handleCommand(GameEngine& engine, const string& command) override;
};

class WinState : public State
{
public:
    // Constructors
    WinState();
    WinState(WinState& someState);

    // Overriden methods
    void print(ostream& out) const override;
    string handleCommand(GameEngine& engine, const string& command) override;
};

#endif
