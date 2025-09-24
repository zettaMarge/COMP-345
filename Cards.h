#pragma once
#include <string>
#include <vector>
#include <iostream>

class CardTypes {
public:
    std::string getType() const;
    virtual void play() = 0;
protected:
    std::string type;
};

class BombCard : public CardTypes {
public:
    BombCard();
    void play() override;
};

class ReinforcementCard : public CardTypes {
public:
    ReinforcementCard();
    void play() override;
};

class BlockadeCard : public CardTypes {
public:
    BlockadeCard();
    void play() override;
};

class AirliftCard : public CardTypes {
public:
    AirliftCard();
    void play() override;
};

class DiplomacyCard : public CardTypes {
public:
    DiplomacyCard();
    void play() override;
};

class Card {
public:
    Card(int type);
    void play() const;
    void print() const;
private:
    enum typeNames { bomb, reinforcement, blockade, airlift, diplomacy };
    typeNames type;
    CardTypes* cardType;
};

class Deck {
public:
    Deck();
    Card draw();
    void addCard(const Card& card);
    void print() const;
private:
    std::vector<Card> cards;
    const int initialSize = 30;
};

class Hand {
public:
    Hand(Deck& deck);
    void addCard();
    void print() const;
    void playCard(int index);
private:
    std::vector<Card> hand;
    Deck& deck;
};
