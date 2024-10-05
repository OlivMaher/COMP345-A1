//library includes
#include "Cards.h"
#include <iostream>
#include <string>
#include <cstdlib> 
using namespace std;

Card::Card() : CardType("") {}; //default constructor
Card::Card(const char* type) : CardType(type) {} //parameterized constructor

Card::Card(Card& a){ //copy constructor
    CardType = a.getCardType();
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

                    const char* type= playing.getCardType();

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

const char* Card::getCardType(){ // returns CardType
    return CardType;
}

Deck::Deck() { //  Default constructor for deck
    cardsInDeck =20;

    const char* types[5] = {"Bomb", "Reinforcement", "Blockade", "Airlift", "Diplomacy"};
    int count = 0;
    for (int i = 0; i < 5; i++){
        for (int j=0; j<4; j++){
            cards[count++] = Card(types[i]);
        }
    }
}

Card Deck::draw(){
    if (getDeck() <= 0){ // prevents a draw if the deck is empty
        cout << "No cards in deck.\n" << "\n";
        return Card();
    }
    else{
        srand(time(0)); // used for random draw form deck
        int ran = rand();
        int take = ran % totalCardsDeck();
        cout << ran << " " << take << "\n";
        Card toRemove(cards[take]); 

        cards[take] = cards[getDeck()-1]; // places the last card in tha array to the position of the removed card
        cards[getDeck()-1] = NULL;

        int newSize = getDeck()-1;
        setDeck(newSize); // updates the number of cards in deck
        
        return toRemove; // returns the card that was removed from the deck
    }
}

void Deck::returnToDeck(Card& card) { // adds a card to the end of the deck
    cards[getDeck()] = card;
    setDeck(getDeck() + 1);
}

void Deck::showDeck(){ // displays the deck
    if (this->getDeck() == 0){
         cout << "Mo cards in Deck.\n";
    }
    else{    
        cout << "Deck List\n";
        cout << "---------------------\n";
        for (int i=0;i<(this->getDeck());i++){
            cout << (i+1)<< ".  "<< cards[i].getCardType() <<"\n";
        }
        cout << "\n";
    }
}

int Deck::getDeck(){ // returns the numbers of cards currently in the deck
    return cardsInDeck;
}

void Deck::setDeck(int a){ // used to change the number of cards in the deck
    cardsInDeck =a;
}


Hand::Hand() {// default constructor for hand
    cardsInHand = 0;
}

void Hand::takeCard(Deck& deck){
    if (getHand() >= 20){
        cout << "All cards in hand.\n\n";
    }
    if (deck.getDeck() <= 0) {
    cout << "No cards available in deck.\n\n";
    }
    else{
        Card take = deck.draw(); // draws a card from deck
        hand[getHand()] = take; // adds the card to the hand
        setHand(getHand()+1); // adjusts the size of the hand
    }
}

void Hand::showHand(){ // displays the hand
    if (this->getHand() == 0){
         cout << "No cards in Hand.\n\n";
    }
    else{    
        cout << "Current Hand\n";
        cout << "---------------------\n";
        for (int i=0;i<(this->getHand());i++){
            cout << (i+1)<< ".  "<< hand[i].getCardType() <<"\n";
        }
        cout << "\n";
    }
}

int Hand::getHand(){ // returns the numbers of cards currently in a hand
    return cardsInHand;
}

void Hand::setHand(int a){ // used to change the number of cards in a hand
    cardsInHand = a;
}

Card Hand::handToDeck(int a,Deck& deck){ // returns a card that was removed from a hand, but returned to the deck
    if ((a < 0 || a >= getHand())){
        //cout << totalCardsHand();
        cout << "Invalid card number.\n\n";
        return Card();
    }
    else{
        int index = a;
        Card toRemove(hand[index]);
        hand[index] = hand[getHand()-1];// positions the last card in the hand to the position of the removed card
        hand[getHand()-1] = NULL;

        int newSize = getHand()-1;
        setHand(newSize); // adjusts the number of cards
        deck.returnToDeck(toRemove); // returns card to deck
        
        return toRemove;
    }
}

Card Hand::copyCard_Hand(int a){ // returns a copy of card from a hand based on the position of the card
    if (a<0 || a >= getHand()){
        cout << "Nothing to return\n\n";
        return Card();
    }
    else{
        return (Card(hand[a]));
    }

}

