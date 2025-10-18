#include "GameEngine.h"
#include <iomanip>

// ===== SimpleState implementation =====
// SimpleState constructor
SimpleState::SimpleState(const std::string& n, const std::string& enterMsg, const std::string& exitMsg)
    : IState(n), enterMessage(enterMsg), exitMessage(exitMsg) {
} 

// simple state Copy constructor
SimpleState::SimpleState(const SimpleState& other)  : IState(other.name), enterMessage(other.enterMessage), exitMessage(other.exitMessage) {
    //deep copy of commands
    for (const auto& cmd : other.availableCommands) {
        availableCommands.emplace_back(std::make_unique<SimpleCommand>(cmd->name, cmd->nextState));
	}
}
// simple state assignment operator
SimpleState& SimpleState::operator=(const SimpleState& other) {
    if (this != &other) {
        name = other.name;
        enterMessage = other.enterMessage;
        exitMessage = other.exitMessage;
        availableCommands.clear();
        for (const auto& cmd : other.availableCommands) {
            availableCommands.emplace_back(std::make_unique<SimpleCommand>(cmd->name, cmd->nextState));
        }
    }
    return *this;
}
// Stream insertion for SimpleState
std::ostream& operator<<(std::ostream& os, const SimpleState& s) {
    os << "State: " << s.name;
    return os;
}

void SimpleState::OnEnter() { std::cout << enterMessage << "\n"; }
void SimpleState::OnExit() { std::cout << exitMessage << "\n"; }

// ===== Concrete States =====
//construction of all the main states and their current functionality which is only messages lol
MainMenuState::MainMenuState()
    : SimpleState("MainMenu", "Welcome to the game(engine)!",
        "Leaving Main Menu...") {}

MapLoadedState::MapLoadedState()
    : SimpleState("MapLoaded", "Entering map loading state",
        "Leaving map loading state") {}

MapValidatedState::MapValidatedState()
    : SimpleState("MapValidated", "Entering map validated state",
		"Leaving map validated state") {}

PlayersAddedState::PlayersAddedState()
    : SimpleState("PlayersAdded", "entering addplayerstate",
        "Leaving players added state") {}

AssignReinforcementState::AssignReinforcementState()
    : SimpleState("AssignReinforcement", "entering reinforcementassignment",
        "Leaving assign reinforcement state") {}

IssueOrdersState::IssueOrdersState()
    : SimpleState("IssueOrders", "entering issueordersstate",
        "Leaving issue orders state") {}

ExecuteOrdersState::ExecuteOrdersState()
    : SimpleState("ExecuteOrders", "entering execorderstate",
        "Leaving execute orders state") {}

WinGameState::WinGameState()
    : SimpleState("WinGame", "Entering winstate",
        "Exiting win state") {}


// ===== GameEngine Implementation =====
GameEngine* GameEngine::instance = nullptr; // initialize singleton instance to nullptr

GameEngine::GameEngine() {
    // Create all states
	//unique pointers to manage state lifetimes that way I dont have do worry about memory leaks
    mainMenuState = std::make_unique<MainMenuState>();
    mapLoadedState = std::make_unique<MapLoadedState>();
    mapValidatedState = std::make_unique<MapValidatedState>();
    playersAddedState = std::make_unique<PlayersAddedState>();
    assignReinforcementState = std::make_unique<AssignReinforcementState>();
    issueOrdersState = std::make_unique<IssueOrdersState>();
    executeOrdersState = std::make_unique<ExecuteOrdersState>();
    winState = std::make_unique<WinGameState>();

    // Hook commands with automatic transitions
	//I feel like there's probably a more elegant way to do this but whatever
    mainMenuState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("loadmap", mapLoadedState.get()));

    mapLoadedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("validatemap", mapValidatedState.get()));

    mapLoadedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("loadmap", nullptr));

    mapValidatedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("addplayer", playersAddedState.get()));

    playersAddedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("addplayer", nullptr));

    playersAddedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("assigncountries", assignReinforcementState.get()));

    assignReinforcementState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("issueorder", issueOrdersState.get()));

    issueOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("issueorder", nullptr));

    issueOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("endissueorders", executeOrdersState.get()));

    executeOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("execorder", nullptr));

    executeOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("endexecorder", assignReinforcementState.get()));

    executeOrdersState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("win", winState.get()));

    winState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("play", mainMenuState.get()));

    winState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("end", nullptr)); // exit game

    // Start the game
	currentState = mainMenuState.get(); // start at main menu
    currentState->OnEnter();
}

