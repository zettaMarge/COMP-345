//-----------------------------------------------------
    //Sara Vargas Aceves 40349399
    //COMP-345-D 2252 Assignment 2 LoggingObserverDriver.cpp
    //Dr. Hamed Jafarpour
    //Submitted: TBD
    //Due: November 11, 2025 
    //I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
    //Sara Vargas Aceves
//-----------------------------------------------------
#include "LoggingObserver.h"
#include "GameEngine.h"
#include "CommandProcessing.h"
#include "Orders.h"
#include "Player.h"
#include "Map.h"
#include <iostream>
#include <fstream>

void testLoggingObserver(){
    std::cout << "Logging Observer test" << std::endl;
    LogObserver* logObserver = new LogObserver();

    // Inheritance
    GameEngine* engine = new GameEngine();
    CommandProcessor* cmdProcessor = new CommandProcessor(engine);
    OrdersList* orderList = new OrdersList();

    // Command and CommandProcessor logs
    cmdProcessor->Attach(logObserver);
    cmdProcessor->SaveCommand("loadmap");
    cmdProcessor->SaveEffect("Map loaded successfully");
    cmdProcessor->SaveCommand("validatemap");
    cmdProcessor->SaveEffect("Map validation complete");
    cmdProcessor->SaveCommand("addplayer");
    cmdProcessor->SaveEffect("Player added");

    // OrdersList logs
    orderList->Attach(logObserver);

    Map* testMap = new Map();
    Continent* continent = testMap->AddContinent("TestContinent", 5);
    Territory* territory1 = testMap->AddTerritory("Territory1", continent);
    Territory* territory2 = testMap->AddTerritory("Territory2", continent);
    Territory* territory3 = testMap->AddTerritory("Territory3", continent);
    testMap->AddAdjacency(territory1, territory2);
    testMap->AddAdjacency(territory2, territory3);
    territory1->SetUnits(10);
    territory2->SetUnits(5);
    territory3->SetUnits(5);

    Player* player1 = new Player();
    player1->SetName("John");
    player1->AddTerritory(territory1);
    player1->AddTerritory(territory2);
    Player* player2 = new Player();
    player2->SetName("Jane");
    player2->AddTerritory(territory3);

    Deploy* deployOrder = new Deploy(player1, 5, territory1);
    orderList->Add(deployOrder);
    Advance* advanceOrder = new Advance(player1, 3, territory1, territory2);
    orderList->Add(advanceOrder);
    Bomb* bombOrder = new Bomb(player1, territory2);
    orderList->Add(bombOrder);
    Negotiate* negotiateOrder = new Negotiate(player1, player2);
    orderList->Add(negotiateOrder);

    // Order logs
    deployOrder->Attach(logObserver);
    advanceOrder->Attach(logObserver);
    bombOrder->Attach(logObserver);
    negotiateOrder->Attach(logObserver);

    deployOrder->Execute();
    advanceOrder->Execute();
    bombOrder->Execute();
    negotiateOrder->Execute();

    // GameEngine logs
    engine->Attach(logObserver);
    engine->changeState(engine->mapLoadedState.get());
    engine->changeState(engine->mapValidatedState.get());
    engine->changeState(engine->playersAddedState.get());
    engine->changeState(engine->assignReinforcementState.get());
    engine->changeState(engine->issueOrdersState.get());
    engine->changeState(engine->executeOrdersState.get());

    delete logObserver;
    delete cmdProcessor;
    delete orderList;
    delete player1;
    delete player2;
    delete testMap;
    delete engine;

    std::cout << "Logging Observer test complete" << std::endl;
}
