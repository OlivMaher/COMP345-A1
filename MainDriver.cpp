#include "Player/PlayerDriver.h"  // Directly include the .cpp file
#include "Player/Player.h"
#include "./Map/MapDriver.h"
#include "./Map/Map.h"
#include "./Orders list/Orders.h"
#include "./Orders list/OrdersDriver.cpp"
#include "./Game Engine/GameEngineDriver.h"
#include "./Game Engine/GameEngine.h"


int main() {
    testLoadMaps();
    testPlayer();
    testOrdersLists();
    testGameStates();


    return 0;
}
