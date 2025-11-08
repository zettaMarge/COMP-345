//-----------------------------------------------------
    //Sara Vargas Aceves 40349399
    //COMP-345-D 2252 Assignment 1 Map.cpp
    //Dr. Hamed Jafarpour
    //Submitted: TBD
    //Due: October 7, 2025 
    //I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
    //Sara Vargas Aceves
//----------------------------------------------------- 
#include "Map.h"
#include "Player.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

//  ----- Territory class -----

// Constructor
Territory::Territory(const std::string& _name, Continent* _continent){
    owner = nullptr;
    continent = _continent;
    adj = std::vector<Territory*>();
    name = _name;
    numUnits = 0;
}

// Copy constructor
Territory::Territory(const Territory& _territory){
    owner = _territory.owner;
    continent = _territory.continent;
    adj = _territory.adj;
    name = _territory.name;
    numUnits = _territory.numUnits;
}

// Assignment operator
Territory& Territory::operator=(const Territory& _territory){
    if(this == &_territory) return *this;
    owner = _territory.owner;
    name = _territory.name;
    continent = _territory.continent;
    numUnits = _territory.numUnits;
    adj = _territory.adj;
    return *this;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Territory& _territory){
    os << "Territory: " << _territory.name
    << " (Owner: " << (_territory.owner ? _territory.owner->GetName() : "No owner")
    << ", Continent: " << _territory.continent->GetName()
    << ", Units: " << _territory.numUnits
    << ", Adjacent territories: " << _territory.adj.size() << ")";
    return os;
}

// Checks if a territory is already in the adjacent territories list.
// Parameters:
// - a: pointer to a territory that is to be looked.
// Returns:
// - true if 'a' is already listed as adjacent to said territory, false otherwise
bool Territory::IsAdjacent(Territory* a) const{
    return std::find(adj.begin(), adj.end(), a) != adj.end();
}

// Sets the territory's owner
// Parameters:
// - _owner: Pointer to a Player.
void Territory::SetOwner(Player* _owner){
    if(owner == _owner) return; // return if no need to update
    Player* oldOwner = owner; // get pointer to old owner to check later
    owner = _owner;
    if(oldOwner != nullptr && oldOwner->IsTerritoryOwned(this)) oldOwner->SwitchTerritory(ths, _owner); // remove from old owner
}

// Gets the territory's owner
// Returns a pointer to a Player
Player* Territory::GetOwner(){
    return owner;
}

// Gets the Continent the territory belongs to
// Returns a pointer to a Continent
Continent* Territory::GetContinentByTerritory() const{
    return continent;
}

// Return the vector of adjacent territories
// Returns:
// - adj: vector of Territories that are adjacent to said Territory.
const std::vector<Territory*>& Territory::AdjacentTerritories() const{
    return adj;
}

// Sets the territory's numUnits
// Parameters:
// - _numUnits: Number of units
void Territory::SetUnits(int _numUnits){
    numUnits = _numUnits;
}

// Gets the territory's numUnits
// Returns an integer, number of units.
int Territory::GetUnits() const{
    return numUnits;
}

// Gets the Territory's name
// Returns a string of the name.
const std::string& Territory::GetName() const{
    return name;
}
//  ----- Continent class -----

// Constructor
Continent::Continent(const std::string& _name, int _points){
    name = _name;
    points = _points;
    territories = std::vector<Territory*>();
}

// Copy constructor
Continent::Continent(const Continent& _continent){
    name = _continent.name;
    points = _continent.points;
    territories = _continent.territories;
}

// Assignment operator
Continent& Continent::operator=(const Continent& _continent){
    if(this == &_continent) return *this;
    name = _continent.name;
    points = _continent.points;
    territories = _continent.territories;
    return *this;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Continent& _continent){
    os << "Continent: " << _continent.name 
    << " (Points: " << _continent.points 
    << ", Territories: " << _continent.territories.size() << ")";
    return os;
}

// Gets the Continent's name
// Returns a string of the name.
const std::string& Continent::GetName() const{
    return name;
}

// Gets the Continent's bonus points
// Returns an integer of the points earned by owning the entire Continent.
int Continent::GetPoints() const{
    return points;
}

// Gets the Continent's vector of pointers to Territories that belong to it.
// Returns a vector of pointers to Territories.
const std::vector<Territory*>& Continent::GetTerritories() const{
    return territories;
}

void Continent::AddTerritory(Territory* _territory){
    territories.push_back(_territory);
}

