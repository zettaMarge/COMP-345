#include "Map.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Territory and Continent constructors

Territory::Territory(const std::string& _name, Continent* _continent){
    name = _name;
    continent = _continent;
}

Continent::Continent(const std::string& _name, int _points){
    name = _name;
    points = _points;
}

// MAP CLASS IMPLEMENTATION

Map::~Map(){
    for (auto c : continents) delete c;
    for (auto t : territories) delete t;
}

// Creates a new continent, adds it to the continent vector and hashmap.
// Parameters:
// - name: the continent's name
// - points: points for controlling the continent
// Returns:
// - A pointer to the newly created Continent object
Continent* Map::addContinent(const std::string& name, int points){
    Continent* continent = new Continent(name, points);
    continents.push_back(continent);
    continentByName[name] = continent;
    return continent;
}

// Creates a new territory, adds it to the territory vector and hashmap.
// Adds the territory to the territories vector of the continent it belongs to.
// Parameters:
// - name: the territory's name
// - continent: pointer to the continent it belongs to
// Returns:
// - A pointer to the newly created Territory object
Territory* Map::addTerritory(const std::string& name, Continent* continent){
    Territory* territory = new Territory(name, continent);
    territories.push_back(territory);
    territoryByName[name] = territory;
    continent->territories.push_back(territory);
    return territory;
}

// Checks if a territory 'b' is already in the adjacent territories list of territory 'a'.
// Parameters:
// - a: pointer to a territory with the adjacent list that is being searched
// - b: pointer to a territory to look for in the adjacent list
// Returns:
// - true if 'b' is already listed as adjacent to 'a', false otherwise
bool Map::isAdjacent(Territory* a, Territory* b){
    return std::find(a->adj.begin(), a->adj.end(), b) != a->adj.end();
}

// Adds bidirectional adjacency between territories avoiding duplicates.
// Parameters:
// - from: Territory from where the adjacency is being added
// - to: Territory to be connected with the first Territory
// Returns:
// - void
void Map::addAdjacency(Territory* from, Territory* to){
    if(!isAdjacent(from, to)){
        from->adj.push_back(to);
    }
    if(!isAdjacent(to, from)){
        to->adj.push_back(from);
    }
}

// Returns pointer to a Continent object by name
Continent* Map::getContinentByName(const std::string& name){
    return continentByName[name];
}

// Returns pointer to a Territory object by name
Territory* Map::getTerritoryByName(const std::string& name){
    return territoryByName[name];
}

// Performs a depth-first search in all the map, starting with any node
// in the map and keeping track of the visited nodes in a set.
// Parameters:
// - current: Pointer to a Territory
// - visited: set of Territory pointers
void Map::dfs(Territory* current, std::unordered_set<Territory*>& visited){
    visited.insert(current);
    for(int i = 0; i < current->adj.size(); i++){
        Territory* neighbor = current->adj[i];
        if(visited.find(neighbor) == visited.end()){
            dfs(neighbor, visited);
        }
    }
}

// Performs a depth-first search in a continent, starting with any node in it.
// Keeps track of the visited territories.
// Parameters:
// - current: Pointer to a Territory
// - visited: set of Territory pointers
// - continent: Pointer to the continent to check
void Map::dfsContinent(Territory* current, std::unordered_set<Territory*>& visited, Continent* continent){
    visited.insert(current);
    for(int i = 0; i < current->adj.size(); i++){
        Territory* neighbor = current->adj[i];
        if(neighbor->continent == continent && visited.find(neighbor) == visited.end()){
            dfsContinent(neighbor, visited, continent);
        }
    }
}

// Validates that:
// - Each territory belongs to one continent
// - The map is a connected graph, using the dfs helper function
// - Each continent is a connected subgraph, using the dfs continent version helper function
// Returns:
// - true if the map has the previous characteristics, false otherwise
bool Map::validate() {
    // Each territory belongs to one continent
    for(int i = 0; i < territories.size(); i++){
        if(territories[i]->continent == nullptr){
            std::cerr << "The territory " << territories[i]->name << "does not belong to a continent." << std::endl;
            return false;
        }
    }

    // The map is a connected graph
    std::unordered_set<Territory*> visited;
    dfs(territories[0], visited);
    
    if(visited.size() != territories.size()) {
        std::cerr << "The map is not a connected graph." << std::endl;
        return false;
    }

    // Each continent is a connected subgraph
    for(int i = 0; i < continents.size(); i++){
        if(continents[i]->territories.empty()){
            std::cerr << "Continent " << continents[i]->name << " has no territories." << std::endl;
            return false;
        }
        std::unordered_set<Territory*> visited;
        dfsContinent(continents[i]->territories[0], visited, continents[i]);

        if(visited.size() != continents[i]->territories.size()){
            std::cerr << "Continent " << continents[i]->name << " is not connected." << std::endl;
            return false;
        }
    }

    return true;
}

