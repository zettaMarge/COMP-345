//-----------------------------------------------------
    //Alexandre Godfroy 40300164
    //COMP-345-D 2252 Assignment 1 Cards.h
    //Dr. Hamed Jafarpour
    //Submitted: TBD
    //Due: October 7, 2025
    //I certify that this submission is my original work and meets the Faculty’s Expectations of Originality
    //Alexandre Godfroy
//----------------------------------------------------- 

#pragma once
#include <string>
#include <vector>
#include <iostream>

// Abstract base class for all card types. The type is changed in each derived class constructor
// and the play() function is overridden to provide specific functionality for each card type.
class CardTypes {
public:
    std::string getType() const;
    virtual void Play() = 0;
protected:
    std::string type;
};

// Class for Bomb type cards, derived from CardTypes
class BombCard : public CardTypes {
public:
    BombCard();
    BombCard(const BombCard& other);
    BombCard& operator=(const BombCard& other);
    ~BombCard();

    void Print() const;
    void Play() override;
};

// Class for Reinforcement type cards, derived from CardTypes
class ReinforcementCard : public CardTypes {
public:
    ReinforcementCard();
    ReinforcementCard(const ReinforcementCard& other);
    ReinforcementCard& operator=(const ReinforcementCard& other);
    ~ReinforcementCard();

    void Print() const;
    void Play() override;
};

// Class for Blockade type cards, derived from CardTypes
class BlockadeCard : public CardTypes {
public:
    BlockadeCard();
    BlockadeCard(const BlockadeCard& other);
    BlockadeCard& operator=(const BlockadeCard& other);
    ~BlockadeCard();

    void Print() const;
    void Play() override;
};

// Class for Airlift type cards, derived from CardTypes
class AirliftCard : public CardTypes {
public:
    AirliftCard();
    AirliftCard(const AirliftCard& other);
    AirliftCard& operator=(const AirliftCard& other);
    ~AirliftCard();

    void Print() const;
    void Play() override;
};

// Class for Diplomacy type cards, derived from CardTypes
class DiplomacyCard : public CardTypes {
public:
    DiplomacyCard();
    DiplomacyCard(const DiplomacyCard& other);
    DiplomacyCard& operator=(const DiplomacyCard& other);
    ~DiplomacyCard();

    void Print() const;
    void Play() override;
};

// Card class that contains a pointer to a CardTypes object. The specific type of card is determined
// by the integer (enum) passed to the constructor. The play() and print() functions call the
// corresponding functions of the CardTypes object.
class Card {
public:
    Card(const Card& other); 
    Card& operator=(const Card& other); 
    ~Card();

    // main constructor
    Card(int type);

    void Play() const;
    void Print() const;
private:
    enum typeNames { bomb, reinforcement, blockade, airlift, diplomacy };
    typeNames type;
    CardTypes* cardType;
};

// Deck class that contains a vector of Card objects. It has functions to draw a card from the deck,
// add a card to the deck, and print the contents of the deck. The deck is initialized with 30 random cards.
class Deck {
public:
    Deck(const Deck& other);
    Deck& operator=(const Deck& other);
    ~Deck();
    void Print() const;

    Deck();
    Card Draw();
    void AddCard(const Card& card);
private:
    std::vector<Card> cards;
    const int initialSize = 30;
};

// Hand class that contains a vector of Card objects. It has functions to add a card to the hand
// and print the contents of the hand. It also has a function to play a card from the hand, which
// removes the card from the hand and calls its play() function. The Hand class is associated
// with a Deck object, from which it draws cards.
class Hand {
public:
    Hand(const Hand& other);
    Hand& operator=(const Hand& other);
    ~Hand();
    void Print() const;

    Hand(Deck& deck);
    void AddCard();
    void PlayCard(int index);
    std::vector<Card*> GetCards();
private:
    std::vector<Card*> hand;
    Deck& deck;
};