GameEngine::~GameEngine() = default; // unique_ptrs auto-cleanup

// Copy constructor, really just for the sake of having one, as you can tell its really annoying to implement
//cuz you have to deep copy everything but yeah it's there
GameEngine::GameEngine(const GameEngine& other) {
    if (other.mainMenuState) mainMenuState = other.mainMenuState->Clone();
    if (other.mapLoadedState) mapLoadedState = other.mapLoadedState->Clone();
    if (other.mapValidatedState) mapValidatedState = other.mapValidatedState->Clone();
    if (other.playersAddedState) playersAddedState = other.playersAddedState->Clone();
    if (other.assignReinforcementState) assignReinforcementState = other.assignReinforcementState->Clone();
    if (other.issueOrdersState) issueOrdersState = other.issueOrdersState->Clone();
    if (other.executeOrdersState) executeOrdersState = other.executeOrdersState->Clone();
    if (other.winState) winState = other.winState->Clone();

	// Set current state pointer to the corresponding state in this instance
	// If the other currentState is null, set this currentState to null as well
	//once again not very elegant but it works
    currentState = (other.currentState == other.mainMenuState.get())
        ? mainMenuState.get()
        : (other.currentState == other.mapLoadedState.get())
        ? mapLoadedState.get()
        : (other.currentState == other.mapValidatedState.get())
        ? mapValidatedState.get()
        : (other.currentState == other.playersAddedState.get())
        ? playersAddedState.get()
        : (other.currentState == other.assignReinforcementState.get())
        ? assignReinforcementState.get()
        : (other.currentState == other.issueOrdersState.get())
        ? issueOrdersState.get()
        : (other.currentState == other.executeOrdersState.get())
        ? executeOrdersState.get()
        : (other.currentState == other.winState.get())
        ? winState.get()
        : nullptr;
}

//GameEngine assignment operator
//once again deep copy everything
GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {
        if (other.mainMenuState) mainMenuState = other.mainMenuState->Clone();
		if (other.mapLoadedState) mapLoadedState = other.mapLoadedState->Clone();
		if (other.mapValidatedState) mapValidatedState = other.mapValidatedState->Clone();
		if (other.playersAddedState) playersAddedState = other.playersAddedState->Clone();
		if (other.assignReinforcementState) assignReinforcementState = other.assignReinforcementState->Clone();
		if (other.issueOrdersState) issueOrdersState = other.issueOrdersState->Clone();
        if (other.executeOrdersState) executeOrdersState = other.executeOrdersState->Clone();
		if (other.winState) winState = other.winState->Clone();

        currentState = (other.currentState == other.mainMenuState.get())
            ? mainMenuState.get()
            : (other.currentState == other.mapLoadedState.get())
            ? mapLoadedState.get()
            : (other.currentState == other.mapValidatedState.get())
            ? mapValidatedState.get()
            : (other.currentState == other.playersAddedState.get())
            ? playersAddedState.get()
            : (other.currentState == other.assignReinforcementState.get())
            ? assignReinforcementState.get()
            : (other.currentState == other.issueOrdersState.get())
            ? issueOrdersState.get()
            : (other.currentState == other.executeOrdersState.get())
            ? executeOrdersState.get()
            : (other.currentState == other.winState.get())
            ? winState.get()
            : nullptr;
    }
	return *this; // return this instance if theyre the same
}

//stream insertion for GameEngine
//returns output stream of current state and available commands
std::ostream& operator<<(std::ostream& os, const GameEngine& engine) {
    os << "==== Current State of the Game Engine:\n";

    if (!engine.currentState) {
        os << "Current state: [none]\n";
        return os;
    }

    os << "Current state: " << engine.currentState->name << "\n";
    os << "Available commands:\n";

    if (engine.currentState->availableCommands.empty()) {
        os << "  (no commands available)\n";
    }
    else {
        for (const auto& cmd : engine.currentState->availableCommands) {
            os << "  - " << std::left << std::setw(15) << cmd->name;
            if (cmd->nextState)
                os << " -> " << cmd->nextState->name;
            else
                os << " (no transition)";
            os << "\n";
        }
    }

    return os;
}

//main game loop
//using a while true loop Ive found no reason not to
//prompts user for input and processes it
int GameEngine::Run() {
    std::string input; // user input

    // clean input stream from previous tests
    std::cin.clear();
    std::cin.ignore(10000, '\n');

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, input)) {
            std::cout << "\nExiting game.\n";
            break;
        }

        // If ProcessInput returns false, break out of the loop 
        if (!ProcessInput(input)) {
            break;
        }
    }
    return 0;
}

