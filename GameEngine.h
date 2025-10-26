#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// ===== Forward declarations =====
class IState;

int TestGameEngine(); // test the game engine

// ===== ICommand =====
//Interface for commands
//contains a name and an optional next state to transition to
//virtual execute method where methods can plug in their own functionality
//will later implement the other class' methods 
//contains copy/assign and stream insertion as requested lol
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
//purely for this iteration of this project to make my life easier
//later on each command will just inherit from ICommand and implement their own Execute method
class SimpleCommand : public ICommand {
public:
    SimpleCommand(const std::string& n, IState* next = nullptr)
        : ICommand(n, next) {
    }
    void Execute() override { std::cout << "Executing " << name << "\n"; }
};

// ===== IState =====
//Interface for states
//contains a name and a list of available commands
//virtual OnEnter and OnExit methods where states can plug in their own functionality
//clone method to facilitate deep copying of states(necessary for GameEngine copy/assign)
class IState {
public:
	std::string name; //state name
	std::vector<std::unique_ptr<ICommand>> availableCommands; //vector of commands available in this state
	IState(const std::string& n) : name(n) {} //constructor

	virtual ~IState() = default; //virtual destructor

    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;

	virtual std::unique_ptr<IState> Clone() const = 0; //for deep copying
};

// ===== SimpleState =====
//similar to Simplecommand, purely for this iteration of this project to make my life easier
//although each state might not need its own functionality, will depend on demands of future iterations
//contains copy/assign and stream insertion as requested lol
class SimpleState : public IState {
public:
    std::string enterMessage;
    std::string exitMessage;

    SimpleState(const std::string& n,
        const std::string& enterMsg,
		const std::string& exitMsg); //constructor

    // Copy / assign
	SimpleState(const SimpleState& other); // copy constructor
	SimpleState& operator=(const SimpleState& other); // copy assignment

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, const SimpleState& s); 

    // Default behavior
    void OnEnter() override;
    void OnExit() override;

    std::unique_ptr<IState> Clone() const override {
        return std::make_unique<SimpleState>(*this);
	} //for deep copying
};

// ===== Specialized States =====
// I gave each state its own class for clarity and I'm guessing I'll need to add functionality later
// Each state implements Clone for deep copying along w a constructor
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
// Singleton class that manages the game states and transitions
// Owns all states and manages the current state
// Contains Run and ProcessInput methods to handle the game loop and user input
class GameEngine {
public:
	static GameEngine* instance; // singleton instance

    // Own all states
    std::unique_ptr<IState> mainMenuState;
    std::unique_ptr<IState> mapLoadedState;
    std::unique_ptr<IState> mapValidatedState;
    std::unique_ptr<IState> playersAddedState;
    std::unique_ptr<IState> assignReinforcementState;
    std::unique_ptr<IState> issueOrdersState;
    std::unique_ptr<IState> executeOrdersState;
    std::unique_ptr<IState> winState;

	IState* currentState = nullptr; // current state pointer

	GameEngine(); // constructor
	~GameEngine(); // destructor

    // Copy / assign
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);

	friend std::ostream& operator<<(std::ostream& os, const GameEngine& engine); // stream insertion

	int Run(); // main game loop
	bool ProcessInput(const std::string& input); // process user input

    // Code for assignment 2
    // Part 3: Game play: main game loop
    private void mainGameLoop();
    private void reinforcementPhase();
    private void issueOrdersPhase();
    private void executeOrdersPhase();

    //Additional methods to check for win condition and player count
    private bool checkWinCondition();
    private void checkPlayerElimination();

    //Code for Assignemnt 2 Part 2
    <Player*> players; //list of players in the game
    private void addPlayers(const string &playerName);
    //private void gameStart();
    //private void validateMap();
    //private void loadMap(const string &mapFileName);
};
