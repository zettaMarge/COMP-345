#pragma once
#include <string>
#include <vector>
#include <fstream>

class ILoggable{
    public:
        virtual ~ILoggable() = default;
        virtual std::string StringToLog() = 0;
};

class Observer{
    public:
        virtual ~Observer() = default;
        virtual void Update(ILoggable* _loggable) = 0;
};

class Subject{
    public:
        virtual ~Subject() = default;

        void Attach(Observer* _observer);
        void Detach(Observer* _observer);
        void Notify(ILoggable* _loggable);
    private:
        std::vector<Observer*> observers;
};

class LogObserver : public Observer{
    public:
        LogObserver();
        ~LogObserver();
        void Update(ILoggable* loggable) override;
        void closeLogFile();
    private:
        std::ofstream logs;
};