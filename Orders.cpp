#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include <algorithm>
#include <cmath>
#include <stdlib.h>

using std::endl;

// ----- OrdersList -----

// Default constructor
OrdersList::OrdersList(){}

// Copy constructor
OrdersList::OrdersList(const OrdersList& obj) {
    for (size_t i = 0; i < obj.orders.size(); i++)
    {
        orders.push_back(obj.orders[i]);
    }
}

// Destructor
OrdersList::~OrdersList() {
    for (auto order : orders)
    {
        delete order;
        order = NULL;
    } 

    orders.clear();
}

// Assignment operator
OrdersList& OrdersList::operator= (const OrdersList& obj) {
    if (this != &obj) {
        for (size_t i = 0; i < obj.orders.size(); i++)
        {
            orders.push_back(obj.orders[i]);
        }
    }

    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& stream, const OrdersList& obj) {
    stream << "Order list:\n";

    for (const auto& order : obj.orders)
    {
        stream << *order << "----------" << std::endl;
    }

    stream << endl;
    return stream;
}

// Getter for the actual list
vector<Order*> OrdersList::GetListItems() {
    return orders;
}

// Add a new order to the list
void OrdersList::Add(Order* newOrder) {
    orders.push_back(newOrder);
}

// Move an order to a new position in the list
void OrdersList::Move(int fromId, int toId) {
    if (orders.size() == 0)
    {
        std::cout << "Error: no order in the list." << std::endl;
    }
    else if (orders.size() == 1)
    {
        std::cout << "Error: unable to swap positions when there is only 1 order in the list." << std::endl;
    }
    else if (fromId >= orders.size() || fromId < 0 || fromId >= orders.size() || fromId < 0)
    {
        std::cout << "Error: invalid swap position(s)." << std::endl;
    }
    else
    {
        std::iter_swap(orders.begin() + fromId, orders.begin() + toId);
    }
}

// Delete an order from the list
void OrdersList::Remove(int id) {
    if (orders.size() == 0)
    {
        std::cout << "Error: no order in the list." << std::endl;
    }
    else if (id >= orders.size() || id < 0)
    {
        std::cout << "Error: unable to remove order in invalid position." << std::endl;
    }
    else
    {
        orders.erase(orders.begin() + id);
    }
}

// Change the owning player for all orders in the list
void OrdersList::SetOwningPlayer(Player* player) {
    for (const auto& order : orders) {
        order->SetOwningPlayer(player);
    }
}
// ----- OrdersList -----


// ----- Order -----
// Stream insertion operator
ostream& operator<<(ostream& stream, const Order& obj) {
    return obj.Print(stream);
}

// Setter for owningPlayer
void Order::SetOwningPlayer(Player* player) {
    owningPlayer = player;
}

// Getter for owningPlayer
Player* Order::GetOwningPlayer() {
    return owningPlayer;
}

// Checks whether a given territory belongs to the order's player
bool Order::TerritoryBelongsToPlayer(Territory* territory) {
    return territory->GetOwner()->GetName() == owningPlayer->GetName();
}

// ----- Order -----


// ----- Advance -----
// Default constructor
Advance::Advance() {}

// Parameterized constructor
Advance::Advance(Player* owningPlayer, int nbUnits, Territory* src, Territory* target) {
    this->owningPlayer = owningPlayer;
    this->nbUnits = nbUnits;
    this->src = src;
    this->target = target;
}

// Copy constructor
Advance::Advance(const Advance& obj) {
    owningPlayer = obj.owningPlayer;
    nbUnits = obj.nbUnits;
    src = obj.src;
    target = obj.target;
}

// Destructor
Advance::~Advance() {
    delete src;
    src = NULL;

    delete target;
    target = NULL;
}

// Assignment operator
Advance& Advance::operator= (const Advance& obj) {
    if (this != &obj) {
        owningPlayer = obj.owningPlayer;
        nbUnits = obj.nbUnits;
        src = obj.src;
        target = obj.target;
    }

    return *this;
}

// Setter for nbUnits
void Advance::SetNbUnits(int nb) {
    nbUnits = nb;
}

// Getter for nbUnits
int Advance::GetNbUnits() {
    return nbUnits;
}

// Setter for src
void Advance::SetSrc(Territory* territory) {
    src = territory;
}

// Getter for src
Territory* Advance::GetSrc() {
    return src;
}

// Setter for target
void Advance::SetTarget(Territory* Target) {
    target = Target;
}

