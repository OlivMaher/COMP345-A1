#include "MapDriver.h"

void testLoadMaps(){
    MapLoader mapLoader{};
    mapLoader.loadMap(".\\MapFiles\\Invalid.map");
}
