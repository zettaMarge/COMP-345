
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
    GameEngine* gameEngine =new GameEngine();
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
    //Setting adjacency
    testMap->AddContinent(cont->GetName(), cont->GetPoints());
    testMap->AddTerritory(t1->GetName(), cont);
    testMap->AddTerritory(t2->GetName(), cont);
    testMap->AddTerritory(t3->GetName(), cont);
    testMap->AddTerritory(t4->GetName(), cont);
    testMap->AddTerritory(t5->GetName(), cont);
    testMap->AddTerritory(t6->GetName(), cont);
    testMap->AddTerritory(t7->GetName(), cont);
    testMap->AddAdjacency(t1, t5);
    testMap->AddAdjacency(t5, t4);
    testMap->AddAdjacency(t5, t6);
    testMap->AddAdjacency(t5, t7);
    testMap->AddAdjacency(t4, t2);
    testMap->AddAdjacency(t2, t3);
    testMap->AddAdjacency(t1, t6);
    testMap->AddAdjacency(t3, t1);
    gameEngine->gameMap = testMap;
   

    //Creating players, stategies, and giving them territories
    //P1 is semi center of map which is good for aggressive testing
    Player *p1 = new Player();
    gameEngine->players.push_back(p1);
    p1->SetName("Alice");
    p1->AddTerritory(t1);
    p1->SetStrategy(new BenevolentPlayerStrategy());

    Player* p2 = new Player();
    gameEngine->players.push_back(p2);
    p2->SetName("Lidell");
    p2->SetStrategy(new AggressivePlayerStrategy());
    p2->AddTerritory(t2);
    p2->AddTerritory(t4);
    Player* p3 = new Player();
    gameEngine->players.push_back(p3);
    p3->SetName("Mac");
    p3->SetStrategy(new NeutralPlayerStrategy());
    p3->AddTerritory(t3);
    Player* p4 = new Player();
    gameEngine->players.push_back(p4);
    p4->SetName("Dennis");
    //might change to run the code after runniong other tests
    p4->AddTerritory(t5);
    p4->AddTerritory(t6);
    p4->AddTerritory(t7);
    p4->SetStrategy(new CheaterPlayerStrategy());
    
    for(auto player : gameEngine->players){
        player->AddReinforcements(10); //giving all players 10 reinforcements for testing
    }
    

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

    cout << "Player Strategies test completed." << endl;
    //mempory cleanup omitted because its just a temporary test

}

