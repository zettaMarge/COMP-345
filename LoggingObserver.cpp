#include "LoggingObserver.h"
#include <algorithm>
#include <iostream>

void Subject::attach(Observer* _observer){
    if(_observer != nullptr){
        observers.push_back(_observer);
    }
}

void Subject::detach(Observer* _observer){
    observers.erase(
        std::remove(observers.begin(), observers.end(), _observer),
        observers.end()
    );
}

void Subject::notify(ILoggable* loggable){
    for(Observer* o : observers){
        if(o != nullptr && loggable != nullptr){
            o->update(loggable);
        }
    }
}

// LOG OBSERVER

LogObserver::LogObserver(){
    logs.open("gamelog.txt", std::ios::app);
    if(!logs.is_open()){
        std::cerr << "Could not open log file" << std::endl;
    }
}

LogObserver::~LogObserver(){
    closeLogFile();
}

void LogObserver::update(ILoggable* loggable){
    if(loggable != nullptr && logs.is_open()){
        std::string logEntry = loggable->stringToLog();
        logs << logEntry << std::endl;
        logs.flush();
    }
}

void LogObserver::closeLogFile() {
    if(logs.is_open()){
        logs.close();
    }
}