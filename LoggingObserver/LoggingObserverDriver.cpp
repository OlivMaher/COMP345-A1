#include "LoggingObserver.h"
#include "../CommandProcessing/CommandProcessing.h"

void testLoggingObserver(){
    LogObserver logObserver;

    CommandProcessor commandProcessor;
    commandProcessor.attach(&logObserver);
    
    Command cmd1("Attack");
    cmd1.attach(&logObserver);
    commandProcessor.saveCommand(cmd1);
    cmd1.saveEffect("Attacked sucessfully");

}

int main(){
    testLoggingObserver();
    return 0;
}