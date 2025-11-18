//-----------------------------------------------------
//Mia Letendre 40274681
//COMP-345-D 2252 Assignment 1 Players.cpp
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
#include "GameEngine.h"
#include <vector>
#include <iostream>
#include <unordered_set>
#include <algorithm>

using namespace std;

bool Player::Equals(Player* p) {
    return this == p;
};

// Default constructor
Player::Player() {

    name = "Luara Palmer";
    playerTerritories = vector<Territory*>();
    playerHand = nullptr;
    playersOrders = new OrdersList();
    conqueredThisTurn = false;
    negotiators = vector<Player*>();
}

// Parameterized constructor
// Deep copies of everything
Player::Player(std::string& name, vector<Territory*> playerTerritories, Hand &playerHand, OrdersList &playerOrders, vector<Player*> negotiators){
    
    this->name = name;
    for (Territory* t : playerTerritories) {
        this->AddTerritory(t);
    }
    this->playerHand = new Hand(playerHand);
    this->playersOrders = new OrdersList(playerOrders);
    this->conqueredThisTurn = false;
    this->negotiators = vector<Player*>();
    for (Player* p : negotiators) {
        this->AddNegotiator(p);
    }
}

//This is a stub meant to be able to code players class
//will return all the territiories that are not owned by the player and can be attacked   
//assumes that adjacentTerritories() returns a list of territories adjacent to the territory
//aslo assumes that the territories var hold all the territories owned by the player
// Compares Owner pointer to this Player instance
vector<Territory*> Player::ToAttack() const {
    return playerStrategy->ToAttack();
};

//This is a stub meant to be able to code players class
//will return all the territiories that are owned by the player and can be defended
// Compares Owner pointer to this Player instance
vector<Territory*> Player::ToDefend() const {
    return playerStrategy->ToDefend();
};


//This is a stub meant to be able to code players class
//Processes which order to make
void Player::IssueOrder() {
    playerStrategy->IssueOrder();
};

//Adds an order to the player's list of orders
void Player::AddOrderToList(Order* x) {
    playersOrders->Add(x);
};

std::vector<GameEngine::OrderNames> Player::availableOrders() {
    std::vector<GameEngine::OrderNames> orders;

    //while there are still reinforcements, deploy is the only available order
    if (GetReinforcements() > 0) {
        orders.push_back(GameEngine::DeployEnum);
    }
    else {
        orders.push_back(GameEngine::AdvanceEnum);

        for (Card card : GetPlayerHand()->GetCards()) {
            Card* ptr = &card;
            int type = ptr->GetType();
            switch (type) {
                case GameEngine::AirliftEnum:
                    orders.push_back(GameEngine::AirliftEnum);
                    break;
                case GameEngine::BombEnum:
                    orders.push_back(GameEngine::BombEnum);
                    break;
                case GameEngine::BlockadeEnum:
                    orders.push_back(GameEngine::BlockadeEnum);
                    break;
                case GameEngine::NegotiateEnum:
                    orders.push_back(GameEngine::NegotiateEnum);
                    break;
                default:
                    break;
            }
        }
    }

    return orders;
}

void Player::AddReinforcements(int num) {
    reinforcements += num;
}

//Returns true when a Territory is found in a Player's owned territories
bool Player::IsTerritoryOwned(Territory* t) {
    return std::find(playerTerritories.begin(), playerTerritories.end(), t) != playerTerritories.end();
}

//Adds territory to players list of territories
//temporarally tracks index for temp adjacent territories fucntion
//assumes in territory that owner is public
void Player::AddTerritory(Territory* t) {
    if (IsTerritoryOwned(t)) {
        cout << "Territory already exists in player's list." << endl;
        return;
    }

    playerTerritories.push_back(t);

    if (t->GetOwner() != this) {
        t->SetOwner(this);
    }
}

//Swicthes territory from this player to another player
//also sets territory owner to nullptr if this player owns it
//needs to be updated to actually erase from vector
void Player::SwitchTerritory(Territory* t, Player* p) {
    auto it = std::remove(playerTerritories.begin(), playerTerritories.end(), t);

    if (it != playerTerritories.end()) {
        playerTerritories.erase(it, playerTerritories.end());
        p->AddTerritory(t);
    }
    else {
        cout << "Territory not found in player's list." << endl;
    }
    playerTerritories.erase(it, playerTerritories.end());
}

//Add a player to be in negotiations with the current player, if they aren't already
void Player::AddNegotiator(Player* p) {
    if (!IsNegotiatingWith(p)) {
        negotiators.push_back(p);
    }
    else {
        cout << "Already in negotiations with " << p->GetName() << endl;
    }  
}

