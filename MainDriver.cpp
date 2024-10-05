#include "Player/PlayerDriver.h"  // Directly include the .cpp file
#include "Map/MapDriver.h"
#include "Orders list/OrdersDriver.h"
#include "./Game Engine/GameEngineDriver.h"


int main() {
    testLoadMaps();
    testPlayer();
    testOrdersLists();
    testGameStates();


    return 0;
}
