#include "GameEngine.h"

// Game Egnine implementation
GameEngine::GameEngine(State* initialState) : currentState(initialState) {}
GameEngine::GameEngine(GameEngine& engine) : currentState(engine.currentState){}

GameEngine::~GameEngine() 
{
    delete currentState;
}

void GameEngine::setCurrentState(State* newState) 
{
    delete currentState;  // Free the old state
    currentState = newState;
}

State* GameEngine::getCurrentState() const
{
    return currentState;
}

ostream & operator<< (ostream &out, State* state)
{
    out << "The current state of the game is: " << state->getStateName() << endl;
    return out;
}

void GameEngine::handleCommand(const string& command) 
{
    if (currentState)
    {
        currentState->handleCommand(*this, command);
    }
}

// State logic implementation

// State class
State::State(string newName)
{
    this->name = newName;
}
State::State(const State& someState)
{
    this->name = someState.name;
}
State::State()
{
    this->name = "default name";
}

ostream & operator<< (ostream &out, const State& state)
{    
    out << "\n---- state info ----" << "\n";
    out << "the name of this class is: " << state.name << endl;
    state.print(out);
    return out;
}

string State::getStateName() const
{
    return this->name;
}

// StartState implementation
StartState::StartState()
{
    this->name = "Start";
}
StartState::StartState(StartState& someState)
{
    this->name = someState.name;
}

void StartState::print(ostream& out) const
{
    out << "The available commands for this state are:" << "\n";
    out << "loadmap" << "\n";
    out << "--------------------" << endl;
}

void StartState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "loadmap")
    {
        // load map command here
        cout << "\nloading map..." << "\n";
        engine.setCurrentState(new MapLoadedState()); // Transition state
        cout << "transitioning to MapLoadedState" << endl;
    }
    else
    {
        cout << "\n!!! invalid command !!!" << endl;
    }
}

// MapLoadedState implementation
MapLoadedState::MapLoadedState()
{
    this->name = "MapLoadedState";
}
MapLoadedState::MapLoadedState(MapLoadedState& someState)
{
    this->name = someState.name;
}

void MapLoadedState::print(ostream& out) const
{
    out << "The available commands for this state are:" << "\n";
    out << "loadmap" << "\n";
    out << "validatemap" << "\n";
    out << "--------------------" << endl;
}

void MapLoadedState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "loadmap")
    {
        // load map function here
        cout << "\nloading map" << endl;
    }
    else if (command == "validatemap")
    {
        //map validation function here
        cout << "\nvalidating map..." << "\n";
        engine.setCurrentState(new MapValidatedState());
        cout << "Transitioning to MapValidatedState" << endl;
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << endl;
    }
}

// MapValidatedState implementation
MapValidatedState::MapValidatedState()
{
    this->name = "MapValidatedState";
}
MapValidatedState::MapValidatedState(MapValidatedState& someState)
{
    this->name = someState.name;
}

void MapValidatedState::print(ostream& out) const
{
    out << "The available commands for this state are:" << "\n";
    out << "addplayer" << "\n";
    out << "--------------------" << endl;
}

void MapValidatedState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "addplayer")
    {
        // addplayer function here
        cout << "\nAdding player..." << "\n";
        engine.setCurrentState(new PlayersAddedState());
        cout << "Transitioning to PlayersAddedState" << endl;
    }
    else
    {
        cout << "\n!!! invalid command !!!" << endl;
    }
}

// PlayersAddedState implementation
PlayersAddedState::PlayersAddedState()
{
    this->name = "PlayersAddedState";
}
PlayersAddedState::PlayersAddedState(PlayersAddedState& someState)
{
    this->name = someState.name;
}

void PlayersAddedState::print(ostream& out) const
{
    out << "The available commands for this state are:" << "\n";
    out << "addplayer" << "\n";
    out << "assigncountries" << "\n";
    out << "--------------------" << endl;
}

void PlayersAddedState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "addplayer")
    {
        // load map function here
        cout << "\nAdding Player" << endl;
    }
    else if (command == "assigncountries")
    {
        //map validation function here
        cout << "\nassigning countries..." << "\n";
        engine.setCurrentState(new AssignReinforcementsState());
        cout << "Transitioning to AssignReinforcementsState" << endl;
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << endl;
    }
}

// AssignReinfocementState implementation
AssignReinforcementsState::AssignReinforcementsState()
{
    this->name = "AssignReinforcementsState";
}
AssignReinforcementsState::AssignReinforcementsState(AssignReinforcementsState& someState)
{
    this->name = someState.name;
}

void AssignReinforcementsState::print(ostream& out) const
{
    out << "The available commands for this state are:" << "\n";
    out << "issueorder" << "\n";
    out << "--------------------" << endl;
}

void AssignReinforcementsState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "issueorder")
    {
        // issue order function here
        cout << "\nIssueing order..." << "\n";
        engine.setCurrentState(new IssueOrdersState());
        cout << "Transitioning to IssueOrdersState" << endl;
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << endl;
    }
}

// IssueOrdersState implementation
IssueOrdersState::IssueOrdersState()
{
    this->name = "IssueOrdersState";
}
IssueOrdersState::IssueOrdersState(IssueOrdersState& someState)
{
    this->name = someState.name;
}

void IssueOrdersState::print(ostream& out) const
{
    out << "The available commands for this state are:" << "\n";
    out << "issueorder" << "\n";
    out << "endissueorders" << "\n";
    out << "--------------------" << endl;
}

void IssueOrdersState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "issueorder")
    {
        // issue order function here
        cout << "\nIssueing order" << endl;
    }
    else if (command == "endissueorders")
    {
        //end issue order function here
        cout << "\nending issueing orders..." << "\n";
        engine.setCurrentState(new ExecuteOrdersState());
        cout << "Transitioning to ExecuteOrdersState" << endl;
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << endl;
    }
}

// ExecuteOrdersState implementation
ExecuteOrdersState::ExecuteOrdersState()
{
    this->name = "ExecuteOrdersState";
}
ExecuteOrdersState::ExecuteOrdersState(ExecuteOrdersState& someState)
{
    this->name = someState.name;
}

void ExecuteOrdersState::print(ostream& out) const
{
    out << "The available commands for this state are:" << "\n";
    out << "execorder" << "\n";
    out << "endexecorder" << "\n";
    out << "win" << "\n";
    out << "--------------------" << endl;
}

void ExecuteOrdersState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "execorder")
    {
        // Execute order function here
        cout << "\nExecuting order..." << endl;
    }
    else if (command == "endexecorder")
    {
        // End execute order function here
        cout << "\nending executing orders..." << "\n";
        engine.setCurrentState(new AssignReinforcementsState());
        cout << "Transitioning to AssignReinforcementsState" << endl;
    }
    else if (command == "win")
    {
        // Win function here
        cout << "\nGame Over, you won!" << "\n";
        engine.setCurrentState(new WinState());
        cout << "Transitioning to WinState" << endl;
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << endl;
    }
}

// WinState implementation
WinState::WinState()
{
    this->name = "WinState";
}
WinState::WinState(WinState& someState)
{
    this->name = someState.name;
}

void WinState::print(ostream& out) const
{
    out << "The available commands for this state are:" << "\n";
    out << "end" << "\n";
    out << "play" << "\n";
    out << "--------------------" << endl;
}

void WinState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "play")
    {
        // reset game function?
        cout << "\nRestarting Game..." << "\n";
        engine.setCurrentState(new StartState());
        cout << "Transitioning to StartState" << endl;
    }
    else
    {
        cout << "\n!!! invalid command !!!" << endl;
    }
}