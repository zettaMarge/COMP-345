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

}

// Parameterized constructor
// Deep copies of everything
Player::Player(std::string& name, vector<Territory*> playerTerritories, Hand& playerHand, OrdersList& playerOrders) {

    this->name = name;
    for (Territory* t : playerTerritories) {
        this->AddTerritory(t);
    }
    this->playerHand = new Hand(playerHand);
    this->playersOrders = new OrdersList(playerOrders);
}

//This is a stub meant to be able to code players class
//will return all the territiories that are not owned by the player and can be attacked   
//assumes that adjacentTerritories() returns a list of territories adjacent to the territory
//aslo assumes that the territories var hold all the territories owned by the player
// Compares Owner pointer to this Player instance
vector<Territory*> Player::ToAttack() const {
    vector<Territory*> toAttackList;
    for (Territory* territory : playerTerritories) {
        for (Territory* neighbor : territory->AdjacentTerritories()) {
            if (std::find(toAttackList.begin(), toAttackList.end(), neighbor) == toAttackList.end()) {
                if (neighbor->GetOwner() != this) {
                    toAttackList.push_back(neighbor);
                }
            }
        }
    }
    if (toAttackList.empty()) {
        cout << "No territories to attack." << endl;
    }
    return toAttackList;
};

//This is a stub meant to be able to code players class
//will return all the territiories that are owned by the player and can be defended
// Compares Owner pointer to this Player instance
vector<Territory*> Player::ToDefend() const {

    if (playerTerritories.empty()) {
        cout << "No territories to defend." << endl;
    }
    return playerTerritories;
};


//This is a stub meant to be able to code players class
// will add newOrder to players list of orders
void Player::IssueOrder(Order* x) {
    playersOrders->Add(x);
};

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

// Copy constructor
// Clear existing territories and shallow copies, because otherwise there would be two owners of the same territory
Player::Player(const Player& p) {
    name = p.name;
    playerTerritories = p.playerTerritories;
    playerHand = new Hand(*(p.playerHand));
    playersOrders = new OrdersList(*(p.playersOrders));
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

void Player::SetReinforcements(int reinforcements) {
    this->reinforcements = reinforcements;
};