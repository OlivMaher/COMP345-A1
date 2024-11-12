#include "Player.h"
#include "../Map/Map.h"
#include "../Orders_list/Orders.h"
#include "../Cards/Cards.h"
#include <cstdlib>
#include <memory> 

using namespace std;

Player::Player(): ordersList(new OrdersList()) 
,pHand(new Hand()) 
{
    name = "Unnamed Player";
    reinforcementPool = 0;
}

Player::Player(string name): name(name), ordersList(new OrdersList()) 
,pHand(new Hand()) {
    reinforcementPool = 0;
}

Player::Player(const Player& other){
    name = other.name;
    for (Territory* territory : other.territories) {
        territories.push_back(new Territory(*territory)); 
    }
    
    //pHand = new Hand(*other.pHand);
    ordersList = new OrdersList(*other.ordersList);

}

Player& Player::operator=(const Player& other) {
    if (this != &other) {  
        // Clean up existing resources
        delete ordersList;  // Delete the existing ordersList
        ordersList = nullptr;  // Set to nullptr to avoid dangling pointers

        delete pHand; 
        pHand = nullptr;
        
        // Clean up existing territories
        for (Territory* territory : territories) {
            delete territory;  // Delete each Territory pointer
        }
        territories.clear();  // Clear the vector after deleting the pointers

        
        // Copy resources from other
        name = other.name;  // Copy name

        // Deep copy territories
        for (Territory* territory : other.territories) {
            territories.push_back(new Territory(*territory));  // Assuming Territory has a copy constructor
        }

        // Deep copy ordersList
        ordersList = new OrdersList(*other.ordersList);  // Create a new OrdersList
        pHand = new Hand(*other.pHand);
    }
    return *this;  
}

Player::~Player() {
    // Delete each territory in the territories vector
    for (Territory* territory : territories) {
        delete territory;  // Deallocate memory for each Territory
    }
  
    // Delete the ordersList if it's dynamically allocated
    delete ordersList;  // Deallocate memory for OrdersList
    delete pHand;
}

void Player:: addTerritory(Territory* t){ 
    territories.push_back(t);
    t->setOwner(this);
}


const vector<Territory *> Player:: getTerritories() const{
    return vector<Territory*>(territories);
}




int Player::get_num_orders() const{
    return ordersList->getOrders().size();
}
//returns an arbitrary list of territories to defend
const vector<Territory *> Player:: toDefend() const{
    return vector<Territory*>(territories);
} 

//returns a list of territories to attack
const vector<Territory *> Player:: toAttack() const{
    vector<Territory*> toAttack = vector<Territory*>();
    for (Territory* t : territories) {
        vector<shared_ptr<Territory>> adjacentsShared = t->getAdjacentTerritories();
        vector<Territory*> adjacents;
        for (const auto& adj : adjacentsShared) {
            adjacents.push_back(adj.get());
        }

        for (Territory* adjTerritory : adjacents) {
            if (adjTerritory->getOwner() != this && 
                std::find(toAttack.begin(), toAttack.end(), adjTerritory) == toAttack.end()) {
                toAttack.push_back(adjTerritory);
            }
        }
    }
    return toAttack;
}



const vector<Territory*> Player:: toAttackFrom() const {
    vector<Territory*> toAttackFrom;
    for (Territory* t : territories) {
        vector<shared_ptr<Territory>> adjacentsShared = t->getAdjacentTerritories();
        for (const auto& adj : adjacentsShared) {
            if (adj->getOwner() != this) {
                toAttackFrom.push_back(t);
                break; // No need to check further adjacents for this territory
            }
        }
    }
    return toAttackFrom;
}

