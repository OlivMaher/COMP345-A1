#include "Orders.h"
#include "../Player/Player.h"

#include "../Map/Map.h"

extern Player neutralPlayer;  // Declare an external instance representing the neutral player

// Implement Order class methods

// Default constructor for the Order class
Order::Order() { // Initialize description, effect, and executed
    description = new std::string("Order");
    effect = new std::string("");
    executed = new bool(false);
}
// Copy constructor for the Order class
Order::Order(const Order& other) { // Create new copies of the description, effect, and executed
    description = new std::string(*other.description);
    effect = new std::string(*other.effect);
    executed = new bool(*other.executed);
}
// Destructor for the Order class
Order::~Order() {       // Clean up dynamically allocated memory
    delete description;
    delete effect;
    delete executed;
}
// Assignment operator for the Order class
Order& Order::operator=(const Order& other) {
    if (this != &other) {       // Copy the values from the other Order
        *description = *other.description;
        *effect = *other.effect;
        *executed = *other.executed;
    }
    return *this;
}
// Get the description of the Order
std::string Order::getDescription() const {
    return *description;
}
// Get the effect of the Order after execution
std::string Order::getEffect() const {
    return *effect;
}
// Overloaded stream insertion operator for the Order class
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.getDescription();       // Output the description of the order
    if (*order.executed) {
        os << " [Executed]: " << order.getEffect();      // Check if the order has been executed
    } else {
        os << " [Not Executed]";
    }
    return os;
}
std::string Order::stringToLog() const{
    return "Order: " + *description + ", Executed: " + (*executed ? "Yes" : "No") + ", Effect: " + *effect;
}

// Implement Deploy class methods
// Default constructor for the Deploy class
Deploy::Deploy() : Order() {
    *description = "Deploy";         // Set the description specific to Deploy
}

//List Constructor
Deploy::Deploy(Player* p, Territory* t, int u) : 
    Order(), 
    player(p), 
    target(t), 
    units(u) { *description = "Deploy"; }

Deploy::Deploy(const Deploy& other) : Order(other), 
    player(other.player), 
    target(other.target), 
    units(other.units) {}       // Copy constructor for the Deploy class

Deploy::~Deploy() {}        // Destructor for the Deploy class

Deploy& Deploy::operator=(const Deploy& other) {        // Assignment operator for the Deploy class
    Order::operator=(other); return *this; 
}
// Validate the Deploy order
bool Deploy::validate() {
    return target->getOwner() == player;;
}
// Execute the Deploy order
void Deploy::execute() {
    if (validate()) {
        *executed = true;
        target->addArmies(units);
        *effect = "Deploy order executed: " + std::to_string(units) + " units deployed to " + target->getName();
    } else {
        *effect = "Deploy order is invalid.";
    }
    notify();
}

// Implement Advance class methods
// Default constructor for the Advance class
Advance::Advance() : Order() {
    *description = "Advance";        // Set the description specific to Advance
}
// list constructor
Advance::Advance(Player* p, Territory* s, Territory* t, int u) : 
    Order(), 
    player(p), 
    source(s), 
    target(t), 
    units(u) { *description = "Advance"; }

// Copy constructor for the Advance class
Advance::Advance(const Advance& other) : 
    Order(other), 
    player(other.player), 
    source(other.source), 
    target(other.target), 
    units(other.units) {}

// Destructor for the Advance class
Advance::~Advance() {}

Advance& Advance::operator=(const Advance& other) {     // Assignment operator for the Advance class
    if (this != &other) {             // Use the base class assignment operator
        Order::operator=(other);
    }
    return *this;
}
// Validate the Advance order
bool Advance::validate() {
    return source->getOwner() == player && source->isAdjacent(target);
}
// Execute the Advance order
void Advance::execute() {
    if (validate()) {
        *executed = true;
        if (target->getOwner() == player) {
            target->addArmies(units);
            source->removeArmies(units);
            *effect = "Advance order executed: " + std::to_string(units) + " units moved from " + source->getName() + " to " + target->getName();
        } else {
            int attackingUnits = units;
            int defendingUnits = target->getArmies();
            int attackersLost = 0;
            int defendersLost = 0;

            for (int i = 0; i < attackingUnits; ++i) {
                if (rand() % 100 < 60) ++defendersLost;
            }
            for (int i = 0; i < defendingUnits; ++i) {
                if (rand() % 100 < 70) ++attackersLost;
            }

            if (defendersLost >= defendingUnits) {
                target->setOwner(player);
                target->setArmies(attackingUnits - attackersLost);
                source->setArmies(units - attackersLost);
                *effect = "Advance order executed: Territory " + target->getName() + " captured by " + player->getName();
            } else {
                *effect = "Advance order executed: Battle fought but " + target->getName() + " not captured.";
                target->setArmies(defendingUnits - defendersLost);
                if ((units - attackersLost)< 0){
                    source->setArmies(0);
                }
                else
                    source->setArmies(units - attackersLost);
            }
        }
    } else {
        *effect = "Advance order is invalid.";
    }
    notify();
}

