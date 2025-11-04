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

// Code for assignment 2//-----------------------------------------------------
//Alexandre Godfroy 40300165
//COMP-345-D 2252 Assignment 2 Game play: main game Loop
//Dr. Hamed Jafarpour
//Submitted: TBD
//Due: November 11, 2025 
//I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
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
        // checkPlayerElimination();
        // gameWon = checkWinCondition();
    }
}

// Reinforcement phase - Players are given a number of army units that depends on the number of
// territories they own

//void GameEngine::reinforcementPhase() {
//    /*
//       std::cout << "Reinforcement Phase started.\n";
//
//    // Iterate through each player and calculate reinforcements
//    for (int i = 0; i < players.size(); i++) {
//        //initiates the player for every iteration
//        Player* player = players[i]; // players vector needs to be defined somewhere in GameEngine
//        int numTerritories = player->GetPlayerTerritories().size();
//        int reinforcementUnits = numTerritories / 3; // 1 unit per 3 territories owned
//        if (reinforcementUnits < 3) { // minimum of 3 units
//            reinforcementUnits = 3;
//        }
//
//        // Check for continent control bonuses
//        std::vector<Continent*> continents = player->GetOwnedContinents();
//        for (int j = 0; j < continents.size(); j++) {
//            reinforcementUnits += continents[j]->GetPoints();
//        }
//
//        //add reinforcements to player's pool
//        player->AddReinforcements(reinforcementUnits);
//        std::cout << "Player " << player->GetName() << " receives " << reinforcementUnits << " reinforcement units.\n";
//    }
//
//    std::cout << "Reinforcement Phase ended.\n";
//    */
//}
//
//// Issue Orders phase - Players issue orders such as deploying armies, attacking other players,
//// and fortifying their positions
//void GameEngine::issueOrdersPhase() {
//    std::cout << "Issue Orders Phase started.\n";
//    std::vector<bool> isFinished(players.size(), false);
//    int finishedPlayers = 0;
//    while (finishedPlayers < players.size()) {
//        for (int i = 0; i < players.size(); i++) {
//            if (isFinished[i]) {
//                continue; // Skip players who have finished issuing orders
//            }
//
//            Player* player = players[i]; // players vector needs to be defined somewhere in GameEngine
//            std::cout << "Player " << player->GetName() << "'s turn to issue an order.\n";
//            std::vector<OrderNames> orders = availableOrders(i);
//            if (orders.empty()) {
//                std::cout << "No available orders for player " << player->GetName() << ". Ending turn.\n";
//                isFinished[i] = true;
//                finishedPlayers++;
//                continue;
//            }
//            std::cout << "Available orders:\n";
//            for (int j = 0; j < orders.size(); j++) {
//                switch (orders[j]) {
//                case Deploy:
//                    std::cout << j << " - Deploy\n";
//                    break;
//
//                case Advance:
//                    std::cout << j << " - Advance\n";
//                    break;
//
//                case Bomb:
//                    std::cout << j << " - Bomb\n";
//                    break;
//
//                case Airlift:
//                    std::cout << j << " - Airlift\n";
//                    break;
//
//                case Blockade:
//                    std::cout << j << " - Blockade\n";
//                    break;
//
//                case Negotiate:
//                    std::cout << j << " - Negotiate\n";
//                    break;
//
//                default:
//                    break;
//                }
//            }
//
//            std::cout << "Enter the number of the order to issue, or -1 to end your turn: ";
//            int choice;
//            std::cin >> choice;
//            // Issue the selected order
//            switch (orders[choice]) {
//            case -1: {
//                isFinished[i] = true;
//                finishedPlayers++;
//                std::cout << "Player " << player->GetName() << " has ended their turn.\n";
//                break;
//            }case Advance: {
//                string startTerritory;
//                string targetTerritory;
//                int numUnits;
//
//                //getting user input for advance order
//                std::cout << "You chose to issue an Advance order.\nPlease enter the starting territory: " << std::endl;
//                cin >> startTerritory;
//                std::cout << "Please enter the target territory: " << std::endl;
//                cin >> targetTerritory;
//                std::cout << "Please enter the number of units to move: " << std::endl;
//                cin >> numUnits;
//
//                //creating and issuing the advance order
//                Order* advanceOrder = new Advance(player->GetName(), numUnits, gameMap->GetTerritoryByName(startTerritory), gameMap->GetTerritoryByName(targetTerritory));
//                player->IssueOrder(advanceOrder);
//
//                std::cout << "Advance order issued.\n";
//                break;
//
//            }case Airlift: {
//                string startTerritory;
//                string targetTerritory;
//                int numUnits = 5;  // Example number of units to airlift    
//
//                //getting user input for airlift order
//                std::cout << "You chose to issue an Airlift order.\nPlease enter the starting territory: " << std::endl;
//                cin >> startTerritory;
//                std::cout << "Please enter the target territory: " << std::endl;
//                cin >> targetTerritory;
//                std::cout << "Please enter the number of units to move: " << std::endl;
//                cin >> numUnits;
//
//                //creating and issuing the airlift order
//                Order* airliftOrder = new Airlift(player->GetName(), numUnits, gameMap->GetTerritoryByName(startTerritory), gameMap->GetTerritoryByName(targetTerritory));
//                player->IssueOrder(airliftOrder);
//
//                std::cout << "Airlift order issued.\n";
//                break;
//
//            }case Blockade: {
//                string targetTerritory;
//
//                //getting user input for blockade order
//                std::cout << "You chose to issue a Blockade order.\nPlease enter the target territory: " << std::endl;
//                cin >> targetTerritory;
//
//                //creating and issuing the blockade order
//                Order* blockadeOrder = new Blockade(player->GetName(), gameMap->GetTerritoryByName(targetTerritory));
//                player->IssueOrder(blockadeOrder);
//
//                std::cout << "Blockade order issued.\n";
//                break;
//
//            }case Bomb: {
//                string targetTerritory;
//
//                //getting user input for bomb order
//                std::cout << "You chose to issue a Bomb order.\nPlease enter the target territory: " << std::endl;
//                cin >> targetTerritory;
//
//                //creating and issuing the bomb order
//                Order* bombOrder = new Bomb(player->GetName(), gameMap->GetTerritoryByName(targetTerritory));
//                player->IssueOrder(bombOrder);
//
//                std::cout << "Bomb order issued.\n";
//                break;
//
//            }   case Deploy: {
//                string targetTerritory;
//                int numUnits;
//
//                //getting user input for deploy order
//                std::cout << "You chose to issue a Deploy order.\nPlease enter the target territory: " << std::endl;
//                cin >> targetTerritory;
//                std::cout << "Please enter the number of units to deploy: " << std::endl;
//                cin >> numUnits;
//
//                //creating and issuing the deploy order
//                Order* deployOrder = new Deploy(player->GetName(), numUnits, gameMap->GetTerritoryByName(targetTerritory));
//                player->IssueOrder(deployOrder);
//
//                std::cout << "Deploy order issued.\n";
//                break;
//
//            case Negotiate:
//                string targetPlayerName;
//
//                //getting user input for negotiate order
//                std::cout << "You chose to issue a Negotiate order.\nPlease enter the target player name: " << std::endl;
//                cin >> targetPlayerName;
//
//                //creating and issuing the negotiate order
//                Order* negotiateOrder = new Negotiate(player->GetName(), targetPlayerName);
//                player->IssueOrder(negotiateOrder);
//
//                std::cout << "Negotiate order issued.\n";
//                break;
//            } default: {
//                std::cout << "Invalid choice. Please try again.\n";
//                i--; // Repeat this player's turn
//                break;
//            }
//            }
//        }
//    }
//    std::cout << "Issue Orders Phase ended.\n";
//}
//
//std::vector<OrderNames> GameEngine::availableOrders(int playerID) {
//    std::vector<OrderNames> orders;
//    //while there are still reinforcements, deploy is the only available order
//    if (players[playerID]->GetReinforcements() > 0) {
//        orders.push_back(Deploy);
//    }
//    else {
//        orders.push_back(Advance);
//        for (Card* card : players[playerID]->GetPlayerHand()->GetCards()) {
//            switch (card->GetType()) {
//            case Card::Airlift:
//                orders.push_back(Airlift);
//                break;
//            case Card::Bomb:
//                orders.push_back(Bomb);
//                break;
//            case Card::Blockade:
//                orders.push_back(Blockade);
//                break;
//            case Card::Negotiate:
//                orders.push_back(Negotiate);
//                break;
//            default:
//                break;
//            }
//        }
//    }
//    return orders;
//}
//
//// Execute Orders phase - The issued orders are executed in the order they were issued
//void GameEngine::executeOrdersPhase() {
//    std::cout << "Execute Orders Phase started.\n";
//    std::vector<bool> isFinished(players.size(), false);
//    int finishedPlayers = 0;
//    while (finishedPlayers < players.size()) {
//        for (int i = 0; i < players.size(); i++) {
//            if (isFinished[i]) {
//                continue; // Skip players who have finished executing orders
//            }
//
//            Player* player = players[i]; // players vector needs to be defined somewhere in GameEngine
//            OrdersList* ordersList = player->GetPlayerOrders();
//            if (ordersList->IsEmpty()) {
//                std::cout << "No more orders for player " << player->GetName() << ". Ending turn.\n";
//                isFinished[i] = true;
//                finishedPlayers++;
//                continue;
//            }
//
//            std::cout << "Player " << player->GetName() << "'s turn to execute an order.\n";
//            Order* order = ordersList->GetNextOrder();
//            if (order) {
//                order->Execute();
//                ordersList->RemoveOrder(0); // Remove the executed order from the list
//                delete order; // Assuming ownership of Order pointers
//            }
//        }
//    }
//    std::cout << "Execute Orders Phase ended.\n";
//}
//
//// Check for win condition - returns true if a player has won the game
//bool GameEngine::checkWinCondition() {
//    if (players.size() == 1) {
//        std::cout << "Player " << players[0]->GetName() << " has won the game!\n";
//        return true;
//    }
//    return false;
//}
//
//// Check for player elimination - returns the number of players eliminated in the current turn
//void GameEngine::checkPlayerElimination() {
//
//    // Check for player elimination
//    std::vector<Player*> eliminatedPlayers;
//    for (int i = 0; i < players.size(); i++) {
//        Player* player = players[i]; // players vector needs to be defined somewhere in GameEngine
//        if (player->GetPlayerTerritories().empty()) {
//            eliminatedPlayers.push_back(player);
//            std::cout << "Player " << player->GetName() << " has been eliminated from the game.\n";
//
//            //removes player from memory
//            delete player;
//
//            //removes player from players vector
//            for (int j = i; j < players.size() - 1; j++) {
//                players[j] = players[j + 1];
//            }
//            players.pop_back();
//            i--; // Adjust index after removal
//        }
//    }
//
//    // Remove eliminated players from the game
//    for (Player* eliminatedPlayer : eliminatedPlayers) {
//        players.erase(std::remove(players.begin(), players.end(), eliminatedPlayer), players.end());
//        delete eliminatedPlayer; // Assuming ownership of Player pointers
//    }
//}

//-----------------------------------------------------
//Mia Letendre 40274681
//COMP-345-D 2252 Assignment 2 Game Startup Phase
//Dr. Hamed Jafarpour
//Submitted: TBD
//Due: November 11, 2025 
//I certify that this submission is my original work and meets the Faculty’s Expectations of Originality 
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
//d) let each player draw 2 initial cards from the deck using the deck’s draw() method
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
    std::cout << "Players added successfully. Starting game...\n";
    GameStart();
}

void GameEngine::TestStartupPhase() {
    StartupPhase();
}