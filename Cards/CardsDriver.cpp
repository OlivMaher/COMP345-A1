#include "Cards.h"
#include "Cards.cpp"
#include <cstdlib> 

void testCards(){
    srand(time(0));
    Deck D;
    Hand H;
    Card C;

    D.showDeck();

    // adds cards to hand
    H.takeCard(D);
    H.takeCard(D);
    H.takeCard(D);
    H.takeCard(D);
    H.takeCard(D);

    D.showDeck();
    H.showHand();

    C.play(D,H);
    D.showDeck();
    H.showHand();


}
int main(){
    testCards();
    return 0;
}
