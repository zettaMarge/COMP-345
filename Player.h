
//-----------------------------------------------------
//Mia Letendre 40274681
//COMP-345-D 2252 Assignment 1 Players.h
//Dr. Hamed Jafarpour
//Submitted: TBD
//Due: October 7, 2025 
//I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
//Mia Letendre
//-----------------------------------------------------
#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

using namespace std;
class Player{
public:
    vector<Territory*> ToAttack() const; //returns list of territories to attack
    vector<Territory*> ToDefend() const ; //returns list of territories to defend
    void AddTerritory(Territory* t); //adds territory to players list of territories
    void RemoveTerritory(Territory* t); //removes territory from players list of territories
   
    void IssueOrder(Order* x);//creates order and puts in players list of orders
    bool Equals(Player* p); //checks if two players are the same

    Player(); // Default constructor
    Player(std::string& name, vector<Territory*> playerTerritories, Hand &playerHand, OrdersList &playerOrders); // Parameterized constructor
    Player(const Player& p); // Copy constructor
    Player& operator=(const Player& p); // Assignment operator
    friend ostream& operator<<(ostream& os, const Player& p); // Stream insertion operator
    ~Player(); // Destructor

    //setters and getters
    string GetName();
    vector<Territory*> GetPlayerTerritories();
    Hand* GetPlayerHand();
    OrdersList* GetPlayerOrders() const;
    void SetName(string name);
    void setPlayerTerritories(vector<Territory*> territories);
    void SetPlayerHand(Hand* hand);
    void SetPlayerOrders(OrdersList* orders);
    bool IsTerritoryOwned(Territory* t);

private:
    string name; //name of player, used to identify who owns territory
    vector<Territory*> playerTerritories; //list of territories owned by player
    Hand* playerHand; //list of cards owned by player or their hand
    OrdersList* playersOrders; //list of orders owned by player
    
};
