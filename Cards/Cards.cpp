#include "Cards.h"
#include "../Player/Player.h"
#include "../Orders_list/Orders.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// **Card Class Implementation**

Card::Card() : CardType("Unknown") {}

Card::Card(const std::string& type) : CardType(type) {}

Card::Card(const Card& a) : CardType(a.CardType) {}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        CardType = other.CardType;
    }
    return *this;
}

std::string Card::getCardType() const {
    return CardType;
}

// Original play method for human players
void Card::play(Deck& deck, Hand& hand) {
    bool useCard = true;

    while (useCard) {
        if (hand.getHand() <= 0) {
            cout << "No Cards to play.\n\n";
            useCard = false;
        } else {
            cout << "Would you like to play a card? (Y/N) \n";
            char answer;
            cin >> answer;

            if (answer != 'Y') {
                cout << "No card played.\n\n";
                useCard = false; // Ends function
            } else {
                useCard = true;

                hand.showHand();
                cout << "Which card would you like to use?\n";
                int choice; // Which card should be played
                cin >> choice;
                if (choice < 1 || choice > hand.getHand()) { // Checks for valid card number
                    cout << "Invalid Card.\n\n";
                } else { // If valid card number is given
                    Card playing = hand.handToDeck(choice - 1, deck); // Returns card to deck

                    const string& type = playing.getCardType();

                    if (type == "Bomb") {
                        cout << "Playing Bomb Card\n";
                    } else if (type == "Reinforcement") {
                        cout << "Playing Reinforcement Card\n";
                    } else if (type == "Blockade") {
                        cout << "Playing Blockade Card\n";
                    } else if (type == "Airlift") {
                        cout << "Playing Airlift Card\n";
                    } else if (type == "Diplomacy") {
                        cout << "Playing Diplomacy Card\n";
                    }
                    cout << "\n";
                }
            }
        }
    }
}

// AI-friendly play method
void Card::play(Deck& deck, Hand& hand, Player* player, OrdersList* ordersList) {
    // Implement AI logic for playing the card
    if (this->getCardType() == "Bomb") {
        // AI logic to select a target territory
        vector<Territory*> toAttack = player->toAttack();
        if (!toAttack.empty()) {
            Territory* target = toAttack.front(); // Simplified selection
            Order* bombOrder = new Bomb(player, target);
            ordersList->add(bombOrder);
            cout << "AI player " << player->getName() << " plays Bomb card on " << target->getName() << endl;
        }
    }
    // Handle other card types similarly...

    // Return the card to the deck
    deck.returnCard(*this);
    // Remove the card from the player's hand
    hand.removeCard(*this);
}

// **Deck Class Implementation**

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

void Deck::returnCard(Card& card) {
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

// **Hand Class Implementation**

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
        Card take = deck.draw(); // Draws a card from deck
        hand[getHand()] = take; // Adds the card to the hand
        setHand(getHand() + 1); // Adjusts the size of the hand
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

        setHand(getHand() - 1); // Adjusts the number of cards
        deck.returnCard(toRemove); // Returns card to deck

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

void Hand::removeCard(Card& card) {
    for (int i = 0; i < getHand(); ++i) {
        if (hand[i].getCardType() == card.getCardType()) {
            // Move the last card to this position
            hand[i] = hand[getHand() - 1];
            setHand(getHand() - 1);
            break;
        }
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
