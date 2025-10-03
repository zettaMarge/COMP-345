#include "Orders.h"
//#include "Map.h"
//#include "Player.h"

using std::endl;

// ----- OrdersList -----
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
    stream << "Order list: ";

    for (const auto& order : obj.orders)
    {
        stream << order << "\n----------\n";
    }

    stream << endl;
    return stream;
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
        Order* temp = orders[fromId];
        orders[fromId] = orders[toId];
        orders[toId] = orders[fromId];
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
// ----- OrdersList -----


// ----- Order -----
// Destrcutor
Order::~Order() {
    delete owningPlayer;
    owningPlayer = NULL;
}

ostream& operator<<(ostream& stream, const Order& obj) {
    return obj.Print(stream);
}

// ----- Order -----


// ----- Advance -----
// Constructor
Advance::Advance(Player* owningPlayer, int nbUnits, Territory* src, Territory* target) {
    owningPlayer = owningPlayer;
    nbUnits = nbUnits;
    src = src;
    target = target;
}

// Copy constructor
Advance::Advance(const Advance& obj) {
    owningPlayer = new Player;
    *owningPlayer = *obj.owningPlayer;

    nbUnits = obj.nbUnits;

    src = new Territory;
    *src = *obj.src;

    target = new Territory;
    *target = *obj.target;
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
        owningPlayer = new Player;
        *owningPlayer = *obj.owningPlayer;

        nbUnits = obj.nbUnits;

        src = new Territory;
        *src = *obj.src;

        target = new Territory;
        *target = *obj.target;
    }

    return *this;
}

// Prints the order to the specified stream
ostream& Advance::Print(ostream& stream) const {
    stream << "Advance: " << GetEffect() << std::endl;
}

// Returns the effect of the order as a string
std::string Advance::GetEffect() const {
    return std::string("moving ") + std::to_string(nbUnits) + std::string(" units from [TERRITORY] to [TERRITORY]");
}

// Move a certain number of army units from one of the current player’s territories to another territory that is adjacent to the source territory
void Advance::Execute() {
    if (Validate()) {
        std::cout << "Advance order successful in" << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Advance order is invalid." << std::endl;
    }
}

// Validates the order
bool Advance::Validate() {
    //TODO if src belongs to player, has enough units to move, AND target is adjacent
}
// ----- Advance -----


// ----- Airlift -----
// Constructor
Airlift::Airlift(Player* owningPlayer, int nbUnits, Territory* src, Territory* target) {
    owningPlayer = owningPlayer;
    nbUnits = nbUnits;
    src = src;
    target = target;
}

// Copy constructor
Airlift::Airlift(const Airlift& obj) {
    owningPlayer = new Player;
    *owningPlayer = *obj.owningPlayer;

    nbUnits = obj.nbUnits;

    src = new Territory;
    *src = *obj.src;

    target = new Territory;
    *target = *obj.target;
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
        owningPlayer = new Player;
        *owningPlayer = *obj.owningPlayer;

        nbUnits = obj.nbUnits;

        src = new Territory;
        *src = *obj.src;

        target = new Territory;
        *target = *obj.target;
    }

    return *this;
}

// Prints the order to the specified stream
ostream& Airlift::Print(ostream& stream) const {
    stream << "Airlift: moving " << GetEffect() << std::endl; 
}

// Returns the effect of the order as a string
std::string Airlift::GetEffect() const {
    return std::string("moving ") + std::to_string(nbUnits) + std::string(" units from [TERRITORY] to [TERRITORY]");
}

// Advance a certain number of army units from one of the current player’s territories to any another territory
void Airlift::Execute() {
    if (Validate()) {
        std::cout << "Airlift order successful in" << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Airlift order is invalid." << std::endl;
    }
}

// Validates the order
bool Airlift::Validate() {
    //TODO if src belongs to player
}
// ----- Airlift -----


// ----- Blockade -----
// Constructor
Blockade::Blockade(Player* owningPlayer, Territory* target) {
    owningPlayer = owningPlayer;
    target = target;
}

// Copy constructor
Blockade::Blockade(const Blockade& obj) {
    owningPlayer = new Player;
    *owningPlayer = *obj.owningPlayer;

    target = new Territory;
    *target = *obj.target;
}

// Destructor
Blockade::~Blockade() {
    delete target;
    target = NULL;
}

// Assignment operator
Blockade& Blockade::operator= (const Blockade& obj) {
    if (this != &obj) {
        owningPlayer = new Player;
        *owningPlayer = *obj.owningPlayer;

        target = new Territory;
        *target = *obj.target;
    }

    return *this;
}

// Prints the order to the specified stream
ostream& Blockade::Print(ostream& stream) const {
    stream << "Blockade: " << GetEffect() << std::endl; 
}

// Returns the effect of the order as a string
std::string Blockade::GetEffect() const {
    return "tripling the number of units on [[TERRITORY]]";
}

