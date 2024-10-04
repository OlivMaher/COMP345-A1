#ifndef CARDS_H
#define CARDS_H

//library includes
#include <iostream>
#include <string>
using namespace std;

// early declarations
class Deck;
class Hand;

// Card class
class Card {
public:
    Card();  // default constructor
    Card(const char* type); //parametrized constructor
    Card(Card& a); // copy constructor
    void play(Deck& deck, Hand& hand); //plays a card and returns it to the deck
    const char* getCardType(); // Card type accessor

private:
    const char* CardType; 
};

class Deck {
public:
    Deck();  // default constructor
    Card draw(); //returns and removes a card from the deck
    void returnToDeck(Card& card); // returns a card to the deck
    void showDeck(); // display deck

    int getDeck();  // returs cardsInDeck
    void setDeck(int a); //updates the number of cards in the deck


private:
    Card cards[20]; // array of cards in the deck
    int cardsInDeck; // num of cards in deck
};

class Hand {
public:
    Hand(); 
    void takeCard(Deck& deck); 
    void showHand(); // display hand
    int getHand(); //// returs cardsInHand
    void setHand(int a); //updates the number of cards in the hand
    Card handToDeck(int a, Deck& deck); // returns card a to the deck
    Card copyCard_Hand(int a); // returns a copy of a card from the hand

private:
    Card hand[20]; // array of cards
    int cardsInHand; //num of cards in hand
};



#endif
