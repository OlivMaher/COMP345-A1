#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>
#include <sstream>
#include <memory> 
#include <unordered_map>
#include "../Player/Player.h"
using namespace std;

class Player;

class Territory {
private:
    string name;
    Player* owner; // Player who has control of this territory
    int armies = 0;
    vector<shared_ptr<Territory>> adjacentTerritories; // Adjacency list of territories
    int x = 0, y = 0; // Coordinates
    string continent;

public:
    // Constructors
    Territory(const string& name, int x, int y, const string& continent);
    Territory(const string& name);

    // Setters & Getters
    string getName() const;
    void setOwner(Player* p);
    Player* getOwner() const;
    void setArmies(const int&);
    int getArmies() const;
    string getContinent() const;
    void setCoordinates(int xCoord, int yCoord);
    void setContinent(const string&);

    void addAdjacentTerritories(shared_ptr<Territory>);

    const vector<shared_ptr<Territory>>& getAdjacentTerritories() const;

    inline void addArmies(int num) {
        armies += num;
    }

    inline void removeArmies(int num) {
        armies = std::max(0, armies - num); // Prevent negative armies
    }

    bool isAdjacent(Territory* other) const;

    // Operator overloading for printing
    friend ostream& operator<<(ostream& os, const Territory& territory);


};

class Continent {
private:
    string name;
    vector<shared_ptr<Territory>> territories; // List of territories within the continent
    int bonus = 0; // Bonus armies or score for holding this continent

public:
    // Constructor
    Continent(const string& name, int bonus);

    // Setters & Getters
    string getName() const;
    void addTerritories(shared_ptr<Territory> territory);
    const vector<shared_ptr<Territory>>& getTerritories() const;

    int getBonus() const;

    // Operator overloading for printing


    friend ostream& operator<<(ostream& os, const Continent& continent);
};

class Map {
private:
    vector<shared_ptr<Territory>> territories; // List of every territory
    vector<shared_ptr<Continent>> continents; // List of every continent

public:
    ~Map(); // Destructor
    bool validateMap(); // Implement with Depth First Search
    void DFS(shared_ptr<Territory> territory, vector<shared_ptr<Territory>>& visited, const string& continentName); // Depth First Search
    bool validateContinents(); // Validates if each Continent is a connected subgraph
    bool validateTerritories(); // Validates if each territory belongs to one Continent
    void addTerritory(shared_ptr<Territory> territory);
    void addContinent(shared_ptr<Continent> continent);
    shared_ptr<Continent> findContinentByName(const string& name); 
    
    // Operator overloading for printing
    friend ostream& operator<<(ostream& os, const Map& map);

    const vector<shared_ptr<Territory> >& getTerritories() const;
    const vector<shared_ptr<Continent> >& getContinents() const;
};

class MapLoader {
public:
    shared_ptr<Map> loadMap(const string& fileName);
};
