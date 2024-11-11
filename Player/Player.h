#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <set>
#include "../Map/Map.h"
#include "../Orders_list/Orders.h"
#include "../Cards/Cards.h"


using namespace std;

class Player{
    private:
        string name;
        vector<Territory*> territories; //List of territories controlled by the player
        Hand  * pHand;
        OrdersList * ordersList;
        set<Player*> negotiatedPlayers;
    public:
        //Constructor
        Player();
        Player(string name);
        Player(const Player& other);
        Player& operator=(const Player& other);
        // Destructor
        ~Player();
        string getName() const;

        void addTerritory(Territory*);

        const vector<Territory*>  toDefend() const; 
        const vector<Territory*>  toAttack() const;

        void issueOrder(Order*);

       friend ostream& operator<<(ostream& out, const Player& player);

        void addNegotiatedPlayer(Player* player);
        bool hasNegotiatedWith(Player* player) const;
        void clearNegotiations(); // Clear negotiated players for a new turn
};

#endif
