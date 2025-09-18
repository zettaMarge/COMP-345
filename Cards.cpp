#include "Cards.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

class CardTypes {
public:
    virtual ~CardTypes() {}
    std::string getType() {
        return type;
    };
    virtual void play() const = 0;
protected:
    std::string type;
};

class BombCard : public CardTypes {
public:
    BombCard() {
        type = "Bomb";
    }
    void play() const override {
        std::cout << "Playing Bomb Card!" << std::endl;
    }
};

class ReinforcementCard : public CardTypes {
public:
    ReinforcementCard() {
        type = "Reinforcement";
    }
    void play() const override {
        std::cout << "Playing Reinforcement Card!" << std::endl;
    }
};

class BlockadeCard : public CardTypes {
public:
    BlockadeCard() {
        type = "Blockade";
    }
    void play() const override {
        std::cout << "Playing Blockade Card!" << std::endl;
    }
};

class AirliftCard : public CardTypes {
public:
    AirliftCard() {
        type = "Airlift";
    }
    void play() const override {
        std::cout << "Playing Airlift Card!" << std::endl;
    }
};

class DiplomacyCard : public CardTypes {
public:
    DiplomacyCard() {
        type = "Diplomacy";
    }
    void play() const override {
        std::cout << "Playing Diplomacy Card!" << std::endl;
    }
};

class Card {
    public:

        Card(int type) {
            if (type < 0 || type > 4) throw std::invalid_argument("Invalid card type");
            this->type = static_cast<typeNames>(type);

            switch (this->type) {
                case bomb:
                    cardType = new BombCard();
                    break;
                case reinforcement:
                    cardType = new ReinforcementCard();
                    break;
                case blockade:
                    cardType = new BlockadeCard();
                    break;
                case airlift:
                    cardType = new AirliftCard();
                    break;
                case diplomacy:
                    cardType = new DiplomacyCard();
                    break;
            }
        }
        void print() const {
            std::cout << "Card Type: " << cardType->getType() << std::endl;
        }
    
        void play() const {
            cardType->play();
        }
    private:
        enum typeNames { bomb, reinforcement, blockade, airlift, diplomacy };
        typeNames type;
        CardTypes* cardType;
};

class Deck {
public:
    Deck() {
        //initialize deck with 30 random cards
        for (int i = 0; i < initialSize; ++i) {
            int typeIndex = rand() % 5; //5 types of cards
            cards.push_back(Card(typeIndex));
        }
    }

    Card draw() {
        //verify deck not empty
        if (cards.empty()) throw std::out_of_range("Deck is empty");
        
        //find random card in deck
        int cardIndex = rand() % cards.size();

        //swap with last card
        Card temp = cards[cardIndex];
        cards[cardIndex] = cards[cards.size() - 1];
        cards[cards.size() - 1] = temp;
    
        //pop last card
        Card c = cards.back();
        cards.pop_back();
        return c;
    }

    void addCard(const Card& card) {
        cards.push_back(card);
    }
private:
    std::vector<Card> cards;
    const int initialSize = 30;
};

class Hand {
public:

    //if this doesnt work, try this->deck = deck; and remove reference from declaration
    Hand(Deck& deck) : deck(deck) {}

    void addCard() {
        hand.push_back(deck.draw());
    }

    void print() const {
        std::cout << "Hand contains " << hand.size() << " cards." << std::endl;
        for (size_t i = 0; i < hand.size(); ++i) {
            std::cout << i << "- ";
            hand[i].print();
        }
    }

    void playCard(int index) {
        if (index < 0 || index >= hand.size()) throw std::out_of_range("Invalid card index");
        hand[index].play();
        Card card = hand[index];
        hand.erase(hand.begin() + index);

    }
private:
    std::vector<Card> hand;
    Deck& deck;
};