// Triple the number of army units on one of the current player’s territories and make it a neutral territory.
void Blockade::Execute() {
    if (Validate()) {
        std::cout << "Blockade order successful in" << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Blockade order is invalid." << std::endl;
    }
}

// Validates the order
bool Blockade::Validate() {
    //TODO if target belongs to player
}
// ----- Blockade -----


// ----- Bomb -----
// Constructor
Bomb::Bomb(Player* owningPlayer, Territory* target) {
    owningPlayer = owningPlayer;
    target = target;
}

// Copy constructor
Bomb::Bomb(const Bomb& obj) {
    owningPlayer = new Player;
    *owningPlayer = *obj.owningPlayer;

    target = new Territory;
    *target = *obj.target;
}

// Destructor
Bomb::~Bomb() {
    delete target;
    target = NULL;
}

// Assignment operator
Bomb& Bomb::operator= (const Bomb& obj) {
    if (this != &obj) {
        owningPlayer = new Player;
        *owningPlayer = *obj.owningPlayer;

        target = new Territory;
        *target = *obj.target;
    }

    return *this;
}

// Prints the order to the specified stream
ostream& Bomb::Print(ostream& stream) const {
    stream << "Bomb: " << std::endl;  
}

// Returns the effect of the order as a string
std::string Bomb::GetEffect() const {
    return "destroy half of the units on [[TERRITORY]]";
}

// Destroy half of the army units located on an opponent’s territory that is adjacent to one of the current player’s territories
void Bomb::Execute() {
    if (Validate()) {
        std::cout << "Bomb order successful in" << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Bomb order is invalid." << std::endl;
    }
}

// Validates the order
bool Bomb::Validate() {
    //TODO if target doesnt belong to player, and is adjacent to a territory owned by player
}
// ----- Bomb -----


// ----- Deploy -----
// Constructor
Deploy::Deploy(Player* owningPlayer, int nbUnits, Territory* target) {
    owningPlayer = owningPlayer;
    nbUnits = nbUnits;
    target = target;
}

// Copy constructor
Deploy::Deploy(const Deploy& obj) {
    owningPlayer = new Player;
    *owningPlayer = *obj.owningPlayer;

    nbUnits = obj.nbUnits;

    target = new Territory;
    *target = *obj.target;
}

// Destructor
Deploy::~Deploy() {
    delete target;
    target = NULL;
}

// Assignment operator
Deploy& Deploy::operator=(const Deploy& obj) {
    if (this != &obj) {
        owningPlayer = new Player;
        *owningPlayer = *obj.owningPlayer;

        nbUnits = obj.nbUnits;

        target = new Territory;
        *target = *obj.target;
    }

    return *this;
}

// Prints the order to the specified stream
ostream& Deploy::Print(ostream& stream) const {
    stream << "Deploy: " << GetEffect() << std::endl;
}

// Returns the effect of the order as a string
std::string Deploy::GetEffect() const {
    return std::string("adding ") + std::to_string(nbUnits) + std::string(" units to [TERRITORY]");
}

// Move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories
void Deploy::Execute() {
    if (Validate()) {
        std::cout << "Deploy order successful in" << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Deploy order is invalid." << std::endl;
    }
}

// Validates the order
bool Deploy::Validate() {
    //TODO if territory belongs to player
}

// ----- Deploy -----


// ----- Negotiate -----
// Constructor
Negotiate::Negotiate(Player* owningPlayer, Player* otherPlayer) {
    owningPlayer = owningPlayer;
    otherPlayer = otherPlayer;
}

// Copy constructor
Negotiate::Negotiate(const Negotiate& obj) {
    owningPlayer = new Player;
    *owningPlayer = *obj.owningPlayer;

    otherPlayer = new Player;
    *otherPlayer = *obj.otherPlayer;
}

// Destructor
Negotiate::~Negotiate() {
    delete otherPlayer;
    otherPlayer = NULL;
}

// Assignment operator
Negotiate& Negotiate::operator= (const Negotiate& obj) {
    if (this != &obj) {
        owningPlayer = new Player;
        *owningPlayer = *obj.owningPlayer;

        otherPlayer = new Player;
        *otherPlayer = *obj.otherPlayer;
    }
}

// Prints the order to the specified stream
ostream& Negotiate::Print(ostream& stream) const {
    stream << "Negotiate: " << GetEffect() << std::endl; 
}

// Returns the effect of the order as a string
std::string Negotiate::GetEffect() const {
    return std::string("preventing attacks between [PLAYER] and [PLAYER]");
}

// Prevent attacks between the current player and the player targeted by the negotiate order until the end of the turn.
void Negotiate::Execute() {
    if (Validate()) {
        std::cout << "Negotiate order successful in" << GetEffect() << std::endl;
    }
    else {
        std::cout << "This Negotiate order is invalid." << std::endl;
    }
}

// Validates the order
bool Negotiate::Validate() {
    //TODO if player != otherPlayer
}
// ----- Negotiate -----