#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include "../Map/Map.h"
#include "../Orders list/Orders.h"
//#include "../Cards/Cards.h"

using namespace std;

class Player{
    private:
        string name;
        vector<Territory*> territories; //List of territories controlled by the player
        //vector<Card*> cards;
        OrdersList * ordersList;
    public:
        //Constructor

        Player(string name, Territory* territory);
        Player(const Player& other);
        Player& operator=(const Player& other);
        // Destructor
        ~Player();

        //Setters & Getters 
        string getName() const;

        void addTerritory(Territory*);

        const vector<Territory*>  toDefend() const; 
        const vector<Territory*>  toAttack() const;

        void issueOrder(Order*);


        //void printPlayer() const;

       friend ostream& operator<<(ostream& out, const Player& player);
};

#endif