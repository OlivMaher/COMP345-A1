#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Orders_list/Orders.h"
#include "../Cards/Cards.h"

#include <iostream>
#include <vector>

/*
g++ ../CommandProcessing/CommandProcessing.cpp PlayerStrategies.cpp ../Map/Map.cpp ../Player/Player.cpp ../Cards/Cards.cpp ../Orders_list/Orders.cpp ../Game_Engine/GameEngine.cpp ../LoggingObserver/LoggingObserver.cpp PlayerStrategiesDriver.cpp
*/

void displayTerritoryState(Player* player) {
    cout << "\nTerritories owned by " << player->getName() << ":\n";
    for (Territory* t : player->getTerritories()) {
        cout << t->getName() << " (" << t->getArmies() << " units)" << endl;
    }
    cout << endl;
}

void displayReinforcements(Player* player) {
    cout << player->getName() << " has " << player->getReinforcementPool() << " reinforcements available.\n";
}

void displayOrders(Player* player) {
    cout << "\nOrders issued by " << player->getName() << ":\n";
    for (Order* order : player->getOrders()) {
        cout << *order << endl;
    }
    cout << endl;
}

void testPlayerStrategies() {
    cout << "Testing Player Strategies\n";
    cout << "==========================\n";


    Player* humanPlayer = new Player("Human");
    Player* aggressivePlayer = new Player("Aggressive");
    Player* benevolentPlayer = new Player("Benevolent");
    Player* neutralPlayer = new Player("Neutral");
    Player* cheaterPlayer = new Player("Cheater");


    humanPlayer->setStrategy(new HumanPlayerStrategy());
    aggressivePlayer->setStrategy(new AggressivePlayerStrategy());
    benevolentPlayer->setStrategy(new BenevolentPlayerStrategy());
    neutralPlayer->setStrategy(new NeutralPlayerStrategy());
    cheaterPlayer->setStrategy(new CheaterPlayerStrategy());


    humanPlayer->setReinforcementPool(10);
    aggressivePlayer->setReinforcementPool(10);
    benevolentPlayer->setReinforcementPool(10);
    neutralPlayer->setReinforcementPool(10);
    cheaterPlayer->setReinforcementPool(10);


    shared_ptr<Territory> t1 = make_shared<Territory>("HumanTerritory1");
    shared_ptr<Territory> t2 = make_shared<Territory>("HumanTerritory2");
    shared_ptr<Territory> t3 = make_shared<Territory>("HumanTerritory3");
    shared_ptr<Territory> t4 = make_shared<Territory>("AggressiveTerritory1");
    shared_ptr<Territory> t5 = make_shared<Territory>("AggressiveTerritory2");
    shared_ptr<Territory> t6 = make_shared<Territory>("NeutralTerritory1");
    shared_ptr<Territory> t7 = make_shared<Territory>("CheaterTerritory1");
    shared_ptr<Territory> t8 = make_shared<Territory>("BenevolentTerritory1");


    t1->setOwner(humanPlayer); t1->setArmies(5);
    t2->setOwner(humanPlayer); t2->setArmies(3);
    t3->setOwner(humanPlayer); t3->setArmies(2);

    t4->setOwner(aggressivePlayer); t4->setArmies(6);
    t5->setOwner(aggressivePlayer); t5->setArmies(4);

    t6->setOwner(neutralPlayer); t6->setArmies(3);

    t7->setOwner(cheaterPlayer); t7->setArmies(10);
    t8->setOwner(benevolentPlayer); t8->setArmies(10);
    humanPlayer->addTerritory(t1.get());
    humanPlayer->addTerritory(t2.get());
    humanPlayer->addTerritory(t3.get());

    aggressivePlayer->addTerritory(t4.get());
    aggressivePlayer->addTerritory(t5.get());

    neutralPlayer->addTerritory(t6.get());

    cheaterPlayer->addTerritory(t7.get());

 
    t1->addAdjacentTerritories(t2);
    t2->addAdjacentTerritories(t1);
    t1->addAdjacentTerritories(t4);
    t4->addAdjacentTerritories(t1);
    t4->addAdjacentTerritories(t2);
    t2->addAdjacentTerritories(t4);
    t5->addAdjacentTerritories(t3);
    t3->addAdjacentTerritories(t5);
    t7->addAdjacentTerritories(t1);
    t1->addAdjacentTerritories(t7);


    Deck* deck = new Deck();


    cout << "Human Player's Turn:\n";
    cout << "====================\n";
    displayTerritoryState(humanPlayer);
    displayReinforcements(humanPlayer);
    humanPlayer->issueOrder(deck);
    displayOrders(humanPlayer);

    cout << "Aggressive Player's Turn:\n";
    cout << "=========================\n";
    displayTerritoryState(aggressivePlayer);
    displayReinforcements(aggressivePlayer);
    aggressivePlayer->issueOrder(deck);
    displayOrders(aggressivePlayer);

    cout << "Benevolent Player's Turn:\n";
    cout << "==========================\n";
    displayTerritoryState(benevolentPlayer);
    displayReinforcements(benevolentPlayer);
    benevolentPlayer->issueOrder(deck);
    displayOrders(benevolentPlayer);

    cout << "Neutral Player's Turn:\n";
    cout << "=======================\n";
    displayTerritoryState(neutralPlayer);
    //neutralPlayer->issueOrder(deck);
    displayOrders(neutralPlayer);

    cout << "Cheater Player's Turn:\n";
    cout << "=======================\n";
    displayTerritoryState(cheaterPlayer);
    cheaterPlayer->issueOrder(deck);
    displayOrders(cheaterPlayer);

    cout << "\nChanging Neutral Player to Aggressive...\n";
    neutralPlayer->setStrategy(new AggressivePlayerStrategy());
    displayReinforcements(neutralPlayer);
    neutralPlayer->issueOrder(deck);
    displayOrders(neutralPlayer);

    delete humanPlayer;
    delete aggressivePlayer;
    delete benevolentPlayer;
    delete neutralPlayer;
    delete cheaterPlayer;
    delete deck;
}

int main() {
    testPlayerStrategies();
    return 0;
}