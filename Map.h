//-----------------------------------------------------
    //Sara Vargas Aceves 40349399
    //COMP-345-D 2252 Assignment 1 Map.h
    //Dr. Hamed Jafarpour
    //Submitted: TBD
    //Due: October 7, 2025 
    //I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
    //Sara Vargas Aceves
//----------------------------------------------------- 
#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

struct Continent; // Forward declaration for Continent struct
class Player; // Forward declaration for Player class
class GameEngine; // Forward declaration for GameEngine class

//  ----- Territory class -----
// Represents a single Territory in a Map.
// A Territory has a pointer to the Player that owns it, pointer to the Continent it belongs to,
// a vector of pointers to adjacent Territories, a name, and a number of army units.
// Contains methods to return the adjacent terriories, check if it's adjacent to another territory,
// and set the amount of army units.
class Territory{
    friend class Map;
    public:
        // Constructor
        Territory(const std::string& _name, Continent* _continent);

        // Copy constructor
        Territory(const Territory& _territory);

        // Assignment operator
        Territory& operator=(const Territory& _territory);

        // Stream insertion operator
        friend std::ostream& operator << (std::ostream& os, const Territory& _territory);

        bool IsAdjacent(Territory* a) const;

        // Setters and Getters
        void SetOwner(Player* _owner);
        Player* GetOwner();
        Continent* GetContinentByTerritory() const;
        const std::vector<Territory*>& AdjacentTerritories() const;
        const std::string& GetName() const;
        void SetUnits(int);
        int GetUnits() const;
    private:
        Player* owner;
        Continent* continent;
        std::vector<Territory*> adj;
        std::string name;
        int numUnits;
};

//  ----- Continent class -----
// Represents a single Continent in a Map.
// A Continent has a name, points obtained if the whole Continent is owned,
// and a vector of pointers to the Territories that belong in it.
class Continent{
    public:
        // Constructor
        Continent(const std::string& _name, int _points);

        // Copy constructor
        Continent(const Continent& _continent);

        // Assignment operator
        Continent& operator=(const Continent& _continent);

        // Stream insertion operator
        friend std::ostream& operator << (std::ostream& os, const Continent& _continent);

        // Getters
        const std::string& GetName() const;
        int GetPoints() const;
        const std::vector<Territory*>& GetTerritories() const;

        void AddTerritory(Territory* _territory);
    private:
        std::string name;
        int points;
        std::vector<Territory*> territories;
};

//  ----- Map class -----
// Represents a Warzone Map.
// A Map has 2 hashmaps to keep track by name of Continents and Territories respectively
// and point to their corresponding object.
// It also has a vector of Continent pointers and a vector of Territory pointers.
// It has methods to add Continents, Territories, and Adjacencies, and validate if the Map is connected,
// between territories and between countries, with the aid of a depth-first search method.
class Map {
    public:
        // Destructor
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
        std::vector<Continent*> GetContinents() const {
			return continents;
        };
        void AddAdjacency(Territory* from, Territory* to);

        Continent* GetContinentByName(const std::string& name);
        Territory* GetTerritoryByName(const std::string& name);

        bool IsAdjacent(Territory* a, Territory* b);
        bool Validate();
        friend class GameEngine;

    private:
        std::unordered_map<std::string, Continent*> continentByName;
        std::unordered_map<std::string, Territory*> territoryByName;

        std::vector<Continent*> continents;
        std::vector<Territory*> territories;

        void Dfs(Territory* current, std::unordered_set<Territory*>& visited);
        void DfsContinent(Territory* current, std::unordered_set<Territory*>& visited, Continent* continent);
};

//  ----- Maploader class -----
// Loads a Warzone map from the conquest map website and creates a Map if the
// format is valid.
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
        Map* CreateMap();
    private:
        std::vector<std::string> content;
        bool LoadFile(const std::string& filename);
        bool ValidateMap();
};