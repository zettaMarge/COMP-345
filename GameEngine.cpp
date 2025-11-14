#include "GameEngine.h"
#include <iomanip>
#include <iostream>
#include <filesystem>
#include "Map.h"
#include "Player.h"
#include <random>
#include <algorithm>
#include "Orders.h"

void GameEngine::RunTournament(const TournamentParameters& params)
{
    std::cout << "=== Running Tournament Mode ===\n";
    std::cout << "Maps (" << params.maps.size() << "): ";
    for (const auto& m : params.maps)
        std::cout << m << " ";
    std::cout << "\n";

    std::cout << "Player Strategies (" << params.strategies.size() << "): ";
    for (const auto& p : params.strategies)
        std::cout << p << " ";
    std::cout << "\n";

    std::cout << "Number of Games (G): " << params.games << "\n";
    std::cout << "Max Turns (D): " << params.maxTurns << "\n";

    std::cout << "=====================================\n";
    // Results matrix: rows = maps, columns = strategies
    // each cell stores list of results like ["A", "-", "W"]
    std::vector<std::vector<std::vector<std::string>>> results;

    results.resize(params.maps.size(),
        std::vector<std::vector<std::string>>(
            params.strategies.size()
        )
    );

    for (int m = 0; m < params.maps.size(); ++m) {
            for (int g = 1; g <= params.games; ++g) {
                std::cout << "  Game " << g << " of " << params.games << "... ";
				//simulate game here
                auto loadMapCmd = std::make_shared<LoadMapCommand>(params.maps[m]);
                loadMapCmd->Execute();
				auto validateMapCmd = std::make_shared<ValidateMapCommand>();
				validateMapCmd->Execute();
                for(int p = 0; p < params.strategies.size(); ++p) {
                    std::string playerName = "Player" + std::to_string(p + 1) + "_" + params.strategies[p];
                    auto addPlayerCmd = std::make_shared<AddPlayerCommand>(playerName+ params.strategies[p]);
                    addPlayerCmd->Execute();
				}
                auto gameStartCmd =  std::make_shared<GameStartCommand>(std::to_string(params.maxTurns));
                std::string winner = gameStartCmd->Execute();
                // Record results
                for (int s = 0; s < params.strategies.size(); ++s) {
                    std::string playerName = "Player" + std::to_string(s + 1) + "_" + params.strategies[s];
                    if (winner == "Draw") {
                        results[m][s].push_back("-");
                    }
                    else if (winner == playerName) {
                        results[m][s].push_back("W");
                    }
                    else {
                        results[m][s].push_back("L");
                    }
                }
				std::cout << "Done.\n";
             }
    }

    std::cout << "\n=== TOURNAMENT RESULTS ===\n\n";

    std::cout << "Map \\ Strategy";
    for (const auto& strat : params.strategies)
        std::cout << "\t" << strat;
    std::cout << "\"\n";

    for (int m = 0; m < params.maps.size(); ++m) {
        std::cout << params.maps[m];

        for (int s = 0; s < params.strategies.size(); ++s) {

            std::cout << "\t";

            for (const auto& r : results[m][s])
                std::cout << r;   
        }

        std::cout << "\n";
    }

    std::cout << "\n=== Tournament Complete ===\n";

}
/*  
 tournament -M "_64_ BIG BLUE.map" "_11_ Against All Odds.map" -P benevolent aggressive neutral -G 2 -D 10
 */
std::string TournamentCommand::Execute() {
    this->effect = "Tournament command under construction.";
    GameEngine::instance->RunTournament(
        TournamentParameters{
            this->mapFiles,
            this->playerStrategies,
            this->numberOfGames,
            this->maxTurns
        }
	);
    return this->effect;
};

