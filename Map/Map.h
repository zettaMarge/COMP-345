#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

struct Continent; // Forward declaration for Continent struct

struct Territory{
    Player* owner;
    std::string name;
    Continent* continent;
    std::vector<Territory*> adj;
    int numUnits;

    // Constructor
    Territory(const std::string& _name, Continent* _continent);

    // Copy constructor
    Territory(const Territory& _territory);

    // Assignment operator
    Territory& operator=(const Territory& _territory);

    // Stream insertion operator
    friend std::ostream& operator << (std::ostream& os, const Territory& _territory);

    const std::vector<Territory*>& AdjacentTerritories() const;
    bool IsAdjacent(Territory* a) const;
    void SetUnits(int);
};

struct Continent{
    std::string name;
    int points;
    std::vector<Territory*> territories;

    // Constructor
    Continent(const std::string& _name, int _points);

    // Copy constructor
    Continent(const Continent& _continent);

    // Assignment operator
    Continent& operator=(const Continent& _continent);

    // Stream insertion operator
    friend std::ostream& operator << (std::ostream& os, const Continent& _continent);
};

class Map {
    public:
        //Destructor
        ~Map();

        // Constructor
        Map();

        // Copy constructor
        Map(const Map& _map);

        // Assignment operator
        Map& operator=(const Map& _map);

        // Stream insertion operator
        friend std::ostream& operator << (std::ostream& os, const Map& _map);

        Continent* AddContinent(const std::string& name, int points);
        Territory* AddTerritory(const std::string& name, Continent* continent);
        void AddAdjacency(Territory* from, Territory* to);

        Continent* GetContinentByName(const std::string& name);
        Territory* GetTerritoryByName(const std::string& name);

        bool IsAdjacent(Territory* a, Territory* b);
        bool Validate();

    private:
        std::unordered_map<std::string, Continent*> continentByName;
        std::unordered_map<std::string, Territory*> territoryByName;

        std::vector<Continent*> continents;
        std::vector<Territory*> territories;

        void Dfs(Territory* current, std::unordered_set<Territory*>& visited);
        void DfsContinent(Territory* current, std::unordered_set<Territory*>& visited, Continent* continent);
};

class MapLoader{
    public:
        MapLoader();

        // Copy constructor
        MapLoader(const MapLoader& _mapLoader);

        // Assignment operator
        MapLoader& operator=(const MapLoader& _mapLoader);

        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& os, const MapLoader& _mapLoader);

        bool LoadMapFile(const std::string& filename);
        Map CreateMap();
    private:
        std::vector<std::string> content;
        bool LoadFile(const std::string& filename);
        bool ValidateMap();
};