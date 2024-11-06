#ifndef LOGGINGOBSERVER_H
#define LOGGINGOBSERVER_H


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
        virtual ~Subject() = default;

        void attach(Observer* obs);
        void detach(Observer* obs);
        void notify();
        virtual string stringToLog() const = 0;
};

class Iloggable {
    public:
        virtual ~Iloggable() = default;
        virtual string stringToLog() const = 0;
};

class LogObserver : public Observer {
    private:
        ofstream logFile;
    private:
        LogObserver();
        ~LogObserver();
        void update(Subject* subject) override;
};


#endif