std::string LoadMapCommand::Execute() {
    if (GameEngine::instance->currentState->name != GameEngine::instance->mapLoadedState.get()->name &&
        GameEngine::instance->currentState->name != GameEngine::instance->mainMenuState.get()->name) {

        this->effect = "Error: tried to execute loadmap from invalid state";
        std::cout << this->effect << "\n";
        return this->effect;
    }

    std::filesystem::path mapPath;

    // If user provided file name
    if (!argument.empty()) {
        std::filesystem::path fileName(argument);

        // If not absolute, assume maps folder
        if (!fileName.is_absolute()) {
            fileName = std::filesystem::current_path() / "maps" / fileName;
        }

        mapPath = fileName;
    }
    else {
        std::string input;
        std::cout << "No map selected!\nPlease input the directory of the maps, or press Enter to use current directory:\n";
        std::getline(std::cin, input);

        if (input.empty()) {
            input = std::filesystem::current_path().string();
        }

        mapPath = std::filesystem::path(input) / "maps";

        std::vector<std::string> mapFiles;
        int mapNumber = 1;

        std::cout << "Available maps:\n";
        for (auto& entry : std::filesystem::directory_iterator(mapPath)) {
            std::cout << "Map " << mapNumber << ": " << entry.path().filename().string() << "\n";
            mapFiles.push_back(entry.path().string());
            mapNumber++;
        }

        std::cout << "Enter the map number: ";
        int selectedOption = -1;
        std::cin >> selectedOption;

        if (selectedOption < 1 || selectedOption > mapFiles.size()) {
            this->effect = "Invalid map selection.";
            std::cout << this->effect << "\n";
            return this->effect;
        }

        mapPath = mapFiles[selectedOption - 1];
    }

    // Load map
    std::unique_ptr<MapLoader> loader = std::make_unique<MapLoader>();

    try {
        if (!loader->LoadMapFile(mapPath.string())) {
            this->effect = "Failed to load map: " + mapPath.string();
            std::cout << this->effect << "\n";
            return this->effect;
        }

        GameEngine::instance->gameMap = new Map(loader->CreateMap());
        this->effect = "Successfully loaded map: " + mapPath.string();
        std::cout << this->effect << "\n";
    }
    catch (const std::exception& e) {
        this->effect = std::string("Exception during map loading: ") + e.what();
        std::cout << this->effect << "\n";
        return this->effect;
    }

    // Change state if needed
    if (GameEngine::instance->currentState->name ==
        GameEngine::instance->mainMenuState->name) {
        GameEngine::instance->changeState(GameEngine::instance->mapLoadedState.get());
    }

    return this->effect;
};

std::string ValidateMapCommand::Execute() {
    std::cout << "Validating map...\n";

    // Check valid state
    if (GameEngine::instance->currentState != GameEngine::instance->mapLoadedState.get()) {
        this->effect = "Error: tried to execute validatemap from invalid state";
        std::cout << this->effect << "\n";
        return this->effect;
    }

    // Validate map
    if (GameEngine::instance->gameMap->Validate()) {
        this->effect = "Map validated successfully";
        std::cout << this->effect << "\n";
    }
    else {
        this->effect = "Map validation failed";
        std::cout << this->effect << "\n";
        return this->effect;
    }

    // Change state after successful validation
    if (GameEngine::instance->currentState == GameEngine::instance->mapLoadedState.get()) {
        GameEngine::instance->changeState(GameEngine::instance->mapValidatedState.get());
    }

    return this->effect;
};

std::string AddPlayerCommand::Execute() {
    // Prompt if no argument given
    if (argument.empty()) {
        std::cout << "Please specify a player name to add (e.g., 'addplayer Alice').\n";
        std::getline(std::cin, argument);
    }

    // Validate correct game state
    if (GameEngine::instance->currentState != GameEngine::instance->mapValidatedState.get() &&
        GameEngine::instance->currentState != GameEngine::instance->playersAddedState.get()) {
        this->effect = "Error: tried to execute addplayer from invalid state";
        std::cout << this->effect << "\n";
        return this->effect;
    }

    std::cout << "Adding player: " << argument << "\n";

    try {
        Player* newPlayer = new Player();
        newPlayer->SetName(argument);
        GameEngine::instance->players.push_back(newPlayer);
    }
    catch (const std::exception& e) {
        this->effect = "Exception occurred while adding player: " + std::string(e.what());
        std::cout << this->effect << "\n";
        return this->effect;
    }

    this->effect = "Player " + argument + " added successfully";
    std::cout << this->effect << "\n";

    // Transition state only after adding player
    if (GameEngine::instance->currentState == GameEngine::instance->mapValidatedState.get()) {
        GameEngine::instance->changeState(GameEngine::instance->playersAddedState.get());
    }

    return this->effect;
}

