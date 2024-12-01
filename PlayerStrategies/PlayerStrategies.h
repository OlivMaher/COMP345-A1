#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H

#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Orders_list/Orders.h"
#include "../Cards/Cards.h"

#include <vector>

class PlayerStrategy {
public:
    virtual void issueOrder(Player* player, Deck* deck, OrdersList* ordersList) = 0;
    virtual std::vector<Territory*> toDefend(Player* player) = 0;
    virtual std::vector<Territory*> toAttack(Player* player) = 0;
    virtual ~PlayerStrategy() {}
};

// HumanPlayerStrategy class
class HumanPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, Deck* deck, OrdersList* ordersList) override;
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toAttackFrom(Player* player);
};

// AggressivePlayerStrategy class
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, Deck* deck, OrdersList* ordersList) override;
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    std::vector<Territory*> toAttackFrom(Player* player);
};

// BenevolentPlayerStrategy class
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, Deck* deck, OrdersList* ordersList) override;
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
};

// NeutralPlayerStrategy class
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, Deck* deck, OrdersList* ordersList) override;
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
};

// CheaterPlayerStrategy class
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, Deck* deck, OrdersList* ordersList) override;
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
};

#endif // PLAYERSTRATEGIES_H
