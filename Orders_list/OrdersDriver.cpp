#include "Orders.h"
#include "Orders.cpp"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Player/Player.cpp"
#include "../Map/Map.cpp"
#include "../Cards/Cards.cpp"
#include "../LoggingObserver/LoggingObserver.h"
#include "../LoggingObserver/LoggingObserver.cpp"
#include <iostream>
#include <memory>


    Player neutralPlayer("Neutral");
    
void testOrderExecution() {
    Deck deck;
    // Create the Neutral player


    // Create sample players
    Player player1("Player 1");
    Player player2("Player 2");

    // Create territories and set owners
    auto territory1 = std::make_shared<Territory>("Territory 1", 0, 0, "Continent 1");
    auto territory2 = std::make_shared<Territory>("Territory 2", 1, 1, "Continent 1");
    auto territory3 = std::make_shared<Territory>("Territory 3", 2, 2, "Continent 1");

    territory1->setOwner(&player1);
    territory2->setOwner(&player1);
    territory3->setOwner(&player2);

    territory1->setArmies(10);
    territory2->setArmies(5);
    territory3->setArmies(8);

    player1.addTerritory(territory1.get());
    player1.addTerritory(territory2.get());
    player2.addTerritory(territory3.get());

    // Set adjacency between territories
    territory1->addAdjacentTerritories(territory2);
    territory2->addAdjacentTerritories(territory3);

// 1. Validate and execute Deploy Order
    Deploy deployOrder(&player1, territory1.get(), 3);
    if (deployOrder.validate()) {
        deployOrder.execute();
        std::cout << "Deploy Order: " << deployOrder.getEffect() << std::endl;
    } else {
        std::cout << "Deploy Order: Invalid" << std::endl;
    }

    std::cout <<" "<<endl;

// 2. Validate and execute Advance Order (defensive)
    Advance advanceOrder(&player1, territory1.get(), territory2.get(), 5);
    if (advanceOrder.validate()) {
        advanceOrder.execute();
        std::cout << "Advance Order (defensive): " << advanceOrder.getEffect() << std::endl;
    } else {
        std::cout << "Advance Order (defensive): Invalid" << std::endl;
    }

    std::cout <<" "<<endl;

// 3. Validate and execute Advance Order (offensive)
    Advance attackOrder(&player1, territory2.get(), territory3.get(), 4);
    if (attackOrder.validate()) {
        attackOrder.execute();
        std::cout << "Advance Order (offensive): " << attackOrder.getEffect() << std::endl;

        // Check if player1 gets a card for conquering
        if (territory3->getOwner() == &player1) {
            // Draw a card from the deck and add it to player1's hand
            Card drawnCard = deck.draw();
        }
    } else {
        std::cout << "Advance Order (offensive): Invalid" << std::endl;
    }

    std::cout <<" "<<endl;

// 4. Validate and execute Bomb Order
    Bomb bombOrder(&player1, territory3.get());
    if (bombOrder.validate()) {
        bombOrder.execute();
        std::cout << "Bomb Order: " << bombOrder.getEffect() << std::endl;
    } else {
        std::cout << "Bomb Order: Invalid" << std::endl;
    }
    
    std::cout <<" "<<endl;


// 5. Validate and execute Blockade Order
    Blockade blockadeOrder(&player1, territory1.get());
    if (blockadeOrder.validate()) {
        blockadeOrder.execute();
        territory1->setOwner(&neutralPlayer);  // Transfer to the Neutral player
        std::cout << "Blockade Order: " << blockadeOrder.getEffect() << std::endl;
    } else {
        std::cout << "Blockade Order: Invalid" << std::endl;
    }
    
    std::cout <<" "<<endl;


// 6. Validate and execute Airlift Order
    Airlift airliftOrder(&player1, territory2.get(), territory1.get(), 2);
    if (airliftOrder.validate()) {
        airliftOrder.execute();
        std::cout << "Airlift Order: " << airliftOrder.getEffect() << std::endl;
    } else {
        std::cout << "Airlift Order: Invalid" << std::endl;
    }

    territory1->setOwner(&player1);
    player1.addTerritory(territory1.get());
    
    std::cout <<" "<<endl;

// 6. Validate and execute Airlift Order
    airliftOrder = Airlift(&player1, territory2.get(), territory1.get(), 2);
    if (airliftOrder.validate()) {
        airliftOrder.execute();
        std::cout << "Airlift Order: " << airliftOrder.getEffect() << std::endl;
    } else {
        std::cout << "Airlift Order: Invalid" << std::endl;
    }

    std::cout <<" "<<endl;

// 7. Validate and execute Negotiate Order
    Negotiate negotiateOrder(&player1, &player2);
    if (negotiateOrder.validate()) {
        negotiateOrder.execute();
        std::cout << "Negotiate Order: " << negotiateOrder.getEffect() << std::endl;
    } else {
        std::cout << "Negotiate Order: Invalid" << std::endl;
    }

    std::cout <<" "<<endl;

// Print final state of the territories
    std::cout << "Final state of territories:" << std::endl;
    std::cout << territory1->getName() << " owned by " << (territory1->getOwner() ? territory1->getOwner()->getName() : "None") << std::endl;
    std::cout << territory2->getName() << " owned by " << (territory2->getOwner() ? territory2->getOwner()->getName() : "None") << std::endl;
    std::cout << territory3->getName() << " owned by " << (territory3->getOwner() ? territory3->getOwner()->getName() : "None") << std::endl;
}

int main() {
    testOrderExecution();
    return 0;
}
