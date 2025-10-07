//-----------------------------------------------------
    //Alexandre Godfroy 40300164
    //COMP-345-D 2252 Assignment 1 CardsDriver.cpp
    //Dr. Hamed Jafarpour
    //Submitted: TBD
    //Due: October 7, 2025
    //I certify that this submission is my original work and meets the Faculty’s Expectations of Originality
    //Alexandre Godfroy
//----------------------------------------------------- 

#include <iostream>
#include "Cards.h"

int TestCards() {
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
        hand.AddCard();
    }

    //prints hand
    std::cout << "Hand after drawing cards:" << std::endl;
    hand.Print();

    std::cout << "---------------------------------------------" << std::endl;

    //play 3 cards from hand, selected by user. After each play, print the hand again
    for (int i = 0; i < 3; ++i) {
        std::cout << "Please select a card to play: ";
        std::cin >> cardIndex;
        hand.PlayCard(cardIndex);
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Hand after playing card:" << std::endl;
        hand.Print();
    }

    std::cout << "Card test complete." << std::endl;

    return 0;
}
