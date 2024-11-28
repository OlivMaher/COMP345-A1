//using Strategy patern

//PlayerStrategies abstract class

#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include <iostream>
#include <string>
#include <vector>
#include "../Orders_list/Orders.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"

using namespace std;

class Player;


class PlayerStrategy {
    public:
        virtual void issueOrder(Player *player, Deck *deck, OrdersList* ordersList) = 0;
        virtual vector<Territory*> toDefend(Player *player) = 0;
        virtual vector<Territory*> toAttack(Player *player) = 0;


};

class HumanPlayerStrategy : public PlayerStrategy {
    public:
        void issueOrder(Player *player, Deck *deck, OrdersList* ordersList) override;
        vector<Territory*> toDefend(Player *player) override;
        vector<Territory*> toAttack(Player *player) override;
        vector<Territory*> toAttackFrom(Player *player);

};

#endif