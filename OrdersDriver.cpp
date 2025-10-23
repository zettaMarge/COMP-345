#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include <iostream>

void TestOrdersLists() {
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
    p2->SetName("Player2");
    p2->AddTerritory(mexico);


    std::cout << "----- Creating Orders -----" << std::endl;
    Advance* advanceValid = new Advance();
    advanceValid->SetOwningPlayer(p1);
    advanceValid->SetNbUnits(3);
    advanceValid->SetSrc(usa);
    advanceValid->SetTarget(mexico);

    Advance* advanceInvalid = new Advance(*advanceValid);
    advanceInvalid->SetTarget(mexico);

    Airlift* airlift = new Airlift(p1, 3, canada, mexico);
    Blockade* blockade = new Blockade(p1, canada);
    Bomb* bomb = new Bomb(p1, mexico);
    Deploy* deploy = new Deploy(p1, 3, canada);
    Negotiate* negotiate = new Negotiate(p1, p2);


    std::cout << "----- Adding orders to p1's list -----" << std::endl;
    p1->IssueOrder(advanceValid);
    p1->IssueOrder(advanceInvalid);
    p1->IssueOrder(airlift);
    p1->IssueOrder(blockade);
    p1->IssueOrder(bomb);
    p1->IssueOrder(deploy);
    p1->IssueOrder(negotiate);

    std::cout << *p1->GetPlayerOrders() << std::endl;

    
    std::cout << "----- Validating & Executing orders -----" << std::endl;
    std::vector<Order*> orders = p1->GetPlayerOrders()->GetListItems();
    for (const auto& order : orders) {
        order->Execute();
        std::cout << "----------" << std::endl;
    }

    
    std::cout << "\n\n----- Moving orders in list -----" << std::endl;
    p1->GetPlayerOrders()->Move(1, orders.size() - 1);
    std::cout << *p1->GetPlayerOrders() << std::endl;

    std::cout << "----- Removing orders from list -----" << std::endl;
    p1->GetPlayerOrders()->Remove(p1->GetPlayerOrders()->GetListItems().size() - 1);
    std::cout << *p1->GetPlayerOrders() << std::endl;

    std::cout << "-------------------------" << std::endl;
    std::cout << "- OrdersDriver complete -" << std::endl;
    std::cout << "-------------------------" << std::endl;
}

void testOrderExecution() {
    //each order is validated before being executed according to the above description
    //ownership of a territory is transferred to the attacking player if a territory is conquered as a result of an advance order
    //one card is given to a player if they conquer at least one territory in a turn (not more than one card per turn
    //the negotiate order prevents attacks between the two players involved
    //the blockade order transfers ownership to the Neutral player
    //all the orders described above can be issued by a player and executed by the game engine
}