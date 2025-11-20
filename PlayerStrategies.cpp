#include "PlayerStrategies.h"
#include "GameEngine.h"
#include "Player.h"
#include <algorithm>
#include <iostream>
#include <vector>

void PlayerStrategies::SetPlayer(Player* p) {
    player = p;
}

PlayerStrategies::~PlayerStrategies() {
    player = NULL;
}


// ----- Human -----
std::vector<Territory*> HumanPlayerStrategy::ToAttack() const {
    std::vector<Territory*> toAttackList;

    for (Territory* territory : player->GetPlayerTerritories()) {
        for (Territory* neighbor : territory->AdjacentTerritories()) {
            if (std::find(toAttackList.begin(), toAttackList.end(), neighbor) == toAttackList.end()) {
                if (neighbor->GetOwner() != player) {
                    toAttackList.push_back(neighbor);
                }
            }
        }
    }

    if (toAttackList.empty()) {
        std::cout << "No territories to attack." << endl;
    }

    return toAttackList;
}

std::vector<Territory*> HumanPlayerStrategy::ToDefend() const {
    std::vector<Territory*> toDefendList;

    for (Territory* territory : player->GetPlayerTerritories()) {
        for (Territory* neighbor : territory->AdjacentTerritories()) {
            if (neighbor->GetOwner() != player && neighbor->GetOwner() != GameEngine::instance->neutralPlayer) { //neutral territories dont attack, not to be confused with the neutral player strategy
                toDefendList.push_back(territory);
                break;
            }
        }
    }

    if (toDefendList.empty()) {
        std::cout << "No territories to defend." << endl;
    }

    return toDefendList;
}