// Getter for target
Territory* Advance::GetTarget() {
    return target;
}

// Prints the order to the specified stream
ostream& Advance::Print(ostream& stream) const {
    stream << "Advance: " << GetEffect() << std::endl;
    return stream;
}

// Returns the effect of the order as a string
std::string Advance::GetEffect() const {
    return std::string("moving ") + std::to_string(nbUnits) + 
        std::string(" units from ") + src->GetName() + std::string(" to ") + target->GetName();
}

// Simulate a territory battle
void SimulateBattle(Player* attackingPlayer, int attackingUnits, int defendingUnits, Territory* src, Territory* target) {
    int attackersKilled = 0;
    int defendersKilled = 0;

    // Attacking phase result
    for (int i = 0; i < attackingUnits; ++i) {
        if ((rand() % 100) < 60) {
            ++defendersKilled;
        }

        if (defendersKilled >= defendingUnits) {
            break;
        }
    }

    // Defending phase result
    for (int i = 0; i < defendingUnits; ++i) {
        if ((rand() % 100) < 70) {
            ++attackersKilled;
        }

        if (attackersKilled >= attackingUnits) {
            break;
        }
    }

    if (attackersKilled < attackingUnits && defendersKilled >= defendingUnits) {
        int remainingUnits = attackingUnits - attackersKilled;
        src->SetUnits(src->GetUnits() - attackingUnits);
        target->SetUnits(remainingUnits);
        target->SetOwner(attackingPlayer);

        //TODO if no previous conquer, get card + set conquer
        std::cout << "TERRITORY was successfully conquered by PLAYER." << std::endl;
    }
    else {
        std::cout << "PLAYER failed to conquer TERRITORY; ";

        if (attackersKilled < attackingUnits) {
            std::cout << "UNITS returned to SRC; ";
            src->SetUnits(src->GetUnits() - attackersKilled);
        }
        else {
            std::cout << "All attacking units lost in combat; ";
            src->SetUnits(src->GetUnits() - attackingUnits);
        }

        if (defendersKilled < defendingUnits) {
            std::cout << "UNITS remaining on TARGET" << std::endl;
            target->SetUnits(target->GetUnits() - defendersKilled);
        }
        else {
            std::cout << "All defending units lost in combat." << std::endl;
            target->SetUnits(0);
        }
    }
}

// Move a certain number of army units from one of the current player’s territories to
// another territory that is adjacent to the source territory
void Advance::Execute() {
    if (Validate()) {
        if (TerritoryBelongsToPlayer(target)) {
            src->SetUnits(src->GetUnits() - nbUnits);
            target->SetUnits(target->GetUnits() + nbUnits);

            std::cout << "Advance order successful in " << GetEffect() << std::endl;
        }
        else if (true) {
            //IF not negotiating w other
            SimulateBattle(owningPlayer, nbUnits, target->GetUnits(), src, target);
        }
        else {
            std::cout << "Unable to attack TERRITORY; NAME is currently in negotiations with NAME" << std::endl;
        }
    }
    else {
        std::cout << "This Advance order is invalid." << std::endl;
    }
}

// Check if the source territory belongs to the same player as the order, has enough units to move,
// and that the target is adjacent to it
bool Advance::Validate() {
    return TerritoryBelongsToPlayer(src) && src->GetUnits() >= nbUnits && src->IsAdjacent(target);
}
// ----- Advance -----


// ----- Airlift -----
// Default constructor
Airlift::Airlift() {}

// Parameterized constructor
Airlift::Airlift(Player* owningPlayer, int nbUnits, Territory* src, Territory* target) {
    this->owningPlayer = owningPlayer;
    this->nbUnits = nbUnits;
    this->src = src;
    this->target = target;
}

// Copy constructor
Airlift::Airlift(const Airlift& obj) {
    owningPlayer = obj.owningPlayer;
    nbUnits = obj.nbUnits;
    src = obj.src;
    target = obj.target;
}

// Destructor
Airlift::~Airlift() {
    delete src;
    src = NULL;

    delete target;
    target = NULL;
}

// Assignment operator
Airlift& Airlift::operator= (const Airlift& obj) {
    if (this != &obj) {
        owningPlayer = obj.owningPlayer;
        nbUnits = obj.nbUnits;
        src = obj.src;
        target = obj.target;
    }

    return *this;
}

// Setter for nbUnits
void Airlift::SetNbUnits(int nb) {
    nbUnits = nb;
}

// Getter for nbUnits
int Airlift::GetNbUnits() {
    return nbUnits;
}