std::string GameStartCommand::Execute() {
    if (GameEngine::instance->currentState != GameEngine::instance->playersAddedState.get()) {
        this->effect = "Error: tried to execute gamestart from invalid state";
        std::cout << this->effect << "\n";
        return this->effect;
    }

    std::cout << "Running game...\n";

    this->effect = "Game started successfully";
    std::cout << this->effect << "\n";

    
    GameEngine::instance->changeState(GameEngine::instance->winState.get());

    return this->effect;

    // Shuffle players
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(GameEngine::instance->players.begin(), GameEngine::instance->players.end(), g);

    // Distribute territories round-robin
    std::vector<Territory*> territories = GameEngine::instance->gameMap->territories;
    int numPlayers = GameEngine::instance->players.size();
    for (int i = 0; i < territories.size(); i++) {
        Player* currentPlayer = GameEngine::instance->players[i % numPlayers];
        Territory* currentTerritory = territories[i];
        currentPlayer->AddTerritory(currentTerritory);
    }

    //50 initial army units to players
    for (int i = 0; i < GameEngine::instance->players.size(); i++) {
        Player* player = GameEngine::instance->players[i];
        player->AddReinforcements(50);
    }

    //each player draws 2 initial cards from the deck
    for (int i = 0; i < GameEngine::instance->players.size(); i++) {
        Player* player = GameEngine::instance->players[i];
        player->GetPlayerHand()->AddCard();
        player->GetPlayerHand()->AddCard();
    }
};

std::string ReplayCommand::Execute() {
    std::cout << "Replaying game...\n";

    if (GameEngine::instance->currentState == GameEngine::instance->winState.get()) {
        GameEngine::instance->changeState(GameEngine::instance->mainMenuState.get());
        this->effect = "Game restarted successfully";
        std::cout << this->effect << "\n";
        return this->effect;
    }

    std::cout << "Trying to execute replay from invalid state, force replay anyway? (y/n)\n";
    char response;
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
        GameEngine::instance->changeState(GameEngine::instance->mainMenuState.get());
        this->effect = "Game restarted successfully (forced)";
        std::cout << this->effect << "\n";
    }
    else {
        this->effect = "Replay command ignored";
        std::cout << this->effect << "\n";
    }

    return this->effect;
}

std::string QuitCommand::Execute() {
    // Valid quit only if game already finished
    if (GameEngine::instance->currentState == GameEngine::instance->winState.get()) {
        std::cout << "Quitting game...\n";
        this->effect = "Game quit successfully";
        std::cout << this->effect << "\n";
         exit(0); 

        return this->effect;
    }

    std::cout << "Trying to execute quit mid game, force quit anyway? (y/n)\n";
    char response;
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
        std::cout << "Quitting game...\n";
        this->effect = "Game quit successfully (forced)";
        std::cout << this->effect << "\n";
         exit(0); 
    }
    else {
        this->effect = "Quit command ignored";
        std::cout << this->effect << "\n";
    }

    return this->effect;
}

