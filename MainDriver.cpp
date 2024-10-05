#include "./Player/PlayerDriver.h"  // Directly include the .cpp file
#include "./Map/MapDriver.h"
#include "./Orders_list/OrdersDriver.cpp"
#include "./Game_Engine/GameEngineDriver.h"
#include "./Cards/CardsDriver.h"



int main() {
    testLoadMaps();
    testPlayer();
    testCards();
    testOrdersLists();
    testGameStates();


    return 0;
}
