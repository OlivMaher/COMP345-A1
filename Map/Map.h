#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Territory{
    private:
        string name; 
        string owner; //Player who has control of this territory TODO: Replace with player
        int armies = 0;
        vector<Territory*> adjacentTerritories; //Adjacency list of territories
        int x, y; //Cordinates
        string continent;
    public:
        //Constructor
        Territory(string name, int x, int y, string continent);
        Territory(string name);
        //Setters & Getters 
        string getName() const;

        void setOwner(const string&);
        string getOwner() const;

        void setArmies(const int&);
        int getArmies() const;

        void addAdjacentTerritories(Territory*);
        const vector<Territory*>& getAdjacentTerritories() const;

        void printTerritory() const;
};
class Continent{
    private:
        string name;
        vector<Territory*> territories; //List of territories within the continent
        int bonus; // ?? unsure what the number represents
    public:
        //Constructor
        Continent(string name, int bonus);

        //Setters & Getters
        string getName() const;

        void addTerritories(Territory* territory);
        const vector<Territory*>& getTerritories() const;

        void printContinent() const;

};
class Map{
    private:
        vector<Territory*> territories; //List of every territory
        vector<Continent*> continents; //List of every continent
    public:
        ~Map(); //Deconstructor
        bool validateMap(); //Implement with Depth First Search
        void DFS(Territory* territory, vector<Territory*>& visited); //Depth First Search
        bool validateContinents(); //Validates if each Continent is a connected subgraph
        bool validateTerritories(); //Validates if each territory belongs to one Continent
        void addTerritory(Territory* territory);
        void addContinent(Continent* continent);


};
class MapLoader{
    public:
        void loadMap(string fileName);
};