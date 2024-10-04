#include "Player.h"
#include "../Orders list/Orders.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
using namespace std;



void testPlayers(){
    Territory *t1 = new Territory("New York", 9, 2, "North America");
    Player * p1 = new Player("Jack");
    Advance* o1 = new Advance();

    p1 -> addTerritory(t1);
    p1->issueOrder(o1);

    cout << *p1;

    delete p1;
}

int main(){
    testPlayers();
    return 0;
}