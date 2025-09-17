#include <iostream>
#include "Cards.h"

int main() {
    int cardIndex;
    std::cout << "CardsDriver started." << std::endl;
    // Your code here

    std::cout << "Creating a deck..." << std::endl;
    Deck deck = Deck();
    std::cout << "Deck created." << std::endl;
    std::cout << "Creating a hand and drawing cards..." << std::endl;

    Hand hand(deck);
    for (int i = 0; i < 5; ++i) {
        hand.addCard();
    }
    std::cout << "Hand after drawing cards:" << std::endl;
    hand.print();

    std::cout << "---------------------------------------------" << std::endl;

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
