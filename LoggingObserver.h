#pragma once
#include <string>
#include <vector>
#include <fstream>

class ILoggable{
    public:
        virtual ~ILoggable() = default;
        virtual std::string stringToLog() = 0;
};

class Observer{
    public:
        virtual ~Observer() = default;
        virtual void update(ILoggable* _loggable) = 0;
};

class Subject{
    public:
        virtual ~Subject() = default;

        void attach(Observer* _observer);
        void detach(Observer* _observer);
        void notify(ILoggable* _loggable);
    private:
        std::vector<Observer*> observers;
};

class LogObserver : public Observer{
    public:
        LogObserver();
        ~LogObserver();
        void update(ILoggable* loggable) override;
        void closeLogFile();
    private:
        std::ofstream logs;
};