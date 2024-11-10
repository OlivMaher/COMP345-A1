#include "LoggingObserver.h"
#include "../CommandProcessing/CommandProcessing.h"
#include "../Orders_list/Orders.h"
#include "../Game_Engine/GameEngine.h"

void testLoggingObserver() {
    LogObserver logObserver;

    // Test CommandProcessor and Command
    CommandProcessor commandProcessor;
    commandProcessor.attach(&logObserver);

    Command cmd1("Attack");
    cmd1.attach(&logObserver);
    commandProcessor.saveCommand(cmd1);
    cmd1.saveEffect("Attacked successfully");

    Command cmd2("Defend");
    cmd2.attach(&logObserver);
    commandProcessor.saveCommand(cmd2);
    cmd2.saveEffect("Defended successfully");

    // Test Orders and OrdersList
    OrdersList ordersList;
    ordersList.attach(&logObserver);

    Order* deployOrder = new Deploy();
    deployOrder->attach(&logObserver);
    ordersList.add(deployOrder);

    Order* advanceOrder = new Advance();
    advanceOrder->attach(&logObserver);
    ordersList.add(advanceOrder);

    deployOrder->execute();
    advanceOrder->execute();

    // Test GameEngine state transitions
    GameEngine gameEngine(new StartState());
    gameEngine.attach(&logObserver);

    gameEngine.handleCommand("loadmap");
    gameEngine.handleCommand("validatemap");
    gameEngine.handleCommand("addplayer");
    gameEngine.handleCommand("assigncountries");
    gameEngine.handleCommand("issueorder");
    gameEngine.handleCommand("endissueorder");
    gameEngine.handleCommand("execorder");
    gameEngine.handleCommand("endexecorder");
    gameEngine.handleCommand("issueorder");
    gameEngine.handleCommand("endissueorder");
    gameEngine.handleCommand("win");
    gameEngine.handleCommand("play");

    // Clean up
    delete deployOrder;
    delete advanceOrder;
}

int main() {
    testLoggingObserver();
    return 0;
}