// Implement Bomb class methods
// Default constructor for the Bomb class
Bomb::Bomb() : Order() {        // Set the description specific to Bomb
    *description = "Bomb";
}

//List constructor
Bomb::Bomb(Player* p, Territory* t) : 
Order(), 
player(p), 
target(t) { *description = "Bomb"; }

// Copy constructor for the Bomb class
Bomb::Bomb(const Bomb& other) : 
Order(other), 
player(other.player), 
target(other.target) {}

// Destructor for the Bomb class
Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& other) {      // Assignment operator for the Bomb class
    if (this != &other) {
        Order::operator=(other);         // Use the base class assignment operator
    }
    return *this;
}
// Validate the Bomb order
bool Bomb::validate() {     // Placeholder validation logic; always returns true for now
    return target->getOwner() != player;
}
// Execute the Bomb order
void Bomb::execute() {       // Mark the order as executed and set the effect message
    if (validate()) {
        *executed = true;
        target->setArmies(target->getArmies() / 2);
        *effect = "Bomb order executed: Half the armies on " + target->getName() + " have been destroyed.";
    } else {
        *effect = "Bomb order is invalid.";
    }
    notify();
}

// Implement Blockade class methods
// Default constructor for the Blockade class
Blockade::Blockade() : Order() {
    *description = "Blockade";
}

// List Constructor
Blockade::Blockade(Player* p, Territory* t) : 
Order(), 
player(p), 
target(t) { *description = "Blockade"; }

// Copy constructor for the Blockade class
Blockade::Blockade(const Blockade& other) : 
Order(other), 
player(other.player), 
target(other.target) {}

// Destructor for the Blockade class
Blockade::~Blockade() {}

