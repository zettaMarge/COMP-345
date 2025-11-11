//-----------------------------------------------------
    //Sara Vargas Aceves 40349399
    //COMP-345-D 2252 Assignment 2 LoggingObserver.h
    //Dr. Hamed Jafarpour
    //Submitted: TBD
    //Due: November 11, 2025 
    //I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
    //Sara Vargas Aceves
//----------------------------------------------------- 
#pragma once
#include <string>
#include <vector>
#include <fstream>

//  ----- ILoggable Interface -----
// Interface that defines the text to be logged.
class ILoggable{
    public:
        virtual ~ILoggable() = default;
        virtual std::string StringToLog() = 0;
};

//  ----- Observer Class -----
// Notified when a subject changes, defines the behavior for update.
class Observer{
    public:
        virtual ~Observer() = default;
        virtual void Update(ILoggable* _loggable) = 0;
};

//  ----- Subject Class -----
// Class that can be inherited by others, has a vector of observers that are
// notified when something changes.
class Subject{
    public:
        // Default constructor
        Subject() = default;

        // Copy constructor
        Subject(const Subject& other);
        
        // Assignment operator
        Subject& operator=(const Subject& other);

        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& os, const Subject& subject);

        // Destructor
        virtual ~Subject() = default;

        void Attach(Observer* _observer);
        void Detach(Observer* _observer);
        void Notify(ILoggable* _loggable);
    private:
        std::vector<Observer*> observers;
};

//  ----- LogObserver Class -----
// Logs events to a file, and implements the observer interface to
// write logs.
class LogObserver : public Observer{
    public:
        // Constructor
        LogObserver();

        // Copy constructor
        LogObserver(const LogObserver& other);

        // Assignment operator
        LogObserver& operator=(const LogObserver& other);

        // Stream operator
        friend std::ostream& operator<<(std::ostream& os, const LogObserver& log);

        // Destructor
        ~LogObserver();

        void Update(ILoggable* loggable) override;
        void closeLogFile();
    private:
        std::ofstream logs;
        std::string filename;
};