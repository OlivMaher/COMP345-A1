#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <set>
#include "../Player/Player.h"
using namespace std;

//-------------
// Territory
//-------------

Territory::Territory(const string& name, int x, int y, const string& continent) : name(name), x(x), y(y), continent(continent) {}

Territory::Territory(const string& name) : name(name) {};

string Territory::getName() const {
    return name;
}

void Territory::setOwner(Player* p) {
    owner = p;
}

Player* Territory::getOwner() const {
    return owner;
}

void Territory::setArmies(const int& troops) {
    armies = troops;
}

int Territory::getArmies() const {
    return armies;
}

string Territory::getContinent() const{
    return continent;
}

void Territory::setCoordinates(int xCoord, int yCoord) {
    x = xCoord;
    y = yCoord;
}

void Territory::setContinent(const string& continentName) {
    continent = continentName;
}


void Territory::addAdjacentTerritories(shared_ptr<Territory> territory) {
    adjacentTerritories.push_back(territory);
    cout << "Added adjacent territory: " << territory->getName() << " to " << name << endl;
}

const vector<shared_ptr<Territory>>& Territory::getAdjacentTerritories() const {
    return adjacentTerritories;
}

ostream& operator<<(ostream& os, const Territory& territory) {
    os << "\nTerritory: " << territory.name << "\n-----------------------\n Continent: " << territory.continent
       << " X-cord: " << territory.x << " Y-cord: " << territory.y << " Owner: " << territory.owner
       << " Armies: " << territory.armies << "\nAdjacent Territories: ";
    for (const auto& adjacent : territory.adjacentTerritories) {
        os << adjacent->getName() << " ";
    }
    return os;
}

//-------------
// Continent
//-------------

Continent::Continent(const string& name, int bonus) : name(name), bonus(bonus) {
    cout << "Created Continent: " << name << endl;
}

string Continent::getName() const {
    return name;
}

void Continent::addTerritories(shared_ptr<Territory> territory) {
    territories.push_back(territory);
    cout << "Added territory: " << territory->getName() << " to continent: " << name << endl;
}

const vector<shared_ptr<Territory>>& Continent::getTerritories() const {
    return territories;
}

ostream& operator<<(ostream& os, const Continent& continent) {
    os << "Continent: " << continent.name << " Territories: ";
    for (const auto& territory : continent.territories) {
        os << territory->getName() << ", ";
    }
    os << endl;
    return os;
}

//-------------
// Map
//-------------

Map::~Map() {
    cout << "Map destructor called." << endl;
    // No need to manually delete territories and continents, shared_ptr takes care of it
}

bool Map::validateMap() {
    bool result = validateContinents() && validateTerritories();
    cout << "Map validation result: " << (result ? "Valid" : "Invalid") << endl;
    return result;
}

void Map::DFS(shared_ptr<Territory> territory, vector<shared_ptr<Territory>>& visited, const string& continentName) {
    visited.push_back(territory);

    for (const auto& adjacent : territory->getAdjacentTerritories()) {
        // Check if adjacent territory belongs to the same continent
        if (adjacent->getContinent() == continentName) {
            // Only continue DFS if it hasn't been visited yet
            if (find(visited.begin(), visited.end(), adjacent) == visited.end()) {
                DFS(adjacent, visited, continentName);
            }
        }
    }
}

bool Map::validateContinents() {
    for (const auto& continent : continents) {
        vector<shared_ptr<Territory>> visited;

        // Start DFS from the first territory of the continent
        if (continent->getTerritories().empty()) {
            continue; // Skip continents without territories
        }

        auto startTerritory = continent->getTerritories().front();
        
        // Pass the continent's name to restrict DFS to the same continent
        DFS(startTerritory, visited, continent->getName());

        // Create sets for comparison
        set<shared_ptr<Territory>> visitedSet(visited.begin(), visited.end());
        set<shared_ptr<Territory>> continentTerritories(continent->getTerritories().begin(), continent->getTerritories().end());

        // Check if all continent territories were visited
        if (visitedSet.size() != continentTerritories.size()) {
            cout << "Continent validation failed for: " << continent->getName() << endl;
            return false; // Not all territories in this continent were visited
        }
    }
    cout << "Continents validated successfully." << endl;
    return true;
}

bool Map::validateTerritories() {
    set<shared_ptr<Territory>> allTerritories;
    for (const auto& continent : continents) {
        for (const auto& territory : continent->getTerritories()) {
            if (allTerritories.find(territory) != allTerritories.end()) {
                cout << "Territory validation failed: Duplicate territory found: " << territory->getName() << endl;
                return false;
            }
            allTerritories.insert(territory);
        }
    }
    bool result = allTerritories.size() == territories.size();
    cout << "Territories validation result: " << (result ? "Valid" : "Invalid") << endl;
    return result;
}