// Assignment operator for the Blockade class
Blockade& Blockade::operator=(const Blockade& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

// Validate the Blockade order
bool Blockade::validate() {
    return target->getOwner() == player;
}

// Execute the Blockade order
void Blockade::execute() {
    if (validate()) {
        *executed = true;
        target->setArmies(target->getArmies() * 2);
        target->setOwner(Player::getNeutralPlayer());  // Set the owner to the neutral player
        *effect = "Blockade order executed: Territory " + target->getName() + " now belongs to the Neutral player.";
    } else {
        *effect = "Blockade order is invalid.";
    }
    notify();
}

// Implement Airlift class methods
// Default constructor for the Airlift class
Airlift::Airlift() : Order() {
    *description = "Airlift";
}

// List Constructor
Airlift::Airlift(Player* p, Territory* s, Territory* t, int u) : 
    Order(), 
    player(p), 
    source(s), 
    target(t), 
    units(u) { *description = "Airlift"; }

// Copy constructor for the Airlift class
Airlift::Airlift(const Airlift& other) : 
    Order(other), 
    player(other.player), 
    source(other.source), 
    target(other.target), 
    units(other.units) {}

// Destructor for the Airlift class
Airlift::~Airlift() {}

// Assignment operator for the Airlift class
Airlift& Airlift::operator=(const Airlift& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

// Validate the Airlift order
bool Airlift::validate() {
    if (source->getOwner() != player || target->getOwner() != player) {
        std::cout << "Airlift validation failed: Player does not own both source and target territories." << std::endl;
        return false;
    }
    if (units > source->getArmies()) {
        std::cout << "Airlift validation failed: Not enough units in the source territory." << std::endl;
        return false;
    }
    return true;
}

// Execute the Airlift order
void Airlift::execute() {
    if (validate()) {
        *executed = true;
        source->removeArmies(units);
        target->addArmies(units);
        *effect = "Airlift order executed: " + std::to_string(units) + " units airlifted from " + source->getName() + " to " + target->getName();
    } else {
        *effect = "Airlift order is invalid.";
    }
    notify();
}

// Implement Negotiate class methods
// Default constructor for the Negotiate class
Negotiate::Negotiate() : Order() {
    *description = "Negotiate";
}

//List Constructor
Negotiate::Negotiate(Player* p, Player* tp) : 
    Order(), 
    player(p), 
    targetPlayer(tp) { *description = "Negotiate"; }

// Copy constructor for the Negotiate class
Negotiate::Negotiate(const Negotiate& other) : 
    Order(other), 
    player(other.player), 
    targetPlayer(other.targetPlayer) {}

// Destructor for the Negotiate class
Negotiate::~Negotiate() {}

// Assignment operator for the Negotiate class
Negotiate& Negotiate::operator=(const Negotiate& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

// Validate the Negotiate order
bool Negotiate::validate() {
    return player != targetPlayer;
}

// Execute the Negotiate order
void Negotiate::execute() {
    if (validate()) {
        *executed = true;
        player->addNegotiatedPlayer(targetPlayer);
        targetPlayer->addNegotiatedPlayer(player);
        *effect = "Negotiate order executed: " + player->getName() + " and " + targetPlayer->getName() + " cannot attack each other for the rest of the turn.";
    } else {
        *effect = "Negotiate order is invalid.";
    }
    notify();
}

// Implement OrdersList class methods
// Default constructor for the OrdersList class
OrdersList::OrdersList() {
    orders = new std::vector<Order*>();
}
// Copy constructor for the OrdersList class
OrdersList::OrdersList(const OrdersList& other) {
    orders = new std::vector<Order*>();                     // Create a new vector for the orders
    for (Order* order : *other.orders) {

        orders->push_back(order); // Copy the Orders only a Shallow copy
    }
}
// Destructor for the OrdersList class
OrdersList::~OrdersList() {
    for (Order* order : *orders) {
        delete order;           // Delete all orders in the list to free memory
    }
    delete orders;                  // Delete the orders vector
}

OrdersList& OrdersList::operator=(const OrdersList& other) {            // Assignment operator for the OrdersList class
    if (this != &other) {
        for (Order* order : *orders) {       // Delete existing orders
            delete order;
        }
        orders->clear();
        for (Order* order : *other.orders) {
            orders->push_back(order); // Shallow copy
        }
    }
    return *this;
}
// Add an order to the list
void OrdersList::add(Order* order) {
    orders->push_back(order);
    notify();
}
// Remove an order from the list at the specified index
void OrdersList::remove(int index) {
    if (index >= 0 && index < orders->size()) {     // Get the order to remove
        Order* order = (*orders)[index];
        orders->erase(orders->begin() + index);     // Remove it from the vector
        delete order;                                       // Delete the order to free memory
    } else {
        std::cout << "Invalid index for remove operation." << std::endl;            // Index is out of bounds
    }
}
// Move an order within the list from one index to another
void OrdersList::move(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < orders->size() && toIndex >= 0 && toIndex <= orders->size()) {
        Order* order = (*orders)[fromIndex];        // Get the order to move
        orders->erase(orders->begin() + fromIndex);      // Remove it from its current position
        orders->insert(orders->begin() + toIndex, order);        // Insert it at the new position
    } else {
        std::cout << "Invalid indices for move operation." << std::endl;        // Indices are out of bounds
    }
}

const std::vector<Order*>& OrdersList::getOrders() const {      // Get the list of orders
    return *orders;
}

std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList) {      // Overloaded stream insertion operator for the OrdersList class
    os << "Orders List:" << std::endl;
    const std::vector<Order*>& orders = ordersList.getOrders();
    for (size_t i = 0; i < orders.size(); ++i) {         // Iterate over the orders and output each one
        os << i << ": " << *orders[i] << std::endl;
    }
    return os;
}

std::string OrdersList::stringToLog() const {
    string log = "OrdersList: ";
    for(const Order* order: *orders){
        log += order->stringToLog() + "; ";
    }
    return log;
}
