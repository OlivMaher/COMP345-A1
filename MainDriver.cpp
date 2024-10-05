#include "Player/PlayerDriver.h"  // Directly include the .cpp file
<<<<<<< HEAD
#include "Player/Player.h"
#include "./Map/MapDriver.h"
#include "./Map/Map.h"
#include "./Orders list/Orders.h"
#include "./Orders list/OrdersDriver.cpp"
=======
#include "Map/MapDriver.h"
#include "Orders list/OrdersDriver.h"
>>>>>>> d89f4ab9a542679c73f441cb4cce98a47ec0ac72
#include "./Game Engine/GameEngineDriver.h"
#include "./Game Engine/GameEngine.h"


int main() {
    testLoadMaps();
    testPlayer();
    testOrdersLists();
    testGameStates();


    return 0;
}
