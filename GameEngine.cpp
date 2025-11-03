#include "GameEngine.h"
#include <iomanip>
#include <iostream>
#include <filesystem>
#include "Map.h"
#include "Player.h"
#include <random>


void LoadMapCommand::Execute() {
    std::string input;
    std::cout << "Please input the name of the directory containing the maps, or press Enter to use the current directory: \n";
    std::getline(std::cin, input);
    if (input.empty()) {
        input = std::filesystem::current_path().string();
    }
    const string fileName = input;
    std::filesystem::path mapPath = std::filesystem::path(fileName) / "maps";
    vector<std::string> mapFiles;
    int mapNumber = 1;
    std::cout << "The following are the Maps in the inputed directory, \n";
    for (auto& entry : std::filesystem::directory_iterator(mapPath)) {
        std::cout << "Map " << mapNumber << entry.path().filename().string() << std::endl;
        mapFiles.push_back(entry.path().string());
        mapNumber++;
    }
    std::cout << "Please input the number associated with the map you want: \n";
    int selectedOption = -1;
    std::cin >> selectedOption;
    if (selectedOption < 1 || selectedOption > mapFiles.size()) {
        std::cout << "Invalid selection. Please try again.\n";
        int selectedOption = -1;
        std::cin >> selectedOption;
        if (selectedOption < 1 || selectedOption > mapFiles.size()) {
            std::cout << "Invalid selection. Exiting map loading.\n";
            return;
        }
    }
    else {
        mapPath = mapFiles[selectedOption - 1];
    }

    MapLoader* loader = new MapLoader();
    try {
        bool loadedMap = loader->LoadMapFile(mapPath.string()); //automatically validates the map
        if (!loadedMap) {
            std::cout << "Failed to load map from file: " << mapPath.string() << std::endl;
            return;
        }
        GameEngine::instance->gameMap = new Map(loader->CreateMap());
    }
    catch (const std::exception& e) {
        std::cout << "Exception occurred while loading map: " << e.what() << std::endl;
        delete loader;
        return;
    }
    delete loader;
    if (GameEngine::instance->currentState->name == GameEngine::instance->mainMenuState->name) {
        GameEngine::instance->changeState(GameEngine::instance->mapLoadedState.get());
    }
    else if (GameEngine::instance->currentState->name != GameEngine::instance->mapLoadedState.get()->name) {
        std::cout << "Error: trying to execute loadmap from invalid state\n";
        return;
    }
};

void ValidateMapCommand::Execute() {
    std::cout << "Validating map...\n";
    if (GameEngine::instance->gameMap->Validate()) {
        std::cout << "Map validated successfully.\n";
    }
    else {
        std::cout << "Map validation failed.\n";
		return;
    }
    if(GameEngine::instance->currentState == GameEngine::instance->mapLoadedState.get()) {
        GameEngine::instance->changeState(GameEngine::instance->mapValidatedState.get());
    }
    else {
        std::cout << "Error: trying to execute validatemap from invalid state\n";
        return;
    }
};

void AddPlayerCommand::Execute() {
    if (argument.empty()) {
        std::cout << "Please specify a player name to add (e.g., 'addplayer Alice').\n";
        std::getline(std::cin, argument);
    }
    std::cout << "Adding player: " << argument << "\n";
    //code to add player would go here
    try {
        Player* newPlayer = new Player();
        newPlayer->SetName(argument);
        GameEngine::instance->players.push_back(newPlayer);
    }
    catch (const std::exception& e) {
        std::cout << "Exception occurred while adding player: " << e.what() << std::endl;
        return;
	}
    std::cout << "Player " << argument << " added successfully.\n";
    if(GameEngine::instance->currentState == GameEngine::instance->mapValidatedState.get()) {
        GameEngine::instance->changeState(GameEngine::instance->playersAddedState.get());
    }
    else if(GameEngine::instance->currentState != GameEngine::instance->playersAddedState.get()) {
        std::cout << "Error: trying to execute addplayer from invalid state\n";
        return;
    }
};

// ----- GameStartCommand Execute method -----
//needs work!!!!
void GameStartCommand::Execute() {
    std::cout << "Starting game...\n";
    //shuffling players 
    std::random_device rd; // seed from machines random device
    std::mt19937 g(rd()); //random number generator 
    std::shuffle(GameEngine::instance->players.begin(), GameEngine::instance->players.end(), g);
    std::cout << "2Starting game...\n";

    //Distributing territories
    vector<Territory*> territories = GameEngine::instance->gameMap->territories;
    int numPlayers = GameEngine::instance->players.size();
    for (int i = 0; i < territories.size(); i++) {
        Player* currentPlayer = GameEngine::instance->players[i % numPlayers];
        Territory* currentTerritory = territories[i];
        currentPlayer->AddTerritory(currentTerritory);
    }
    std::cout << "3Starting game...\n";

    //50 initial army units to players
    for (int i = 0; i < GameEngine::instance->players.size(); i++) {
        Player* player = GameEngine::instance->players[i];
       // player->AddReinforcements(50);
    }
    std::cout << "4Starting game...\n";

    //each player draws 2 initial cards from the deck
    for (int i = 0; i < GameEngine::instance->players.size(); i++) {
        Player* player = GameEngine::instance->players[i];
       // player->GetPlayerHand()->AddCard();
        //player->GetPlayerHand()->AddCard();
    }
    std::cout << "5Starting game...\n";

	GameEngine::instance->mainGameLoop();
    std::cout << "Game started successfully.\n";
    if(GameEngine::instance->currentState == GameEngine::instance->playersAddedState.get()) {
        GameEngine::instance->changeState(GameEngine::instance->assignReinforcementState.get());
    }
    else {
        std::cout << "Error: trying to execute gamestart from invalid state\n";
        return;
    }
};

