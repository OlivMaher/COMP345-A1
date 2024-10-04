#include "Orders.h"

// Implement Order class methods

Order::Order() {
    description = new std::string("Order");
    effect = new std::string("");
    executed = new bool(false);
}

Order::Order(const Order& other) {
    description = new std::string(*other.description);
    effect = new std::string(*other.effect);
    executed = new bool(*other.executed);
}

Order::~Order() {
    delete description;
    delete effect;
    delete executed;
}

Order& Order::operator=(const Order& other) {
    if (this != &other) {
        *description = *other.description;
        *effect = *other.effect;
        *executed = *other.executed;
    }
    return *this;
}

std::string Order::getDescription() const {
    return *description;
}

std::string Order::getEffect() const {
    return *effect;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.getDescription();
    if (*order.executed) {
        os << " [Executed]: " << order.getEffect();
    } else {
        os << " [Not Executed]";
    }
    return os;
}

// Implement Deploy class methods

Deploy::Deploy() : Order() {
    *description = "Deploy";
}

Deploy::Deploy(const Deploy& other) : Order(other) {}

Deploy::~Deploy() {}

Deploy& Deploy::operator=(const Deploy& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool Deploy::validate() {
    // Placeholder validation logic
    return true;
}

void Deploy::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Deploy order executed.";
    } else {
        *effect = "Deploy order is invalid.";
    }
}

// Implement Advance class methods

Advance::Advance() : Order() {
    *description = "Advance";
}

Advance::Advance(const Advance& other) : Order(other) {}

Advance::~Advance() {}

Advance& Advance::operator=(const Advance& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool Advance::validate() {
    return true;
}

void Advance::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Advance order executed.";
    } else {
        *effect = "Advance order is invalid.";
    }
}

// Implement Bomb class methods

Bomb::Bomb() : Order() {
    *description = "Bomb";
}

Bomb::Bomb(const Bomb& other) : Order(other) {}

Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool Bomb::validate() {
    return true;
}

void Bomb::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Bomb order executed.";
    } else {
        *effect = "Bomb order is invalid.";
    }
}

// Implement Blockade class methods

Blockade::Blockade() : Order() {
    *description = "Blockade";
}

Blockade::Blockade(const Blockade& other) : Order(other) {}

Blockade::~Blockade() {}

Blockade& Blockade::operator=(const Blockade& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool Blockade::validate() {
    return true;
}

void Blockade::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Blockade order executed.";
    } else {
        *effect = "Blockade order is invalid.";
    }
}

// Implement Airlift class methods

Airlift::Airlift() : Order() {
    *description = "Airlift";
}

Airlift::Airlift(const Airlift& other) : Order(other) {}

Airlift::~Airlift() {}

Airlift& Airlift::operator=(const Airlift& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool Airlift::validate() {
    return true;
}

void Airlift::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Airlift order executed.";
    } else {
        *effect = "Airlift order is invalid.";
    }
}

// Implement Negotiate class methods

Negotiate::Negotiate() : Order() {
    *description = "Negotiate";
}

Negotiate::Negotiate(const Negotiate& other) : Order(other) {}

Negotiate::~Negotiate() {}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    if (this != &other) {
        Order::operator=(other);
    }
    return *this;
}

bool Negotiate::validate() {
    return true;
}

void Negotiate::execute() {
    if (validate()) {
        *executed = true;
        *effect = "Negotiate order executed.";
    } else {
        *effect = "Negotiate order is invalid.";
    }
}

// Implement OrdersList class methods

OrdersList::OrdersList() {
    orders = new std::vector<Order*>();
}

OrdersList::OrdersList(const OrdersList& other) {
    orders = new std::vector<Order*>();
    for (Order* order : *other.orders) {
        // Assuming deep copy is needed; create new orders by copying
        orders->push_back(order); // Shallow copy here for simplicity
    }
}

OrdersList::~OrdersList() {
    for (Order* order : *orders) {
        delete order;
    }
    delete orders;
}

OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        for (Order* order : *orders) {
            delete order;
        }
        orders->clear();
        for (Order* order : *other.orders) {
            orders->push_back(order); // Shallow copy here for simplicity
        }
    }
    return *this;
}

void OrdersList::add(Order* order) {
    orders->push_back(order);
}

void OrdersList::remove(int index) {
    if (index >= 0 && index < orders->size()) {
        Order* order = (*orders)[index];
        orders->erase(orders->begin() + index);
        delete order;
    } else {
        std::cout << "Invalid index for remove operation." << std::endl;
    }
}

void OrdersList::move(int fromIndex, int toIndex) {
    if (fromIndex >= 0 && fromIndex < orders->size() && toIndex >= 0 && toIndex <= orders->size()) {
        Order* order = (*orders)[fromIndex];
        orders->erase(orders->begin() + fromIndex);
        orders->insert(orders->begin() + toIndex, order);
    } else {
        std::cout << "Invalid indices for move operation." << std::endl;
    }
}

const std::vector<Order*>& OrdersList::getOrders() const {
    return *orders;
}

std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList) {
    os << "Orders List:" << std::endl;
    const std::vector<Order*>& orders = ordersList.getOrders();
    for (size_t i = 0; i < orders.size(); ++i) {
        os << i << ": " << *orders[i] << std::endl;
    }
    return os;
}