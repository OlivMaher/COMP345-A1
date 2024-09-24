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

//-------------
// Continent
//-------------
Continent::Continent(string name): name(name) {}

string Continent::getName() const{
    return name;
}
void Continent::addTerritories(Territory* territory){
    territories.push_back(territory);
}
const vector<Territory*>& Continent::getTerritories() const{
    return territories;
}
void Continent::printContinent() const{
    cout << "Continent: " << name << "\n" << "Territories: ";
    for(Territory* territory: this->territories){
        cout << territory->getName() << ", ";
    }
}

//-------------
// Map
//-------------
Map::~Map(){
    for(Territory* t: territories){
        delete t;
    }
    for(Continent* c: continents){
        delete c;
    }
}
bool Map::validateMap(){
    //TODO: DFS to validate every node is reachable
}
void Map::DFS(Territory* territory, vector<Territory*>& visited){
    visited.push_back(territory);
    for(Territory* adjacent: territory->getAdjacentTerritories()){
        if(find(visited.begin(), visited.end(), adjacent) == visited.end()){
            DFS(adjacent, visited);
        }
    }
}
bool Map::validateContinents(){
    //TODO: connected subgraph
}
bool Map::validateTerritories(){
    //TODO: Territory -> One Continent
}

//Adders
void Map::addTerritory(Territory* territory){
    territories.push_back(territory);
}
void Map::addContinent(Continent* continent){
    continents.push_back(continent);
}   


int main(){
    Territory* a1 = new Territory("A");
    Territory* b1 = new Territory("b");
    Continent* one = new Continent("one");

    one->addTerritories(a1);
    one->addTerritories(b1);
    one->printContinent();



    delete a1;
    delete b1;
    delete one;

    return 0;
}