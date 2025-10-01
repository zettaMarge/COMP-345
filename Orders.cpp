#include "Orders.h"

// ----- Order -----
// Destrcutor
Order::~Order()
{
    delete m_owningPlayer;
}

// -----------------


// ----- Deploy -----
// Constructor
Deploy::Deploy(Player* owningPlayer, int nbUnits, Territory* target)
{
    type = "Deploy";
    m_owningPlayer = owningPlayer;
    m_nbUnits = nbUnits;
    m_target = target;
}

// Copy constructor
Deploy::Deploy(const Deploy& obj)
{
    m_owningPlayer = new Player;
    *m_owningPlayer = *obj.m_owningPlayer;

    type = "Deploy";
    m_nbUnits = obj.m_nbUnits;

    m_target = new Territory;
    *m_target = *obj.m_target;
}

// Destructor
Deploy::~Deploy()
{
    delete m_target;
}

// Assignment operator
Deploy& Deploy::operator=(const Deploy& obj)
{
    if (this != &obj) {
        m_owningPlayer = new Player;
        *m_owningPlayer = *obj.m_owningPlayer;

        type = "Deploy";
        m_nbUnits = obj.m_nbUnits;

        m_target = new Territory;
        *m_target = *obj.m_target;
    }

    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& stream, const Deploy& obj)
{
    stream << "Deploy: TODO\n";
    return stream;
}

// Move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories
void Deploy::execute()
{
    if (validate()) {
        std::cout << "Executing: Deploy order." << std::endl;
    }
    else {
        std::cout << "Invalid order." << std::endl;
    }
}

// Validates the order
bool Deploy::validate()
{

}

// ------------------


// ----- Advance -----
// Constructor
Advance::Advance(Player* owningPlayer, int nbUnits, Territory* src, Territory* target)
{
    type = "Advance";
    m_owningPlayer = owningPlayer;
    m_nbUnits = nbUnits;
    m_src = src;
    m_target = target;
}

// Copy constructor
Advance::Advance(const Advance& obj)
{
    m_owningPlayer = new Player;
    *m_owningPlayer = *obj.m_owningPlayer;

    type = "Advance";
    m_nbUnits = obj.m_nbUnits;

    m_src = new Territory;
    *m_src = *obj.m_src;

    m_target = new Territory;
    *m_target = *obj.m_target;
}

// Destructor
Advance::~Advance()
{
    delete m_src;
    delete m_target;
}

// Assignment operator
Advance& Advance::operator= (const Advance& obj)
{
    if (this != &obj) {
        m_owningPlayer = new Player;
        *m_owningPlayer = *obj.m_owningPlayer;

        type = "Advance";
        m_nbUnits = obj.m_nbUnits;

        m_src = new Territory;
        *m_src = *obj.m_src;

        m_target = new Territory;
        *m_target = *obj.m_target;
    }

    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& stream, const Advance& obj)
{
    stream << "Advance: TODO\n";
    return stream;
}

// Move a certain number of army units from one of the current player’s territories to another territory that is adjacent to the source territory
void Advance::execute()
{
    if (validate()) {
        std::cout << "Executing: Advance order." << std::endl;
    }
    else {
        std::cout << "Invalid order." << std::endl;
    }
}

// Validates the order
bool Advance::validate()
{

}
// -------------------


// ----- Bomb -----
// Constructor
Bomb::Bomb(Player* owningPlayer, Territory* target)
{
    type = "Bomb";
    m_owningPlayer = owningPlayer;
    m_target = target;
}

// Copy constructor
Bomb::Bomb(const Bomb& obj)
{
    type = "Bomb";

    m_owningPlayer = new Player;
    *m_owningPlayer = *obj.m_owningPlayer;

    m_target = new Territory;
    *m_target = *obj.m_target;
}

// Destructor
Bomb::~Bomb()
{
    delete m_target;
}

// Assignment operator
Bomb& Bomb::operator= (const Bomb& obj)
{
    if (this != &obj) {
        type = "Bomb";

        m_owningPlayer = new Player;
        *m_owningPlayer = *obj.m_owningPlayer;

        m_target = new Territory;
        *m_target = *obj.m_target;
    }

    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& stream, const Bomb& obj)
{
    stream << "Bomb: TODO\n";
    return stream;
}

// Destroy half of the army units located on an opponent’s territory that is adjacent to one of the current player’s territories
void Bomb::execute()
{
    if (validate()) {
        std::cout << "Executing: Bomb order." << std::endl;
    }
    else {
        std::cout << "Invalid order." << std::endl;
    }
}

// Validates the order
bool Bomb::validate()
{

}
// ----------------


// ----- Blockade -----
// Constructor
Blockade::Blockade(Player* owningPlayer, Territory* target)
{
    type = "Blockade";
    m_owningPlayer = owningPlayer;
    m_target = target;
}

// Copy constructor
Blockade::Blockade(const Blockade& obj)
{
    type = "Blockade";

    m_owningPlayer = new Player;
    *m_owningPlayer = *obj.m_owningPlayer;

    m_target = new Territory;
    *m_target = *obj.m_target;
}

// Destructor
Blockade::~Blockade()
{
    delete m_target;
}

