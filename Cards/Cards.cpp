//library includes
#include "Cards.h"
#include <iostream>
#include <string>
#include <cstdlib> 
using namespace std;
// Default constructor
Card::Card() : CardType("Unknown") {}

// Parameterized constructor
Card::Card(const std::string& type) : CardType(type) {}

// Copy constructor
Card::Card(const Card& a) : CardType(a.CardType) {}

// Assignment operator
Card& Card::operator=(const Card& other) {
    if (this != &other) {
        CardType = other.CardType;
    }
    return *this;
}

std::string Card::getCardType() const {
    return CardType;
}
void Card::play(Deck& deck, Hand& hand) { //uses a card from a hand, returns it to deck
    bool useCard = true;


    while (useCard){ // loops until turn is over
        
        if (hand.getHand() <=0){ // case if hand is empty
        cout << "No Cards to play.\n\n";
        useCard = false;
        }
        else{
            cout << "Would you like to play a card? (Y/N) \n";
            char answer ;
            cin >> answer;

            if (answer != 'Y'){
                cout << "No card played.\n\n";
                useCard = false; // ends function
            }

            if (answer == 'Y'){
                useCard = true;

                hand.showHand();
                cout << "Which card would you like to use?\n";
                int choice; // which card should be played
                cin >> choice;
                if (choice < 0 || choice > hand.getHand()){ // checks for valid card number
                    cout << "Invalid Card.\n\n";
                }
                else{ // if valid card number is given
                    Card playing = hand.handToDeck(choice-1,deck); // returns card to deck

                    const char* type= playing.getCardType().c_str();

                    if (type == "Bomb"){ 
                        cout << "Playing Bomb Card\n";
                    }
                    if (type == "Reinforcement"){
                        cout << "Playing Reinforcement Card\n";
                    }             
                    if (type == "Blockade"){
                        cout << "Playing Blockade Card\n";
                    }
                    if (type == "Airlift"){
                        cout << "Playing Airlift Card\n";
                    }
                    if (type == "Diplomacy"){
                        cout << "Playing Diplomacy Card\n";
                    }
                    cout <<"\n";
                }

            }
        }   
    }
} 


Deck::Deck() {
    cardsInDeck = 20;
    std::string types[5] = {"Bomb", "Reinforcement", "Blockade", "Airlift", "Diplomacy"};
    int count = 0;
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            cards[count++] = Card(types[i]);
        }
    }
}

Card Deck::draw() {
    if (getDeck() <= 0) {
        cout << "No cards in deck.\n" << "\n";
        return Card();
    } else {
        srand(time(0)); // Use current time as seed for random generator
        int take = rand() % getDeck();
        Card toRemove = cards[take];

        // Move the last card to the position of the removed card
        cards[take] = cards[getDeck() - 1];

        setDeck(getDeck() - 1); // Decrease the size of the deck

        return toRemove;
    }
}

void Deck::returnToDeck(const Card& card) {
    if (getDeck() < 20) {
        cards[getDeck()] = card;
        setDeck(getDeck() + 1);
    } else {
        cout << "Deck is full, cannot return card.\n";
    }
}

void Deck::showDeck() {
    if (getDeck() == 0){
        cout << "No cards in Deck.\n";
    } else {
        cout << "Deck List\n";
        cout << "---------------------\n";
        for (int i = 0; i < getDeck(); i++){
            cout << (i + 1) << ".  " << cards[i].getCardType() << "\n";
        }
        cout << "\n";
    }
}

int Deck::getDeck() const {
    return cardsInDeck;
}

void Deck::setDeck(int a) {
    cardsInDeck = a;
}

Hand::Hand() {
    cardsInHand = 0;
}

void Hand::takeCard(Deck& deck){
    if (getHand() >= 20){
        cout << "All cards in hand.\n\n";
    }
    else if (deck.getDeck() <= 0) {
        cout << "No cards available in deck.\n\n";
    }
    else{
        Card take = deck.draw(); // draws a card from deck
        hand[getHand()] = take; // adds the card to the hand
        setHand(getHand() + 1); // adjusts the size of the hand
    }
}

void Hand::showHand() {
    if (getHand() == 0){
        cout << "No cards in Hand.\n\n";
    } else {
        cout << "Current Hand\n";
        cout << "---------------------\n";
        for (int i = 0; i < getHand(); i++){
            cout << (i + 1) << ".  " << hand[i].getCardType() << "\n";
        }
        cout << "\n";
    }
}

int Hand::getHand() const {
    return cardsInHand;
}

void Hand::setHand(int a) {
    cardsInHand = a;
}

Card Hand::handToDeck(int a, Deck& deck) {
    if (a < 0 || a >= getHand()){
        cout << "Invalid card number.\n\n";
        return Card();
    } else {
        int index = a;
        Card toRemove = hand[index];

        // Move the last card to the position of the removed card
        hand[index] = hand[getHand() - 1];

        setHand(getHand() - 1); // adjusts the number of cards
        deck.returnToDeck(toRemove); // returns card to deck

        return toRemove;
    }
}

Card Hand::copyCard_Hand(int a) {
    if (a < 0 || a >= getHand()){
        cout << "Nothing to return\n\n";
        return Card();
    } else {
        return hand[a];
    }
}

// Copy constructor for Hand
Hand::Hand(const Hand& other) {
    cardsInHand = other.cardsInHand;
    for (int i = 0; i < cardsInHand; i++) {
        hand[i] = other.hand[i];
    }
}

// Assignment operator for Hand
Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        cardsInHand = other.cardsInHand;
        for (int i = 0; i < cardsInHand; i++) {
            hand[i] = other.hand[i];
        }
    }
    return *this;
}
