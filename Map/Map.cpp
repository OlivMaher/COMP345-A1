#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <set>

using namespace std;

//-------------
// Territory
//-------------

Territory::Territory(const string& name, int x, int y, const string& continent) : name(name), x(x), y(y), continent(continent) {
    cout << "Created Territory: " << name << endl;
}

Territory::Territory(const string& name) : name(name) {
    cout << "Created Territory with name only: " << name << endl;
}

string Territory::getName() const {
    return name;
}

void Territory::setOwner(const string& newOwner) {
    owner = newOwner;
}

string Territory::getOwner() const {
    return owner;
}

void Territory::setArmies(const int& troops) {
    armies = troops;
}

int Territory::getArmies() const {
    return armies;
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
    cout << "Validating map..." << endl;
    bool result = validateContinents() && validateTerritories();
    cout << "Map validation result: " << (result ? "Valid" : "Invalid") << endl;
    return result;
}

void Map::DFS(shared_ptr<Territory> territory, vector<shared_ptr<Territory>>& visited) {
    visited.push_back(territory);
    cout << "Visiting territory: " << territory->getName() << endl; // Debugging output

    for (const auto& adjacent : territory->getAdjacentTerritories()) {
        if (find(visited.begin(), visited.end(), adjacent) == visited.end()) {
            DFS(adjacent, visited);
        }
    }
}

bool Map::validateContinents() {
    cout << "Validating continents..." << endl;
    for (const auto& continent : continents) {
        cout << "Validating continent: " << continent->getName() << endl;
        vector<shared_ptr<Territory>> visited;

        // Start DFS from the first territory of the continent
        if (continent->getTerritories().empty()) {
            cout << "Warning: Continent " << continent->getName() << " has no territories." << endl;
            continue; // Skip continents without territories
        }

        auto startTerritory = continent->getTerritories().front();
        cout << "Starting DFS from territory: " << startTerritory->getName() << endl;
        DFS(startTerritory, visited);

        // Create sets for comparison
        set<shared_ptr<Territory>> visitedSet(visited.begin(), visited.end());
        set<shared_ptr<Territory>> continentTerritories(continent->getTerritories().begin(), continent->getTerritories().end());

        cout << "Visited territories: ";
        for (const auto& territory : visitedSet) {
            cout << territory->getName() << " ";
        }
        cout << endl;

        cout << "Continent territories: ";
        for (const auto& territory : continentTerritories) {
            cout << territory->getName() << " ";
        }
        cout << endl;

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
    cout << "Validating territories..." << endl;
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
    cout << "Added territory to map: " << territory->getName() << endl;
}

void Map::addContinent(shared_ptr<Continent> continent) {
    continents.push_back(continent);
    cout << "Added continent to map: " << continent->getName() << endl;
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
            ss.ignore();           // Ignore comma
            ss >> y;                // Read Y coordinate
            ss.ignore();           // Ignore comma
            getline(ss, continentName, ','); // Read continent name

            // Validate and create Territory
            if (name.empty() || continentName.empty()) {
                cout << "Error: Territory name or continent name is missing in line: " << line << endl;
                return nullptr;  // Invalid territory data
            }

            // Retrieve or create Territory
            shared_ptr<Territory> territory;
            if (territoriesMap.find(name) != territoriesMap.end()) {
                territory = territoriesMap[name];
            } else {
                territory = make_shared<Territory>(name, x, y, continentName);
                territoriesMap[name] = territory;
                map->addTerritory(territory);
            }

            // Handle adjacent territories
            string adjacent;
            while (getline(ss, adjacent, ',')) {
                // Create or retrieve adjacent territory
                shared_ptr<Territory> adjTerritory;
                if (territoriesMap.find(adjacent) != territoriesMap.end()) {
                    adjTerritory = territoriesMap[adjacent];
                } else {
                    // Create a new Territory for adjacency if it doesn't exist yet
                    adjTerritory = make_shared<Territory>(adjacent);
                    territoriesMap[adjacent] = adjTerritory;
                    map->addTerritory(adjTerritory);
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