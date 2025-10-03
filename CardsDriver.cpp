#include <iostream>
#include "Cards.h"
#include "Cards.cpp"

int main() {
    // index of card to play
    int cardIndex;

    //startup message
    std::cout << "CardsDriver started." << std::endl;

    //create deck
    std::cout << "Creating a deck..." << std::endl;
    Deck deck = Deck();
    std::cout << "Deck created." << std::endl;

    //create hand from deck and draw 5 cards
    std::cout << "Creating a hand and drawing cards..." << std::endl;
    Hand hand(deck);
    for (int i = 0; i < 5; ++i) {
        hand.addCard();
    }

    //prints hand
    std::cout << "Hand after drawing cards:" << std::endl;
    hand.print();

    std::cout << "---------------------------------------------" << std::endl;

    //play 3 cards from hand, selected by user. After each play, print the hand again
    for (int i = 0; i < 3; ++i) {
        std::cout << "Please select a card to play: ";
        std::cin >> cardIndex;
        hand.playCard(cardIndex);
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Hand after playing card:" << std::endl;
        hand.print();
    }

    std::cout << "Card test complete." << std::endl;

    return 0;
}
