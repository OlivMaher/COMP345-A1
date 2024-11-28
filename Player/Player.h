#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <set>
#include <vector>
#include "../Map/Map.h"
#include "../Orders_list/Orders.h"
#include "../Cards/Cards.h"



using namespace std;

class PlayerStrategy;

class Player{
    private:
        string name;
        vector<Territory*> territories; //List of territories controlled by the player
        Hand  * pHand;
        OrdersList * ordersList;
        set<Player*> negotiatedPlayers;
        PlayerStrategy* strategy;
        int reinforcementPool;
    public:
        //Constructor
        Player();
        Player(string name);
        Player(const Player& other);
        Player& operator=(const Player& other);
        // Destructor
        ~Player();
        string getName() const;
         static Player* getNeutralPlayer();

        void setStrategy(PlayerStrategy* strategy);
        void addTerritory(Territory*);

        const vector<Territory*>  toDefend() const; 
        const vector<Territory*>  toAttack() const;

        void issueOrder(Deck* deck);

       friend ostream& operator<<(ostream& out, const Player& player);

        void addNegotiatedPlayer(Player* player);
        bool hasNegotiatedWith(Player* player) const;
        void clearNegotiations(); // Clear negotiated players for a new turn

        const vector<Territory *> getTerritories() const;

        void setReinforcementPool(int armies);
        int getReinforcementPool() const;
        Hand* getHand();

        const vector<Territory*> toAttackFrom() const;
        int get_num_orders() const;
        Order* popOrder();

        vector<Order*> getOrders();
};

#endif