void HumanPlayerStrategy::IssueOrder() {
    std::vector<OrderNames> orders = player->availableOrders();
    int i = GameEngine::instance->GetPlayerIndex(player);

    if (orders.empty()) {
        std::cout << "No available orders for player " << player->GetName() << ". Ending turn.\n";
        GameEngine::instance->finishedPlayers[i] = true;
        return;
    }

    std::cout << "Available orders:\n";
    for (int j = 0; j < orders.size(); j++) {
        switch (orders[j]) {
        case DeployEnum:
            std::cout << j << " - Deploy\n";
            break;

        case AdvanceEnum:
            std::cout << j << " - Advance\n";
            break;

        case BombEnum:
            std::cout << j << " - Bomb\n";
            break;

        case AirliftEnum:
            std::cout << j << " - Airlift\n";
            break;

        case BlockadeEnum:
            std::cout << j << " - Blockade\n";
            break;

        case NegotiateEnum:
            std::cout << j << " - Negotiate\n";
            break;

        default:
            break;
        }
    }

    std::cout << "Enter the number of the order to issue, or -1 to end your turn: ";
    int choice;
    std::cin >> choice;

    // Issue the selected order
    switch (orders[choice]) {
        case  AdvanceEnum: {
            string startTerritory;
            string targetTerritory;
            int numUnits;

            //getting user input for advance order
            std::cout << "You chose to issue an Advance order.\nPlease enter the starting territory: "<< std::endl;
            std::cin >> startTerritory;
            std::cout << "Please enter the target territory: "<< std::endl;
            std::cin >> targetTerritory;
            std::cout << "Please enter the number of units to move: "<< std::endl;
            std::cin >> numUnits;
        
            //creating and issuing the advance order
            Advance* advanceOrder = new Advance(player, numUnits, GameEngine::instance->gameMap->GetTerritoryByName(startTerritory), GameEngine::instance->gameMap->GetTerritoryByName(targetTerritory));
            Advance* ptr = advanceOrder;
            player->AddOrderToList(ptr);

            std::cout << "Advance order issued.\n";
            return;
        }
        case AirliftEnum: {
            string startTerritory;
            string targetTerritory;
            int numUnits; 

            //getting user input for airlift order
            std::cout << "You chose to issue an Airlift order.\nPlease enter the starting territory: "<< std::endl;
            std::cin >> startTerritory;
            std::cout << "Please enter the target territory: "<< std::endl;
            std::cin >> targetTerritory;
            std::cout << "Please enter the number of units to move: "<< std::endl;
            std::cin >> numUnits;

            //creating and issuing the airlift order
            int cardIndex = player->GetPlayerHand()->GetCardIndex(AirliftEnum);

            if (cardIndex == player->GetPlayerHand()->GetHandSize()) {
                std::cout << "ERROR: could not find the card in hand, please try again.";
                IssueOrder();
                return;
            }

            player->GetPlayerHand()->PlayCard(cardIndex);
            Airlift* airliftOrder = new Airlift(player, numUnits, GameEngine::instance->gameMap->GetTerritoryByName(startTerritory), GameEngine::instance->gameMap->GetTerritoryByName(targetTerritory));
            Airlift* ptr = airliftOrder;
            player->AddOrderToList(ptr);

            std::cout << "Airlift order issued.\n";
            return;
        }
        case BlockadeEnum: {
            string targetTerritory;

            //getting user input for blockade order
            std::cout << "You chose to issue a Blockade order.\nPlease enter the target territory: "<< std::endl;
            std::cin >> targetTerritory;

            //creating and issuing the blockade order
            int cardIndex = player->GetPlayerHand()->GetCardIndex(BlockadeEnum);

            if (cardIndex == player->GetPlayerHand()->GetHandSize()) {
                std::cout << "ERROR: could not find the card in hand, please try again.";
                IssueOrder();
                return;
            }

            player->GetPlayerHand()->PlayCard(cardIndex);
            Blockade* blockadeOrder = new Blockade(player, GameEngine::instance->gameMap->GetTerritoryByName(targetTerritory));
            Blockade* ptr = blockadeOrder;
            player->AddOrderToList(ptr);

            std::cout << "Blockade order issued.\n";
            return;
        }
        case BombEnum: {
            string targetTerritory;

            //getting user input for bomb order
            std::cout << "You chose to issue a Bomb order.\nPlease enter the target territory: "<< std::endl;
            std::cin >> targetTerritory;

            //creating and issuing the bomb order
            int cardIndex = player->GetPlayerHand()->GetCardIndex(BombEnum);

            if (cardIndex == player->GetPlayerHand()->GetHandSize()) {
                std::cout << "ERROR: could not find the card in hand, please try again.";
                IssueOrder();
                return;
            }

            player->GetPlayerHand()->PlayCard(cardIndex);
            Bomb* bombOrder = new Bomb(player, GameEngine::instance->gameMap->GetTerritoryByName(targetTerritory));
            Bomb* ptr = bombOrder;
            player->AddOrderToList(ptr);

            std::cout << "Bomb order issued.\n";
            return;
        }
        case DeployEnum: {
            string targetTerritory;
            int numUnits;

            //getting user input for deploy order
            std::cout << "You chose to issue a Deploy order.\nPlease enter the target territory: "<< std::endl;
            std::cin >> targetTerritory;
            std::cout << "Please enter the number of units to deploy: "<< std::endl;
            std::cin >> numUnits;

            while (numUnits < 0 || numUnits > player->GetReinforcements()) {
                std::cout << "Please enter a valid number of units to deploy: "<< std::endl;
                std::cin >> numUnits;
            }

            //creating and issuing the deploy order
            Deploy* deployOrder = new Deploy(player, numUnits, GameEngine::instance->gameMap->GetTerritoryByName(targetTerritory));
            Deploy* ptr = deployOrder;
            player->AddOrderToList(ptr);
            player->SetReinforcements(player->GetReinforcements() - numUnits); //in order to be able to issue other kinds of orders this turn

            std::cout << "Deploy order issued.\n";
            return;
        }
        case NegotiateEnum: {
            string targetPlayerName;

            //getting user input for negotiate order
            std::cout << "You chose to issue a Negotiate order.\nPlease enter the target player name: "<< std::endl;
            std::cin >> targetPlayerName;
            Player* targetPlayer = GameEngine::instance->FindPlayerByName(targetPlayerName);

            while (targetPlayer == nullptr) {
                std::cout << "Could not find a player by that name, please try again: " << std::endl;
                std::cin >> targetPlayerName;
                targetPlayer = GameEngine::instance->FindPlayerByName(targetPlayerName);
            }

            //creating and issuing the negotiate order
            int cardIndex = player->GetPlayerHand()->GetCardIndex(NegotiateEnum);

            if (cardIndex == player->GetPlayerHand()->GetHandSize()) {
                std::cout << "ERROR: could not find the card in hand, please try again.";
                IssueOrder();
                return;
            }

            player->GetPlayerHand()->PlayCard(cardIndex);
            Negotiate* negotiateOrder = new Negotiate(player, targetPlayer);
            Negotiate* ptr = negotiateOrder;
            player->AddOrderToList(ptr);

            std::cout << "Negotiate order issued.\n";
            break;
        }
        default: {
            if (choice == -1){
                GameEngine::instance->finishedPlayers[i] = true;
                std::cout << "Player " << player->GetName() << " has ended their turn.\n";
            }
            else {
                std::cout << "Invalid choice. Please try again.\n";
                IssueOrder(); // Repeat this player's turn
            }

            return;
        }
    }
}
// ----- Human -----


