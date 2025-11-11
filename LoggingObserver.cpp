//-----------------------------------------------------
    //Sara Vargas Aceves 40349399
    //COMP-345-D 2252 Assignment 2 LoggingObserver.cpp
    //Dr. Hamed Jafarpour
    //Submitted: TBD
    //Due: November 11, 2025 
    //I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
    //Sara Vargas Aceves
//-----------------------------------------------------
#include "LoggingObserver.h"
#include <algorithm>
#include <iostream>

//  ----- Subject class -----

// Copy constructor
Subject::Subject(const Subject& other) : observers(other.observers) {}

// Assignment operator
Subject& Subject::operator=(const Subject& other){
    if(this != &other){
        observers = other.observers;
    }
    return *this;
}

// Stream operator
std::ostream& operator<<(std::ostream& os, const Subject& subject){
    os << "Subject with " << subject.observers.size() << " observers";
    return os;
}

// Attaches an observer to itself by adding a pointer to it to a vector of observers.
// Parameters:
// - _observer: pointer to an observer
void Subject::Attach(Observer* _observer){
    if(_observer != nullptr){
        observers.push_back(_observer);
    }
}

// Dettaches an observer by removing it from a vector of observers.
// Parameters:
// - _observer: the observer to be removed
void Subject::Detach(Observer* _observer){
    observers.erase(
        std::remove(observers.begin(), observers.end(), _observer),
        observers.end()
    );
}

// Notifies all attached observers of a loggable event, goes through
// all of them and calls their Update()
// Parameters:
// - loggable: pointer to an ILoggable with the data to be logged
void Subject::Notify(ILoggable* loggable){
    for(Observer* o : observers){
        if(o != nullptr && loggable != nullptr){
            o->Update(loggable);
        }
    }
}

//  ----- LogObserver class -----

// Constructor
LogObserver::LogObserver() : filename("gamelog.txt"){
    logs.open(filename, std::ios::app);
    if(!logs.is_open()){
        std::cerr << "Could not open log file" << std::endl;
    }
}

// Copy constructor
LogObserver::LogObserver(const LogObserver& other) : filename(other.filename){
    logs.open(filename, std::ios::app);
    if(!logs.is_open()){
        std::cerr << "Could not open log file" << std::endl;
    }
}

// Assignment operator
LogObserver& LogObserver::operator=(const LogObserver& other){
    if(this != &other){
        closeLogFile();
        filename = other.filename;
        logs.open(filename, std::ios::app);
        if(!logs.is_open()){
            std::cerr << "Could not open log file" << std::endl;
        }
    }
    return *this;
}

// Stream operator
std::ostream& operator<<(std::ostream& os, const LogObserver& log){
    os << "LogObserver writing to " << log.filename;
    return os;
}

// Destructor
LogObserver::~LogObserver(){
    closeLogFile();
}

// Writes log entries to the file that it receives with StringToLog
// Parameters
// - loggable: pointer to an ILoggable object with the information to log
void LogObserver::Update(ILoggable* loggable){
    if(loggable != nullptr && logs.is_open()){
        std::string logEntry = loggable->StringToLog();
        logs << logEntry << std::endl;
        logs.flush();
    }
}

// Closes the log file
void LogObserver::closeLogFile(){
    if(logs.is_open()){
        logs.close();
    }
}