void ReplayCommand::Execute() {
    std::cout << "Replaying game...\n";
    //code to reset and replay the game would go here
    if(GameEngine::instance->currentState == GameEngine::instance->winState.get()) {
        GameEngine::instance->changeState(GameEngine::instance->mainMenuState.get());
        std::cout << "Game restarted successfully.\n";
    }
    else {
        std::cout << "Trying to execute replay from invalid state, force replay anyway? (y/n)\n";
		char response;
		std::cin >> response;
        if(response == 'y' || response == 'Y') {
			GameEngine::instance->changeState(GameEngine::instance->mainMenuState.get());
            std::cout << "Game restarted successfully.\n";
		}
        else {
			std::cout << "Okay, replay command ignored.\n";
        }
        return;
	}   
};

void QuitCommand::Execute() {
    if (GameEngine::instance->currentState == GameEngine::instance->winState.get()) {
        std::cout << "Quitting game...\n";
    }
    else {
        std::cout << "Trying to execute quit mid game, force quit anyway? (y/n)\n";
        char response;
        std::cin >> response;
        if (response == 'y' || response == 'Y') {
            std::cout << "Quitting game...\n";
        }
        else {
            std::cout << "Okay, quit command ignored.\n";
            return;
        }
    }
    //code to cleanly exit the game would go here
    std::cout << "Game quit successfully.\n";
    exit(0); //terminate program
};

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
    : SimpleState("MainMenu", "Welcome to the game! This is the main menu state",
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
    instance = this;
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
        std::make_unique<LoadMapCommand>("", mapLoadedState.get()));

    mapLoadedState->availableCommands.emplace_back(
        std::make_unique<ValidateMapCommand>("validatemap", mapValidatedState.get()));

    mapLoadedState->availableCommands.emplace_back(
        std::make_unique<LoadMapCommand>("loadmap", nullptr));

    mapValidatedState->availableCommands.emplace_back(
        std::make_unique<AddPlayerCommand>("addplayer", playersAddedState.get()));

    playersAddedState->availableCommands.emplace_back(
        std::make_unique<AddPlayerCommand>("addplayer", nullptr));

    playersAddedState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("gamestart", assignReinforcementState.get()));

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
       /* if (!ProcessInput(input)) {
            break;
        }*/
    }
    return 0;
}

//process user input
//checks if input matches any available commands in the current state
//if so, executes the command and transitions to the next state if applicable
//if not returs list of available commands
//if command is end it returns false to signal quitting the game
//bool GameEngine::ProcessInput(const std::string& input) {
//    for (auto& cmd : currentState->availableCommands) {
//        if (cmd->name == input) {
//            cmd->Execute();
//            if (cmd->name == "end") {
//                std::cout << "Game Over.\n";
//                return false;   // signal quit
//            }
//            if (cmd->nextState) {
//                currentState->OnExit();
//                currentState = cmd->nextState;
//                currentState->OnEnter();
//            }
//            return true;  // valid command
//        }
//    }
//
//    std::cout << "Invalid command. Available commands:\n";
//    for (auto& cmd : currentState->availableCommands) {
//        std::cout << " - " << cmd->name << "\n";
//    }
//
//    return true; // return true so the loop continues
//}

//execute command
void GameEngine::ExecuteCommand(const std::shared_ptr<ICommand>& cmd) {
    if (!cmd) return;
    if (!currentState) {
        std::cout << "Error: No current state.\n";
        return;
    }
    if (IsValidCommand(cmd->name)) cmd->Execute();
    else if(cmd->name == "replay") {
		ExecuteCommand(std::make_shared<ReplayCommand>());
	}
    else {
        std::cout << "Invalid command. Available commands:\n";
        for (auto& cmd : currentState->availableCommands) {
            std::cout << " - " << cmd->name << "\n";
        }
    }
}

//validate command
bool GameEngine::IsValidCommand(const std::string& cmd) {
    for (const auto& command : currentState->availableCommands) {
        if (command->name == cmd) {
            return true;
        }
    }
    return false;
}

//change current state
void GameEngine::changeState(IState* next) {
    if (!next) {
        std::cout << "Error: Cannot change to null state.\n";
        return;
    }
    currentState->OnExit();
    currentState = next;
    currentState->OnEnter();
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
      //  reinforcementPhase();
      //  issueOrdersPhase();
      //  executeOrdersPhase();
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
      //  std::vector<Continent*> continents = player->GetOwnedContinents();
       // for (int j = 0; j < continents.size(); j++) {
      //      reinforcementUnits += continents[j]->GetPoints();
        }

        //add reinforcements to player's pool
      //  player->AddReinforcements(reinforcementUnits);
     //   std::cout << "Player " << player->GetName() << " receives " << reinforcementUnits << " reinforcement units.\n";
   // }

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