// MAP LOADER IMPLEMENTATION

// Reads any .txt file and validates if it is a valid map.
// Parameters:
// - filename: the name of the file
// Returns:
// - true if the .txt is non empty and follows a valid map format
bool MapLoader::loadMapFile(const std::string& filename){
    if(!loadFile(filename) || content.empty() || !validateMap()) return false;
    return true;
}

// Opens .txt file and adds all lines to a vector of strings, avoiding empty lines.
// Parameters:
// - filename: the name of the file
// Returns:
// - true if the .txt is non empty and follows a valid map format, false otherwise
bool MapLoader::loadFile(const std::string& filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "Could not open file: " << filename;
        return false;
    }

    std::string line;
    while(std::getline(file, line)){
        if(!line.empty() && !std::all_of(line.begin(), line.end(), isspace)){
            content.push_back(line);
        }
    }

    file.close();
    return true;
}

// Checks if the file format corresponds to a Conquest map.
// Conquest maps have 3 sections:
// - [Map] contains key-value strings for author, image file, etc.
// - [Continents] contains the continent name and the scores for owning them completely.
// - [Territories] contains the territory, coordinates, continent it belongs to, and adjacent territories.
// Returns:
// - true if it is a valid map format, false otherwise
bool MapLoader::validateMap(){
    bool hasMetaData = false, hasContinents = false, hasTerritories = false;
    std::string checking = "None";

    for(int i = 0; i < content.size(); i++){
        // Checks if the file contains any of the headers used in a Conquest map
        if(content[i] == "[Map]"){
            hasMetaData = true;
            checking = "MetaData";
            continue;
        }else if(content[i] == "[Continents]"){
            hasContinents = true;
            checking = "Continents";
            continue;
        }else if(content[i] == "[Territories]"){
            hasTerritories = true;
            checking = "Territories";
            continue;
        }

        // Check if the information within a section has a valid format
        if(checking == "MetaData"){
            if(content[i].find("=") == std::string::npos) return false;
        }else if(checking == "Continents"){
            size_t valueAssign = content[i].find("=");
            if(valueAssign == std::string::npos) return false;

            // Check if the value is an integer
            try{
                std::stoi(content[i].substr(valueAssign + 1));
            }catch(...){
                return false;
            }
        }else if(checking == "Territories"){
            // Check if a territory has minimum parameters
            if(std::count(content[i].begin(), content[i].end(), ',') < 3) return false;
        }
    }

    return hasMetaData && hasContinents && hasTerritories;
}

// Creates a Map object using the validated Map data.
// After parsing each section, it adds the adjacencies between each territory.
// Returns:
// - Map object
Map MapLoader::createMap(){
    std::string checking = "None";
    std::vector<std::pair<std::string, std::vector<std::string>>> pending;
    Map gameMap;

    // Check the map's headers
    for(int i = 0; i < content.size(); i++){
        if(content[i] == "[Continents]"){
            checking = "Continents";
            continue;
        }else if(content[i] == "[Territories]"){
            checking = "Territories";
            continue;
        }

        // Create and add the continents
        if(checking == "Continents"){
            size_t valueAssign = content[i].find("=");
            std::string key = content[i].substr(0, valueAssign);
            int value = std::stoi(content[i].substr(valueAssign + 1));
            gameMap.addContinent(key, value);
        }else if(checking == "Territories"){
            std::stringstream ss(content[i]);
            std::string token;

            std::getline(ss, token, ',');
            std::string name = token;

            // Ignore the x and y coordinates
            std::getline(ss, token, ',');
            std::getline(ss, token, ',');

            std::getline(ss, token, ',');
            std::string continent = token;

            Continent* cont = gameMap.getContinentByName(continent);
            Territory* terr = gameMap.addTerritory(name, cont);

            std::vector<std::string> adjTerr;
            while(std::getline(ss, token, ',')){
                adjTerr.push_back(token);
            }

            // Add territory with its adjacent territories to "pending" vector
            pending.push_back({name, adjTerr});
        }
    }

    // Add adjacencies of each territory by going through the "pending" vector
    for(int i = 0; i < pending.size(); i++){
        Territory* from = gameMap.getTerritoryByName(pending[i].first);
        for(int j = 0; j < pending[i].second.size(); j++){
            Territory* to = gameMap.getTerritoryByName(pending[i].second[j]);
            if(from && to){
                gameMap.addAdjacency(from, to);
            }
        }
    }

    return gameMap;
}