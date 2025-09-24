#include "Cards.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>


std::string CardTypes::getType() const {
    return type;
}

BombCard::BombCard() {
    type = "Bomb";
}

void BombCard::play() {
    std::cout << "Playing Bomb Card!" << std::endl;
}

ReinforcementCard::ReinforcementCard() {
    type = "Reinforcement";
}

void ReinforcementCard::play() {
    std::cout << "Playing Reinforcement Card!" << std::endl;
}

BlockadeCard::BlockadeCard() {
    type = "Blockade";
}

void BlockadeCard::play() {
    std::cout << "Playing Blockade Card!" << std::endl;
}
AirliftCard::AirliftCard() {
    type = "Airlift";
}

void AirliftCard::play() {
    std::cout << "Playing Airlift Card!" << std::endl;
}

DiplomacyCard::DiplomacyCard() {
    type = "Diplomacy";
}

void DiplomacyCard::play() {
    std::cout << "Playing Diplomacy Card!" << std::endl;
}

Card::Card(int type){
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

void Card:: print() const {
    std::cout << "Card Type: " << cardType->getType() << std::endl;
}

void Card::play() const{
    cardType->play();
}

Deck::Deck(){
    //initialize deck with 30 random cards
    for (int i = 0; i < initialSize; ++i) {
        int typeIndex = rand() % 5; //5 types of cards
        cards.push_back(Card(typeIndex));
    }
}

Card Deck::draw(){
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

void Deck::addCard(const Card& card){
    cards.push_back(card);
}

Hand::Hand (Deck& deck) : deck(deck) {}

void Hand::addCard() {
    hand.push_back(deck.draw());
}

void Hand::print() const {
    std::cout << "Hand contains " << hand.size() << " cards." << std::endl;
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << i << "- ";
        hand[i].print();
    }
}

void Hand::playCard(int index) {
    if (index < 0 || index >= hand.size()) throw std::out_of_range("Invalid card index");
    hand[index].play();
    Card card = hand[index];
    hand.erase(hand.begin() + index);

}
