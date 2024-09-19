#include "Map.h"

using namespace std;

//-------------
// Territory
//-------------

Territory::Territory(string name): name(name) {}

string Territory::getName() const{
    return name;
}
void Territory::setOwner(const string& newOwner){
    owner = newOwner;
}
string Territory::getOwner() const{
    return owner;
}
void Territory::setArmies(const int& troops){
    armies = troops;
}
int Territory::getArmies() const{
    return armies;
}
void Territory::addAdjacentTerritories(Territory* territory){
    adjacentTerritories.push_back(territory);
}
const vector<Territory*>& Territory::getAdjacentTerritories() const{
    return adjacentTerritories;
}
void Territory::printTerritory() const{
    cout << "Territory: " << name << "\n"
              << "Owner: " << owner << "\n"
              << "Armies: " << armies << "\n";
}


int main(){
    Territory* a1 = new Territory("A");
    a1->printTerritory();
    delete a1;

    return 0;
}