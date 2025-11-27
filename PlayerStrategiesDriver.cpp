
//-----------------------------------------------------
//Mia Letendre 40274681
//COMP-345-D 2252 Assignment 3 PlayerStategies.cpp
//Dr. Hamed Jafarpour
//Submitted: November 26, 2025 
//Due: November 26, 2025 
//I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
//Mia Letendre
//-----------------------------------------------------
#include "PlayerStrategies.h"
#include "GameEngine.h"
#include "Player.h"
#include "Orders.h"
#include "Map.h"
#include "Cards.h"
#include <algorithm>
#include <string>
using namespace std;



void testPlayerStrategies() {
    GameEngine* gameEngine = new GameEngine();
    //Buncha territories for testing
    Map* testMap = new Map();
    Continent* cont = testMap->AddContinent("Cont", 5);
    Territory* t1 = testMap->AddTerritory("Arlington", cont);
    Territory* t2 = testMap->AddTerritory("Fortnite", cont);
    Territory* t3 = testMap->AddTerritory("DavidLynchsMind", cont);
    Territory* t4 = testMap->AddTerritory("TwinPeaksTheReturn", cont);
    Territory* t5 = testMap->AddTerritory("NightCity", cont);
    Territory* t6 = testMap->AddTerritory("Mexico", cont);
    Territory* t7 = testMap->AddTerritory("BaldursGate3", cont);
    Territory* t8 = testMap->AddTerritory("Humanland", cont);

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
    gameEngine->gameMap = testMap;

    for (auto t : testMap->territories) {
        t->SetUnits(5);
    }
   

    //Creating players, stategies, and giving them territories
    //P1 is semi center of map which is good for aggressive testing
    Deck* deck = new Deck();
    Hand* h1 = new Hand(*deck);
    Hand* h2 = new Hand(*deck);
    Hand* h3 = new Hand(*deck);
    Hand* h4 = new Hand(*deck);
    Hand* h5 = new Hand(*deck);


    Player *p1 = new Player();
    gameEngine->players.push_back(p1);
    p1->SetName("Alice");
    p1->AddTerritory(t1);
    p1->SetStrategy(new BenevolentPlayerStrategy());
    p1->SetPlayerHand(h1);

    Player* p2 = new Player();
    gameEngine->players.push_back(p2);
    p2->SetName("Lidell");
    p2->SetStrategy(new AggressivePlayerStrategy());
    p2->AddTerritory(t2);
    p2->AddTerritory(t4);
    p2->SetPlayerHand(h2);

    Player* p3 = new Player();
    gameEngine->players.push_back(p3);
    p3->SetName("Mac");
    p3->SetStrategy(new NeutralPlayerStrategy());
    p3->AddTerritory(t3);
    p3->SetPlayerHand(h3);

    Player* p4 = new Player();
    gameEngine->players.push_back(p4);
    p4->SetName("Dennis");
    //might change to run the code after runniong other tests
    p4->AddTerritory(t5);
    p4->AddTerritory(t6);
    p4->AddTerritory(t7);
    p4->SetStrategy(new CheaterPlayerStrategy());
    p4->SetPlayerHand(h4);

    Player* p5 = new Player();
    gameEngine->players.push_back(p5);
    p5->SetName("Human");
    p5->AddTerritory(t8);
    p5->SetStrategy(new HumanPlayerStrategy());
    p5->SetPlayerHand(h5);
    
    for(auto player : gameEngine->players){
        player->AddReinforcements(10); //giving all players 10 reinforcements for testing
    }

    p5->SetReinforcements(0);
    gameEngine->InitFinishedPlayers();
    

    //showing different players stateges work
    for (auto player : gameEngine->players) {
        std::cout << "\n\nPlayer: " << player->GetName() << " Strategy: " << player->GetStrategyName() << std::endl;
        std::cout << "Territories to Attack: ";
        std::vector<Territory*> attackList = player->ToAttack();
        for (Territory* t : attackList) {
            std::cout << t->GetName() << " ";
        }
        std::cout << "\nTerritories to Defend: ";
        std::vector<Territory*> defendList = player->ToDefend();
        for (Territory* t : defendList) {
            std::cout << t->GetName() << " ";
        }
        std::cout << "\nIssuing Orders...\n";
        player->IssueOrder();
    }

    
    //Executing human order to show the Neutral strategy change to Aggressive
    //To be sure to test this, Human needs to input an Advance order towards "DavidLynchsMind"
    OrdersList* ordersList = p5->GetPlayerOrders();
    if (!ordersList->IsEmpty()) {
        Order* order = ordersList->GetNextOrder();
        if (order) {
            order->Execute();
        }
    }

    cout << "Initially Neutral player's current strategy: " + p3->GetStrategyName() << endl;

    cout << "Player Strategies test completed." << endl;
    //memory cleanup omitted because its just a temporary test
}

