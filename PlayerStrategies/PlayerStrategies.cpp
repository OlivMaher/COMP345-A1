#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Orders_list/Orders.h"
#include "../Cards/Cards.h"

#include <iostream>
#include <vector>

using namespace std;

//definition of the HumanPlayerStrategy class

void HumanPlayerStrategy::issueOrder(Player *player, Deck *deck, OrdersList* ordersList) {
    //clear orderList causes crash when list has orders i.e. on the second round of order issing
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
            cardToPlay.play(*deck, *pHand, player, ordersList); // Play the card to issue an order
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

//definition of the AggressivePlayerStrategy class
vector<Territory*> AggressivePlayerStrategy::toDefend(Player *player) {
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

vector<Territory*> AggressivePlayerStrategy::toAttack(Player* player) {
    vector<Territory*> territories = player->getTerritories();
    vector<Territory*> toAttack;

    for (Territory* territory : territories) {
        vector<shared_ptr<Territory>> adjacents = territory->getAdjacentTerritories();

        for (const auto& adjacent : adjacents) {
            if (adjacent->getOwner() != player) {
                // Check if the territory is already in toAttack
                if (std::find(toAttack.begin(), toAttack.end(), adjacent.get()) == toAttack.end()) {
                    toAttack.push_back(adjacent.get());
                }
            }
        }
    }

    return toAttack;
}


vector<Territory*> AggressivePlayerStrategy::toAttackFrom(Player *player) {
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

void AggressivePlayerStrategy::issueOrder(Player *player, Deck *deck, OrdersList* ordersList) {
    //clear orderList
    for (int i = 0; i < player->getOrders().size(); i++) {
        ordersList->remove(0);
    }
    cout<< "Start of strategy turn for " << player->getName() << endl;
    vector<Territory*> territories;
    territories.clear();
    territories = toAttackFrom(player);
    //deploy reinforcements to territory with most troops
    //find strongest territory that has enemy adjacent territories
    Territory* strongest = territories[0];
    for (int i = 1; i < territories.size(); i++) {
        if (territories[i]->getArmies() > strongest->getArmies()) {
            strongest = territories[i];
        }
    }
    Deploy* deployOrder = new Deploy(player, strongest, player->getReinforcementPool());
    int reinforcements = player->getReinforcementPool();
    player-> setReinforcementPool(0);
    ordersList->add(deployOrder);

     //advance armies from strongest territory 
    vector<shared_ptr<Territory>> adjacents = strongest->getAdjacentTerritories();
    //find a territory in adjacents that is not owned by player. This is what you want to attack
    for(int i = 0; i < adjacents.size(); i++) {
        if (adjacents[i]->getOwner() != player) {
            int fullForce = strongest->getArmies() + reinforcements;
            Advance* advanceOrder = new Advance(player, strongest, adjacents[i].get(), fullForce);
            ordersList->add(advanceOrder);
            break;
        }
    }

    Hand * pHand = player->getHand();
    //play an aggressive card only. Play first card with type bomb
    for (int i = 0; i < pHand->getHand(); i++) {
        Card cardToPlay = pHand->copyCard_Hand(i);
        if (cardToPlay.getCardType() == "Bomb") {
            cardToPlay.play(*deck, *pHand, player, ordersList);
            break;
        }
    }

}

vector<Territory*> BenevolentPlayerStrategy::toDefend(Player *player) {
    return player->getTerritories();
}

vector<Territory*> BenevolentPlayerStrategy::toAttack(Player *player) {
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

void BenevolentPlayerStrategy::issueOrder(Player *player, Deck *deck, OrdersList* ordersList) {
    //clear orderList
    for (int i = 0; i < player->getOrders().size(); i++) {
        ordersList->remove(0);
    }
    cout<< "Start of strategy turn for " << player->getName() << endl;
    vector<Territory*> territories;
    territories.clear();
    territories = toDefend(player);
    //deploy reinforcements to territory with least troops
    //find weakest territory
    Territory* weakest = territories[0];
    for (int i = 1; i < territories.size(); i++) {
        if (territories[i]->getArmies() < weakest->getArmies()) {
            weakest = territories[i];
        }
    }
    Deploy* deployOrder = new Deploy(player, weakest, player->getReinforcementPool());
    int reinforcements = player->getReinforcementPool();
    player-> setReinforcementPool(0);
    ordersList->add(deployOrder);

    Hand * pHand = player->getHand();
    //play a benevolent card only. Play first card with type reinforcement
    for (int i = 0; i < pHand->getHand(); i++) {
        Card cardToPlay = pHand->copyCard_Hand(i);
        if (cardToPlay.getCardType() != "Bomb") {
            cardToPlay.play(*deck, *pHand, player, ordersList);
            break;
        }
    }
}

//definition of the NautralPlayerStrategy class
void NeutralPlayerStrategy::issueOrder(Player* player, Deck* deck, OrdersList* ordersList) {
    cout << player->getName() << " is neutral and does not issue orders." << endl;
}

std::vector<Territory*> NeutralPlayerStrategy::toDefend(Player* player) {
    return std::vector<Territory*>(); // no territories to defend
}

std::vector<Territory*> NeutralPlayerStrategy::toAttack(Player* player) {
    return std::vector<Territory*>(); // can't attack
}

//definition of the CheaterPlayerStrategy class
void CheaterPlayerStrategy::issueOrder(Player* player, Deck* deck, OrdersList* ordersList) {
    // Automatically conquer adjacent enemy territories
    cout << player->getName() << " is cheating and conquering adjacent territories!" << endl;

    // Gather territories to conquer
    vector<Territory*> toConquer;
    const vector<Territory*> territories = player->getTerritories();
    for (Territory* territory : territories) {
        const vector<shared_ptr<Territory>>& adjacents = territory->getAdjacentTerritories();
        for (const auto& adjacent : adjacents) {

            if (adjacent->getOwner() != player) {
                toConquer.push_back(adjacent.get());
            }
        }
    }


    for (Territory* target : toConquer) {
        cout << "Cheating: Conquering " << target->getName() << "!" << endl;
        target->setOwner(player);
        player->addTerritory(target);
    }
}

std::vector<Territory*> CheaterPlayerStrategy::toDefend(Player* player) {
    return std::vector<Territory*>();
}

std::vector<Territory*> CheaterPlayerStrategy::toAttack(Player* player) {
    return std::vector<Territory*>();
}