//process user input
//checks if input matches any available commands in the current state
//if so, executes the command and transitions to the next state if applicable
//if not returs list of available commands
//if command is end it returns false to signal quitting the game
bool GameEngine::ProcessInput(const std::string& input) {
    for (auto& cmd : currentState->availableCommands) {
        if (cmd->name == input) {
            cmd->Execute();
            if (cmd->name == "end") {
                std::cout << "Game Over.\n";
                return false;   // signal quit
            }
            if (cmd->nextState) {
                currentState->OnExit();
                currentState = cmd->nextState;
                currentState->OnEnter();
            }
            return true;  // valid command
        }
    }

    std::cout << "Invalid command. Available commands:\n";
    for (auto& cmd : currentState->availableCommands) {
        std::cout << " - " << cmd->name << "\n";
    }

    return true; // return true so the loop continues
}

// ===== Entry point =====
//using main for now
//I'll rename this to testgameegine or whatever when its time
int TestGameEngine() {
    GameEngine::instance = new GameEngine();
    GameEngine::instance->Run();

    std::cout << "Press Enter to exit...";
    std::cin.get();

    delete GameEngine::instance;   
    GameEngine::instance = nullptr;
    return 0;
}

// Code for assignment 2
// Part 3: Game play: main game loop

//Main game loop that cycles through the three phases
void GameEngine::mainGameLoop() {
    bool gameWon = false;
    while (!gameWon) {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
        // checkPlayerElimination();
        // gameWon = checkWinCondition();
    }
}

// Reinforcement phase - Players are given a number of army units that depends on the number of
// territories they own
void GameEngine::reinforcementPhase() {
    std::cout << "Reinforcement Phase started.\n";

    // Iterate through each player and calculate reinforcements
    for (int i = 0; i < players.size(); i++) {
        //initiates the player for every iteration
        Player* player = players[i]; // players vector needs to be defined somewhere in GameEngine
        int numTerritories = player->GetPlayerTerritories().size();
        int reinforcementUnits = numTerritories / 3; // 1 unit per 3 territories owned
        if (reinforcementUnits < 3) { // minimum of 3 units
            reinforcementUnits = 3;
        }

        // Check for continent control bonuses
        std::vector<Continent*> continents = player->GetOwnedContinents();
        for (int j = 0; j < continents.size(); j++) {
            reinforcementUnits += continents[j]->GetPoints();
        }
        
        //add reinforcements to player's pool
        player->AddReinforcements(reinforcementUnits);
        std::cout << "Player " << player->GetName() << " receives " << reinforcementUnits << " reinforcement units.\n";
    }
    
    // Implement reinforcement logic here
    std::cout << "Reinforcement Phase ended.\n";
}

// Issue Orders phase - Players issue orders such as deploying armies, attacking other players,
// and fortifying their positions
void GameEngine::issueOrdersPhase() {
    std::cout << "Issue Orders Phase started.\n";
    // Implement order issuing logic here
    std::cout << "Issue Orders Phase ended.\n";
}

// Execute Orders phase - The issued orders are executed in the order they were issued
void GameEngine::executeOrdersPhase() {
    std::cout << "Execute Orders Phase started.\n";
    // Implement order execution logic here
    std::cout << "Execute Orders Phase ended.\n";
}

// Check for win condition - returns true if a player has won the game
bool GameEngine::checkWinCondition() {
    if (players.size() == 1) {
        std::cout << "Player " << players[0]->GetName() << " has won the game!\n";
        return true;
    }
    return false;
}

// Check for player elimination - returns the number of players eliminated in the current turn
void GameEngine::checkPlayerElimination() {

    // Check for player elimination
    std::vector<Player*> eliminatedPlayers;
    for (int i = 0; i < players.size(); i++) {
        Player* player = players[i]; // players vector needs to be defined somewhere in GameEngine
        if (player->GetPlayerTerritories().empty()) {
            eliminatedPlayers.push_back(player);
            std::cout << "Player " << player->GetName() << " has been eliminated from the game.\n";

            //removes player from memory
            delete player;

            //removes player from players vector
            for (int j = i; j < players.size() - 1; j++) {
                players[j] = players[j + 1];
            }
            players.pop_back();
            i--; // Adjust index after removal
        }
    }

    // Remove eliminated players from the game
    for (Player* eliminatedPlayer : eliminatedPlayers) {
        players.erase(std::remove(players.begin(), players.end(), eliminatedPlayer), players.end());
        delete eliminatedPlayer; // Assuming ownership of Player pointers
    }
}