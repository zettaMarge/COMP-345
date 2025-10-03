#include "Cards.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

//----------------------------------------------------------------
// Implementations of CardTypes and its derived classes
// Base class type implementation
std::string CardTypes::getType() const {
    return type;
}

// BombCard class implementation
// Constructor sets type to "Bomb"
BombCard::BombCard() {
    type = "Bomb";
}

// Copy constructor for BombCard
BombCard::BombCard(const BombCard& other) {
    type = other.type;
}

// Assignment operator for BombCard
BombCard& BombCard::operator=(const BombCard& other) {
    if (this != &other) {
        type = other.type;
    }
    return *this;
}

// Destructor for BombCard
BombCard::~BombCard() {
    // No dynamic memory to free
}

// Print function for BombCard
void BombCard::print() const {
    std::cout << "Card Type: " << type << std::endl;
}

// Implementation of play function for BombCard
void BombCard::play() {
    std::cout << "Playing Bomb Card!" << std::endl;
}

// ReinforcementCard class implementation
// Constructor sets type to "Reinforcement"
ReinforcementCard::ReinforcementCard() {
    type = "Reinforcement";
}

// Copy constructor for ReinforcementCard
ReinforcementCard::ReinforcementCard(const ReinforcementCard& other) {
    type = other.type;
}

// Assignment operator for ReinforcementCard
ReinforcementCard& ReinforcementCard::operator=(const ReinforcementCard& other) {
    if (this != &other) {
        type = other.type;
    }
    return *this;
}

// Destructor for ReinforcementCard
ReinforcementCard::~ReinforcementCard() {
    // No dynamic memory to free
}

// Print function for ReinforcementCard
void ReinforcementCard::print() const {
    std::cout << "Card Type: " << type << std::endl;
}

// Implementation of play function for ReinforcementCard
void ReinforcementCard::play() {
    std::cout << "Playing Reinforcement Card!" << std::endl;
}

// BlockadeCard class implementation
// Constructor sets type to "Blockade"
BlockadeCard::BlockadeCard() {
    type = "Blockade";
}

// Copy constructor for BlockadeCard
BlockadeCard::BlockadeCard(const BlockadeCard& other) {
    type = other.type;
}

// Assignment operator for BlockadeCard
BlockadeCard& BlockadeCard::operator=(const BlockadeCard& other) {
    if (this != &other) {
        type = other.type;
    }
    return *this;
}

// Destructor for BlockadeCard
BlockadeCard::~BlockadeCard() {
    // No dynamic memory to free
}

// Print function for BlockadeCard
void BlockadeCard::print() const {
    std::cout << "Card Type: " << type << std::endl;
}

// Implementation of play function for BlockadeCard
void BlockadeCard::play() {
    std::cout << "Playing Blockade Card!" << std::endl;
}

// AirliftCard class implementation
// Constructor sets type to "Airlift"
AirliftCard::AirliftCard() {
    type = "Airlift";
}

// Copy constructor for AirliftCard
AirliftCard::AirliftCard(const AirliftCard& other) {
    type = other.type;
}

// Assignment operator for AirliftCard
AirliftCard& AirliftCard::operator=(const AirliftCard& other) {
    if (this != &other) {
        type = other.type;
    }
    return *this;
}

// Destructor for AirliftCard
AirliftCard::~AirliftCard() {
    // No dynamic memory to free
}

// Print function for AirliftCard
void AirliftCard::print() const {
    std::cout << "Card Type: " << type << std::endl;
}

// Implementation of play function for AirliftCard
void AirliftCard::play() {
    std::cout << "Playing Airlift Card!" << std::endl;
}

// DiplomacyCard class implementation
// Constructor sets type to "Diplomacy"
DiplomacyCard::DiplomacyCard() {
    type = "Diplomacy";
}

// Copy constructor for DiplomacyCard
DiplomacyCard::DiplomacyCard(const DiplomacyCard& other) {
    type = other.type;
}

// Assignment operator for DiplomacyCard
DiplomacyCard& DiplomacyCard::operator=(const DiplomacyCard& other) {
    if (this != &other) {
        type = other.type;
    }
    return *this;
}

// Destructor for DiplomacyCard
DiplomacyCard::~DiplomacyCard() {
    // No dynamic memory to free
}

// Print function for DiplomacyCard
void DiplomacyCard::print() const {
    std::cout << "Card Type: " << type << std::endl;
}

// Implementation of play function for DiplomacyCard
void DiplomacyCard::play() {
    std::cout << "Playing Diplomacy Card!" << std::endl;
}


