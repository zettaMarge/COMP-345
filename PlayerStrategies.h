//-----------------------------------------------------
//Marjorie Dudemaine 40287779, OTHER NAMES HERE
//COMP-345-D 2252 Assignment 3 PlayerStrategies.h
//Dr. Hamed Jafarpour
//Submitted: TBD
//Due: November 26, 2025 
//We certify that this submission is our original work and meets the Faculty’s Expectations of Originality 
//Marjorie Dudemaine, OTHER NAMES HERE
//-----------------------------------------------------

#pragma once
#include "Map.h"
#include <vector>

class Player;

class PlayerStrategies {
    public:
        enum OrderNames {
                AdvanceEnum = 5,
                AirliftEnum = 0,
                BlockadeEnum = 1,
                BombEnum = 2,
                DeployEnum = 3,
                NegotiateEnum = 4
            };
        ~PlayerStrategies(); // Destructor
        virtual std::vector<Territory*> ToAttack() const = 0; //returns list of territories to attack
        virtual std::vector<Territory*> ToDefend() const = 0; //returns list of territories to defend
        virtual void IssueOrder() = 0; //processes which order to make then puts it in its player's list of orders
        void SetPlayer(Player* p);

    protected:
        Player* player;
};

class HumanPlayerStrategy : public PlayerStrategies {
    public:
        std::vector<Territory*> ToAttack() const;
        std::vector<Territory*> ToDefend() const;
        void IssueOrder();
};

class AggressivePlayerStrategy : public PlayerStrategies {
    public:
        std::vector<Territory*> ToAttack() const;
        std::vector<Territory*> ToDefend() const;
        void IssueOrder();
};

class BenevolentPlayerStrategy : public PlayerStrategies {
    public:
        std::vector<Territory*> ToAttack() const;
        std::vector<Territory*> ToDefend() const;
        void IssueOrder();
    private:
        Territory* findWeakestTerritory(const std::vector<Territory*>& territories) const;
        Territory* findStrongestTerritory(const std::vector<Territory*>& territories) const;
};

class NeutralPlayerStrategy : public PlayerStrategies {
    public:
        std::vector<Territory*> ToAttack() const;
        std::vector<Territory*> ToDefend() const;
        void IssueOrder();
};

class CheaterPlayerStrategy : public PlayerStrategies {
    public:
        std::vector<Territory*> ToAttack() const;
        std::vector<Territory*> ToDefend() const;
        void IssueOrder();
};