// Assignment operator
Blockade& Blockade::operator= (const Blockade& obj)
{
    if (this != &obj) {
        type = "Blockade";

        m_owningPlayer = new Player;
        *m_owningPlayer = *obj.m_owningPlayer;

        m_target = new Territory;
        *m_target = *obj.m_target;
    }

    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& stream, const Blockade& obj)
{
    stream << "Blockade: TODO\n";
    return stream;
}

// Triple the number of army units on one of the current player’s territories and make it a neutral territory.
void Blockade::execute()
{
    if (validate()) {
        std::cout << "Executing: Blockade order." << std::endl;
    }
    else {
        std::cout << "Invalid order." << std::endl;
    }
}

// Validates the order
bool Blockade::validate()
{

}
// --------------------


// ----- Airlift -----
// Constructor
Airlift::Airlift(Player* owningPlayer, int nbUnits, Territory* src, Territory* target)
{
    type = "Airlift";
    m_owningPlayer = owningPlayer;
    m_nbUnits = nbUnits;
    m_src = src;
    m_target = target;
}

// Copy constructor
Airlift::Airlift(const Airlift& obj)
{
    m_owningPlayer = new Player;
    *m_owningPlayer = *obj.m_owningPlayer;

    type = "Airlift";
    m_nbUnits = obj.m_nbUnits;

    m_src = new Territory;
    *m_src = *obj.m_src;

    m_target = new Territory;
    *m_target = *obj.m_target;
}

// Destructor
Airlift::~Airlift()
{
    delete m_src;
    delete m_target;
}

// Assignment operator
Airlift& Airlift::operator= (const Airlift& obj)
{
    if (this != &obj) {
        m_owningPlayer = new Player;
        *m_owningPlayer = *obj.m_owningPlayer;

        type = "Airlift";
        m_nbUnits = obj.m_nbUnits;

        m_src = new Territory;
        *m_src = *obj.m_src;

        m_target = new Territory;
        *m_target = *obj.m_target;
    }

    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& stream, const Airlift& obj)
{
    stream << "Airlift: TODO\n";
    return stream;
}

// Advance a certain number of army units from one of the current player’s territories to any another territory
void Airlift::execute()
{
    if (validate()) {
        std::cout << "Executing: Airlift order." << std::endl;
    }
    else {
        std::cout << "Invalid order." << std::endl;
    }
}

// Validates the order
bool Airlift::validate()
{

}
// -------------------


// ----- Negotiate -----
// Constructor
Negotiate::Negotiate(Player* owningPlayer, Player* otherPlayer)
{
    type = "Negotiate";
    m_owningPlayer = owningPlayer;
    m_otherPlayer = otherPlayer;
}

// Copy constructor
Negotiate::Negotiate(const Negotiate& obj)
{
    type = "Negotiate";

    m_owningPlayer = new Player;
    *m_owningPlayer = *obj.m_owningPlayer;

    m_otherPlayer = new Player;
    *m_otherPlayer = *obj.m_otherPlayer;
}

// Destructor
Negotiate::~Negotiate()
{
    delete m_otherPlayer;
}

// Assignment operator
Negotiate& Negotiate::operator= (const Negotiate& obj)
{
    if (this != &obj) {
        type = "Negotiate";
        
        m_owningPlayer = new Player;
        *m_owningPlayer = *obj.m_owningPlayer;

        m_otherPlayer = new Player;
        *m_otherPlayer = *obj.m_otherPlayer;
    }
}

// Stream insertion operator
ostream& operator<<(ostream& stream, const Negotiate& obj)
{
    stream << "Negotiate: TODO\n";
    return stream;
}

// Prevent attacks between the current player and the player targeted by the negotiate order until the end of the turn.
void Negotiate::execute()
{
    if (validate()) {
        std::cout << "Executing: Negotiate order." << std::endl;
    }
    else {
        std::cout << "Invalid order." << std::endl;
    }
}

// Validates the order
bool Negotiate::validate()
{

}
// ---------------------


// ----- OrdersList -----
// Copy constructor
OrdersList::OrdersList(const OrdersList& obj)
{
    for (size_t i = 0; i < obj.m_orders.size(); i++)
    {
        m_orders.push_back(obj.m_orders[i]);
    }
}

// Assignment operator
OrdersList& OrdersList::operator= (const OrdersList& obj)
{
    if (this != &obj) {
        for (size_t i = 0; i < obj.m_orders.size(); i++)
        {
            m_orders.push_back(obj.m_orders[i]);
        }
    }

    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& stream, const OrdersList& obj)
{
    stream << "Order list: ";

    for (const auto& order : obj.m_orders)
    {
        stream << order.type << " // ";
    }

    stream << endl;
    return stream;
}

// Add a new order to the list
void OrdersList::add(Order& newOrder)
{
    m_orders.push_back(newOrder);
}

// Move an order to a new position in the list
void OrdersList::move(int fromId, int toId)
{

}

// Delete an order from the list
void OrdersList::remove(int id)
{
    if (id >= m_orders.size() || id < 0)
    {
        std::cout << "Error: unable to remove order.";
    }
    else
    {
        m_orders.erase(m_orders.begin() + id);
    }
}
// ----------------------