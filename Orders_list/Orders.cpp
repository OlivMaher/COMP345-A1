#include "Orders.h"

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

Deploy::Deploy(const Deploy& other) : Order(other) {}       // Copy constructor for the Deploy class

Deploy::~Deploy() {}        // Destructor for the Deploy class

Deploy& Deploy::operator=(const Deploy& other) {        // Assignment operator for the Deploy class
    if (this != &other) {
        Order::operator=(other);        // Use the base class assignment operator
    }
    return *this;
}
// Validate the Deploy order
bool Deploy::validate() {
    // Always true, for now
    return true;
}
// Execute the Deploy order
void Deploy::execute() {
    if (validate()) {   // Mark the order as executed and set the effect message
        *executed = true;
        *effect = "Deploy order executed.";
    } else {
        *effect = "Deploy order is invalid.";       // Set the effect message to indicate invalid order
    }
    notify();
}

// Implement Advance class methods
// Default constructor for the Advance class
Advance::Advance() : Order() {
    *description = "Advance";        // Set the description specific to Advance
}
// Copy constructor for the Advance class
Advance::Advance(const Advance& other) : Order(other) {}
// Destructor for the Advance class
Advance::~Advance() {}

Advance& Advance::operator=(const Advance& other) {     // Assignment operator for the Advance class
    if (this != &other) {             // Use the base class assignment operator
        Order::operator=(other);
    }
    return *this;
}
// Validate the Advance order
bool Advance::validate() {        // Always true, for now
    return true;
    return true;
}
// Execute the Advance order
void Advance::execute() {
    if (validate()) {       // Mark the order as executed and set the effect message
        *executed = true;
        *effect = "Advance order executed.";
    } else {
        *effect = "Advance order is invalid.";      // Set the effect message to indicate invalid order
    }
}

// Implement Bomb class methods
// Default constructor for the Bomb class
Bomb::Bomb() : Order() {        // Set the description specific to Bomb
    *description = "Bomb";
}
// Copy constructor for the Bomb class
Bomb::Bomb(const Bomb& other) : Order(other) {}
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
    return true;
}
// Execute the Bomb order
void Bomb::execute() {       // Mark the order as executed and set the effect message
    if (validate()) {
        *executed = true;
        *effect = "Bomb order executed.";
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
// Copy constructor for the Blockade class
Blockade::Blockade(const Blockade& other) : Order(other) {}
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
    return true;
}
// Execute the Blockade order
void Blockade::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Blockade order executed.";
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
// Copy constructor for the Airlift class
Airlift::Airlift(const Airlift& other) : Order(other) {}
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
    return true;
}
// Execute the Airlift order
void Airlift::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Airlift order executed.";
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
// Copy constructor for the Negotiate class
Negotiate::Negotiate(const Negotiate& other) : Order(other) {}
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
    return true;
}
// Execute the Negotiate order
void Negotiate::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Negotiate order executed.";
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