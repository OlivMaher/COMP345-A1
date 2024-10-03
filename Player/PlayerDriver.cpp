#include "Player.cpp"
using namespace std;



void testPlayers(){

    

    Territory *t2 = new Territory("New Jersey", 9, 3, "North America");
    Territory *t1 = new Territory("New York", 9, 2, "North America");
    //Territory *t3 = new Territory("Florida", 9, 10, "North America");


    Player * p1 = new Player("Jack", t1);
    Player * p2 = new Player("Jack", t2);

    Advance* o1 = new Advance(p1, t1, t2, 4);

    p1-> issueOrder(o1);

    cout << p1;

    delete t1; 
    delete t2; 
    delete p1;
    delete p2;
    delete o1;



}

int main(){
    testPlayers();
    return 0;
}