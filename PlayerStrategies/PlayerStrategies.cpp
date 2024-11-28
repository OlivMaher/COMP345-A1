
#include <iostream>
#include <vector>


#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Orders_list/Orders.h"
#include "../Cards/Cards.h"

using namespace std;

//definition of the HumanPlayerStrategy class

void HumanPlayerStrategy::issueOrder(Player *player, Deck *deck, OrdersList* ordersList) {
    //clear orderList
    for (int i = 0; i < player->getOrders().size(); i++) {
        ordersList->remove(0);
    }
    
    cout<< "Start of strategy turn for " << player->getName() << endl;
    vector<Territory*> territories;
    territories.clear();
    territories = player->getTerritories();
        while (player->getReinforcementPool() > 0) {
        //ask players which territory they would like to deploy units to and how many, restart reinforcement loop
        cout << "Which territory would you like to deploy units to?you have " << player->getReinforcementPool() << " units available" << endl;
        for (int i = 0; i < territories.size(); i++) {
            cout << i << ". " << territories[i]->getName() << ": " << territories[i]->getArmies() << " units available"<< endl;
        }
        int terChoice = 0;
        cin >> terChoice;
        cout << "How many units would you like to deploy?\n";
        int units = 0;
        cin >> units;
        Deploy* deployOrder = new Deploy(player, territories[terChoice], units);
        ordersList->add(deployOrder);
        player-> setReinforcementPool(player->getReinforcementPool() - units);
    } 
    int choice = 0;
    while (choice != 3) {
        cout<< "Hello " << player->getName() << ", it is your turn to issue orders.\n";
        // Ask the player to choose an order
        cout << "Would you like to either\n1). move army units between your territories\n2). attack an enemy territory\n3). neither\n";
        cin >> choice;
        switch (choice) {
            case 1: {
                cout << "Which territory would you like to move units from?\n";
                for (int i = 0; i < territories.size(); i++) {
                    cout << i << ". " << territories[i]->getName() << ": " << territories[i]->getArmies() << " units available"<< endl;
                }
                int terChoice = 0;
                cin >> terChoice;
                cout << "How many units would you like to move?\n";
                int units = 0;
                cin >> units;
                cout << "Which territory would you like to move the units to?\n";
                vector<shared_ptr<Territory>> adjacents = territories[terChoice]->getAdjacentTerritories();

                // Remove territories owned by another player
                adjacents.erase(
                    std::remove_if(adjacents.begin(), adjacents.end(), [player](const shared_ptr<Territory>& t) {
                        return t->getOwner() != player;
                    }),
                    adjacents.end()
                );

                for (int i = 0; i < adjacents.size(); i++) {
                    cout << i << ". " << adjacents[i] ->getName() << endl;
                }
                int terChoice2 = 0;
                cin >> terChoice2;
                Territory* destination = adjacents[terChoice2].get();
                Advance* advanceOrder = new Advance(player, territories[terChoice], destination, units);
                ordersList->add(advanceOrder);
                break;
            }
            case 2: {
                cout<< "Which territory would you like to attack from?\n";
                vector<Territory*> attackFromList = toAttackFrom(player);
                for(int i = 0; i < attackFromList.size(); i++) {
                    cout << i << ". " << attackFromList[i]->getName() << endl;
                }
                int terChoice;
                cin >> terChoice;
                cout << "Which territory would you like to attack ?\n";
                for (int i = 0; i < attackFromList[terChoice]->getAdjacentTerritories().size(); i++) {
                    cout << i << ". " << attackFromList[terChoice]->getAdjacentTerritories()[i]->getName() << endl;
                }
                int terChoice2;

                cin >> terChoice2;
                int attackUnits;
                cout << "How many units would you like to attack with?\n";
                cin >> attackUnits;
                Advance* advanceOrder = new Advance(player, attackFromList[terChoice], attackFromList[terChoice]->getAdjacentTerritories()[terChoice2].get() , attackUnits);
                ordersList->add(advanceOrder);
                break;
            }
            case 3: {
                break;
            }
            default:{
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }

    Hand * pHand = player->getHand();

        if (pHand->getHand() > 0) {
            Card cardToPlay = pHand->copyCard_Hand(0);
            cardToPlay.play(*deck, *pHand); // Play the card to issue an order
        }
        else{
            cout << "You have no cards to play.\n";
        }

}

vector<Territory*> HumanPlayerStrategy::toDefend(Player *player) {
    vector<Territory*> territories;
    territories.clear();
    territories = player->getTerritories();
    vector<Territory*> toDefend;
    toDefend.clear();
    for (int i = 0; i < territories.size(); i++) {
        if (territories[i]->getArmies() > 1) {
            toDefend.push_back(territories[i]);
        }
    }
    return toDefend;
}

vector<Territory*> HumanPlayerStrategy::toAttack(Player *player) {
    vector<Territory*> territories;
    territories.clear();
    territories = player->getTerritories();
    vector<Territory*> toAttack;
    toAttack.clear();
    for (int i = 0; i < territories.size(); i++) {
        vector<shared_ptr<Territory>> adjacents = territories[i]->getAdjacentTerritories();
        for (int j = 0; j < adjacents.size(); j++) {
            if (adjacents[j]->getOwner() != player) {
                toAttack.push_back(territories[i]);
                break;
            }
        }
    }
    return toAttack;
}

vector<Territory*> HumanPlayerStrategy::toAttackFrom(Player *player) {
    vector<Territory*> territories;
    territories.clear();
    territories = player->getTerritories();
    vector<Territory*> toAttackFrom;
    toAttackFrom.clear();
    for (int i = 0; i < territories.size(); i++) {
        vector<shared_ptr<Territory>> adjacents = territories[i]->getAdjacentTerritories();
        for (int j = 0; j < adjacents.size(); j++) {
            if (adjacents[j]->getOwner() != player) {
                toAttackFrom.push_back(territories[i]);
                break;
            }
        }
    }
    return toAttackFrom;
}