std::string NewGameCommand::Execute() {
    std::cout << "Starting a new game...\n";
    if (GameEngine::instance->currentState == GameEngine::instance->mainMenuState.get()) {
        this->effect = "Already in main menu state, new game ignored";
        std::cout << this->effect << "\n";
        return this->effect;
    }
    GameEngine::instance->changeState(GameEngine::instance->mainMenuState.get());
    this->effect = "New game started successfully";
    std::cout << this->effect << "\n";
    return this->effect;
}

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
	gameMap = nullptr;
	neutralPlayer = new Player();
    neutralPlayer->SetName("NEUTRAL");
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
		std::make_unique<TournamentCommand>("tournament", nullptr));

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
        std::make_unique<SimpleCommand>("newgame", mainMenuState.get()));

    winState->availableCommands.emplace_back(
        std::make_unique<SimpleCommand>("quit", nullptr)); // exit game

    // Start the game
	instance -> currentState = instance->mainMenuState.get(); // start at main menu
    instance-> currentState->OnEnter();
}

GameEngine::~GameEngine() = default; // unique_ptrs auto-cleanup

// Copy constructor, really just for the sake of having one, as you can tell its really annoying to implement
//cuz you have to deep copy everything but yeah it's there
GameEngine::GameEngine(const GameEngine& other) {
	gameMap = other.gameMap; //shallow copy map pointer
	neutralPlayer = other.neutralPlayer; //shallow copy neutral player pointer
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
    Notify(this);
}

