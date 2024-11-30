#include "../CommandProcessing/CommandProcessing.h"
#include "../Game_Engine/GameEngine.h"

void testTournament(){
    GameEngine gameEngine(new StartState());
    CommandProcessor CommandProcessor(&gameEngine);
    Command command("tournament -M USA.map Alabama.map -P Aggressive Benevolent Neutral Cheater -G 4 -D 30");

    if(CommandProcessor.processTournamentCommand(command)){
        cout << "\nCommand Valid";
    }
    else{
        cout << "\nCommand Invalid";
    }

}

int main(){
    testTournament();
    return 0;
}