// Setter for src
void Airlift::SetSrc(Territory* territory) {
    src = territory;
}

// Getter for src
Territory* Airlift::GetSrc() {
    return src;
}

// Setter for target
void Airlift::SetTarget(Territory* Target) {
    target = Target;
}

// Getter for target
Territory* Airlift::GetTarget() {
    return target;
}

// Prints the order to the specified stream
ostream& Airlift::Print(ostream& stream) const {
    stream << "Airlift: moving " << GetEffect() << std::endl;
    return stream;
}

// Returns the effect of the order as a string
std::string Airlift::GetEffect() const {
    return std::string("moving ") + std::to_string(nbUnits) + 
        std::string(" units from ") + src->GetName() + std::string(" to ") + target->GetName();
}

// Advance a certain number of army units from one of the current player’s territories to any another territory
void Airlift::Execute() {
    if (Validate()) {
        src->SetUnits(src->GetUnits() - nbUnits);
        target->SetUnits(target->GetUnits() + nbUnits);
        std::cout << "Airlift order successful in " << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Airlift order is invalid." << std::endl;
    }
}

// Check if the source and target territories belong to the same player as the order, and the source has enough units to move
bool Airlift::Validate() {
    return TerritoryBelongsToPlayer(src) && TerritoryBelongsToPlayer(target) && src->GetUnits() >= nbUnits;
}
// ----- Airlift -----


// ----- Blockade -----
// Default constructor
Blockade::Blockade() {}

// Parameterized constructor
Blockade::Blockade(Player* owningPlayer, Territory* target) {
    this->owningPlayer = owningPlayer;
    this->target = target;
}

// Copy constructor
Blockade::Blockade(const Blockade& obj) {
    owningPlayer = obj.owningPlayer;
    target = obj.target;
}

// Destructor
Blockade::~Blockade() {
    delete target;
    target = NULL;
}

// Assignment operator
Blockade& Blockade::operator= (const Blockade& obj) {
    if (this != &obj) {
        owningPlayer = obj.owningPlayer;
        target = obj.target;
    }

    return *this;
}

// Setter for target
void Blockade::SetTarget(Territory* Target) {
    target = Target;
}

// Getter for target
Territory* Blockade::GetTarget() {
    return target;
}

// Prints the order to the specified stream
ostream& Blockade::Print(ostream& stream) const {
    stream << "Blockade: " << GetEffect() << std::endl; 
    return stream;
}

// Returns the effect of the order as a string
std::string Blockade::GetEffect() const {
    return "tripling the number of units on " + target->GetName();
}

// Triple?? Double?? the number of army units on one of the current player’s territories and make it a neutral territory.
void Blockade::Execute() {
    if (Validate()) {
        target->SetUnits(target->GetUnits() * 2);
        //TODO territory becomes Neutral (create if doesnt exist yet)
        std::cout << "Blockade order successful in " << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Blockade order is invalid." << std::endl;
    }
}

// Check if the target territory belongs to the same player as the order
bool Blockade::Validate() {
    return TerritoryBelongsToPlayer(target);
}
// ----- Blockade -----


// ----- Bomb -----
// Default constructor
Bomb::Bomb() {}

// Parameterized constructor
Bomb::Bomb(Player* owningPlayer, Territory* target) {
    this->owningPlayer = owningPlayer;
    this->target = target;
}

// Copy constructor
Bomb::Bomb(const Bomb& obj) {
    owningPlayer = obj.owningPlayer;
    target = obj.target;
}

// Destructor
Bomb::~Bomb() {
    delete target;
    target = NULL;
}

// Assignment operator
Bomb& Bomb::operator= (const Bomb& obj) {
    if (this != &obj) {
        owningPlayer = obj.owningPlayer;
        target = obj.target;
    }

    return *this;
}

// Setter for target
void Bomb::SetTarget(Territory* Target) {
    target = Target;
}

// Getter for target
Territory* Bomb::GetTarget() {
    return target;
}

// Prints the order to the specified stream
ostream& Bomb::Print(ostream& stream) const {
    stream << "Bomb: " << GetEffect() << std::endl;  
    return stream;
}

// Returns the effect of the order as a string
std::string Bomb::GetEffect() const {
    return "destroying half of the units on " + target->GetName();
}