//Checks if a given player is currently negotiating with the current player
bool Player::IsNegotiatingWith(Player* p) {
    return std::find(negotiators.begin(), negotiators.end(), p) != negotiators.end();
}

//Resets the list of current negotiators and whether a territory was conquered
void Player::ResetNegotiationsAndConquer() {
    negotiators.clear();
    conqueredThisTurn = false;
}

// Copy constructor
// Clear existing territories and shallow copies, because otherwise there would be two owners of the same territory
Player::Player(const Player& p) {
    name = p.name;
    playerTerritories = p.playerTerritories;
    playerHand = new Hand(*(p.playerHand));
    playersOrders = new OrdersList(*(p.playersOrders));
    conqueredThisTurn = p.conqueredThisTurn;
}

// Assignment operator
// Clear existing territories and shallow copies, because otherwise there would be two owners of the same territory
Player& Player::operator=(const Player& p) {
    if (this != &p) {
        name = p.name;
        for (Territory* t : p.playerTerritories) {
            playerTerritories.push_back(t);
        }

        playerHand = new Hand(*(p.playerHand));
        playersOrders = new OrdersList(*(p.playersOrders));
    }
    return *this;

}

// Stream insertion operator
ostream& operator<<(ostream& os, const Player& p) {
    os << "Player Name: " << p.name << endl;
    os << "Territories owned: ";
    for (const Territory* t : p.playerTerritories) {
        os << t << " ";
    }
    vector<Territory*> defendList = p.ToDefend();
    cout << "Territories to defend: ";
    if (defendList.size() == 0) {
        cout << "None";
    }
    for (Territory* t : defendList) {
        cout << t->GetName() << " ";
    }

    cout << endl;
    vector<Territory*> attackList = p.ToAttack();
    cout << "Territories to attack: ";
    if (attackList.size() == 0) {
        cout << "None";
    }
    for (Territory* t : attackList) {
        cout << t->GetName() << " ";
    }
    cout << endl;
    os << endl;
    os << "Cards in hand: " << endl;
    if (p.playerHand) {
        p.playerHand->Print();
    }
    else {
        os << "No hand assigned." << endl;
    }
    os << "Orders issued: ";
    for (Order* o : p.GetPlayerOrders()->GetListItems()) {
        cout << *o;
    }
    return os;
}


// Clean up dynamically allocated Orders
// Note: Territories are not deleted here as they may be shared among players
Player::~Player() {
    playerTerritories.clear();
    negotiators.clear();
    delete playerStrategy;
    playerStrategy = NULL;
}

string Player::GetName() {
    return name;
};

vector<Territory*> Player::GetPlayerTerritories() {
    return playerTerritories;
};

vector<Continent*> Player::GetOwnedContinents(const Map* map) const {
    vector<Continent*> fullyOwnedContinents;

    // Assume you have access to a list of all continents in the game
    for (Continent* continent : map->GetContinents()) {
        bool ownsAll = true;

        for (Territory* terr : continent->GetTerritories()) {
            if (terr->GetOwner() != this) {
                ownsAll = false;
                break;
            }
        }

        if (ownsAll) {
            fullyOwnedContinents.push_back(continent);
        }
    }
    return fullyOwnedContinents;
};

Hand* Player::GetPlayerHand() {
    return playerHand;
};

OrdersList* Player::GetPlayerOrders() const {
    return playersOrders;
};

int Player::GetReinforcements() {
    return reinforcements;
};

void Player::SetName(string name) {
    this->name = name;
};

void Player::SetPlayerTerritories(vector<Territory*> territories) {
    playerTerritories.clear();
    for (Territory* t : territories) {
        this->AddTerritory(t);
    }
};

void Player::SetPlayerHand(Hand* hand) {
    delete playerHand;
    playerHand = new Hand(*hand);
};

void Player::SetPlayerOrders(OrdersList* orders) {
    delete playersOrders;
    this->playersOrders = new OrdersList(*orders);
};

bool Player::HasConqueredThisTurn() {
    return conqueredThisTurn;
}

void Player::SetConqueredThisTurn(bool state) {
    conqueredThisTurn = state;
}

void Player::SetReinforcements(int reinforcements) {
    this->reinforcements = reinforcements;
};

PlayerStrategies* Player::GetStrategy() {
    return playerStrategy;
}

void Player::SetStrategy(PlayerStrategies* strategy) {
    playerStrategy = strategy;
    playerStrategy->SetPlayer(this);
}
