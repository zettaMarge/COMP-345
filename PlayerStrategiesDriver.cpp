
//-----------------------------------------------------
//Mia Letendre 40274681
//COMP-345-D 2252 Assignment 3 PlayerStategies.cpp
//Dr. Hamed Jafarpour
//Submitted: TBD
//Due: November 26, 2025 
//I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
//Mia Letendre
//-----------------------------------------------------
#include "PlayerStrategies.h"
#include "GameEngine.h"
#include "Player.h"
#include "Orders.h"
#include "Map.h"
#include <algorithm>
#include <string>
using namespace std;



void testPlayerStrategies() {
    //Buncha territories for testing
    Map* testMap = new Map();
    Continent* cont = new Continent("Cont", 5);
    Territory* t1 = new Territory("Arlington", cont);
    Territory* t2 = new Territory("Fortnite", cont);
    Territory* t3 = new Territory("David Lynch's Mind", cont);
    Territory* t4 = new Territory("Twin Peaks: The Return", cont);
    Territory* t5 = new Territory("Night City", cont);
    Territory* t6 = new Territory("Mexico", cont);
    Territory* t7 = new Territory("Baldurs Gate 3", cont);
    Territory* t8 = new Territory("Humanland", cont);
    t8->SetUnits(5);

    //Setting adjacency
    testMap->AddAdjacency(t1, t5);
    testMap->AddAdjacency(t5, t4);
    testMap->AddAdjacency(t5, t6);
    testMap->AddAdjacency(t5, t7);
    testMap->AddAdjacency(t4, t2);
    testMap->AddAdjacency(t2, t3);
    testMap->AddAdjacency(t1, t6);
    testMap->AddAdjacency(t3, t1);
    testMap->AddAdjacency(t3, t8);


    //Creating players, stategies, and giving them territories
    //P1 is semi center of map which is good for aggressive testing
    Player *p1 = new Player();
    p1->SetName("Alice");
    p1->AddTerritory(t1);
    p1->SetStrategy(new AggressivePlayerStrategy());

    Player* p2 = new Player();
    p2->SetName("Lidell");
    p2->SetStrategy(new BenevolentPlayerStrategy());
    p2->AddTerritory(t2);
    p2->AddTerritory(t4);

    Player* p3 = new Player();
    p3->SetName("Mac");
    p3->SetStrategy(new NeutralPlayerStrategy());
    p3->AddTerritory(t3);

    Player* p4 = new Player();
    p4->SetName("Dennis");
    //might change to run the code after runniong other tests
    p4->AddTerritory(t5);
    p4->AddTerritory(t6);
    p4->AddTerritory(t7);
    p4->SetStrategy(new CheaterPlayerStrategy());

    Player* p5 = new Player();
    p5->SetName("Human");
    p5->AddTerritory(t8);
    p5->SetStrategy(new HumanPlayerStrategy());
    

    //showing different players stateges work
    cout << "Aggressive Strategy Player" << endl;
    cout << "Attacking List for " + p1->GetName() + ": ";
    for (auto t : p1->ToAttack()) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Defending list for " + p1->GetName() + ": ";
    for (auto t : p1->ToDefend()){
         cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Issuing Orders: ";
    p1->IssueOrder();


    cout << "Benevolent Strategy" << endl;
    cout << "Attacking list for " + p2->GetName() + ": ";
    for (auto t : p2->ToAttack()) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Defending List for " + p2->GetName() + ": ";
    for (auto t : p2->ToDefend()) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Issuing Orders: ";
    p2->IssueOrder();


     cout << "Neutral Strategy Player" << endl;
    cout << "Attacking List for " + p3->GetName() + ": ";
    for (auto t : p3->ToAttack()) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Defending List for " + p3->GetName() + ": ";
    for (auto t : p3->ToDefend()){
         cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Issuing Orders: ";
    p3->IssueOrder();


    cout << "Cheater Strategy:" << endl;
    cout << "Attacking List for " + p4->GetName() + ": ";
    for (auto t : p4->ToAttack()) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Defending List for " + p4->GetName() + ": ";
    for (auto t : p4->ToDefend()) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Issuing Orders for " + p4->GetName() + ": ";
    p4->IssueOrder();


    cout << "Human Strategy:" << endl;
    cout << "Attacking List for " + p5->GetName() + ": ";
    for (auto t : p5->ToAttack()) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Defending List for " + p5->GetName() + ": ";
    for (auto t : p5->ToDefend()) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    cout << "Issuing Orders for " + p5->GetName() + ": ";
    p5->IssueOrder(); //To be sure to test the strategy change, need to input an Advance order towards "David Lynch's Mind"
    //Executing human order to show the Neutral strategy change to Aggressive (if the order was issued correctly)
    OrdersList* ordersList = p5->GetPlayerOrders();
    if (!ordersList->IsEmpty()) {
        Order* order = ordersList->GetNextOrder();
        if (order) {
            order->Execute();
        }
    }

    cout << "Initially Neutral player's current strategy: " + p3->GetStrategyName() << endl;

    cout << "Player Strategies test completed." << endl;
    //mempory cleanup omitted because its just a temporary test
}

