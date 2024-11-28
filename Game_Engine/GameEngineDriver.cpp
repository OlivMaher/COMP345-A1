#include "GameEngine.h"
#include "GameEngineDriver.h"
#include <iostream>
#include <string>

using namespace std;

void testStartupPhase()
{
    // Create a GameEngine instance with an initial state
    GameEngine gameEngine(new StartState());

    // Start the startup phase
    gameEngine.startupPhase();

    // After the startup phase, display the game state to demonstrate steps 1-4

    // Display loaded map validation status
    if (gameEngine.getMap() != nullptr && gameEngine.getMap()->validateMap()) {
        cout << "\nMap has been successfully loaded and validated.\n" << endl;
    } else {
        cout << "\nMap is not loaded or invalid.\n" << endl;
    }

    // Display the list of players
    vector<Player*>& players = gameEngine.getPlayers();
    if (!players.empty()) {
        cout << "Players in the game:" << endl;
        for (Player* player : players) {
            cout << "- " << player->getName() << endl;
        }
    } else {
        cout << "No players have been added to the game." << endl;
    }

    // Display the territories assigned to each player
    cout << "\nTerritories assigned to each player:" << endl;
    for (Player* player : players) {
        cout << "\nPlayer: " << player->getName() << endl;
        cout << "Reinforcement Pool: " << player->getReinforcementPool() << endl;

        cout << "Hand: ";
        player->getHand()->showHand();

        cout << "Territories: ";
        const vector<Territory*>& territories = player->toDefend();
        for (Territory* territory : territories) {
            cout << territory->getName() << ", ";
        }
        cout << endl;
    }

    // Indicate that the game is ready to start
    cout << "\nThe game has successfully moved to the play phase.\n" << endl;
}

void testMainGameLoop()
{
     // Create a GameEngine instance with an initial state
    GameEngine gameEngine(new StartState());

    // Start the startup phase
    gameEngine.startupPhase();


    // After the startup phase, display the game state to demonstrate steps 1-4

    // Display loaded map validation status
    if (gameEngine.getMap() != nullptr && gameEngine.getMap()->validateMap()) {
        cout << "\nMap has been successfully loaded and validated.\n" << endl;
    } else {
        cout << "\nMap is not loaded or invalid.\n" << endl;
    }

    gameEngine.mainGameLoop();
}

int main()
{
    //testStartupPhase();
    testMainGameLoop();
    return 0;
}
