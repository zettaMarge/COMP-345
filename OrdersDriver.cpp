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
    Player* p1 = new Player();
    p1->SetName("Player1");
    p1->AddTerritory(canada);
    p1->AddTerritory(usa);

    Player* p2 = new Player();
    p1->SetName("Player2");
    p1->AddTerritory(mexico);


    std::cout << "----- Creating Orders -----" << std::endl;
    Advance* advanceValid = new Advance();
    advanceValid->SetOwningPlayer(p1);
    advanceValid->SetNbUnits(3);
    advanceValid->SetSrc(canada);
    advanceValid->SetTarget(usa);
    std::cout << "----- advanceValid done -----" << std::endl;

    Advance* advanceInvalid = new Advance(*advanceValid);
    advanceInvalid->SetTarget(mexico);
    std::cout << "----- advanceInvalid done -----" << std::endl;

    Airlift* airlift = new Airlift(p1, 3, canada, mexico);
    std::cout << "----- airlift done -----" << std::endl;

    Blockade* blockade;
    blockade->SetOwningPlayer(p1);
    blockade->SetTarget(canada);
    std::cout << "----- blockade done -----" << std::endl;

    Bomb* bomb;
    bomb->SetOwningPlayer(p1);
    bomb->SetTarget(mexico);
    std::cout << "----- bomb done -----" << std::endl;

    Deploy* deploy;
    deploy->SetOwningPlayer(p1);
    deploy->SetNbUnits(3);
    deploy->SetTarget(canada);
    std::cout << "----- deploy done -----" << std::endl;

    Negotiate* negotiate;
    negotiate->SetOwningPlayer(p1);
    negotiate->SetOwningPlayer(p2);
    std::cout << "----- negotiate done -----" << std::endl;


    std::cout << "----- Adding orders to p1's list -----" << std::endl;
    p1->IssueOrder(advanceValid);
    p1->IssueOrder(advanceInvalid);
    p1->IssueOrder(airlift);
    p1->IssueOrder(blockade);
    p1->IssueOrder(bomb);
    p1->IssueOrder(deploy);
    p1->IssueOrder(negotiate);
    std::cout << p1->GetPlayerOrders() << std::endl;

    
    std::cout << "----- Executing orders -----" << std::endl;
    // Fix: Store the result of GetListItems() in a local variable of type vector<Order*>
    std::vector<Order*> orders = p1->GetPlayerOrders()->GetListItems();
    for (const auto& order : orders) {
        order->Execute();
    }

    
    std::cout << "----- Moving orders in list -----" << std::endl;
    p1->GetPlayerOrders()->Move(1, orders.size() - 1);
    std::cout << p1->GetPlayerOrders() << std::endl;

    std::cout << "----- Removing orders from list -----" << std::endl;
    p1->GetPlayerOrders()->Remove(p1->GetPlayerOrders()->GetListItems().size() - 1);
    std::cout << p1->GetPlayerOrders() << std::endl;

    std::cout << "-------------------------" << std::endl;
    std::cout << "- OrdersDriver complete -" << std::endl;
    std::cout << "-------------------------" << std::endl;
}