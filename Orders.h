#pragma once

//placeholders
class Territory{};
class Player{};

// Abstract Order class - cannot be instantiated
class Order {
    public:
        // Validates the order
        virtual bool validate() = 0;

        // Executes the order
        virtual void execute() = 0;

        // Destructor
        virtual ~Order() = default;
};

// Deploy order
class Deploy : public Order {
    public:
        // Constructor
        Deploy(int nbUnits, Territory* target);

        // Validates the order
        bool validate();

        // Move a certain number of army units from the current player’s reinforcement pool to one of the current player’s territories
        void execute();

    private:   
        int m_nbUnits;
        Territory* m_target;
};

// Advance order
class Advance : public Order {
    public:
        // Constructor
        Advance(int nbUnits, Territory* src, Territory* target);

        // Validates the order
        bool validate();

        // Move a certain number of army units from one of the current player’s territories to another territory that is adjacent to the source territory
        void execute();
    
    private:   
        int m_nbUnits;
        Territory* m_src;
        Territory* m_target;

};

// Bomb order
class Bomb : public Order {
    public:
        // Constructor
        Bomb(Territory* target);

        // Validates the order
        bool validate();

        // Destroy half of the army units located on an opponent’s territory that is adjacent to one of the current player’s territories
        void execute();

    private:
        Territory* m_target;
};

// Blockade order
class Blockade : public Order {
    public:
        // Constructor
        Blockade(Territory* target);

        // Validates the order
        bool validate();

        // Triple the number of army units on one of the current player’s territories and make it a neutral territory.
        void execute();

    private:
        Territory* m_target;
};

// Airlift order
class Airlift : public Order {
    public:
        // Constructor
        Airlift(int nbUnits, Territory* src, Territory* target);

        // Validates the order
        bool validate();

        // Advance a certain number of army units from one of the current player’s territories to any another territory
        void execute();
    
    private:   
        int m_nbUnits;
        Territory* m_src;
        Territory* m_target;
};

// Negotiate order
class Negotiate : public Order {
    public:
        // Constructor
        Negotiate(Player* otherPlayer);

        // Validates the order
        bool validate();

        // Prevent attacks between the current player and the player targeted by the negotiate order until the end of the turn.
        void execute();

    private:
        Player* m_otherPlayer;
};

// A player's list of orders
class OrdersList{
    public:
};