// Destroy half of the army units located on an opponent’s territory that is adjacent to one of the current player’s territories
void Bomb::Execute() {
    if (Validate()) {
        int halfNb = round(static_cast<double>(target->GetUnits()) / 2); //If odd number of units, then this half is bigger
        target->SetUnits(target->GetUnits() - halfNb);
        std::cout << "Bomb order successful in " << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Bomb order is invalid." << std::endl;
    }
}

// Check if the target belongs to another player and has an adjacent territory
// that belongs to the player issuing the order
bool Bomb::Validate() {
    std::string ownerName = owningPlayer->GetName();
    std::vector<Territory *> adj = target->AdjacentTerritories();

    return target->GetOwner()->GetName() != ownerName &&
        std::find_if(adj.begin(), adj.end(), [&ownerName](Territory* t) { return t->GetOwner()->GetName() == ownerName; }) != adj.end();
}
// ----- Bomb -----


// ----- Deploy -----
// Default constructor
Deploy::Deploy() {}

// Parameterized constructor
Deploy::Deploy(Player* owningPlayer, int nbUnits, Territory* target) {
    this->owningPlayer = owningPlayer;
    this->nbUnits = nbUnits;
    this->target = target;
}

// Copy constructor
Deploy::Deploy(const Deploy& obj) {
    owningPlayer = obj.owningPlayer;
    nbUnits = obj.nbUnits;
    target = obj.target;
}

// Destructor
Deploy::~Deploy() {
    delete target;
    target = NULL;
}

// Assignment operator
Deploy& Deploy::operator=(const Deploy& obj) {
    if (this != &obj) {
        owningPlayer = obj.owningPlayer;
        nbUnits = obj.nbUnits;
        target = obj.target;
    }

    return *this;
}

// Setter for nbUnits
void Deploy::SetNbUnits(int nb) {
    nbUnits = nb;
}

// Getter for nbUnits
int Deploy::GetNbUnits() {
    return nbUnits;
}

// Setter for target
void Deploy::SetTarget(Territory* Target) {
    target = Target;
}

// Getter for target
Territory* Deploy::GetTarget() {
    return target;
}

// Prints the order to the specified stream
ostream& Deploy::Print(ostream& stream) const {
    stream << "Deploy: " << GetEffect() << std::endl;
    return stream;
}

// Returns the effect of the order as a string
std::string Deploy::GetEffect() const {
    return std::string("adding ") + std::to_string(nbUnits) + std::string(" units to ") + target->GetName();
}

// Move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories
void Deploy::Execute() {
    if (Validate()) {
        target->SetUnits(target->GetUnits() + nbUnits);
        std::cout << "Deploy order successful in " << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Deploy order is invalid." << std::endl;
    }
}

// Check if the target belongs to the same player as the order
bool Deploy::Validate() {
    return TerritoryBelongsToPlayer(target);
}

// ----- Deploy -----


// ----- Negotiate -----
// Default constructor
Negotiate::Negotiate() {}

// Parameterized constructor
Negotiate::Negotiate(Player* owningPlayer, Player* otherPlayer) {
    this->owningPlayer = owningPlayer;
    this->otherPlayer = otherPlayer;
}

// Copy constructor
Negotiate::Negotiate(const Negotiate& obj) {
    owningPlayer = obj.owningPlayer;
    otherPlayer = obj.otherPlayer;
}

// Assignment operator
Negotiate& Negotiate::operator= (const Negotiate& obj) {
    if (this != &obj) {
        owningPlayer = obj.owningPlayer;
        otherPlayer = obj.otherPlayer;
    }

    return *this;
}

// Setter for otherPlayer
void Negotiate::SetOtherPlayer(Player* player) {
    otherPlayer = player;
}

// Getter for otherPlayer
Player* Negotiate::GetOtherPlayer() {
    return otherPlayer;
}

// Prints the order to the specified stream
ostream& Negotiate::Print(ostream& stream) const {
    stream << "Negotiate: " << GetEffect() << std::endl; 
    return stream;
}

// Returns the effect of the order as a string
std::string Negotiate::GetEffect() const {
    return std::string("preventing attacks between ") + owningPlayer->GetName() + std::string(" and ") + otherPlayer->GetName();
}

// Prevent attacks between the current player and the player targeted by the negotiate order until the end of the turn.
void Negotiate::Execute() {
    if (Validate()) {
        //TODO notify other player of negotiations, set negotiating state
        std::cout << "Negotiate order successful in " << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Negotiate order is invalid." << std::endl;
    }
}

// Check if the other player is different than the one issuing the order
bool Negotiate::Validate() {
    return otherPlayer != owningPlayer;
}
// ----- Negotiate -----