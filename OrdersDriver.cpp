#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include <iostream>

void testOrdersLists() {
    std::cout << "------------------------" << std::endl;
    std::cout << "- OrdersDriver started -" << std::endl;
    std::cout << "------------------------" << std::endl;


    std::cout << "----- Creating territories -----" << std::endl;
    Map testMap;
    Continent* northAmerica = testMap.AddContinent("North America", 5);

    Territory* canada = testMap.AddTerritory("Canada", northAmerica);
    canada->SetUnits(5);

    Territory* usa = testMap.AddTerritory("USA", northAmerica);
    usa->SetUnits(5);

    Territory* mexico = testMap.AddTerritory("Mexico", northAmerica);
    mexico->SetUnits(5);

    testMap.AddAdjacency(canada, usa);
    testMap.AddAdjacency(mexico, usa);


    std::cout << "----- Creating players -----" << std::endl;
    Player* p1;
    p1->SetName("Player1");
    p1->AddTerritory(canada);
    p1->AddTerritory(usa);

    Player* p2;
    p1->SetName("Player2");
    p1->AddTerritory(mexico);


    std::cout << "----- Creating Orders -----" << std::endl;
    Advance* advanceValid;
    advanceValid->SetOwningPlayer(p1);
    advanceValid->SetNbUnits(3);
    advanceValid->SetSrc(canada);
    advanceValid->SetTarget(usa);

    Advance* advanceInvalid = new Advance(advanceValid);
    advanceInvalid->SetTarget(mexico);

    Airlift* airlift = new Airlift(p1, 3, canada, mexico);

    Blockade* blockade;
    blockade->SetOwningPlayer(p1);
    blockade->SetTarget(canada);

    Bomb* bomb;
    bomb->SetOwningPlayer(p1);
    bomb->SetTarget(mexico);

    Deploy* deploy;
    deploy->SetOwningPlayer(p1);
    deploy->SetNbUnits(3);
    deploy->SetTarget(canada);

    Negotiate* negotiate;
    negotiate->SetOwningPlayer(p1);
    negotiate->SetOwningPlayer(p2);


    std::cout << "----- Adding orders to p1's list -----" << std::endl;
    p1->GetPlayerOrders()->Add(advanceValid);
    p1->GetPlayerOrders()->Add(advanceInvalid);
    p1->GetPlayerOrders()->Add(airlift);
    p1->GetPlayerOrders()->Add(blockade);
    p1->GetPlayerOrders()->Add(bomb);
    p1->GetPlayerOrders()->Add(deploy);
    p1->GetPlayerOrders()->Add(negotiate);
    std::cout << p1->GetPlayerOrders() << std::endl;

    
    std::cout << "----- Executing orders -----" << std::endl;
    for (const auto& order : p1->GetPlayerOrders()->GetListItems())
    {
        order.Execute();
    }

    
    std::cout << "----- Moving orders in list -----" << std::endl;
    p1->GetPlayerOrders()->Move(1, p1->GetPlayerOrders()->GetListItems().size - 1)
    std::cout << p1->GetPlayerOrders() << std::endl;

    std::cout << "----- Removing orders from list -----" << std::endl;
    p1->GetPlayerOrders()->Remove(p1->GetPlayerOrders()->GetListItems().size - 1)
    std::cout << p1->GetPlayerOrders() << std::endl;

    std::cout << "-------------------------" << std::endl;
    std::cout << "- OrdersDriver complete -" << std::endl;
    std::cout << "-------------------------" << std::endl;
}