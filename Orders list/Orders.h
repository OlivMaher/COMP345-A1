#include <iostream>
#include <string>
#include <vector>
#include "Map.h"

#ifndef ORDERS_H
#define ORDERS_H

class Player;
class Territory;

class Order {
public:
    // Constructors and Destructor
    Order();
    Order(const Order& other);
    virtual ~Order();

    virtual bool validate()= 0; //abstract methods
    virtual void execute()=0;

    friend std::ostream& operator<<(std::ostream& os, const Order& order);  //overloaded OPs

protected:
    std::string description;  //order description
    bool executed; //indicate if order has been executed


};

class Deploy : public Order {
public:
    //constructor and destructor
    Deploy(Player* player, Territory* territory, int numArmies);
    Deploy(const Deploy& other);
    ~Deploy();

    //overide methods
    bool validate() override;
    void execute() override;

private:
    Player* player;
    Territory* territory;
    int numArmies;

};

class Advance : public Order {
public:
    Advance(Player* player, Territory* source, Territory target, int numArmies);
    Advance(const Advance& other);
    ~Advance();

    bool validate() override;
    void execute() override;


private:
    Player* player;
    Territory* source;
    Territory* target;
    int numArmies;
};

class Bomb : public Order {
public:
    Bomb(Player* player, int numArmies, Territory target); //NOT DONE YET
    Bomb(const Bomb& other);
    ~Bomb();

    bool validate() override;
    void execute() override;

    private:
    Player* player;
    // not done yet
    int numArmies;
    Territory* target;
};
class Blockade : public Order {
public:
    Blockade(Player* player, int numArmies, Territory* target); //NOT DONE YET
    Blockade(const Bomb& other);
    ~Blockade();

    bool validate() override;
    void execute() override;


private:
    Player* player;
    // not done yet
    int numArmies;
    Territory* target;
};
class Airlift : public Order {
public:
    Airlift(Player* player, int numArmies, Territory* target); //NOT DONE YET
    Airlift(const Bomb& other);
    ~Airlift();

    bool validate() override;
    void execute() override;

private:
    Player* player;
    // not done yet
    int numArmies;
};
class Negotiate : public Order {
public:
    Negotiate(Player* player, int numArmies); //NOT DONE YET
    Negotiate(const Bomb& other);
    ~Negotiate();

    bool validate() override;
    void execute() override;

private:
    Player* player;
    // not done yet
    int numArmies;
    Territory* target;
};


class OrdersList {
public:
    //constructors and desctructor
    OrdersList();
    OrdersList(const OrdersList& other);
    ~OrdersList();

    //Methods - done
    void add(Order* order);                 //adds order to the list
    void remove(int index);                 // removes an order from the list
    void move(int fromIndex, int toIndex);  // moves an order within the list

//overload ops
    friend std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList);


private:
    std::vector<Order*> orders;         // STL Container to store orders as suggested by Prof Joey Paquet




};





#endif //ORDERS_H
