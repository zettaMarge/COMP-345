//-----------------------------------------------------
    //Marjorie Dudemaine 40287779
    //COMP-345-D 2252 Assignment 1 Orders.h
    //Dr. Hamed Jafarpour
    //Submitted: TBD
    //Due: October 7, 2025 
    //I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
    //Marjorie Dudemaine
//----------------------------------------------------- 

#pragma once

#include <list>
#include <iostream>
#include <vector>
#include <string>

using std::ostream;
using std::vector;

// Forward declarations
class Territory{};
class Player{};

// A player's list of orders
class OrdersList{
    public:
        // Constructor
        OrdersList();

        // Copy constructor
        OrdersList(const OrdersList& obj);

        // Destructor
        ~OrdersList();

        // Assignment operator
        OrdersList& operator= (const OrdersList& obj);

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const OrdersList& obj);

        // Add a new order to the list
        void Add(Order* newOrder);

        // Move an order to a new position in the list
        void Move(int fromId, int toId);

        // Delete an order from the list
        void Remove(int id);

    private:
        vector<Order*> orders;
};


// Abstract Order class - cannot be instantiated
class Order {
    public:
        // Destructor
        ~Order();

        // Stream insertion operator
        friend ostream& operator<<(ostream& stream, const Order& obj);

        // Prints the order to the specified stream
        virtual ostream& Print(ostream& stream) const = 0;

        // Returns the effect of the order as a string
        virtual std::string GetEffect() const = 0;

        // Executes the order
        virtual void Execute() = 0;

    protected:
        Player* owningPlayer;

    private:
        // Validates the order
        virtual bool Validate() = 0;
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

        // Prints the order to the specified stream
        ostream& Print(ostream& stream) const;

        // Returns the effect of the order as a string
        std::string GetEffect() const;

        // Move a certain number of army units from one of the current player’s territories to another territory that is adjacent to the source territory
        void Execute();
    
    private:   
        int nbUnits;
        Territory* src;
        Territory* target;

        // Validates the order
        bool Validate();
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

        // Prints the order to the specified stream
        ostream& Print(ostream& stream) const;

        // Returns the effect of the order as a string
        std::string GetEffect() const;

        // Advance a certain number of army units from one of the current player’s territories to any another territory
        void Execute();
    
    private:   
        int nbUnits;
        Territory* src;
        Territory* target;

        // Validates the order
        bool Validate();
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

        // Prints the order to the specified stream
        ostream& Print(ostream& stream) const;

        // Returns the effect of the order as a string
        std::string GetEffect() const;

        // Triple the number of army units on one of the current player’s territories and make it a neutral territory.
        void Execute();

    private:
        Territory* target;

        // Validates the order
        bool Validate();
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

        // Prints the order to the specified stream
        ostream& Print(ostream& stream) const;

        // Returns the effect of the order as a string
        std::string GetEffect() const;

        // Destroy half of the army units located on an opponent’s territory that is adjacent to one of the current player’s territories
        void Execute();

    private:
        Territory* target;

        // Validates the order
        bool Validate();
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

        // Prints the order to the specified stream
        ostream& Print(ostream& stream) const;

        // Returns the effect of the order as a string
        std::string GetEffect() const;

        // Move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories
        void Execute();

    private:   
        int nbUnits;
        Territory* target;

        // Validates the order
        bool Validate();
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

        // Prints the order to the specified stream
        ostream& Print(ostream& stream) const;

        // Returns the effect of the order as a string
        std::string GetEffect() const;

        // Prevent attacks between the current player and the player targeted by the negotiate order until the end of the turn.
        void Execute();

    private:
        Player* otherPlayer;

        // Validates the order
        bool Validate();
};