#include <iostream>
#include <string>
#include <vector>
#include "MapDriver.h"
#include "Map.h"

using namespace std;

void testLoadMaps() {
    vector<string> mapFiles = {
        "MapFiles\\Alabama.map",
        "MapFiles\\USA.map",
        "MapFiles\\Invalid.map"
    };
    MapLoader mapLoader;
    for (const string& fileName : mapFiles) {
        cout << "\nLoading map: " << fileName << endl;
        auto loadedMap = mapLoader.loadMap(fileName);
        if (loadedMap == nullptr) {
            cout << "Map loading failed for: " << fileName << "\nReason: Invalid map data.\n";
        } 
        else {
            cout << "Map successfully loaded: " << fileName << endl;
            cout << "Validating map..." << endl;
            if (loadedMap->validateMap()) {
                cout << "Validation successful for: " << fileName << endl;
                cout << *loadedMap << endl;  // Display the map details using overloaded << operator
            } else {
                cout << "Validation failed for: " << fileName << "\nThe map is not valid.\n";
            }
        }
    }
}

int main() {
    cout << "Map Driver" << endl;
    testLoadMaps();
    cout << "Map Driver finished." << endl;
    return 0;
}
