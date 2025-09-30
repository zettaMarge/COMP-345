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

    // Copy / assign
    ICommand(const ICommand& other) : name(other.name), nextState(other.nextState) {}
    ICommand& operator=(const ICommand& other) {
        if (this != &other) {
            name = other.name;
            nextState = other.nextState;
        }
        return *this;
    }

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, const ICommand& cmd) {
        os << "Command: " << cmd.name;
        return os;
    }
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

    virtual ~IState() = default;

    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;

    virtual std::unique_ptr<IState> Clone() const = 0;
};

// ===== SimpleState =====
class SimpleState : public IState {
public:
    std::string enterMessage;
    std::string exitMessage;

    SimpleState(const std::string& n,
        const std::string& enterMsg,
        const std::string& exitMsg);

    // Copy / assign
    SimpleState(const SimpleState& other);
    SimpleState& operator=(const SimpleState& other);

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, const SimpleState& s);

    // Default behavior
    void OnEnter() override;
    void OnExit() override;

    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<SimpleState>(*this);
    }
};

// ===== Specialized States =====
class MainMenuState : public SimpleState {
public:
    MainMenuState();
    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<MainMenuState>(*this);
    }
};

class MapLoadedState : public SimpleState {
public:
    MapLoadedState();
    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<MapLoadedState>(*this);
    }
};

class MapValidatedState : public SimpleState {
public:
    MapValidatedState();
    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<MapValidatedState>(*this);
    }
};

class PlayersAddedState : public SimpleState {
public:
    PlayersAddedState();
    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<PlayersAddedState>(*this);
    }
};

class AssignReinforcementState : public SimpleState {
public:
    AssignReinforcementState();
    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<AssignReinforcementState>(*this);
    }
};

class IssueOrdersState : public SimpleState {
public:
    IssueOrdersState();
    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<IssueOrdersState>(*this);
    }
};

class ExecuteOrdersState : public SimpleState {
public:
    ExecuteOrdersState();
    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<ExecuteOrdersState>(*this);
    }
};

class WinGameState : public SimpleState {
public:
    WinGameState();
    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<WinGameState>(*this);
    }
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
    ~GameEngine();

    // Copy / assign
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);

    friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine);

    int Run();
    bool ProcessInput(const std::string& input);
};
