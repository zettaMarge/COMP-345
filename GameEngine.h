#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// ===== Forward declarations =====
class IState;

// ===== ICommand =====
class ICommand {
public:
    std::string name;
    IState* nextState;   // optional state to transition into

    ICommand(const std::string& n, IState* next = nullptr)
        : name(n), nextState(next) {
    }

    virtual void Execute() = 0;
    virtual ~ICommand() = default;
};

// Simple command implementation
class SimpleCommand : public ICommand {
public:
    SimpleCommand(const std::string& n, IState* next = nullptr)
        : ICommand(n, next) {
    }
    void Execute() override { std::cout << "Executing " << name << "\n"; }
};

// ===== IState =====
class IState {
public:
    std::string name;
    std::vector<std::unique_ptr<ICommand>> availableCommands;

    IState(const std::string& n) : name(n) {}
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual ~IState() = default;
};

// ===== GameEngine =====
class GameEngine {
public:
    static GameEngine* instance;

    // Own all states
    std::unique_ptr<IState> mainMenuState;
    std::unique_ptr<IState> mapLoadedState;
    std::unique_ptr<IState> mapValidatedState;
    std::unique_ptr<IState> playersAddedState;
    std::unique_ptr<IState> assignReinforcementState;
    std::unique_ptr<IState> issueOrdersState;
    std::unique_ptr<IState> executeOrdersState;
    std::unique_ptr<IState> winState;

    IState* currentState = nullptr;

    GameEngine();
    int Run();
    bool ProcessInput(const std::string& input);
};