void Player::issueOrder(Deck* deck) {
    cout<< "Automatically issuing deploy orders until reinforcement pool is empty.\n";
    while (reinforcementPool > 0) {
        // automatically Issues deploy orders until reinforcement pool is empty 
        for (Territory* t : toDefend()) {
            int deployUnits = min(3, reinforcementPool);  // predetermined logic for deploying reinforcement troops
            reinforcementPool -= deployUnits;
            // Create and add deploy order to OrdersList
             // Create a Deploy order
            Deploy* deployOrder = new Deploy(this, t, deployUnits );
            ordersList->add(deployOrder);
        }
    } 
    int choice;
    while (choice != 3) {
        // Ask the player to choose an order
        cout << "Would you like to either\n1). move army units between your territories\n2). attack an enemy territory\n3). neither\n";
        cin >> choice;
        switch (choice) {
            case 1: {
                cout << "Which territory would you like to move units from?\n";
                for (int i = 0; i < territories.size(); i++) {
                    cout << i << ". " << territories[i] << endl;
                }
                int terChoice;
                cin >> terChoice;
                cout << "How many units would you like to move?\n";
                int units;
                cin >> units;
                cout << "Which territory would you like to move the units to?\n";
                vector<shared_ptr<Territory>> adjacents = territories[terChoice]->getAdjacentTerritories();

                // Remove territories owned by another player
                adjacents.erase(
                    std::remove_if(adjacents.begin(), adjacents.end(), [this](const shared_ptr<Territory>& t) {
                        return t->getOwner() != this;
                    }),
                    adjacents.end()
                );

                for (int i = 0; i < adjacents.size(); i++) {
                    cout << i << ". " << adjacents[i] << endl;
                }
                int terChoice2;
                cin >> terChoice2;
                Territory* destination = adjacents[terChoice2].get();
                Advance* advanceOrder = new Advance(this, territories[terChoice], destination, units);
                ordersList->add(advanceOrder);
                break;
            }
            case 2: {
                cout<< "Which territory would you like to attack from?\n";
                vector<Territory*> attackFromList = toAttackFrom();
                for(int i = 0; i < attackFromList.size(); i++) {
                    cout << i << ". " << attackFromList[i] << endl;
                }
                int terChoice;
                cin >> terChoice;
                cout << "Which territory would you like to attack ?\n";
                for (int i = 0; i < attackFromList[terChoice]->getAdjacentTerritories().size(); i++) {
                    cout << i << ". " << attackFromList[terChoice]->getAdjacentTerritories()[i] << endl;
                }
                int terChoice2;
                cin >> terChoice2;
                int attackUnits;
                cout << "How many units would you like to attack with?\n";
                cin >> attackUnits;
                Advance* advanceOrder = new Advance(this, attackFromList[terChoice], attackFromList[terChoice]->getAdjacentTerritories()[terChoice2].get() , attackUnits);
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

        if (pHand->getHand() > 0) {
            pHand-> showHand();
            cout<< "Which card would you like to use?\n";
            int choice; // which card should be played
            cin >> choice;
            Card cardToPlay = pHand->copyCard_Hand(choice);
            cardToPlay.play(*deck, *pHand); // Play the card to issue an order
        }
        else{
            cout << "You have no cards to play.\n";
        }
    }

}

Order* Player::popOrder() {
    if (ordersList->getOrders().empty()) {
        return nullptr; // or handle the empty case as needed
    }
    Order* order = ordersList->getOrders().front();
    ordersList->remove(0);
    return order;
}

ostream& operator<<(ostream& out, const Player& player) {
    out << "Player Name: " << player.name << endl;
    out << "Player's territories: " << player.territories.size() << " territories" << endl;

    // Display the territories to defend and attack
    vector<Territory*> defendList = player.toDefend();
    vector<Territory*> attackList = player.toAttack();
    out << "Territories to defend: " << defendList.size() << endl;
    out << "Territories to attack: " << attackList.size() << endl;

    // Check if ordersList is not null before printing it
    if (player.ordersList) {
        out << *(player.ordersList); 
    } else {
        out << "Orders List: None";
    }

    return out;
}

void Player::addNegotiatedPlayer(Player* player) {
    negotiatedPlayers.insert(player);
}

// Check if a player has been negotiated with
bool Player::hasNegotiatedWith(Player* player) const {
    return negotiatedPlayers.find(player) != negotiatedPlayers.end();
}

// Clear the negotiated players for a new turn
void Player::clearNegotiations() {
    negotiatedPlayers.clear();
}

std::string Player::getName() const {
    return name;
}

void Player::setReinforcementPool(int armies) {
    reinforcementPool = armies;
}

int Player::getReinforcementPool() const {
    return reinforcementPool;
}

Hand* Player::getHand() {
    return pHand;
}
Player* Player::getNeutralPlayer() {
    static Player* neutralPlayer = new Player("Neutral Player");
    return neutralPlayer;
}