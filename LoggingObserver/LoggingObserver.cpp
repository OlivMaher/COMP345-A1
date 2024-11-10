#include "LoggingObserver.h"

// ------------------
// Observer
// ------------------
Observer::Observer() {}

Observer::~Observer () {}

// ------------------
// Subject
// ------------------
Subject::Subject() {}

void Subject::attach(Observer* observer){
    observers.push_back(observer);
}
void Subject::detach(Observer* observer){
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}
void Subject::notify(){
    for(Observer* observer: observers){
        observer->update(this);
    }
}


// ------------------
// Log Observer
// ------------------

//Constructor Open log file
LogObserver::LogObserver() {
    logFile.open("gamelog.txt", std::ios::app);
    if (!logFile){
        cerr << "Failed to open log file \n";
    }
}
//Deconstructor closes log file
LogObserver::~LogObserver(){
    if(logFile.is_open()){
        logFile.close();
    }
}

void LogObserver::update(Subject* subject){
    Iloggable* loggable = dynamic_cast<Iloggable*>(subject);
    if(logFile.is_open()){
        logFile << loggable->stringToLog() << endl;
    }
}