#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct Continent; // Forward declaration for Continent struct

struct Territory{
    // Player* owner;
    std::string name;
    Continent* continent;
    std::vector<Territory*> adj;
    int numArmies;

    Territory(const std::string& _name, Continent* _continent); // Add player here later
};

struct Continent{
    std::string name;
    int points;
    std::vector<Territory*> territories;

    Continent(const std::string& _name, int _points);
};

class Map {
    public:
        ~Map();

        Continent* addContinent(const std::string& name, int points);
        Territory* addTerritory(const std::string& name, Continent* continent);
        void addAdjacency(Territory* from, Territory* to);

        Continent* getContinentByName(const std::string& name);
        Territory* getTerritoryByName(const std::string& name);

        bool validate();

    private:
        bool isAdjacent(Territory* a, Territory* b);
        std::unordered_map<std::string, Continent*> continentByName;
        std::unordered_map<std::string, Territory*> territoryByName;

        std::vector<Continent*> continents;
        std::vector<Territory*> territories;

        void dfs(Territory* current, std::unordered_set<Territory*>& visited);
        void dfsContinent(Territory* current, std::unordered_set<Territory*>& visited, Continent* continent);
};

class MapLoader{
    public:
        bool loadMapFile(const std::string& filename);
        Map createMap();
    private:
        std::vector<std::string> content;
        bool loadFile(const std::string& filename);
        bool validateMap();
};