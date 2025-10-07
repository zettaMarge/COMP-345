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
#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include <vector>
#include <iostream>
using namespace std;



int testPlayers() {
    // Test data
    Continent *cont1 = new Continent("Continent1", 5);
    Territory *t1 = new Territory("Territory1", cont1);
    Territory *t2 = new Territory("Territory2", cont1);
    Territory *t3 = new Territory("Territory3", cont1);
    Map* testMap = new Map();
    testMap->AddTerritory(t1->GetName(), cont1);
    testMap->AddTerritory(t2->GetName(), cont1);
    testMap->AddTerritory(t3->GetName(), cont1);
    Player p1;
    testMap->AddAdjacency(t1, t2);
    testMap->AddAdjacency(t2, t3);
    testMap->AddAdjacency(t3, t1);
    p1.SetName("Player1");
    p1.AddTerritory(t1);
    p1.AddTerritory(t2);
    

    cout << "Player Name: " << p1.GetName() << endl;
    cout << "Territories owned by " << p1.GetName() << ": ";
    for (Territory* t : p1.GetPlayerTerritories()) {
        cout << t->GetName() << " ";
    }

    cout << endl;
    vector<Territory*> defendList = p1.ToDefend();
    cout << "Territories to defend: ";
    for (Territory* t : defendList) {
        cout << t->GetName() << " ";
    }

    cout << endl;
    vector<Territory*> attackList = p1.ToAttack(); // why doesnt this work
    cout << "Territories to attack: ";
    if (attackList.size() == 0) {
        cout << "None";
    }
    for (Territory* t : attackList) {
        cout << t->GetName() << " ";
    }
    cout << endl;

    Deploy order1(&p1, 5, t1);
    p1.IssueOrder(&order1);
    Advance order2(&p1, 4, t1, t2);
    p1.IssueOrder(&order2);
    cout << "Orders issued by " << p1.GetName() ;// << ":\n" *p1.GetPlayerOrders() << endl;
    for (Order* o : p1.GetPlayerOrders()->GetListItems()) {
        cout << *o;
    }
    
	Deck* deck1 = new Deck();
    Hand* hand1 = new Hand(*deck1);
    hand1->AddCard();
    hand1->AddCard();
    p1.SetPlayerHand(hand1);
    
    cout << "" << p1.GetName() << " "; //<< ":\n" << *p1.GetPlayerHand() << endl;
    p1.GetPlayerHand()->Print();
    return 0;
}