void Map::addTerritory(shared_ptr<Territory> territory) {
    territories.push_back(territory);
}

void Map::addContinent(shared_ptr<Continent> continent) {
    continents.push_back(continent);
}

shared_ptr<Continent> Map::findContinentByName(const string& name) {
    for (const auto& continent : continents) {
        if (continent->getName() == name) {
            return continent;
        }
    }
    return nullptr; // If continent not found
}

ostream& operator<<(ostream& os, const Map& map) {
    os << "Map Details: \n";
    for (const auto& continent : map.continents) {
        os << *continent << endl;
    }
    for (const auto& territory : map.territories) {
        os << *territory << endl;
    }
    return os;
}

//-------------
// MapLoader
//-------------

shared_ptr<Map> MapLoader::loadMap(const string& fileName) {
    cout << "Loading map from file: " << fileName << endl;
    auto map = make_shared<Map>();
    ifstream inputFile(fileName);
    if (!inputFile) {
        cout << "Error: Could not open file " << fileName << endl;
        return nullptr;
    }

    unordered_map<string, shared_ptr<Territory>> territoriesMap;
    unordered_map<string, shared_ptr<Continent>> continentsMap; // Added to find continents quickly
    string line;
    string currentSection;

    while (getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }
        if (line[0] == '[') {
            currentSection = line;
            continue;
        }

        // Validate and process sections
        if (currentSection == "[Map]") {
            // No processing needed
            continue;
        } 
        else if (currentSection == "[Continents]") {
            istringstream ss(line);
            string name;
            int bonus;

            // Check if the line has correct format
            if (getline(ss, name, '=') && (ss >> bonus)) {
                auto continent = make_shared<Continent>(name, bonus);
                map->addContinent(continent);
                continentsMap[name] = continent; // Store in unordered_map
            } else {
                cout << "Error: Invalid continent data in line: " << line << endl;
                return nullptr;  // Early return if invalid data
            }
        } 
        else if (currentSection == "[Territories]") {
            istringstream ss(line);
            string name, continentName;
            int x, y;

            // Check if the line has correct format
            getline(ss, name, ','); // Read territory name
            ss >> x;                // Read X coordinate
            ss.ignore();            // Ignore comma
            ss >> y;                // Read Y coordinate
            ss.ignore();            // Ignore comma
            getline(ss, continentName, ','); // Read continent name

            // Validate and create Territory
            if (name.empty() || continentName.empty()) {
                return nullptr;  // Invalid territory data
            }

            // Retrieve or create Territory
            shared_ptr<Territory> territory;
            if (territoriesMap.find(name) != territoriesMap.end()) {
                territory = territoriesMap[name];
                // Update the territory's x, y, and continent fields
                territory->setCoordinates(x, y);
                territory->setContinent(continentName);
            } else {
                territory = make_shared<Territory>(name, x, y, continentName);
                territoriesMap[name] = territory;
                map->addTerritory(territory);  // Add territory to map (not necessarily a continent)
            }

            // Add the territory to its continent
            auto continent = map->findContinentByName(continentName);
            if (continent) {
                continent->addTerritories(territory);
            } else {
                return nullptr;  // Fail if continent not found
            }

            // Handle adjacent territories (but do NOT add them to the same continent)
            string adjacent;
            while (getline(ss, adjacent, ',')) {
                // Remove potential leading/trailing whitespace
                adjacent.erase(0, adjacent.find_first_not_of(" \t"));
                adjacent.erase(adjacent.find_last_not_of(" \t") + 1);

                // Create or retrieve adjacent territory
                shared_ptr<Territory> adjTerritory;
                if (territoriesMap.find(adjacent) != territoriesMap.end()) {
                    adjTerritory = territoriesMap[adjacent];
                } else {
                    // Create a new Territory for adjacency if it doesn't exist yet
                    adjTerritory = make_shared<Territory>(adjacent);
                    territoriesMap[adjacent] = adjTerritory;
                    map->addTerritory(adjTerritory);  // Add adjacent territory to the map, but NOT to the continent
                }
                territory->addAdjacentTerritories(adjTerritory); // Link adjacent territories
            }
        }
    }
    inputFile.close();

    // Post-loading validation
    if (!map->validateMap()) {
        cout << "Error: Loaded map is invalid." << endl;
        return nullptr;  // Return nullptr if validation fails
    }

    cout << "Map loaded successfully." << endl;
    return map;
}