#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Observer {    
public:
    Observer(); //Default
    Observer(const Observer &obj); //Copy constructor
    Observer &operator=(const Observer &obj); //asignment operator
    ~Observer();

    virtual void update(Subject* subject) = 0;
};

class Subject {
private:
    vector<Observer*> observers;
public:
    Subject();
    Subject(const Subject &obj);
    Subject &operator=(const Subject &obj);
    ~Subject();

    void attach(Observer* obs);
    void detach(Observer* obs);
    void notify();
};
class ILoggable {
    public:
        virtual string stringToLog() const = 0;
};
class LogObserver : public Observer {
    private:
        ofstream logFile;
    public:
        LogObserver();
        ~LogObserver();

        void update(Subject* subject) override;
    
};