#pragma once

#include <list>
#include <iostream>
#include <vector>
using namespace std;

//placeholders
class Territory{};
class Player{};

// Abstract Order class - cannot be instantiated
class Order {
    public:
        std::string type;

        // Destructor
        ~Order();

        // Executes the order
        virtual void execute() = 0;

    protected:
        Player* m_owningPlayer;

    private:
        // Validates the order
        virtual bool validate() = 0;
};

// Deploy order
class Deploy : public Order {
    public:
        // Constructor
        Deploy(Player* owningPlayer, int nbUnits, Territory* target);

        // Copy constructor
        Deploy(const Deploy& obj);

        // Destructor
        ~Deploy();

        // Assignment operator
        Deploy& operator= (const Deploy& obj);

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const Deploy& obj);

        // Move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories
        void execute();

    private:   
        int m_nbUnits;
        Territory* m_target;

        // Validates the order
        bool validate();
};

// Advance order
class Advance : public Order {
    public:
        // Constructor
        Advance(Player* owningPlayer, int nbUnits, Territory* src, Territory* target);

        //Copy constructor
        Advance(const Advance& obj);

        // Destructor
        ~Advance();

        // Assignment operator
        Advance& operator= (const Advance& obj);

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const Advance& obj);

        // Move a certain number of army units from one of the current player’s territories to another territory that is adjacent to the source territory
        void execute();
    
    private:   
        int m_nbUnits;
        Territory* m_src;
        Territory* m_target;

        // Validates the order
        bool validate();

};

// Bomb order
class Bomb : public Order {
    public:
        // Constructor
        Bomb(Player* owningPlayer, Territory* target);

        //Copy construcotr
        Bomb(const Bomb& obj);

        // Destructor
        ~Bomb();

        // Assignment operator
        Bomb& operator= (const Bomb& obj);

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const Bomb& obj);

        // Destroy half of the army units located on an opponent’s territory that is adjacent to one of the current player’s territories
        void execute();

    private:
        Territory* m_target;

        // Validates the order
        bool validate();
};

// Blockade order
class Blockade : public Order {
    public:
        // Constructor
        Blockade(Player* owningPlayer, Territory* target);

        // Copy constructor
        Blockade(const Blockade& obj);

        // Destructor
        ~Blockade();

        // Assignment operator
        Blockade& operator= (const Blockade& obj);

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const Blockade& obj);

        // Triple the number of army units on one of the current player’s territories and make it a neutral territory.
        void execute();

    private:
        Territory* m_target;

        // Validates the order
        bool validate();
};

// Airlift order
class Airlift : public Order {
    public:
        // Constructor
        Airlift(Player* owningPlayer, int nbUnits, Territory* src, Territory* target);

        //Copy construcotr
        Airlift(const Airlift& obj);

        // Destructor
        ~Airlift();

        // Assignment operator
        Airlift& operator= (const Airlift& obj);

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const Airlift& obj);

        // Advance a certain number of army units from one of the current player’s territories to any another territory
        void execute();
    
    private:   
        int m_nbUnits;
        Territory* m_src;
        Territory* m_target;

        // Validates the order
        bool validate();
};

// Negotiate order
class Negotiate : public Order {
    public:
        // Constructor
        Negotiate(Player* owningPlayer, Player* otherPlayer);

        // Copy constructor
        Negotiate(const Negotiate& obj);

        // Destructor
        ~Negotiate();

        // Assignment operator
        Negotiate& operator= (const Negotiate& obj);

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const Negotiate& obj);

        // Prevent attacks between the current player and the player targeted by the negotiate order until the end of the turn.
        void execute();

    private:
        Player* m_otherPlayer;

        // Validates the order
        bool validate();
};

// A player's list of orders
class OrdersList{
    public:
        // Copy constructor
        OrdersList(const OrdersList& obj);

        // Destructor
        ~OrdersList() = default;

        // Assignment operator
        OrdersList& operator= (const OrdersList& obj);

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const OrdersList& obj);

        // Add a new order to the list
        void add(Order& newOrder);

        // Move an order to a new position in the list
        void move(int fromId, int toId);

        // Delete an order from the list
        void remove(int id);

    private:
        vector<Order> m_orders;
};