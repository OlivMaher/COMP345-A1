#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Territory{
    private:
        string name; 
        string owner; //Player who has control of this territory TODO: Replace with player
        int armies = 0;
        vector<Territory*> adjacentTerritories; //Adjacency list of territories
    public:
        //Constructor
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
    public:
        //Constructor
        Continent(string name);

        //Setters & Getters
        string getName() const;

        void addTerritories(Territory* territory);
        const vector<Territory*>& getTerritories() const;

        void printContinent() const;

};