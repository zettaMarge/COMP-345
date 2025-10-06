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
        OrdersList();
        OrdersList(const OrdersList& obj);
        ~OrdersList();

        OrdersList& operator= (const OrdersList& obj);
        friend ostream& operator<<(ostream& stream, const OrdersList& obj);

        vector<Order*> GetListItems();
        void Add(Order* newOrder);
        void Move(int fromId, int toId);
        void Remove(int id);

    private:
        vector<Order*> orders;
};


// Abstract Order class - cannot be instantiated
class Order {
    public:
        ~Order();

        friend ostream& operator<<(ostream& stream, const Order& obj);

        void SetOwningPlayer(Player* player);
        Player* GetOwningPlayer();

        virtual ostream& Print(ostream& stream) const = 0;
        virtual std::string GetEffect() const = 0;
        virtual void Execute() = 0;

    protected:
        Player* owningPlayer;

        bool TerritoryBelongsToPlayer(Territory* territory);

    private:
        virtual bool Validate() = 0;
};


// Advance order
class Advance : public Order {
    public:
        Advance();
        Advance(Player* owningPlayer, int nbUnits, Territory* src, Territory* target);
        Advance(const Advance& obj);
        ~Advance();

        Advance& operator= (const Advance& obj);

        void SetNbUnits(int nb);
        int GetNbUnits();
        void SetSrc(Territory* territory);
        Territory* GetSrc();
        void SetTarget(Territory* Target);
        Territory* GetTarget();

        ostream& Print(ostream& stream) const;
        std::string GetEffect() const;
        void Execute();
    
    private:   
        int nbUnits;
        Territory* src;
        Territory* target;

        bool Validate();
};


// Airlift order
class Airlift : public Order {
    public:
        Airlift();
        Airlift(Player* owningPlayer, int nbUnits, Territory* src, Territory* target);
        Airlift(const Airlift& obj);
        ~Airlift();

        Airlift& operator= (const Airlift& obj);

        void SetNbUnits(int nb);
        int GetNbUnits();
        void SetSrc(Territory* territory);
        Territory* GetSrc();
        void SetTarget(Territory* Target);
        Territory* GetTarget();

        ostream& Print(ostream& stream) const;
        std::string GetEffect() const;
        void Execute();
    
    private:   
        int nbUnits;
        Territory* src;
        Territory* target;

        bool Validate();
};


// Blockade order
class Blockade : public Order {
    public:
        Blockade();
        Blockade(Player* owningPlayer, Territory* target);
        Blockade(const Blockade& obj);
        ~Blockade();

        Blockade& operator= (const Blockade& obj);

        void SetTarget(Territory* Target);
        Territory* GetTarget();

        ostream& Print(ostream& stream) const;
        std::string GetEffect() const;
        void Execute();

    private:
        Territory* target;

        bool Validate();
};


// Bomb order
class Bomb : public Order {
    public:
        Bomb();
        Bomb(Player* owningPlayer, Territory* target);
        Bomb(const Bomb& obj);
        ~Bomb();

        Bomb& operator= (const Bomb& obj);

        void SetTarget(Territory* Target);
        Territory* GetTarget();

        ostream& Print(ostream& stream) const;
        std::string GetEffect() const;
        void Execute();

    private:
        Territory* target;

        bool Validate();
};


// Deploy order
class Deploy : public Order {
    public:
        Deploy();
        Deploy(Player* owningPlayer, int nbUnits, Territory* target);
        Deploy(const Deploy& obj);
        ~Deploy();

        Deploy& operator= (const Deploy& obj);

        void SetNbUnits(int nb);
        int GetNbUnits();
        void SetTarget(Territory* Target);
        Territory* GetTarget();

        ostream& Print(ostream& stream) const;
        std::string GetEffect() const;
        void Execute();

    private:   
        int nbUnits;
        Territory* target;

        bool Validate();
};


// Negotiate order
class Negotiate : public Order {
    public:
        Negotiate();
        Negotiate(Player* owningPlayer, Player* otherPlayer);
        Negotiate(const Negotiate& obj);
        ~Negotiate();

        Negotiate& operator= (const Negotiate& obj);

        void SetOtherPlayer(Player* player);
        Player* GetOtherPlayer();

        ostream& Print(ostream& stream) const;
        std::string GetEffect() const;
        void Execute();

    private:
        Player* otherPlayer;

        bool Validate();
};