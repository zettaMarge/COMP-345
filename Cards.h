#pragma once
#include <string>
#include <vector>
#include <iostream>

class CardTypes {
public:
    virtual ~CardTypes();
    virtual std::string getType() const = 0;
    virtual void play() const = 0;
protected:
    std::string type;
};

class BombCard : public CardTypes {
public:
    BombCard();
    std::string getType() const override;
    void play() const override;
};

class ReinforcementCard : public CardTypes {
public:
    ReinforcementCard();
    std::string getType() const override;
    void play() const override;
};

class BlockadeCard : public CardTypes {
public:
    BlockadeCard();
    std::string getType() const override;
    void play() const override;
};

class AirliftCard : public CardTypes {
public:
    AirliftCard();
    std::string getType() const override;
    void play() const override;
};

class DiplomacyCard : public CardTypes {
public:
    DiplomacyCard();
    std::string getType() const override;
    void play() const override;
};

class Card {
public:
    Card(int type);
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