// ----- Map class -----

// Constructor
Map::Map(){}

// Destructor
Map::~Map(){
    for (auto c : continents) delete c;
    for (auto t : territories) delete t;
}

// Copy constructor
Map::Map(const Map& _map){
    for(int i = 0; i < _map.continents.size(); i++){
        AddContinent(_map.continents[i]->GetName(), _map.continents[i]->GetPoints());
    }

    for(int i = 0; i < _map.territories.size(); i++){
        Continent* newContinent = GetContinentByName(_map.territories[i]->continent->GetName());
        Territory* newTerritory = AddTerritory(_map.territories[i]->name, newContinent);
        newTerritory->numUnits = _map.territories[i]->numUnits;
    }

    for(int i = 0; i < _map.territories.size(); i++){
        Territory* newFrom = GetTerritoryByName(_map.territories[i]->name);
        for(int j = 0; j < _map.territories[i]->adj.size(); j++){
            Territory* newTo = GetTerritoryByName(_map.territories[i]->adj[j]->name);
            AddAdjacency(newFrom, newTo);
        }
    }
}

// Assignment operator
Map& Map::operator=(const Map& _map){
    if(this == &_map) return *this;

    for(auto c : continents) delete c;
    for(auto t : territories) delete t;
    continents.clear();
    territories.clear();
    continentByName.clear();
    territoryByName.clear();

    for(int i = 0; i < _map.continents.size(); i++){
        AddContinent(_map.continents[i]->GetName(), _map.continents[i]->GetPoints());
    }

    for(int i = 0; i < _map.territories.size(); i++){
        Continent* newContinent = GetContinentByName(_map.territories[i]->continent->GetName());
        Territory* newTerritory = AddTerritory(_map.territories[i]->name, newContinent);
        newTerritory->SetUnits(_map.territories[i]->numUnits);
    }

    for(int i = 0; i < _map.territories.size(); i++){
        Territory* newFrom = GetTerritoryByName(_map.territories[i]->name);
        for(int j = 0; j < _map.territories[i]->adj.size(); j++){
            Territory* newTo = GetTerritoryByName(_map.territories[i]->adj[j]->name);
            AddAdjacency(newFrom, newTo);
        }
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Map& _map){
    os << "Map:\n";
    os << "  Continents: " << _map.continents.size() << "\n";
    for(Continent* c : _map.continents){
        os << "    - " << *c << "\n";
    }
    os << "  Territories: " << _map.territories.size() << "\n";
    for(Territory* t : _map.territories){
        os << "    - " << *t << "\n";
    }
    return os;
}

// Creates a new continent, adds it to the continent vector and hashmap.
// Parameters:
// - name: the continent's name
// - points: points for controlling the continent
// Returns:
// - A pointer to the newly created Continent object
Continent* Map::AddContinent(const std::string& name, int points){
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
Territory* Map::AddTerritory(const std::string& name, Continent* continent){
    Territory* territory = new Territory(name, continent);
    territories.push_back(territory);
    territoryByName[name] = territory;
    continent->AddTerritory(territory);
    return territory;
}

// Checks if a territory 'b' is already in the adjacent territories list of territory 'a'.
// Parameters:
// - a: pointer to a territory with the adjacent list that is being searched
// - b: pointer to a territory to look for in the adjacent list
// Returns:
// - true if 'b' is already listed as adjacent to 'a', false otherwise
bool Map::IsAdjacent(Territory* a, Territory* b){
    return std::find(a->adj.begin(), a->adj.end(), b) != a->adj.end();
}

// Adds bidirectional adjacency between territories avoiding duplicates.
// Parameters:
// - from: Territory from where the adjacency is being added
// - to: Territory to be connected with the first Territory
// Returns:
// - void
void Map::AddAdjacency(Territory* from, Territory* to){
    if(!IsAdjacent(from, to)){
        from->adj.push_back(to);
    }
    if(!IsAdjacent(to, from)){
        to->adj.push_back(from);
    }
}

// Returns pointer to a Continent object by name
Continent* Map::GetContinentByName(const std::string& name){
    return continentByName[name];
}

// Returns pointer to a Territory object by name
Territory* Map::GetTerritoryByName(const std::string& name){
    return territoryByName[name];
}

// Performs a depth-first search in all the map, starting with any node
// in the map and keeping track of the visited nodes in a set.
// Parameters:
// - current: Pointer to a Territory
// - visited: set of Territory pointers
void Map::Dfs(Territory* current, std::unordered_set<Territory*>& visited){
    visited.insert(current);
    for(int i = 0; i < current->adj.size(); i++){
        Territory* neighbor = current->adj[i];
        if(visited.find(neighbor) == visited.end()){
            Dfs(neighbor, visited);
        }
    }
}

// Performs a depth-first search in a continent, starting with any node in it.
// Keeps track of the visited territories.
// Parameters:
// - current: Pointer to a Territory
// - visited: set of Territory pointers
// - continent: Pointer to the continent to check
void Map::DfsContinent(Territory* current, std::unordered_set<Territory*>& visited, Continent* continent){
    visited.insert(current);
    for(int i = 0; i < current->adj.size(); i++){
        Territory* neighbor = current->adj[i];
        if(neighbor->continent == continent && visited.find(neighbor) == visited.end()){
            DfsContinent(neighbor, visited, continent);
        }
    }
}

// Validates that:
// - Each territory belongs to one continent
// - The map is a connected graph, using the Dfs helper function
// - Each continent is a connected subgraph, using the Dfs continent version helper function
// Returns:
// - true if the map has the previous characteristics, false otherwise
bool Map::Validate() {
    // Each territory belongs to one continent
    for(int i = 0; i < territories.size(); i++){
        if(territories[i]->continent == nullptr){
            std::cerr << "The territory " << territories[i]->name << "does not belong to a continent." << std::endl;
            return false;
        }
    }

    // The map is a connected graph
    std::unordered_set<Territory*> visited;
    Dfs(territories[0], visited);
    
    if(visited.size() != territories.size()) {
        std::cerr << "The map is not a connected graph." << std::endl;
        return false;
    }

    // Each continent is a connected subgraph
    for(int i = 0; i < continents.size(); i++){
        const std::vector<Territory*>& territories = continents[i]->GetTerritories();
        if(territories.empty()){
            std::cerr << "Continent " << continents[i]->GetName() << " has no territories." << std::endl;
            return false;
        }
        std::unordered_set<Territory*> visited;
        DfsContinent(territories[0], visited, continents[i]);

        if(visited.size() != territories.size()){
            std::cerr << "Continent " << continents[i]->GetName() << " is not connected." << std::endl;
            return false;
        }
    }
    return true;
}

// ----- Map Loader class -----

// Constructor
MapLoader::MapLoader(){}

// Copy constructor
MapLoader::MapLoader(const MapLoader& _mapLoader){
    content = _mapLoader.content;
}

// Assignment operator
MapLoader& MapLoader::operator=(const MapLoader& _mapLoader){
    if(this == &_mapLoader) return *this;
    content = _mapLoader.content;
    return *this;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const MapLoader& _mapLoader){
    os << "MapLoader: " << _mapLoader.content.size() << " lines loaded";
    return os;
}

// Reads any .txt file and Validates if it is a valid map.
// Parameters:
// - filename: the name of the file
// Returns:
// - true if the .txt is non empty and follows a valid map format
bool MapLoader::LoadMapFile(const std::string& filename){
    if(!LoadFile(filename) || content.empty() || !ValidateMap()) return false;
    return true;
}

// Opens .txt file and adds all lines to a vector of strings, avoiding empty lines.
// Parameters:
// - filename: the name of the file
// Returns:
// - true if the .txt is non empty and follows a valid map format, false otherwise
bool MapLoader::LoadFile(const std::string& filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "Could not open file: " << filename;
        return false;
    }

    std::string line;
    while(std::getline(file, line)){
        if(!line.empty() && !std::all_of(line.begin(), line.end(), [](unsigned char c){ return std::isspace(c); })){
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
bool MapLoader::ValidateMap(){
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

// Creates a Map object using the Validated Map data.
// After parsing each section, it adds the adjacencies between each territory.
// Returns:
// - Map object
Map MapLoader::CreateMap(){
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
            gameMap.AddContinent(key, value);
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

            Continent* cont = gameMap.GetContinentByName(continent);
            Territory* terr = gameMap.AddTerritory(name, cont);

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
        Territory* from = gameMap.GetTerritoryByName(pending[i].first);
        for(int j = 0; j < pending[i].second.size(); j++){
            Territory* to = gameMap.GetTerritoryByName(pending[i].second[j]);
            if(from && to){
                gameMap.AddAdjacency(from, to);
            }
        }
    }

    return gameMap;
}