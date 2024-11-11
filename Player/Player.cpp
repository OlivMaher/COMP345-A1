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
}

Player::Player(string name): name(name), ordersList(new OrdersList()) 
,pHand(new Hand()) 
{}

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

//returns an arbitrary list of territories to defend
const vector<Territory *> Player:: toDefend() const{
    return vector<Territory*>(territories);
} 

//returns an arbitrary list of territories to attack
const vector<Territory *> Player:: toAttack() const{
    return vector<Territory*>(territories);
}

void Player::issueOrder(Order* o1) {
     if (ordersList && o1) {
        ordersList->add(o1); 
    } 
    return;
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
