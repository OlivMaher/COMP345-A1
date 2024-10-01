#include "Map.h"

using namespace std;

//-------------
// Territory
//-------------

Territory::Territory(string name, int x, int y, string continent): name(name), x(x), y(y), continent(continent) {}
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
    cout << "\nTerritory: " << name << "\n-----------------------\n Continent: "<< continent <<" Owner: " << owner << " Armies: " << armies << "\n";
}

//-------------
// Continent
//-------------
Continent::Continent(string name, int bonus): name(name), bonus(bonus){}

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
    if(validateContinents() && validateTerritories()){
        return true;
    }
    else{
        return false;
    }

}
//Checks if the graph is connected
void Map::DFS(Territory* territory, vector<Territory*>& visited){
    visited.push_back(territory);
    for(Territory* adjacent: territory->getAdjacentTerritories()){
        if(find(visited.begin(), visited.end(), adjacent) == visited.end()){
            DFS(adjacent, visited);
        }
    }
}
bool Map::validateContinents(){
    for (Continent* continent : continents) {
        vector<Territory*> visited;
        Territory* startTerritory = continent->getTerritories().front();
        DFS(startTerritory, visited);

        set<Territory*> visitedSet(visited.begin(), visited.end());
        set<Territory*> continentTerritories(continent->getTerritories().begin(), continent->getTerritories().end());

        if (visitedSet.size() != continentTerritories.size()) {
            return false;
        }
    }
    return true;
}

bool Map::validateTerritories(){
    set<Territory*> allTerritories;
    for(Continent* continent: continents){
        for(Territory* territory: continent->getTerritories()){
            if(allTerritories.find(territory) != allTerritories.end()){
                return false;
            }
            allTerritories.insert(territory);
        }
    }
    if (allTerritories.size() != territories.size()){
        return false;
    }
    else{
        return true;    
    }

}

//Adders
void Map::addTerritory(Territory* territory){
    territories.push_back(territory);
}
void Map::addContinent(Continent* continent){
    continents.push_back(continent);
}   
//-------------
// MapLoader
//-------------

void MapLoader::loadMap(string fileName){
    Map map{};
    ifstream inputFile(fileName);
    if(!inputFile){
        cout << "Error: Could not open file" << endl;
        //exit(1);
    }
    string line;
    string currentSection;
    while(getline(inputFile, line)){
        if(line.empty()){
            continue;
        }
        if(line[0] == '['){
            currentSection = line;
            continue;
        }
        //Process Sections
        if(currentSection == "[Map]"){
            //Do nothing
            continue;
        }
        else if (currentSection == "[Continents]")
        {
            istringstream ss(line);
            string name;
            int bonus;
            getline(ss, name, '=');
            ss >> bonus;
            Continent* continent = new Continent(name, bonus);
            map.addContinent(continent);
            continent->printContinent();
            delete continent;
        }
        else if(currentSection == "[Territories]"){
            istringstream ss(line);
            string name, continent, adjacent;
            int x, y;  

            getline(ss, name, ',');
            ss >> x;
            ss.ignore();
            ss >> y;
            ss.ignore();
            getline(ss, continent, ',');
            
            Territory* territory = new Territory(name, x, y, continent);

            while (getline(ss, adjacent, ',')){
                Territory* temp = new Territory(adjacent);
                territory->addAdjacentTerritories(temp);
                delete temp;
            }
            map.addTerritory(territory);
            territory->printTerritory();
            delete territory;
        }
        
    }
    inputFile.close(); 

}



int main(){
    
    MapLoader mapLoader{};
    mapLoader.loadMap("c:\\Users\\olima\\dev\\COMP345\\COMP345-A1\\USA.map");

    return 0;
}