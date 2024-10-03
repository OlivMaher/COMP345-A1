#include "Player.h"
#include "../Map/Map.cpp"
#include "../Orders list/Orders.cpp"
#include <cstdlib>
#include <memory> 

using namespace std;

Player::Player(string name, Territory* t): name(name) {
    addTerritory(t);
}

Player::Player(const Player& other){
    name = other.name;
    for (Territory* territory : other.territories) {
        territories.push_back(new Territory(*territory)); // Assuming Territory has a copy constructor
    }
    //cards
    ordersList = new OrdersList(*other.ordersList);
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {  // Check for self-assignment
        // Clean up existing resources
        delete ordersList;  // Delete the existing ordersList
        ordersList = nullptr;  // Set to nullptr to avoid dangling pointers
        
        // Clean up existing territories
        for (Territory* territory : territories) {
            delete territory;  // Delete each Territory pointer
        }
        territories.clear();  // Clear the vector after deleting the pointers

        // Clean up existing cards if you have them
        // for (Card* card : cards) {
        //     delete card;  // Uncomment this if cards are managed with new
        // }
        // cards.clear(); // Uncomment this if you have a vector for cards

        // Copy resources from other
        name = other.name;  // Copy name

        // Deep copy territories
        for (Territory* territory : other.territories) {
            territories.push_back(new Territory(*territory));  // Assuming Territory has a copy constructor
        }

        // Deep copy ordersList
        ordersList = new OrdersList(*other.ordersList);  // Create a new OrdersList
    }
    return *this;  // Return *this to allow for chained assignments
}

Player::~Player() {
    // Delete each territory in the territories vector
    for (Territory* territory : territories) {
        delete territory;  // Deallocate memory for each Territory
    }
    // Delete the ordersList if it's dynamically allocated
    delete ordersList;  // Deallocate memory for OrdersList
}

void Player:: addTerritory(Territory* t){ 
    territories.push_back(t);
    t->setOwner(name);
}

//returns an arbitrary list of territories to defend
const vector<Territory *> Player:: toDefend() const{
    return territories;
} 

//returns an arbitrary list of territories to attack
const vector<Territory *> Player:: toAttack() const{
    return territories;
}

void Player::issueOrder(Order* o1) {
    // Add the order to the orders list
    if (o1) {
        ordersList->add(o1); 
    }
    return;
}

ostream& operator<<(ostream& out, const Player& player) {
    out << "Player Name: " << player.name;
    out << "Player's territories: " << player.territories.size() << " territories\n";
    vector<Territory*> defendList = player.toDefend();
    vector<Territory*> attackList = player.toAttack();
    out << "Territories to defend: " << defendList.size() << endl;
    out << "Territories to attack: " << attackList.size() << endl;
    out << "Orders List info:";
    out << player.ordersList;
    return out;
}