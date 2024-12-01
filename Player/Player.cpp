#include "Player.h"
#include "../Map/Map.h"
#include "../Orders_list/Orders.h"
#include "../Cards/Cards.h"
#include <cstdlib>
#include <memory> 
#include "../PlayerStrategies/PlayerStrategies.h"

using namespace std;

Player::Player(): ordersList(new OrdersList()) 
, pHand(new Hand()), strategy(new HumanPlayerStrategy())
{
    name = "Unnamed Player";
    reinforcementPool = 0;
}

Player::Player(string name): name(name), ordersList(new OrdersList()), pHand(new Hand()), strategy(new HumanPlayerStrategy()) {
    reinforcementPool = 0;
}

Player::Player(const Player& other){
    name = other.name;
    for (Territory* territory : other.territories) {
        territories.push_back(new Territory(*territory)); 
    }
    
    //pHand = new Hand(*other.pHand);
    ordersList = new OrdersList(*other.ordersList);
    pHand = new Hand(*other.pHand);
    strategy = new HumanPlayerStrategy();

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
    for (Territory* territory : territories) { // Deallocate memory for each Territory
        territory = nullptr;  // Set to nullptr to avoid dangling pointers
    }
  
    // Delete the ordersList if it's dynamically allocated
    delete ordersList;  // Deallocate memory for OrdersList
    ordersList = nullptr;  // Set to nullptr to avoid dangling pointers
    delete pHand;
    pHand = nullptr;
    delete strategy;
    strategy = nullptr;
}

void Player::setStrategy(PlayerStrategy* strategy) {
    this->strategy = strategy;
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

    //returns all trerritories with at least one adjacent enemy territory
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
    cout << "Start of turn for " << name << endl;

    
    strategy -> issueOrder(this, deck, ordersList);

    // Display the orders issued by the player
    cout << "Orders issued by " << name << ":" << endl;
    for (Order* order : ordersList->getOrders()) {
        cout << *order << endl;
    }



    cout << "End of turn for " << name << endl;

}

Order* Player::popOrder() {
    if (ordersList->getOrders().empty()) {
        return nullptr; // or handle the empty case as needed
    }
    Order* order = ordersList->getOrders().front();
    ordersList->remove(0);
    return order;
}

vector<Order*> Player::getOrders() {
    return ordersList->getOrders();
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

OrdersList* Player::getOrdersList() {
    return ordersList;
}