//----------------------------------------------------------------
// Card class implementation
// Constructor creates a CardTypes object based on the type passed through the enum
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

// Destructor to clean up dynamically allocated memory
Card::~Card() {
    delete cardType;
}

// Print function to display the type of the card
void Card:: print() const {
    std::cout << "Card Type: " << cardType->getType() << std::endl;
}

// Play function to invoke the play method of the specific CardTypes object
void Card::play() const{
    cardType->play();
}

// Copy constructor for Card
Card::Card(const Card& other) {
    type = other.type;
    // Deep copy of CardTypes object based on the type
    switch (type) {
        case bomb:
            cardType = new BombCard(*(static_cast<BombCard*>(other.cardType)));
            break;
        case reinforcement:
            cardType = new ReinforcementCard(*(static_cast<ReinforcementCard*>(other.cardType)));
            break;
        case blockade:
            cardType = new BlockadeCard(*(static_cast<BlockadeCard*>(other.cardType)));
            break;
        case airlift:
            cardType = new AirliftCard(*(static_cast<AirliftCard*>(other.cardType)));
            break;
        case diplomacy:
            cardType = new DiplomacyCard(*(static_cast<DiplomacyCard*>(other.cardType)));
            break;
    }
}

// Assignment operator for Card
Card& Card::operator=(const Card& other) {
    if (this != &other) {
        delete cardType; // Free existing resource
        type = other.type;
        // Deep copy of CardTypes object based on the type
        switch (type) {
            case bomb:
                cardType = new BombCard(*(static_cast<BombCard*>(other.cardType)));
                break;
            case reinforcement:
                cardType = new ReinforcementCard(*(static_cast<ReinforcementCard*>(other.cardType)));
                break;
            case blockade:
                cardType = new BlockadeCard(*(static_cast<BlockadeCard*>(other.cardType)));
                break;
            case airlift:
                cardType = new AirliftCard(*(static_cast<AirliftCard*>(other.cardType)));
                break;
            case diplomacy:
                cardType = new DiplomacyCard(*(static_cast<DiplomacyCard*>(other.cardType)));
                break;
        }
    }
    return *this;
}

//----------------------------------------------------------------
// Deck class implementation
// Constructor initializes the deck with 30 random cards
Deck::Deck(){
    for (int i = 0; i < initialSize; ++i) {
        int typeIndex = rand() % 5; //5 types of cards
        cards.push_back(Card(typeIndex));
    }
}

// Copy constructor for Deck
Deck::Deck(const Deck& other) : cards(other.cards) {
    // Deep copy of cards vector
    for (size_t i = 0; i < cards.size(); i++) {
        this->cards.push_back(other.cards[i]);
    }
    
}

// Assignment operator for Deck
Deck& Deck::operator=(const Deck& other) {
    if (this != &other) {
        cards = other.cards; // Deep copy of cards vector
    }
    return *this;
}

// Destructor for Deck
Deck::~Deck() {
    // No dynamic memory to free since vector manages its own memory
}

// Print function to display the contents of the deck
void Deck::print() const {
    std::cout << "Deck contains " << cards.size() << " cards." << std::endl;
    for (const auto& card : cards) {
        card.print();
    }
}

// Card draw function to draw a random card from the deck
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

// Function to add a card to the deck
void Deck::addCard(const Card& card){
    cards.push_back(card);
}


//----------------------------------------------------------------
// Hand class implementation
// Constructor associates the hand with a deck
Hand::Hand (Deck& deck) : deck(deck) {}

// Copy constructor for Hand
Hand::Hand(const Hand& other) : hand(other.hand), deck(other.deck) {
    // Deep copy of hand vector
}

// Assignment operator for Hand
Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        hand = other.hand; // Deep copy of hand vector
        // Note: deck reference is not copied as it should refer to the same deck
    }
    return *this;
}

// Destructor for Hand
Hand::~Hand() {
    // No dynamic memory to free since vector manages its own memory
}

// Function to add a card to the hand by drawing from the deck
void Hand::addCard() {
    hand.push_back(deck.draw());
}

// Function to print the contents of the hand and its indexes
void Hand::print() const {
    std::cout << "Hand contains " << hand.size() << " cards." << std::endl;
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << i << "- ";
        hand[i].print();
    }
}

// Function to play a card from the hand by index in hand
void Hand::playCard(int index) {
    //verifies if the index is valid
    if (index < 0 || index >= hand.size()) throw std::out_of_range("Invalid card index");
    
    //plays the card, removes it from hand, and adds it back to the deck
    hand[index].play();
    Card card = hand[index];
    hand.erase(hand.begin() + index);
    deck.addCard(card);
}