// ----- Aggressive -----
std::vector<Territory*> AggressivePlayerStrategy::ToAttack() const {

}

std::vector<Territory*> AggressivePlayerStrategy::ToDefend() const {
    
}

void AggressivePlayerStrategy::IssueOrder() {
    
}
// ----- Aggressive -----


// ----- Benevolent -----
std::vector<Territory*> BenevolentPlayerStrategy::ToAttack() const {
    return nullptr;
}

std::vector<Territory*> BenevolentPlayerStrategy::ToDefend() const {
    std::vector<Territory*> toDefendList;

    for (Territory* territory : player->GetPlayerTerritories()) {
        for (Territory* neighbor : territory->AdjacentTerritories()) {
            if (neighbor->GetOwner() != player && neighbor->GetOwner() != GameEngine::instance->neutralPlayer) { //neutral territories dont attack, not to be confused with the neutral player strategy
                toDefendList.push_back(territory);
                break;
            }
        }
    }

    if (toDefendList.empty()) {
        std::cout << "No territories to defend." << endl;
    }

    return toDefendList;
}

void BenevolentPlayerStrategy::IssueOrder() {
    std::vector<OrderNames> orders = player->availableOrders();
    int PlayerIndex = GameEngine::instance->GetPlayerIndex(player);

    if (orders.empty()) {
        std::cout << "No available orders for player " << player->GetName() << ". Ending turn.\n";
        GameEngine::instance->finishedPlayers[PlayerIndex] = true;
        return;
    }

    int reinforcements = player->GetReinforcements();
    std::vector<Territory*> defendList = ToDefend();

    while (reinforcements > 0) {
        Territory* territory = findWeakestTerritory(defendList);
        if (territory == nullptr) {
            std::cout << "No territories to defend." << std::endl;
            return;
        }
        
    }

}

int unitsToDeploy = std::min(1, reinforcements); // Deploy at least 1 unit or the remaining reinforcements
                Deploy* deployOrder = new Deploy(player, unitsToDeploy, territory);
                player->AddOrderToList(deployOrder);
                player->SetReinforcements(reinforcements - unitsToDeploy);
                deployedFinished = false; // Continue deploying until all territories have at least 1 unit


Territory* BenevolentPlayerStrategy::findWeakestTerritory(const std::vector<Territory*>& territories) {
    if (territories.empty()) {
        return nullptr;
    }

    Territory* weakest = territories[0];
    for (Territory* territory : territories) {
        if (territory->GetUnits() < weakest->GetUnits()) {
            weakest = territory;
        }
    }
    return weakest;
}
// ----- Benevolent -----


// ----- Neutral -----
std::vector<Territory*> NeutralPlayerStrategy::ToAttack() const {

}

std::vector<Territory*> NeutralPlayerStrategy::ToDefend() const {
    
}

void NeutralPlayerStrategy::IssueOrder() {
    
}
// ----- Neutral -----


// ----- Cheater -----
std::vector<Territory*> CheaterPlayerStrategy::ToAttack() const {

}

std::vector<Territory*> CheaterPlayerStrategy::ToDefend() const {
    
}

void CheaterPlayerStrategy::IssueOrder() {
    
}
// ----- Cheater -----