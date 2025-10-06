//-----------------------------------------------------
//Mia Letendre 40274681
//COMP-345-D 2252 Assignment 1 PlayerDriver.cpp
//Dr. Hamed Jafarpour
//Submitted: TBD
//Due: October 7, 2025 
//I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
//Mia Letendre
//-----------------------------------------------------
#include "Cards.h"
#include "Map/Map.h"
#include "Orders.h"
#include "Player.h"
#include <vector>
#include <iostream>
using namespace std;

//need to run with g++ -mconsole PlayerDriver.cpp Players.cpp Map.cpp Orders.cpp Cards.cpp -o testPlayers.exe


int testPlayers() {
    // Test data
    Continent *cont1 = new Continent("Continent1", 5);
    Territory *t1 = new Territory("Territory1", cont1);
    Territory *t2 = new Territory("Territory2", cont1);
    Territory *t3 = new Territory("Territory3", cont1);
    Map* testMap = new Map();
    testMap->AddTerritory(t1->name, cont1);
    testMap->AddTerritory(t2->name, cont1);
    testMap->AddTerritory(t3->name, cont1);
    Player p1;
    p1.SetName("Player1");
    p1.AddTerritory(t1);
    p1.AddTerritory(t2);
    

    cout << "Player Name: " << p1.GetName() << endl;
    cout << "Territories owned by " << p1.GetName() << ": ";
    for (Territory* t : p1.GetPlayerTerritories()) {
        cout << t->name << " ";
    }

    cout << endl;
    vector<Territory*> defendList = p1.ToDefend();
    cout << "Territories to defend: ";
    for (Territory* t : defendList) {
        cout << t->name << " ";
    }

    cout << endl;
    vector<Territory*> attackList = p1.ToAttack(); // why doesnt this work
    cout << "Territories to attack: ";
    for (Territory* t : attackList) {
        cout << t->name << " ";
    }
    cout << endl;

    Deploy order1(&p1, 5, t1);
    p1.IssueOrder(&order1);
    Advance order2(&p1, 4, t1, t2);
    p1.IssueOrder(&order2);
    cout << "Orders issued by " << p1.GetName() << ": ";
    cout << p1.GetPlayerOrders() << endl;
    cout << endl;

	Deck* deck1 = new Deck();
    Hand* hand1 = new Hand(*deck1);
    hand1->AddCard();
    hand1->AddCard();
    p1.SetPlayerHand(hand1);
    cout << "Hand of Player1: ";
    cout<< p1.GetPlayerHand() << endl;
    cout << endl;
    return 0;
}