std::string GameEngine::StringToLog() {
    if (currentState) {
        return "GameEngine: Transitioned to " + currentState->name;
    }
    return "GameEngine: No current state";
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

// Code for assignment 2//-----------------------------------------------------
//Alexandre Godfroy 40300165
//COMP-345-D 2252 Assignment 2 Game play: main game Loop
//Dr. Hamed Jafarpour
//Submitted: TBD
//Due: November 11, 2025 
//I certify that this submission is my original work and meets the Facultys Expectations of Originality 
//Alexandre Godfroy
//-----------------------------------------------------

// Part 3: Game play: main game loop

//Main game loop that cycles through the three phases
void GameEngine::mainGameLoop() {
    bool gameWon = false;
    while (!gameWon) {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
        checkPlayerElimination();
        gameWon = checkWinCondition();
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
        std::vector<Continent*> continents = player->GetOwnedContinents(gameMap);
        for (int j = 0; j < continents.size(); j++) {
            reinforcementUnits += continents[j]->GetPoints();
        }
        
        //add reinforcements to player's pool
        player->AddReinforcements(reinforcementUnits);
        std::cout << "Player " << player->GetName() << " receives " << reinforcementUnits << " reinforcement units.\n";
    }
    
    std::cout << "Reinforcement Phase ended.\n";
    
}

// Issue Orders phase - Players issue orders such as deploying armies, attacking other players,
// and fortifying their positions
void GameEngine::issueOrdersPhase() {
    std::cout << "Issue Orders Phase started.\n";
    std::vector<bool> isFinished(players.size(), false);
    int finishedPlayers = 0;
    while (finishedPlayers < players.size()) {
        for (int i = 0; i < players.size(); i++) {
            if (isFinished[i]) {
                continue; // Skip players who have finished issuing orders
            }

            Player* player = players[i]; // players vector needs to be defined somewhere in GameEngine
            std::cout << "Player " << player->GetName() << "'s turn to issue an order.\n";
            std::vector<OrderNames> orders = availableOrders(i);
            if (orders.empty()) {
                std::cout << "No available orders for player " << player->GetName() << ". Ending turn.\n";
                isFinished[i] = true;
                finishedPlayers++;
                continue;
            }
            std::cout << "Available orders:\n";
            for (int j = 0; j < orders.size(); j++) {
                switch (orders[j]) {
                case DeployEnum:
                    std::cout << j << " - Deploy\n";
                    break;

                case AdvanceEnum:
                    std::cout << j << " - Advance\n";
                    break;

                case BombEnum:
                    std::cout << j << " - Bomb\n";
                    break;

                case AirliftEnum:
                    std::cout << j << " - Airlift\n";
                    break;

                case BlockadeEnum:
                    std::cout << j << " - Blockade\n";
                    break;

                case NegotiateEnum:
                    std::cout << j << " - Negotiate\n";
                    break;

                default:
                    break;
                }
            }

            std::cout << "Enter the number of the order to issue, or -1 to end your turn: ";
            int choice;
            std::cin >> choice;
            // Issue the selected order
            switch (orders[choice]) {
                case  AdvanceEnum:{
                    string startTerritory;
                    string targetTerritory;
                    int numUnits;

                    //getting user input for advance order
                    std::cout << "You chose to issue an Advance order.\nPlease enter the starting territory: "<< std::endl;
                    cin >> startTerritory;
                    std::cout << "Please enter the target territory: "<< std::endl;
                    cin >> targetTerritory;
                    std::cout << "Please enter the number of units to move: "<< std::endl;
                    cin >> numUnits;
                
                    //creating and issuing the advance order
                    Advance* advanceOrder = new Advance(player, numUnits, gameMap->GetTerritoryByName(startTerritory), gameMap->GetTerritoryByName(targetTerritory));
                    Advance* ptr = advanceOrder;
                    player->IssueOrder(ptr);

                    std::cout << "Advance order issued.\n";
                    break;

                }case AirliftEnum:{
                    string startTerritory;
                    string targetTerritory;
                    int numUnits = 5;  // Example number of units to airlift    

                    //getting user input for airlift order
                    std::cout << "You chose to issue an Airlift order.\nPlease enter the starting territory: "<< std::endl;
                    cin >> startTerritory;
                    std::cout << "Please enter the target territory: "<< std::endl;
                    cin >> targetTerritory;
                    std::cout << "Please enter the number of units to move: "<< std::endl;
                    cin >> numUnits;

                    //creating and issuing the airlift order
                    int cardIndex = player->GetPlayerHand()->GetCardIndex(AirliftEnum);

                    if (cardIndex == player->GetPlayerHand()->GetHandSize()) {
                        std::cout << "ERROR: could not find the card in hand, please try again.";
                        --i;
                        break;
                    }

                    player->GetPlayerHand()->PlayCard(cardIndex);
                    Airlift* airliftOrder = new Airlift(player, numUnits, gameMap->GetTerritoryByName(startTerritory), gameMap->GetTerritoryByName(targetTerritory));
                    Airlift* ptr = airliftOrder;
                    player->IssueOrder(ptr);

                    std::cout << "Airlift order issued.\n";
                    break;

                }case BlockadeEnum:{
                    string targetTerritory;

                    //getting user input for blockade order
                    std::cout << "You chose to issue a Blockade order.\nPlease enter the target territory: "<< std::endl;
                    cin >> targetTerritory;

                    //creating and issuing the blockade order
                    int cardIndex = player->GetPlayerHand()->GetCardIndex(BlockadeEnum);

                    if (cardIndex == player->GetPlayerHand()->GetHandSize()) {
                        std::cout << "ERROR: could not find the card in hand, please try again.";
                        --i;
                        break;
                    }

                    player->GetPlayerHand()->PlayCard(cardIndex);
                    Blockade* blockadeOrder = new Blockade(player, gameMap->GetTerritoryByName(targetTerritory));
                    Blockade* ptr = blockadeOrder;
                    player->IssueOrder(ptr);

                    std::cout << "Blockade order issued.\n";
                    break;

                }case BombEnum:{
                    string targetTerritory;

                    //getting user input for bomb order
                    std::cout << "You chose to issue a Bomb order.\nPlease enter the target territory: "<< std::endl;
                    cin >> targetTerritory;

                    //creating and issuing the bomb order
                    int cardIndex = player->GetPlayerHand()->GetCardIndex(BombEnum);

                    if (cardIndex == player->GetPlayerHand()->GetHandSize()) {
                        std::cout << "ERROR: could not find the card in hand, please try again.";
                        --i;
                        break;
                    }

                    player->GetPlayerHand()->PlayCard(cardIndex);
                    Bomb* bombOrder = new Bomb(player, gameMap->GetTerritoryByName(targetTerritory));
                    Bomb* ptr = bombOrder;
                    player->IssueOrder(ptr);

                    std::cout << "Bomb order issued.\n";
                    break;

                }case DeployEnum:{
                    string targetTerritory;
                    int numUnits;

                   //getting user input for deploy order
                    std::cout << "You chose to issue a Deploy order.\nPlease enter the target territory: "<< std::endl;
                    cin >> targetTerritory;
                    std::cout << "Please enter the number of units to deploy: "<< std::endl;
                    cin >> numUnits;

                    while (numUnits < 0 || numUnits > player->GetReinforcements()) {
                        std::cout << "Please enter a valid number of units to deploy: "<< std::endl;
                        cin >> numUnits;
                    }

                    //creating and issuing the deploy order
                    Deploy* deployOrder = new Deploy(player, numUnits, gameMap->GetTerritoryByName(targetTerritory));
                    Deploy* ptr = deployOrder;
                    player->IssueOrder(ptr);
                    player->SetReinforcements(player->GetReinforcements() - numUnits); //in order to be able to issue other kinds of orders this turn

                    std::cout << "Deploy order issued.\n";
                    break;

                }case NegotiateEnum:{
                    string targetPlayerName;

                    //getting user input for negotiate order
                    std::cout << "You chose to issue a Negotiate order.\nPlease enter the target player name: "<< std::endl;
                    cin >> targetPlayerName;
                    Player* targetPlayer = FindPlayerByName(targetPlayerName);

                    while (targetPlayer == nullptr) {
                        std::cout << "Could not find a player by that name, please try again: " << std::endl;
                        cin >> targetPlayerName;
                        targetPlayer = FindPlayerByName(targetPlayerName);
                    }

                    //creating and issuing the negotiate order
                    int cardIndex = player->GetPlayerHand()->GetCardIndex(NegotiateEnum);

                    if (cardIndex == player->GetPlayerHand()->GetHandSize()) {
                        std::cout << "ERROR: could not find the card in hand, please try again.";
                        --i;
                        break;
                    }

                    player->GetPlayerHand()->PlayCard(cardIndex);
                    Negotiate* negotiateOrder = new Negotiate(player, targetPlayer);
                    Negotiate* ptr = negotiateOrder;
                    player->IssueOrder(ptr);

                    std::cout << "Negotiate order issued.\n";
                    break;
                } default:{
                    if (choice == -1){
                        isFinished[i] = true;
                        finishedPlayers++;
                        std::cout << "Player " << player->GetName() << " has ended their turn.\n";
                    }else {
                        std::cout << "Invalid choice. Please try again.\n";
                    i--; // Repeat this player's turn
                    }
                    break;
                }
            }
        }
    }
    std::cout << "Issue Orders Phase ended.\n";
}

std::vector<GameEngine::OrderNames> GameEngine::availableOrders(int playerID) {
    std::vector<GameEngine::OrderNames> orders;
    //while there are still reinforcements, deploy is the only available order
    if (players[playerID]->GetReinforcements() > 0) {
        orders.push_back(DeployEnum);
    } else {
        orders.push_back(AdvanceEnum);
        for (Card card : players[playerID]->GetPlayerHand()->GetCards()) {
            Card* ptr = &card;
            int type = ptr->GetType();
            switch (type) {
                case AirliftEnum:
                    orders.push_back(AirliftEnum);
                    break;
                case BombEnum:
                    orders.push_back(BombEnum);
                    break;
                case BlockadeEnum:
                    orders.push_back(BlockadeEnum);
                    break;
                case NegotiateEnum:
                    orders.push_back(NegotiateEnum);
                    break;
                default:
                    break;
            }
        }
    }
    return orders;
}

// Execute Orders phase - The issued orders are executed in the order they were issued
void GameEngine::executeOrdersPhase() {
    std::cout << "Execute Orders Phase started.\n";
    std::vector<bool> isFinished(players.size(), false);
    int finishedPlayers = 0;
    while (finishedPlayers < players.size()) {
        for (int i = 0; i < players.size(); i++) {
            if (isFinished[i]) {
                continue; // Skip players who have finished executing orders
            }

            Player* player = players[i]; // players vector needs to be defined somewhere in GameEngine
            OrdersList* ordersList = player->GetPlayerOrders();
            if (ordersList->IsEmpty()) {
                std::cout << "No more orders for player " << player->GetName() << ". Ending turn.\n";
                isFinished[i] = true;
                finishedPlayers++;
                continue;
            }

            std::cout << "Player " << player->GetName() << "'s turn to execute an order.\n";
            Order* order = ordersList->GetNextOrder();
            if (order) {
                order->Execute();
                ordersList->Remove(0); // Remove the executed order from the list
                delete order; // Assuming ownership of Order pointers
            }
        }
    }

    for (int i = 0; i < players.size(); i++) {
        Player* player = players[i];
        player->ResetNegotiationsAndConquer();
    }

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

//-----------------------------------------------------
//Mia Letendre 40274681
//COMP-345-D 2252 Assignment 2 Game Startup Phase
//Dr. Hamed Jafarpour
//Submitted: TBD
//Due: November 11, 2025 
//I certify that this submission is my original work and meets the Facultys Expectations of Originality 
//Mia Letendre
//-----------------------------------------------------
    //Creates a new player and adds them to the players vector
    //Requires a player name input as a string
void GameEngine::AddPlayers(const string& playerName) {
    Player* newPlayer = new Player();
    newPlayer->SetName(playerName);
    players.push_back(newPlayer);
}

//Loads a map from a file using MapLoader
//Takes map file name input as a string
//Assumes directory has a folder named "maps" in the current working directory
//Accepts the directory filename as input string
//default path is std::filesystem::current_path()
void GameEngine::LoadMap(const string& fileName) {
    std::filesystem::path mapPath = std::filesystem::path(fileName) / "maps";
    vector<std::string> mapFiles;
    int mapNumber = 1;
    std::cout << "The following are the Maps in the inputed directory, \n";
    for (auto& entry : std::filesystem::directory_iterator(mapPath)) {
        std::cout << "Map " << mapNumber << " " << entry.path().filename().string() << std::endl;
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
    bool loadedMap;
    loadedMap = loader->LoadMapFile(mapPath.string()); //automatically validates the map
    if (!loadedMap) {
        std::cout << "Failed to load map from file: " << mapPath.string() << std::endl;
        return;
    }

    GameEngine::instance->gameMap = new Map(loader->CreateMap());
}

//Validates the map using its Validate method
//Takes a map reference as input
bool GameEngine::ValidateMap() {
    return gameMap->Validate();
}

//a) fairly distribute all the territories to the players
//b) determine randomly the order of play of the players in the game
//c) give 50 initial army units to the players, which are placed in their respective reinforcement pool
//d) let each player draw 2 initial cards from the deck using the decks draw() method
//e) switch the game to the play phase
void GameEngine::GameStart() {

    //shuffling players 
    std::random_device rd; // seed from machines random device
    std::mt19937 g(rd()); //random number generator 
    std::shuffle(players.begin(), players.end(), g);

    //Distributing territories
    vector<Territory*> territories = gameMap->territories;
    int numPlayers = players.size();
    for (int i = 0; i < territories.size(); i++) {
        Player* currentPlayer = players[i % numPlayers];
        Territory* currentTerritory = territories[i];
        currentPlayer->AddTerritory(currentTerritory);
    }

    //50 initial army units to players
    for (int i = 0; i < players.size(); i++) {
        Player* player = players[i];
        player->AddReinforcements(50);
    }

    //each player draws 2 initial cards from the deck
    for (int i = 0; i < players.size(); i++) {
        Player* player = players[i];
        player->GetPlayerHand()->AddCard();
        player->GetPlayerHand()->AddCard();
    }


    //swicthe game to play phase
    mainGameLoop();
}

void GameEngine::StartupPhase() {
    std::string input;
    std::cout << "Starting game... Please input the name of the directory containing the maps, or input \"default\" to use the current directory: \n";
    std::cin >> input;
    if (input == "default") {
        input = std::filesystem::current_path().string();
    }
    LoadMap(input);
    bool isMapValid = ValidateMap();
    if (!isMapValid) {
        std::cout << "Map is invalid. Please try again.\n";
        std::cin >> input;
        if (input.empty()) {
            input = std::filesystem::current_path().string();
        }
        LoadMap(input);
        isMapValid = ValidateMap();
        if (!isMapValid) {
            std::cout << "Map is invalid again. Exiting startup phase.\n";
            return;
        }
    }
    std::cout << "Please input the number of players you would like to have in this game (2-6): \n";
    int numPlayers;
    std::cin >> (numPlayers);
    if (numPlayers < 2 || numPlayers > 6) {
        std::cout << "Invalid number of players. Please try again.\n";
        std::cin >> (numPlayers);
        if (numPlayers < 2 || numPlayers > 6) {
            std::cout << "Invalid number of players. Exiting startup phase.\n";
            return;
        }
    }
    for (int i = 0; i < numPlayers; i++) {
        std::string playerName;
        std::cout << "Please input the name of player " + std::to_string(i + 1) + ": \n";
        std::cin >> playerName;
        AddPlayers(playerName);
    }
    std::cout << "Players added successfully. Start up phase complete!\n";
}

Player* GameEngine::FindPlayerByName(const string &name) {
    for (int i = 0; i < players.size(); i++) {
        Player* p = players[i];

        if (p->GetName() == name) {
            return p;
        }
    }

    return nullptr;
}

void GameEngine::TestStartupPhase() {
    std::cout << "--------------------------" << std::endl;
    std::cout << "- TestStartupPhase Start -" << std::endl;
    std::cout << "--------------------------" << std::endl;
    StartupPhase();
    std::cout << "------------------------" << std::endl;
    std::cout << "- TestStartupPhase End -" << std::endl;
    std::cout << "------------------------" << std::endl;
}

void GameEngine::TestOrderExecution() {
    //dummy map init
    std::cout << "Initializing test map...\n";
    Map* testMap = new Map();
    Continent* testContinent = testMap->AddContinent("Continent", 5);
    Territory* tA = testMap->AddTerritory("TA", testContinent);
    tA->SetUnits(1);
    Territory* tB = testMap->AddTerritory("TB", testContinent);
    tB->SetUnits(15);
    Territory* tC = testMap->AddTerritory("TC", testContinent);
    tC->SetUnits(15);
    Territory* tD = testMap->AddTerritory("TD", testContinent);
    tD->SetUnits(15);
    testMap->AddAdjacency(tA, tB);
    testMap->AddAdjacency(tB, tC);
    testMap->AddAdjacency(tC, tD);
    GameEngine::instance->gameMap = testMap;


    //dummy players init
    std::cout << "Initializing test players...\n";
    Deck* deck = new Deck();
    GameEngine::instance->players.clear();
    GameEngine::instance->AddPlayers("Player1");
    GameEngine::instance->AddPlayers("Player2");

    Player* p1 = GameEngine::instance->players[0];
    p1->AddTerritory(tB);
    p1->AddTerritory(tC);
    p1->SetReinforcements(3);
    Hand* hand1 = new Hand(*deck);
    hand1->SetTestCards();
    p1->SetPlayerHand(hand1);

    Player* p2 = GameEngine::instance->players[1];
    p2->AddTerritory(tA);
    p2->AddTerritory(tD);
    p2->SetReinforcements(3);
    Hand* hand2 = new Hand(*deck);
    hand2->SetTestCards();
    p2->SetPlayerHand(hand2);

    //test the phases
    GameEngine::instance->issueOrdersPhase();
    GameEngine::instance->executeOrdersPhase();
}