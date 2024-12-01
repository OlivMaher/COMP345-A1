#include "GameEngine.h"
#include "../CommandProcessing/CommandProcessing.h"
#include "../PlayerStrategies/PlayerStrategies.h"
#include "../Map/Map.h"
#include <algorithm>
#include <random>
#include <iomanip>

using namespace std;

// Game Egnine implementation
GameEngine::GameEngine(State* initialState) : currentState(initialState) {
    gameMap = nullptr;
    deck = nullptr;
}
GameEngine::GameEngine(GameEngine& engine) : currentState(engine.currentState){gameMap = engine.gameMap;
    players = engine.players;
    deck = engine.deck;}

GameEngine::~GameEngine() 
{
    delete currentState;


    delete deck;
    deck = nullptr;
    for (Player* player : players) {
        delete player;
    }
    players.clear();
}

void GameEngine::setCurrentState(State* newState) 
{
     // Free the old state
    currentState = newState;
    notify();
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

string GameEngine::handleCommand(const string& command) 
{
    if (currentState)
    {
        return currentState->handleCommand(*this, command);
        notify();
    }
    return "No current State available";
}
string GameEngine::stringToLog() const{
    return "GameEngine current state: " + currentState->getStateName();
}

void GameEngine::mainGameLoop(){
    bool gameOver = false;
    while(!gameOver){
        cout << "Starting round" << endl;
        setCurrentState(new IssueOrdersState());
        issueOrdersPhase();
        setCurrentState(new ExecuteOrdersState());
        cout<< "Executing Orders" << endl;
        executeOrdersPhase();

        //remove players with no territories
        for (int i = 0; i< players.size(); i++) {
            if (players[i]->getTerritories().size() == 0){
                cout<< "Player " << players[i]->getName() << " has been eliminated." << endl;
                players.erase(players.begin() + i);
                i--;
            }
        }

        if(players.size() == 1){
            break;
        }

        setCurrentState(new AssignReinforcementsState());
        reinforcementPhase();
    }

    cout << "Player " << players[0]->getName() << " has won the game!" << endl;

}

void GameEngine::reinforcementPhase(){

for (int i = 0; i< players.size(); i++) { //assuming a vector of pointers to players exist

//add to check if territories are null
    vector<Territory*> ters = players[i]->getTerritories();
    int ters_owned = ters.size();
    int rein_num = static_cast<int>(ters_owned /3);
    vector <string> cont_names;
    //find if player owns an entire continent
    for (int j = 0; j< ters.size(); j++){

        //makes sure oyu havent checked that continent already for ownership
        string temp_name = ters[j]->getContinent();
        if (find(cont_names.begin(),cont_names.end(), temp_name) != cont_names.end()) {
            continue;
        } else {
            cont_names.push_back(temp_name);
        }
        //check each continent on that territory for ownership
        shared_ptr<Continent> cont = gameMap-> findContinentByName(temp_name);
        vector<shared_ptr<Territory>> cont_ters = cont->getTerritories();
        string name = players[i]-> getName();

        int needed = cont_ters.size();
        int score = 0;
        for (int k = 0; k< needed; k++){
            if (cont_ters[k]->getName() == name){
                needed += 1;
                if (score ==needed){
                    //if player owns every territory on that continent give its reinforcements
                    rein_num += cont->getBonus();
                }
            }
        }
            
    }
    players[i]->setReinforcementPool( max(3, rein_num));
    cout << "\nPlayer " << players[i]->getName() << " has recieved " << players[i]->getReinforcementPool() << " reinforcements." << endl;
}

}

void GameEngine::issueOrdersPhase(){
    cout<< "Issuing Orders" << endl;
    for (int i = 0; i< players.size(); i++) { //assuming a vector of pointers to players exist
        players[i]-> issueOrder(deck);
        cout<< "Player " << players[i]->getName() << " has issued orders." << endl;
    }
}

void GameEngine::executeOrdersPhase() {
    //keeps track of who has executed all deploy orders
    vector<Player*> doneDeploying;
    doneDeploying.clear(); 
    //holds all playerOrders
    vector<vector<Order*>> playerOrders(players.size());
    playerOrders.clear(); 
    vector<int> ordersExecuted(players.size(), 0);
    ordersExecuted.clear();

     // Fill the vector with each player's order list
    for (int i = 0; i < players.size(); ++i) {
        playerOrders[i] = players[i]->getOrders();
    }


    
    while (doneDeploying.size() < players.size()) {

        doneDeploying.clear(); // Clear the vector at the beginning of each iteration
      
        for (int i = 0; i < players.size(); i++) {
            // Skip players who are done deploying
            if (find(doneDeploying.begin(), doneDeploying.end(), players[i]) != doneDeploying.end()) {
                continue;
            }
            Player* player = players[i];
            Order* order = nullptr;
            //if there are still orders left to execute for the player get next order
            if (ordersExecuted[i] < playerOrders[i].size()) {
                order = playerOrders[i][ordersExecuted[i]];
            }
            
            if (order != nullptr) {
                // Check if the order is a Deploy order by its description and execute it
                if (order->getDescription() == "Deploy") {
                    cout << "Executing Deploy order for player: " << player->getName() << endl;
                    order->execute();
                    ordersExecuted[i]++;
                } else {
                    cout<< "No deploy orders left." << endl;
                    doneDeploying.push_back(player);
                }

            } else {
                cout<< "No deploy orders left." << endl;
                doneDeploying.push_back(player);
            }

        }
                
    }
        

   vector<Player*> doneExecuting;
    doneExecuting.clear();

    cout << "Executing other orders..." << endl;
    
    bool deployOrdersLeft = true;
    while (deployOrdersLeft) {
        deployOrdersLeft = false;
        for (Player* player : players) {
            OrdersList* ordersList = player->getOrdersList();
            // Find and execute the next Deploy order
            for (int i = 0; i < ordersList->getOrders().size(); ++i) {
                Order* order = ordersList->getOrders()[i];
                if (order->getDescription() == "Deploy") {
                    cout << "Executing Deploy order for player: " << player->getName() << endl;
                    order->execute();
                    ordersList->remove(i); // removes and deletes the order
                    deployOrdersLeft = true;
                    break; // move to next player
                }
            }
        }
    }

    // Now execute other orders in round-robin fashion
    bool ordersLeft = true;
    while (ordersLeft) {
        ordersLeft = false;
        for (Player* player : players) {
            OrdersList* ordersList = player->getOrdersList();
            if (!ordersList->getOrders().empty()) {
                Order* order = ordersList->getOrders().front();
                cout << "Executing order of type: " << order->getDescription() << " for player: " << player->getName() << endl;
                order->execute();
                ordersList->remove(0); // removes and deletes the order
                ordersLeft = true;
            }
        }
    }
    
}

void GameEngine::startTournament(const vector<string>& mapFiles, const vector<string>& playerStrategies, int numOfGames, int maxTurns){
    cout << "Starting Tournament Mode..." << endl;
    cout << "M: ";
    for (const auto& map: mapFiles){
        cout << map << " ";
    }
    cout << "\nP: ";
    for (const auto& playerStrat: playerStrategies){
        cout << playerStrat << " ";
    }
    cout << "\nG: " << numOfGames << "\nD: " << maxTurns;

    //Storage for results
    vector<vector<string>> results(mapFiles.size(), vector<string>(numOfGames, "Draw"));
    for(size_t i = 0; i < mapFiles.size(); ++i){

        MapLoader maploader;
        string mapFile = "./Map/MapFiles/"+mapFiles[i];
        cout << mapFile << endl;
        shared_ptr<Map> gameMap = maploader.loadMap(mapFile);

        if(!gameMap){
            cout << "Error: Couldn't open map file";
            continue;
        }

        for(int j =0; j < numOfGames; j++){
            
            vector<Player*> players;
            for(const auto& strategy: playerStrategies){
                Player* player = new Player(strategy);
                if (strategy == "Aggressive"){
                    player->setStrategy(new AggressivePlayerStrategy());
                }
                else if (strategy == "Benevolent"){
                    player->setStrategy(new BenevolentPlayerStrategy());
                }
                else if (strategy == "Neutral"){
                    player->setStrategy(new NeutralPlayerStrategy());
                }
                else if(strategy == "Cheater"){
                    player->setStrategy(new CheaterPlayerStrategy());
                }
                players.push_back(player);
            }

                        // Initialize the game engine
            GameEngine* gameEngine = new GameEngine(new StartState());
            gameEngine->setMap(gameMap);
            gameEngine->setPlayers(players);
            gameEngine->deck = new Deck();

            // Distribute territories among players
            gameEngine->distributeTerritories();

            // Set initial reinforcement pools
            for (Player* player : players) {
                player->setReinforcementPool(50);
            }

            // Draw initial cards
            for (Player* player : players) {
                player->getHand()->takeCard(*gameEngine->deck);
                player->getHand()->takeCard(*gameEngine->deck);
            }

            // Game loop
            bool gameOver = false;
            int turn = 0;
            while (!gameOver && turn < maxTurns) {
                // Reinforcement Phase
                gameEngine->reinforcementPhase();

                // Issue Orders Phase
                gameEngine->issueOrdersPhase();

                // Execute Orders Phase
                gameEngine->executeOrdersPhase();

                // After each turn or when checking for eliminated players
                for (auto it = players.begin(); it != players.end(); ) {
                    if ((*it)->getTerritories().empty()) {
                        delete *it; // Free the memory
                        it = players.erase(it); // Remove from the vector and update the iterator
                    } else {
                        ++it;
                    }
                }

                // Check for a winner
                if (gameEngine->getPlayers().size() == 1) {
                    gameOver = true;
                }

               ++turn;
            }

            // Determine the result
            string result;
            if (gameOver) {
                result = gameEngine->getPlayers()[0]->getName();
            } else {
                result = "Draw";
            }

            // Record the result
            results[i][j] = result;

            // Clean up
            delete gameEngine->deck;
            for (size_t i = 0; i < players.size(); ++i)
            {
                if (players[i] != nullptr)
                {
                    delete players[i];
                    players[i] = nullptr;
                }
            }
            players.clear();
            delete deck;
            deck = nullptr;
        }
    }

    cout << "\nTournament Results:\n";
    cout << setw(15) << " " << setw(15) << "Game 1";
    for (int g = 1; g < numOfGames; ++g) {
        cout << setw(15) << "Game " + to_string(g + 1);
    }
    cout << endl;

    for (size_t i = 0; i < mapFiles.size(); ++i) {
        cout << setw(15) << mapFiles[i];
        for (int j = 0; j < numOfGames; ++j) {
            cout << setw(15) << results[i][j];
        }
        cout << endl;
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

string StartState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "loadmap")
    {
        // load map command here
        cout << "\nloading map..." << "\n";
        engine.setCurrentState(new MapLoadedState()); // Transition state
        cout << "transitioning to MapLoadedState" << endl;
        return "loading map and state transitioning to MaploadedState";
    }
    else
    {
        cout << "\n!!! invalid command !!!" << command <<  endl;
        return "!!! invalid command !!!";
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

string MapLoadedState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "loadmap")
    {
        // load map function here
        cout << "\nloading map" << endl;
        return "loading map";
    }
    else if (command == "validatemap")
    {
        //map validation function here
        cout << "\nvalidating map..." << "\n";
        engine.setCurrentState(new MapValidatedState());
        cout << "Transitioning to MapValidatedState" << endl;
        return "validating map and State transitioning to MapValidatedState";
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << command << endl;
        return "!!! invalid command !!!";
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

string MapValidatedState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "addplayer")
    {
        // addplayer function here
        cout << "\nAdding player..." << "\n";
        engine.setCurrentState(new PlayersAddedState());
        cout << "Transitioning to PlayersAddedState" << endl;
        return "Adding player and State transitioning to PlayersAddedState";
    }
    else
    {
        cout << "\n!!! invalid command !!!" << command <<  endl;
        return "!!! invalid command !!!";
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
    out << "gamestart" << "\n";
    out << "--------------------" << endl;
}

string PlayersAddedState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "addplayer")
    {
        // load map function here
        cout << "\nAdding Player" << endl;
        return "Adding player";
    }
    else if (command == "assigncountries")
    {
        //map validation function here
        cout << "\nassigning countries..." << "\n";
        engine.setCurrentState(new AssignReinforcementsState());
        cout << "Transitioning to AssignReinforcementsState" << endl;
        return "Assigning countries and State transitioning to AssignReinforcementsState";
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << command << endl;
        return "!!! invalid command !!!";
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

string AssignReinforcementsState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "issueorder")
    {
        // issue order function here
        cout << "\nIssueing order..." << "\n";
        engine.setCurrentState(new IssueOrdersState());
        cout << "Transitioning to IssueOrdersState" << endl;
        return "Issueing order and State transitioning to IssueOrdersState";
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << command <<  endl;
        return "!!! invalid command !!!";
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
    out << "endissueorder" << "\n";
    out << "--------------------" << endl;
}

string IssueOrdersState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "issueorder")
    {
        // issue order function here
        cout << "\nIssueing order" << endl;
        return "Issueing order";
    }
    else if (command == "endissueorder")
    {
        //end issue order function here
        cout << "\nending issueing orders..." << "\n";
        engine.setCurrentState(new ExecuteOrdersState());
        cout << "Transitioning to ExecuteOrdersState" << endl;
        return "Ending issueing orders and State transitioning to ExecuteOrdersState";
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << command << endl;
        return "!!! invalid command !!!";
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

string ExecuteOrdersState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "execorder")
    {
        // Execute order function here
        cout << "\nExecuting order..." << endl;
        return "Executing order";
    }
    else if (command == "endexecorder")
    {
        // End execute order function here
        cout << "\nending executing orders..." << "\n";
        engine.setCurrentState(new AssignReinforcementsState());
        cout << "Transitioning to AssignReinforcementsState" << endl;
        return "Ending executing orders and State transitioning to AssignReinforcementsState";
    }
    else if (command == "win")
    {
        // Win function here
        cout << "\nGame Over, you won!" << "\n";
        engine.setCurrentState(new WinState());
        cout << "Transitioning to WinState" << endl;
        return "Game won and State Transitioning to WinState";
    }    
    else
    {
        cout << "\n!!! invalid command !!!" << command <<  endl;
        return "!!! invalid command !!!";
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

string WinState::handleCommand(GameEngine& engine, const string& command)
{
    if (command == "play")
    {
        // reset game function?
        cout << "\nRestarting Game..." << "\n";
        engine.setCurrentState(new StartState());
        cout << "Transitioning to StartState" << endl;
        return "Restarting game and State Transitioning to StartState";
    }
    else if (command == "end")
    {
        // end game function
        cout << "\nEnding Game..." << "\n";
        return "Ending game";
    }
    else
    {
        cout << "\n!!! invalid command !!!" << command << endl;
        return "!!! invalid command !!!";
    }
}
void GameEngine::startupPhase()
{
    // Initialize gameMap, players, and deck
    gameMap = nullptr;
    players.clear();
    deck = new Deck(); // Assuming Deck has a default constructor

    // Create a CommandProcessor to read commands from the console
    CommandProcessor commandProcessor(this);

    bool gameStarted = false;

    while (!gameStarted) {
        // Display current state and available commands
        cout << "\nCurrent State: " << currentState->getStateName() << endl;
        currentState->print(cout);

        // Get command from CommandProcessor
        Command cmd = commandProcessor.getCommand();
        string command = cmd.getCommand();

        // Process the command
        if (command.substr(0, 7) == "loadmap") {
            // Extract filename
            if (command.length() <= 8) {
                cout << "Please specify a map file to load." << endl;
                continue;
            }
            string filename = command.substr(8);

            // Load map
            MapLoader mapLoader;
            shared_ptr<Map> loadedMap = mapLoader.loadMap(filename);
            if (loadedMap != nullptr) {
               // Delete previous map if any
                gameMap = loadedMap;
                cout << "Map '" << filename << "' loaded successfully." << endl;
                setCurrentState(new MapLoadedState());
            } else {
                cout << "Failed to load map '" << filename << "'." << endl;
            }
        }
        else if (command == "validatemap") {
            if (gameMap != nullptr) {
                if (gameMap->validateMap()) {
                    cout << "Map is valid." << endl;
                    setCurrentState(new MapValidatedState());
                } else {
                    cout << "Map is invalid." << endl;
                }
            } else {
                cout << "No map loaded. Please load a map first." << endl;
            }
        }
        else if (command.substr(0, 9) == "addplayer") {
            // Extract player name
            if (command.length() <= 10) {
                cout << "Please specify a player name." << endl;
                continue;
            }
            string playerName = command.substr(10);
            if (players.size() < 6) {
                Player* player = new Player(playerName);
                players.push_back(player);
                cout << "Player '" << playerName << "' added." << endl;
                setCurrentState(new PlayersAddedState());
            } else {
                cout << "Maximum number of players reached (6)." << endl;
            }
        }
        else if (command == "gamestart") {
            if (players.size() >= 2) {
                // Proceed to start the game
                cout << "Starting the game..." << endl;

                // a) Fairly distribute all the territories to the players
                vector<Territory*> allTerritories;
                for (auto& territory : gameMap->getTerritories()) {
                    allTerritories.push_back(territory.get());
                }
                // Shuffle territories
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(allTerritories.begin(), allTerritories.end(), g);

                // Distribute territories
                size_t numPlayers = players.size();
                for (size_t i = 0; i < allTerritories.size(); ++i) {
                    Player* player = players[i % numPlayers];
                    player->addTerritory(allTerritories[i]);
                    allTerritories[i]->setOwner(player);
                }

                // b) Determine randomly the order of play
                std::shuffle(players.begin(), players.end(), g);
                cout << "Order of play:" << endl;
                for (size_t i = 0; i < players.size(); ++i) {
                    cout << i + 1 << ". " << players[i]->getName() << endl;
                }

                // c) Give 50 initial army units to the players
                for (Player* player : players) {
                    player->setReinforcementPool(50);
                }

                // d) Let each player draw 2 initial cards from the deck
                for (Player* player : players) {
                    player->getHand()->takeCard(*deck);
                    player->getHand()->takeCard(*deck);
                }

                // e) Switch the game to the play phase
                setCurrentState(new AssignReinforcementsState());
                cout << "Game has started! The game is now in the play phase." << endl;

                gameStarted = true;
            } else {
                cout << "Not enough players to start the game. Need at least 2 players." << endl;
            }
        }
        else {
            cout << "Invalid command: " << command << endl;
        }
    }
}

shared_ptr<Map> GameEngine::getMap() {
    return gameMap;
}
vector<Player*>& GameEngine::getPlayers() {
    return players;



}
void GameEngine::setMap(shared_ptr<Map> map) {
    this->gameMap = map;
}

void GameEngine::setPlayers(vector<Player*>& players) {
    this->players = players;
}

void GameEngine::distributeTerritories() {
    // Collect all territories from the map
    vector<Territory*> allTerritories;
    for (auto& territory : gameMap->getTerritories()) {
        allTerritories.push_back(territory.get());
    }

    // Shuffle territories
    random_device rd;
    mt19937 g(rd());
    shuffle(allTerritories.begin(), allTerritories.end(), g);

    // Distribute territories to players
    size_t numPlayers = players.size();
    for (size_t i = 0; i < allTerritories.size(); ++i) {
        Player* player = players[i % numPlayers];
        player->addTerritory(allTerritories[i]);
        allTerritories[i]->setOwner(player);
    }
}

