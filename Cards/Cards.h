#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <string>
#include <vector>

class Deck;
class Hand;
class Player;
class OrdersList;


class Card {
public:
    Card();  // default constructor
    Card(const std::string& type); // parameterized constructor
    Card(const Card& a); // copy constructor
    Card& operator=(const Card& other); // assignment operator

    void play(Deck& deck, Hand& hand); // plays a card and returns it to the deck
    void play(Deck& deck, Hand& hand, Player* player, OrdersList* ordersList); // AI-friendly method
    std::string getCardType() const; // Card type accessor


private:
    std::string CardType;
};

class Deck {
public:
    Deck();  // default constructor
    Card draw(); // returns and removes a card from the deck
    void returnToDeck(const Card& card); // returns a card to the deck
    void showDeck(); // display deck

    int getDeck() const;  // returns cardsInDeck
    void setDeck(int a); // updates the number of cards in the deck
    void returnCard(Card& card);

private:
    Card cards[20]; // array of cards in the deck
    int cardsInDeck; // number of cards in deck
};

class Hand {
public:
    Hand();
    void takeCard(Deck& deck);
    void showHand(); // display hand
    int getHand() const; // returns cardsInHand
    void setHand(int a); // updates the number of cards in the hand
    Card handToDeck(int a, Deck& deck); // returns card a to the deck
    Card copyCard_Hand(int a); // returns a copy of a card from the hand
    void removeCard(Card& card);

    // Copy constructor
    Hand(const Hand& other);

    // Assignment operator
    Hand& operator=(const Hand& other);

private:
    Card hand[20]; // array of cards
    int cardsInHand; // number of cards in hand
};

#endif
